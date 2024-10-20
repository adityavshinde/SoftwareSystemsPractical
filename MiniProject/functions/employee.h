#ifndef EMPLOYEE_FUNCTIONS
#define EMPLOYEE_FUNCTIONS

#include "./common.h"

#include <sys/ipc.h>
#include <sys/sem.h>

struct Employee loggedInEmployee;
int semIdentifier;

// Function Declarations

bool employee_operation_handler(int connFD);
bool add_account(int connFD);
int add_customer(int connFD, int newAccountNumber);
bool delete_account(int connFD);
bool process_loan_application(int connFD);
bool approve_or_reject_loan(int connFD);
bool view_assigned_loans(int connFD);

// Function Definitions

bool employee_operation_handler(int connFD)
{

    if (login_handler(3, connFD, NULL, &loggedInEmployee))
    {
        ssize_t writeBytes, readBytes;         
        char readBuffer[1000], writeBuffer[1000]; 
        key_t semKey = ftok(EMPLOYEE_FILE, loggedInEmployee.id); 

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
        strcpy(writeBuffer, EMPLOYEE_LOGIN_SUCCESS);
        while (1)
        {
            strcat(writeBuffer, "\n");
            strcat(writeBuffer, EMPLOYEE_MENU);
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1)
            {
                perror("Error while writing EMPLOYEE_MENU to client!");
                return false;
            }
            bzero(writeBuffer, sizeof(writeBuffer));

            readBytes = read(connFD, readBuffer, sizeof(readBuffer));
            if (readBytes == -1)
            {
                perror("Error while reading client's choice for EMPLOYEE_MENU");
                return false;
            }

            int choice = atoi(readBuffer);
            switch (choice)
            {
            case 1:
                add_account(connFD);
                break;
            case 2:
                modify_customer_info(connFD);
                break;
            case 3: 
                process_loan_application(connFD);
                break;
            case 4:
                approve_or_reject_loan(connFD);
                break;
            case 5:
                view_assigned_loans(connFD);
                break;
            case 6:
                get_transaction_details(connFD, -1);
                break;
            case 7:
                change_password(connFD);
                break;
            case 8:
                delete_account(connFD);
                break;
            default:
                writeBytes = write(connFD, EMPLOYEE_LOGOUT, strlen(EMPLOYEE_LOGOUT));
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

bool add_account(int connFD)
{
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];

    struct Account newAccount, prevAccount;

    int accountFD = open(ACCOUNT_FILE, O_RDONLY);
    if (accountFD == -1 && errno == ENOENT)
    {
        newAccount.accountNumber = 0;
    }
    else if (accountFD == -1)
    {
        perror("Error while opening account file");
        return false;
    }
    else
    {
        int offset = lseek(accountFD, -sizeof(struct Account), SEEK_END);
        if (offset == -1)
        {
            perror("Error seeking to last Account record!");
            return false;
        }

        struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Account), getpid()};
        int lockingStatus = fcntl(accountFD, F_SETLKW, &lock);
        if (lockingStatus == -1)
        {
            perror("Error obtaining read lock on Account record!");
            return false;
        }

        readBytes = read(accountFD, &prevAccount, sizeof(struct Account));
        if (readBytes == -1)
        {
            perror("Error while reading Account record from file!");
            return false;
        }

        lock.l_type = F_UNLCK;
        fcntl(accountFD, F_SETLK, &lock);

        close(accountFD);

        newAccount.accountNumber = prevAccount.accountNumber + 1;
    }
    newAccount.owners[0] = add_customer(connFD, newAccount.accountNumber);
    newAccount.active = true;
    newAccount.balance = 0;

    memset(newAccount.transactions, -1, MAX_TRANSACTIONS * sizeof(int));

    accountFD = open(ACCOUNT_FILE, O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);
    if (accountFD == -1)
    {
        perror("Error while creating / opening account file!");
        return false;
    }

    writeBytes = write(accountFD, &newAccount, sizeof(struct Account));
    if (writeBytes == -1)
    {
        perror("Error while writing Account record to file!");
        return false;
    }

    close(accountFD);

    bzero(writeBuffer, sizeof(writeBuffer));
    sprintf(writeBuffer, "%s%d", EMPLOYEE_ADD_ACCOUNT_NUMBER, newAccount.accountNumber);
    strcat(writeBuffer, "\nRedirecting you to the main menu ...^");
    writeBytes = write(connFD, writeBuffer, sizeof(writeBuffer));
    readBytes = read(connFD, readBuffer, sizeof(read)); 
    return true;
}

int add_customer(int connFD, int newAccountNumber)
{
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];

    struct Customer newCustomer, previousCustomer;

    int customerFD = open(CUSTOMER_FILE, O_RDONLY);
    if (customerFD == -1 && errno == ENOENT)
    {
        newCustomer.id = 0;
    }
    else if (customerFD == -1)
    {
        perror("Error while opening customer file");
        return -1;
    }
    else
    {
        int offset = lseek(customerFD, -sizeof(struct Customer), SEEK_END);
        if (offset == -1)
        {
            perror("Error seeking to last Customer record!");
            return false;
        }

        struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Customer), getpid()};
        int lockingStatus = fcntl(customerFD, F_SETLKW, &lock);
        if (lockingStatus == -1)
        {
            perror("Error obtaining read lock on Customer record!");
            return false;
        }

        readBytes = read(customerFD, &previousCustomer, sizeof(struct Customer));
        if (readBytes == -1)
        {
            perror("Error while reading Customer record from file!");
            return false;
        }

        lock.l_type = F_UNLCK;
        fcntl(customerFD, F_SETLK, &lock);

        close(customerFD);

        newCustomer.id = previousCustomer.id + 1;
    }

        sprintf(writeBuffer, "%s%s", EMPLOYEE_ADD_CUSTOMER_PRIMARY, EMPLOYEE_ADD_CUSTOMER_NAME);
    
    writeBytes = write(connFD, writeBuffer, sizeof(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error writing EMPLOYEE_ADD_CUSTOMER_NAME message to client!");
        return false;
    }

    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading customer name response from client!");
        return false;
    }

    strcpy(newCustomer.name, readBuffer);

    writeBytes = write(connFD, EMPLOYEE_ADD_CUSTOMER_GENDER, strlen(EMPLOYEE_ADD_CUSTOMER_GENDER));
    if (writeBytes == -1)
    {
        perror("Error writing EMPLOYEE_ADD_CUSTOMER_GENDER message to client!");
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading customer gender response from client!");
        return false;
    }

    if (readBuffer[0] == 'M' || readBuffer[0] == 'F' || readBuffer[0] == 'O')
        newCustomer.gender = readBuffer[0];
    else
    {
        writeBytes = write(connFD, EMPLOYEE_ADD_CUSTOMER_WRONG_GENDER, strlen(EMPLOYEE_ADD_CUSTOMER_WRONG_GENDER));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); 
        return false;
    }

    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, EMPLOYEE_ADD_CUSTOMER_AGE);
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error writing EMPLOYEE_ADD_CUSTOMER_AGE message to client!");
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading customer age response from client!");
        return false;
    }

    int customerAge = atoi(readBuffer);
    if (customerAge == 0)
    {
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, ERRON_INPUT_FOR_NUMBER);
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing ERRON_INPUT_FOR_NUMBER message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); 
        return false;
    }
    newCustomer.age = customerAge;

    newCustomer.account = newAccountNumber;

    strcpy(newCustomer.login, newCustomer.name);
    strcat(newCustomer.login, "-");
    sprintf(writeBuffer, "%d", newCustomer.id);
    strcat(newCustomer.login, writeBuffer);

    char hashedPassword[1000];
    strcpy(hashedPassword, crypt(AUTOGEN_PASSWORD, SALT));
    strcpy(newCustomer.password, hashedPassword);

    customerFD = open(CUSTOMER_FILE, O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);
    if (customerFD == -1)
    {
        perror("Error while creating / opening customer file!");
        return false;
    }
    writeBytes = write(customerFD, &newCustomer, sizeof(newCustomer));
    if (writeBytes == -1)
    {
        perror("Error while writing Customer record to file!");
        return false;
    }

    close(customerFD);

    bzero(writeBuffer, sizeof(writeBuffer));
    sprintf(writeBuffer, "%s%s-%d\n%s%s", EMPLOYEE_ADD_CUSTOMER_AUTOGEN_LOGIN, newCustomer.name, newCustomer.id, EMPLOYEE_ADD_CUSTOMER_AUTOGEN_PASSWORD, AUTOGEN_PASSWORD);
    strcat(writeBuffer, "^");
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error sending customer loginID and password to the client!");
        return false;
    }

    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); 

    return newCustomer.id;
}

bool delete_account(int connFD)
{
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];

    struct Account account;

    writeBytes = write(connFD, EMPLOYEE_DEL_ACCOUNT_NO, strlen(EMPLOYEE_DEL_ACCOUNT_NO));
    if (writeBytes == -1)
    {
        perror("Error writing EMPLOYEE_DEL_ACCOUNT_NO to client!");
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading account number response from the client!");
        return false;
    }

    int accountNumber = atoi(readBuffer);

    int accountFD = open(ACCOUNT_FILE, O_RDONLY);
    if (accountFD == -1)
    {
        // Account record doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, ACCOUNT_ID_DOESNT_EXIT);
        strcat(writeBuffer, "^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing ACCOUNT_ID_DOESNT_EXIT message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); 
        return false;
    }


    int offset = lseek(accountFD, accountNumber * sizeof(struct Account), SEEK_SET);
    if (errno == EINVAL)
    {
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, ACCOUNT_ID_DOESNT_EXIT);
        strcat(writeBuffer, "^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing ACCOUNT_ID_DOESNT_EXIT message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); 
        return false;
    }
    else if (offset == -1)
    {
        perror("Error while seeking to required account record!");
        return false;
    }

    struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Account), getpid()};
    int lockingStatus = fcntl(accountFD, F_SETLKW, &lock);
    if (lockingStatus == -1)
    {
        perror("Error obtaining read lock on Account record!");
        return false;
    }

    readBytes = read(accountFD, &account, sizeof(struct Account));
    if (readBytes == -1)
    {
        perror("Error while reading Account record from file!");
        return false;
    }

    lock.l_type = F_UNLCK;
    fcntl(accountFD, F_SETLK, &lock);

    close(accountFD);

    bzero(writeBuffer, sizeof(writeBuffer));
    if (account.balance == 0)
    {
        account.active = false;
        accountFD = open(ACCOUNT_FILE, O_WRONLY);
        if (accountFD == -1)
        {
            perror("Error opening Account file in write mode!");
            return false;
        }

        offset = lseek(accountFD, accountNumber * sizeof(struct Account), SEEK_SET);
        if (offset == -1)
        {
            perror("Error seeking to the Account!");
            return false;
        }

        lock.l_type = F_WRLCK;
        lock.l_start = offset;

        int lockingStatus = fcntl(accountFD, F_SETLKW, &lock);
        if (lockingStatus == -1)
        {
            perror("Error obtaining write lock on the Account file!");
            return false;
        }

        writeBytes = write(accountFD, &account, sizeof(struct Account));
        if (writeBytes == -1)
        {
            perror("Error deleting account record!");
            return false;
        }

        lock.l_type = F_UNLCK;
        fcntl(accountFD, F_SETLK, &lock);

        strcpy(writeBuffer, EMPLOYEE_DEL_ACCOUNT_SUCCESS);
    }
    else
        strcpy(writeBuffer, EMPLOYEE_DEL_ACCOUNT_FAILURE);
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error while writing final DEL message to client!");
        return false;
    }
    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); 

    return true;
}

bool process_loan_application(int connFD) {
    char readBuffer[1000];
    struct Loan loan;

    write(connFD, "Enter loan ID to process: ", strlen("Enter loan ID to process: "));
    read(connFD, readBuffer, sizeof(readBuffer));
    int loanID = atoi(readBuffer);

    if (!get_loan_details(loanID, &loan)) {
        write(connFD, "Loan not found!\n", strlen("Loan not found!\n"));
        return false;
    }

    loan.status = PROCESSED;

    update_loan_in_file(&loan);

    write(connFD, "Loan application processed successfully.\n", strlen("Loan application processed successfully.\n"));
    return true;
}

bool approve_or_reject_loan(int connFD) {
    char readBuffer[1000];
    struct Loan loan;

    write(connFD, "Enter loan ID to approve/reject: ", strlen("Enter loan ID to approve/reject: "));
    read(connFD, readBuffer, sizeof(readBuffer));
    int loanID = atoi(readBuffer);

    if (!get_loan_details(loanID, &loan)) {
        write(connFD, "Loan not found!\n", strlen("Loan not found!\n"));
        return false;
    }

    write(connFD, "Approve (A) or Reject (R)?: ", strlen("Approve (A) or Reject (R)?: "));
    read(connFD, readBuffer, sizeof(readBuffer));

    if (readBuffer[0] == 'A') {
        loan.status = APPROVED;
        write(connFD, "Loan approved!\n", strlen("Loan approved!\n"));
    } else if (readBuffer[0] == 'R') {
        loan.status = REJECTED;
        write(connFD, "Loan rejected!\n", strlen("Loan rejected!\n"));
    } else {
        write(connFD, "Invalid input!\n", strlen("Invalid input!\n"));
        return false;
    }

    update_loan_in_file(&loan);

    return true;
}

bool view_assigned_loans(int connFD) {
    struct Loan loan;
    char writeBuffer[1000];

    int loanFD = open(LOAN_FILE, O_RDONLY);
    if (loanFD == -1) {
        perror("Error while opening loan file");
        return false;
    }

    while (read(loanFD, &loan, sizeof(loan)) > 0) {
        if (loan.employeeID == loggedInEmployee.id) {
            sprintf(writeBuffer, "Loan ID: %d, Customer ID: %d, Amount: %ld, Status: %d\n", loan.loanID, loan.customerID, loan.amount, loan.status);
            write(connFD, writeBuffer, strlen(writeBuffer));
        }
    }

    close(loanFD);
    return true;
}


#endif
