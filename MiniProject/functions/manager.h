#ifndef MANAGER_FUNCTIONS
#define  MANAGER_FUNCTIONS

#include "./common.h"

#include <sys/ipc.h>
#include <sys/sem.h>

// Function Declarations
bool manager_operation_handler(int connFD);
bool activate_deactivate_account(int connFD );
bool assign_loan_to_employee(int connFD);
//bool review_feedback();

// Function Definitions

bool manager_operation_handler(int connFD)
{

    if (login_handler(4, connFD, NULL, NULL))
    {
        ssize_t writeBytes, readBytes;            
        char readBuffer[1000], writeBuffer[1000]; 
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, MANAGER_LOGIN_SUCCESS);
        while (1)
        {
            strcat(writeBuffer, "\n");
            strcat(writeBuffer, MANAGER_MENU);
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1)
            {
                perror("Error while writing MANAGER_MENU to client!");
                return false;
            }
            bzero(writeBuffer, sizeof(writeBuffer));

            readBytes = read(connFD, readBuffer, sizeof(readBuffer));
            if (readBytes == -1)
            {
                perror("Error while reading client's choice for MANAGER_MENU");
                return false;
            }

            int choice = atoi(readBuffer);
            switch (choice)
            {
            case 1:
                activate_deactivate_account(connFD);
                break;
            case 2:
                assign_loan_to_employee(connFD);
                break;
            case 3: 
                //review_feedback(connFD);
                printf("This functionality is under process\n");
                break;
            default:
                writeBytes = write(connFD, MANAGER_LOGOUT, strlen(MANAGER_LOGOUT));
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

bool activate_deactivate_account(int connFD) {
    char readBuffer[1000], writeBuffer[1000];
    ssize_t readBytes, writeBytes;
    
    struct Account account;
    int accountNumber;

    writeBytes = write(connFD, "Enter Account Number: ", strlen("Enter Account Number: "));
    if (writeBytes == -1) {
        perror("Error writing account number request to client!");
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1) {
        perror("Error reading account number from client!");
        return false;
    }

    accountNumber = atoi(readBuffer); 
    account.accountNumber = accountNumber;

    if (!get_account_details(connFD, &account)) {
        writeBytes = write(connFD, "Account not found!\n", strlen("Account not found!\n"));
        return false;
    }

    writeBytes = write(connFD, "Do you want to (1) Activate or (2) Deactivate the account? Enter 1 or 2: ", strlen("Do you want to (1) Activate or (2) Deactivate the account? Enter 1 or 2: "));
    if (writeBytes == -1) {
        perror("Error writing activate/deactivate request to client!");
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1) {
        perror("Error reading activate/deactivate response from client!");
        return false;
    }

    int choice = atoi(readBuffer); 

    struct sembuf semOp;
    lock_critical_section(&semOp);

    if (choice == 1) {
        if (account.active) {
            writeBytes = write(connFD, "Account is already active.\n", strlen("Account is already active.\n"));
        } else {
            account.active = true;
            writeBytes = write(connFD, "Account activated successfully.\n", strlen("Account activated successfully.\n"));
        }
    } else if (choice == 2) {
        if (!account.active) {
            writeBytes = write(connFD, "Account is already deactivated.\n", strlen("Account is already deactivated.\n"));
        } else {
            account.active = false;
            writeBytes = write(connFD, "Account deactivated successfully.\n", strlen("Account deactivated successfully.\n"));
        }
    } else {
        writeBytes = write(connFD, "Invalid choice. Please enter 1 or 2.\n", strlen("Invalid choice. Please enter 1 or 2.\n"));
        unlock_critical_section(&semOp);
        return false;
    }

    int accountFD = open(ACCOUNT_FILE, O_WRONLY);
    if (accountFD == -1) {
        perror("Error opening account file for updating status!");
        unlock_critical_section(&semOp);
        return false;
    }

    off_t offset = lseek(accountFD, account.accountNumber * sizeof(struct Account), SEEK_SET);
    struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(struct Account), getpid()};
    int lockingStatus = fcntl(accountFD, F_SETLKW, &lock);
    if (lockingStatus == -1) {
        perror("Error obtaining write lock on account record!");
        close(accountFD);
        unlock_critical_section(&semOp);
        return false;
    }

    writeBytes = write(accountFD, &account, sizeof(struct Account));
    if (writeBytes == -1) {
        perror("Error updating account status!");
        close(accountFD);
        unlock_critical_section(&semOp);
        return false;
    }

    lock.l_type = F_UNLCK;
    fcntl(accountFD, F_SETLK, &lock);
    close(accountFD);

    unlock_critical_section(&semOp);

    return true; 
}

bool assign_loan_to_employee(int connFD) {
    char readBuffer[1000];
    struct Loan loan;
    int employeeID;

    write(connFD, "Enter loan ID to assign: ", strlen("Enter loan ID to assign: "));
    read(connFD, readBuffer, sizeof(readBuffer));
    int loanID = atoi(readBuffer);

    if (!get_loan_details(loanID, &loan)) {
        write(connFD, "Loan not found!\n", strlen("Loan not found!\n"));
        return false;
    }

    write(connFD, "Enter employee ID to assign loan: ", strlen("Enter employee ID to assign loan: "));
    read(connFD, readBuffer, sizeof(readBuffer));
    employeeID = atoi(readBuffer);

    loan.employeeID = employeeID;

    update_loan_in_file(&loan);

    write(connFD, "Loan assigned to employee successfully.\n", strlen("Loan assigned to employee successfully.\n"));
    return true;
}

#endif
