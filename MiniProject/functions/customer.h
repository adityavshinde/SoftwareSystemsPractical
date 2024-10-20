#ifndef CUSTOMER_FUNCTIONS
#define CUSTOMER_FUNCTIONS

#include <sys/ipc.h>
#include <sys/sem.h>

struct Customer loggedInCustomer;
int semIdentifier;

// Function Declarations

bool customer_operation_handler(int connFD);
bool deposit(int connFD);
bool withdraw(int connFD);
bool get_balance(int connFD);
bool transfer(int connFD);
bool get_account_details_by_number(int accountNumber, struct Account *account);
bool change_password(int connFD);
bool lock_critical_section(struct sembuf *semOp);
bool unlock_critical_section(struct sembuf *sem_op);
void write_transaction_to_array(int *transactionArray, int ID);
int write_transaction_to_file(int accountNumber, long int oldBalance, long int newBalance, bool operation);
bool apply_for_loan(int connFD);


// Function Definitions

bool customer_operation_handler(int connFD)
{
    if (login_handler(2, connFD, &loggedInCustomer, NULL))
    {
        ssize_t writeBytes, readBytes;            
        char readBuffer[1000], writeBuffer[1000]; 

        key_t semKey = ftok(CUSTOMER_FILE, loggedInCustomer.account); 

        union semun
        {
            int val; 
        } semSet;

        int semctlStatus;
        semIdentifier = semget(semKey, 1, 0); 
        if (semIdentifier == -1)
        {
            semIdentifier = semget(semKey, 1, IPC_CREAT | 0700); 
            if (semIdentifier == -1)
            {
                perror("Error while creating semaphore!");
                _exit(1);
            }

            semSet.val = 1; 
            semctlStatus = semctl(semIdentifier, 0, SETVAL, semSet);
            if (semctlStatus == -1)
            {
                perror("Error while initializing a binary sempahore!");
                _exit(1);
            }
        }

        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, CUSTOMER_LOGIN_SUCCESS);
        while (1)
        {
            strcat(writeBuffer, "\n");
            strcat(writeBuffer, CUSTOMER_MENU);
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1)
            {
                perror("Error while writing CUSTOMER_MENU to client!");
                return false;
            }
            bzero(writeBuffer, sizeof(writeBuffer));

            bzero(readBuffer, sizeof(readBuffer));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer));
            if (readBytes == -1)
            {
                perror("Error while reading client's choice for CUSTOMER_MENU");
                return false;
            }
            
            int choice = atoi(readBuffer);
            switch (choice)
            {
            case 1:
                get_customer_details(connFD, loggedInCustomer.id);
                break;
            case 2:
                deposit(connFD);
                break;
            case 3:
                withdraw(connFD);
                break;
            case 4:
                get_balance(connFD);
                break;
            case 5:
                get_transaction_details(connFD, loggedInCustomer.account);
                break;
            case 6:
                change_password(connFD);
                break;
            case 7:
                transfer(connFD);
                break;
            case 8:
                apply_for_loan(connFD);
                break;
            case 9:
                //feedback();
                printf("This operation is under process\n");
                break;
            default:
                writeBytes = write(connFD, CUSTOMER_LOGOUT, strlen(CUSTOMER_LOGOUT));
                return false;
            }
        }
    }
    else
    {
        return false;
    }
    return true;
}

bool deposit(int connFD)
{
    char readBuffer[1000], writeBuffer[1000];
    ssize_t readBytes, writeBytes;

    struct Account account;
    account.accountNumber = loggedInCustomer.account;

    long int depositAmount = 0;

    // Lock the critical section
    struct sembuf semOp;
    lock_critical_section(&semOp);

    if (get_account_details(connFD, &account))
    {
        
        if (account.active)
        {

            writeBytes = write(connFD, DEPOSIT_AMOUNT, strlen(DEPOSIT_AMOUNT));
            if (writeBytes == -1)
            {
                perror("Error writing DEPOSIT_AMOUNT to client!");
                unlock_critical_section(&semOp);
                return false;
            }

            bzero(readBuffer, sizeof(readBuffer));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer));
            if (readBytes == -1)
            {
                perror("Error reading deposit money from client!");
                unlock_critical_section(&semOp);
                return false;
            }

            depositAmount = atol(readBuffer);
            if (depositAmount != 0)
            {

                int newTransactionID = write_transaction_to_file(account.accountNumber, account.balance, account.balance + depositAmount, 1);
                write_transaction_to_array(account.transactions, newTransactionID);

                account.balance += depositAmount;

                int accountFD = open(ACCOUNT_FILE, O_WRONLY);
                off_t offset = lseek(accountFD, account.accountNumber * sizeof(struct Account), SEEK_SET);

                struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(struct Account), getpid()};
                int lockingStatus = fcntl(accountFD, F_SETLKW, &lock);
                if (lockingStatus == -1)
                {
                    perror("Error obtaining write lock on account file!");
                    unlock_critical_section(&semOp);
                    return false;
                }

                writeBytes = write(accountFD, &account, sizeof(struct Account));
                if (writeBytes == -1)
                {
                    perror("Error storing updated deposit money in account record!");
                    unlock_critical_section(&semOp);
                    return false;
                }

                lock.l_type = F_UNLCK;
                fcntl(accountFD, F_SETLK, &lock);

                write(connFD, DEPOSIT_AMOUNT_SUCCESS, strlen(DEPOSIT_AMOUNT_SUCCESS));
                read(connFD, readBuffer, sizeof(readBuffer)); 

                get_balance(connFD);

                unlock_critical_section(&semOp);

                return true;
            }
            else
                writeBytes = write(connFD, DEPOSIT_AMOUNT_INVALID, strlen(DEPOSIT_AMOUNT_INVALID));
        }
        else
            write(connFD, ACCOUNT_DEACTIVATED, strlen(ACCOUNT_DEACTIVATED));
        read(connFD, readBuffer, sizeof(readBuffer)); 

        unlock_critical_section(&semOp);
    }
    else
    {
        unlock_critical_section(&semOp);
        return false;
    }
}

bool withdraw(int connFD)
{
    char readBuffer[1000], writeBuffer[1000];
    ssize_t readBytes, writeBytes;

    struct Account account;
    account.accountNumber = loggedInCustomer.account;

    long int withdrawAmount = 0;

    // Lock the critical section
    struct sembuf semOp;
    lock_critical_section(&semOp);

    if (get_account_details(connFD, &account))
    {
        if (account.active)
        {

            writeBytes = write(connFD, WITHDRAW_AMOUNT, strlen(WITHDRAW_AMOUNT));
            if (writeBytes == -1)
            {
                perror("Error writing WITHDRAW_AMOUNT message to client!");
                unlock_critical_section(&semOp);
                return false;
            }

            bzero(readBuffer, sizeof(readBuffer));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer));
            if (readBytes == -1)
            {
                perror("Error reading withdraw amount from client!");
                unlock_critical_section(&semOp);
                return false;
            }

            withdrawAmount = atol(readBuffer);

            if (withdrawAmount != 0 && account.balance - withdrawAmount >= 0)
            {

                int newTransactionID = write_transaction_to_file(account.accountNumber, account.balance, account.balance - withdrawAmount, 0);
                write_transaction_to_array(account.transactions, newTransactionID);

                account.balance -= withdrawAmount;

                int accountFD = open(ACCOUNT_FILE, O_WRONLY);
                off_t offset = lseek(accountFD, account.accountNumber * sizeof(struct Account), SEEK_SET);

                struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(struct Account), getpid()};
                int lockingStatus = fcntl(accountFD, F_SETLKW, &lock);
                if (lockingStatus == -1)
                {
                    perror("Error obtaining write lock on account record!");
                    unlock_critical_section(&semOp);
                    return false;
                }

                writeBytes = write(accountFD, &account, sizeof(struct Account));
                if (writeBytes == -1)
                {
                    perror("Error writing updated balance into account file!");
                    unlock_critical_section(&semOp);
                    return false;
                }

                lock.l_type = F_UNLCK;
                fcntl(accountFD, F_SETLK, &lock);

                write(connFD, WITHDRAW_AMOUNT_SUCCESS, strlen(WITHDRAW_AMOUNT_SUCCESS));
                read(connFD, readBuffer, sizeof(readBuffer)); 

                get_balance(connFD);

                unlock_critical_section(&semOp);

                return true;
            }
            else
                writeBytes = write(connFD, WITHDRAW_AMOUNT_INVALID, strlen(WITHDRAW_AMOUNT_INVALID));
        }
        else
            write(connFD, ACCOUNT_DEACTIVATED, strlen(ACCOUNT_DEACTIVATED));
        read(connFD, readBuffer, sizeof(readBuffer)); 

        unlock_critical_section(&semOp);
    }
    else
    {
        unlock_critical_section(&semOp);
        return false;
    }
}

bool transfer(int connFD) {
    char readBuffer[1000], writeBuffer[1000];
    ssize_t readBytes, writeBytes;

    struct Account senderAccount, recipientAccount;
    senderAccount.accountNumber = loggedInCustomer.account;

    long int transferAmount = 0;
    int recipientAccountNumber = 0;

    // Lock the critical section
    struct sembuf semOp;
    lock_critical_section(&semOp);

    if (get_account_details(connFD, &senderAccount)) {
        if (senderAccount.active) {

            writeBytes = write(connFD, "Enter the recipient account number: ", strlen("Enter the recipient account number: "));
            if (writeBytes == -1) {
                perror("Error writing recipient account request to client!");
                unlock_critical_section(&semOp);
                return false;
            }

            bzero(readBuffer, sizeof(readBuffer));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer));
            if (readBytes == -1) {
                perror("Error reading recipient account number from client!");
                unlock_critical_section(&semOp);
                return false;
            }

            recipientAccountNumber = atoi(readBuffer);
            if (!get_account_details_by_number(recipientAccountNumber, &recipientAccount)) {
                write(connFD, "Invalid recipient account number.\n", strlen("Invalid recipient account number.\n"));
                unlock_critical_section(&semOp);
                return false;
            }

            writeBytes = write(connFD, "Enter the amount to transfer: ", strlen("Enter the amount to transfer: "));
            if (writeBytes == -1) {
                perror("Error writing transfer amount request to client!");
                unlock_critical_section(&semOp);
                return false;
            }

            bzero(readBuffer, sizeof(readBuffer));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer));
            if (readBytes == -1) {
                perror("Error reading transfer amount from client!");
                unlock_critical_section(&semOp);
                return false;
            }

            transferAmount = atol(readBuffer);

            if (transferAmount > 0 && senderAccount.balance >= transferAmount) {

                int newTransactionID = write_transaction_to_file(senderAccount.accountNumber, senderAccount.balance, senderAccount.balance - transferAmount, 0);
                write_transaction_to_array(senderAccount.transactions, newTransactionID);
                senderAccount.balance -= transferAmount;

                int accountFD = open(ACCOUNT_FILE, O_WRONLY);
                off_t offset = lseek(accountFD, senderAccount.accountNumber * sizeof(struct Account), SEEK_SET);
                struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(struct Account), getpid()};
                int lockingStatus = fcntl(accountFD, F_SETLKW, &lock);
                if (lockingStatus == -1) {
                    perror("Error obtaining write lock on sender's account file!");
                    unlock_critical_section(&semOp);
                    return false;
                }

                writeBytes = write(accountFD, &senderAccount, sizeof(struct Account));
                if (writeBytes == -1) {
                    perror("Error writing sender's account balance!");
                    unlock_critical_section(&semOp);
                    return false;
                }

                lock.l_type = F_UNLCK;
                fcntl(accountFD, F_SETLK, &lock);

                newTransactionID = write_transaction_to_file(recipientAccount.accountNumber, recipientAccount.balance, recipientAccount.balance + transferAmount, 1);
                write_transaction_to_array(recipientAccount.transactions, newTransactionID);
                recipientAccount.balance += transferAmount;

                offset = lseek(accountFD, recipientAccount.accountNumber * sizeof(struct Account), SEEK_SET);
                lock.l_type = F_WRLCK;
                lock.l_start = offset;
                lockingStatus = fcntl(accountFD, F_SETLKW, &lock);
                if (lockingStatus == -1) {
                    perror("Error obtaining write lock on recipient's account file!");
                    unlock_critical_section(&semOp);
                    return false;
                }

                writeBytes = write(accountFD, &recipientAccount, sizeof(struct Account));
                if (writeBytes == -1) {
                    perror("Error updating recipient's balance in account record!");
                    unlock_critical_section(&semOp);
                    return false;
                }

                lock.l_type = F_UNLCK;
                fcntl(accountFD, F_SETLK, &lock);

                close(accountFD);

                write(connFD, "Transfer successful!\n", strlen("Transfer successful!\n"));
                read(connFD, readBuffer, sizeof(readBuffer)); 

                get_balance(connFD);  

                unlock_critical_section(&semOp);

                return true;
            } else {
                write(connFD, "Insufficient funds to complete the transfer.\n", strlen("Insufficient funds to complete the transfer.\n"));
            }
        } else {
            write(connFD, "Account is deactivated.\n", strlen("Account is deactivated.\n"));
        }
        read(connFD, readBuffer, sizeof(readBuffer)); 

        unlock_critical_section(&semOp);
    } else {
        unlock_critical_section(&semOp);
        return false;
    }

    return false;
}

bool get_account_details_by_number(int accountNumber, struct Account *account) {
    int accountFD = open(ACCOUNT_FILE, O_RDONLY);
    if (accountFD == -1) {
        perror("Error opening account file");
        return false;
    }

    off_t offset = lseek(accountFD, accountNumber * sizeof(struct Account), SEEK_SET);
    if (offset == -1) {
        perror("Error seeking to account number in file");
        close(accountFD);
        return false;
    }

    ssize_t readBytes = read(accountFD, account, sizeof(struct Account));
    close(accountFD);

    if (readBytes == -1) {
        perror("Error reading account details");
        return false;
    }

    return true;
}

bool get_balance(int connFD)
{
    char buffer[1000];
    struct Account account;
    account.accountNumber = loggedInCustomer.account;
    if (get_account_details(connFD, &account))
    {
        bzero(buffer, sizeof(buffer));
        if (account.active)
        {
            sprintf(buffer, "Your balance is ₹ %ld !^", account.balance);
            write(connFD, buffer, strlen(buffer));
        }
        else
            write(connFD, ACCOUNT_DEACTIVATED, strlen(ACCOUNT_DEACTIVATED));
        read(connFD, buffer, sizeof(buffer)); 
    }
    else
    {
        return false;
    }
}

bool change_password(int connFD)
{
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000], hashedPassword[1000];

    char newPassword[1000];

    struct sembuf semOp = {0, -1, SEM_UNDO};
    int semopStatus = semop(semIdentifier, &semOp, 1);
    if (semopStatus == -1)
    {
        perror("Error while locking critical section");
        return false;
    }

    writeBytes = write(connFD, PASSWORD_CHANGE_OLD_PASS, strlen(PASSWORD_CHANGE_OLD_PASS));
    if (writeBytes == -1)
    {
        perror("Error writing PASSWORD_CHANGE_OLD_PASS message to client!");
        unlock_critical_section(&semOp);
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading old password response from client");
        unlock_critical_section(&semOp);
        return false;
    }

    if (strcmp(crypt(readBuffer, SALT), loggedInCustomer.password) == 0)
    {
        writeBytes = write(connFD, PASSWORD_CHANGE_NEW_PASS, strlen(PASSWORD_CHANGE_NEW_PASS));
        if (writeBytes == -1)
        {
            perror("Error writing PASSWORD_CHANGE_NEW_PASS message to client!");
            unlock_critical_section(&semOp);
            return false;
        }
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error reading new password response from client");
            unlock_critical_section(&semOp);
            return false;
        }

        strcpy(newPassword, crypt(readBuffer, SALT));

        writeBytes = write(connFD, PASSWORD_CHANGE_NEW_PASS_RE, strlen(PASSWORD_CHANGE_NEW_PASS_RE));
        if (writeBytes == -1)
        {
            perror("Error writing PASSWORD_CHANGE_NEW_PASS_RE message to client!");
            unlock_critical_section(&semOp);
            return false;
        }
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error reading new password reenter response from client");
            unlock_critical_section(&semOp);
            return false;
        }

        if (strcmp(crypt(readBuffer, SALT), newPassword) == 0)
        {

            strcpy(loggedInCustomer.password, newPassword);

            int customerFD = open(CUSTOMER_FILE, O_WRONLY);
            if (customerFD == -1)
            {
                perror("Error opening customer file!");
                unlock_critical_section(&semOp);
                return false;
            }

            off_t offset = lseek(customerFD, loggedInCustomer.id * sizeof(struct Customer), SEEK_SET);
            if (offset == -1)
            {
                perror("Error seeking to the customer record!");
                unlock_critical_section(&semOp);
                return false;
            }

            struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(struct Customer), getpid()};
            int lockingStatus = fcntl(customerFD, F_SETLKW, &lock);
            if (lockingStatus == -1)
            {
                perror("Error obtaining write lock on customer record!");
                unlock_critical_section(&semOp);
                return false;
            }

            writeBytes = write(customerFD, &loggedInCustomer, sizeof(struct Customer));
            if (writeBytes == -1)
            {
                perror("Error storing updated customer password into customer record!");
                unlock_critical_section(&semOp);
                return false;
            }

            lock.l_type = F_UNLCK;
            lockingStatus = fcntl(customerFD, F_SETLK, &lock);

            close(customerFD);

            writeBytes = write(connFD, PASSWORD_CHANGE_SUCCESS, strlen(PASSWORD_CHANGE_SUCCESS));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer)); 

            unlock_critical_section(&semOp);

            return true;
        }
        else
        {
            writeBytes = write(connFD, PASSWORD_CHANGE_NEW_PASS_INVALID, strlen(PASSWORD_CHANGE_NEW_PASS_INVALID));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer)); 
        }
    }
    else
    {
        writeBytes = write(connFD, PASSWORD_CHANGE_OLD_PASS_INVALID, strlen(PASSWORD_CHANGE_OLD_PASS_INVALID));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); 
    }

    unlock_critical_section(&semOp);

    return false;
}

bool lock_critical_section(struct sembuf *semOp)
{
    semOp->sem_flg = SEM_UNDO;
    semOp->sem_op = -1;
    semOp->sem_num = 0;
    int semopStatus = semop(semIdentifier, semOp, 1);
    if (semopStatus == -1)
    {
        perror("Error while locking critical section");
        return false;
    }
    return true;
}

bool unlock_critical_section(struct sembuf *semOp)
{
    semOp->sem_op = 1;
    int semopStatus = semop(semIdentifier, semOp, 1);
    if (semopStatus == -1)
    {
        perror("Error while operating on semaphore!");
        _exit(1);
    }
    return true;
}

void write_transaction_to_array(int *transactionArray, int ID)
{
    int iter = 0;
    while (transactionArray[iter] != -1)
        iter++;

    if (iter >= MAX_TRANSACTIONS)
    {
        for (iter = 1; iter < MAX_TRANSACTIONS; iter++)
            transactionArray[iter - 1] = transactionArray[iter];
        transactionArray[iter - 1] = ID;
    }
    else
    {
        transactionArray[iter] = ID;
    }
}

int write_transaction_to_file(int accountNumber, long int oldBalance, long int newBalance, bool operation)
{
    struct Transaction newTransaction;
    newTransaction.accountNumber = accountNumber;
    newTransaction.oldBalance = oldBalance;
    newTransaction.newBalance = newBalance;
    newTransaction.operation = operation;
    newTransaction.transactionTime = time(NULL);

    ssize_t readBytes, writeBytes;

    int transactionFD = open(TRANSACTION_FILE, O_CREAT | O_APPEND | O_RDWR, S_IRWXU);

    off_t offset = lseek(transactionFD, -sizeof(struct Transaction), SEEK_END);
    if (offset >= 0)
    {
        struct Transaction prevTransaction;
        readBytes = read(transactionFD, &prevTransaction, sizeof(struct Transaction));

        newTransaction.transactionID = prevTransaction.transactionID + 1;
    }
    else
        newTransaction.transactionID = 0;

    writeBytes = write(transactionFD, &newTransaction, sizeof(struct Transaction));

    return newTransaction.transactionID;
}

bool apply_for_loan(int connFD) {
    char readBuffer[1000], writeBuffer[1000];
    struct Loan newLoan;

    write(connFD, "Enter loan amount: ", strlen("Enter loan amount: "));
    read(connFD, readBuffer, sizeof(readBuffer));
    newLoan.amount = atol(readBuffer); 

    newLoan.customerID = loggedInCustomer.id;
    newLoan.status = PENDING;

    int loanFD = open(LOAN_FILE, O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);
    if (loanFD == -1) {
        perror("Error while creating / opening loan file");
        return false;
    }

    write(loanFD, &newLoan, sizeof(newLoan));
    close(loanFD);

    write(connFD, "Loan application submitted successfully!\n", strlen("Loan application submitted successfully!\n"));
    return true;
}

#endif
