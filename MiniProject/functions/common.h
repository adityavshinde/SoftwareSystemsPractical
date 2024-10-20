#ifndef COMMON_FUNCTIONS
#define COMMON_FUNCTIONS

#include <stdio.h>     
#include <unistd.h>   
#include <string.h>   
#include <stdbool.h>   
#include <sys/types.h> 
#include <sys/stat.h>  
#include <fcntl.h>    
#include <stdlib.h>  
#include <errno.h>     

#include "../record-struct/account.h"
#include "../record-struct/customer.h"
#include "../record-struct/loan.h"
#include "../record-struct/employee.h"
#include "../record-struct/transaction.h"
#include "./admin-credentials.h"
#include "./manager-credentials.h"
#include "./server-constants.h"

// Function Declarations

bool login_handler(int isAdmin, int connFD, struct Customer *ptrToCustomer, struct Employee *ptrToEmployeeID);
bool get_account_details(int connFD, struct Account *customerAccount);
bool get_customer_details(int connFD, int customerID);
bool get_employee_details(int connFD, int employeeID);
bool get_loan_details(int loanID, struct Loan *loan);
bool update_loan_in_file(struct Loan *loan);

// Function Definition 

bool login_handler(int isAdmin, int connFD, struct Customer *ptrToCustomerID, struct Employee *ptrToEmployeeID)
{
    ssize_t readBytes, writeBytes;            
    char readBuffer[1000], writeBuffer[1000]; 
    char tempBuffer[1000];
    struct Customer customer;
    struct Employee employee;

    int ID;

    bzero(readBuffer, sizeof(readBuffer));
    bzero(writeBuffer, sizeof(writeBuffer));

    switch(isAdmin){
        case 1: 
        strcpy(writeBuffer, ADMIN_LOGIN_WELCOME);
        break;
        
        case 2:
        strcpy(writeBuffer, CUSTOMER_LOGIN_WELCOME);
        break;
        
        case 3:
        strcpy(writeBuffer, EMPLOYEE_LOGIN_WELCOME);
        break;
        
        case 4:
        strcpy(writeBuffer, MANAGER_LOGIN_WELCOME);
        break;
        
        default:
        break;  
    }

    strcat(writeBuffer, "\n");
    strcat(writeBuffer, LOGIN_ID);

    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error writing WELCOME & LOGIN_ID message to the client!");
        return false;
    }

    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading login ID from client!");
        return false;
    }

    bool userFound = false;

    switch(isAdmin)
    {
      case 1:
        if (strcmp(readBuffer, ADMIN_LOGIN_ID) == 0)
            userFound = true;
        break;
        
      case 2:
        bzero(tempBuffer, sizeof(tempBuffer));
        strcpy(tempBuffer, readBuffer);
        strtok(tempBuffer, "-");
        ID = atoi(strtok(NULL, "-"));

        int customerFileFD = open(CUSTOMER_FILE, O_RDONLY);
        if (customerFileFD == -1)
        {
            perror("Error opening customer file in read mode!");
            return false;
        }

        off_t offset = lseek(customerFileFD, ID * sizeof(struct Customer), SEEK_SET);
        if (offset >= 0)
        {
            struct flock lock = {F_RDLCK, SEEK_SET, ID * sizeof(struct Customer), sizeof(struct Customer), getpid()};

            int lockingStatus = fcntl(customerFileFD, F_SETLKW, &lock);
            if (lockingStatus == -1)
            {
                perror("Error obtaining read lock on customer record!");
                return false;
            }

            readBytes = read(customerFileFD, &customer, sizeof(struct Customer));
            if (readBytes == -1)
            {
                perror("Error reading customer record from file!");
            }

            lock.l_type = F_UNLCK;
            fcntl(customerFileFD, F_SETLK, &lock);

            if (strcmp(customer.login, readBuffer) == 0)
                userFound = true;

            close(customerFileFD);
        }
        else
        {
            writeBytes = write(connFD, CUSTOMER_LOGIN_ID_DOESNT_EXIT, strlen(CUSTOMER_LOGIN_ID_DOESNT_EXIT));
        }
        break;
        
      case 3:
        bzero(tempBuffer, sizeof(tempBuffer));
        strcpy(tempBuffer, readBuffer);
        strtok(tempBuffer, "-");
        ID = atoi(strtok(NULL, "-"));

        int employeeFileFD = open(EMPLOYEE_FILE, O_RDONLY);
        if (employeeFileFD == -1)
        {
            perror("Error opening employee file in read mode!");
            return false;
        }

        off_t offset1 = lseek(employeeFileFD, ID * sizeof(struct Employee), SEEK_SET);
        if (offset1 >= 0)
        {
            struct flock lock = {F_RDLCK, SEEK_SET, ID * sizeof(struct Employee), sizeof(struct Employee), getpid()};

            int lockingStatus = fcntl(employeeFileFD, F_SETLKW, &lock);
            if (lockingStatus == -1)
            {
                perror("Error obtaining read lock on employee record!");
                return false;
            }

            readBytes = read(employeeFileFD, &employee, sizeof(struct Employee));
            if (readBytes == -1)
            {
                perror("Error reading employee record from file!");
            }

            lock.l_type = F_UNLCK;
            fcntl(employeeFileFD, F_SETLK, &lock);

            if (strcmp(employee.login, readBuffer) == 0)
                userFound = true;

            close(employeeFileFD);
        }
        else
        {
            writeBytes = write(connFD, EMPLOYEE_LOGIN_ID_DOESNT_EXIT, strlen(EMPLOYEE_LOGIN_ID_DOESNT_EXIT));
        }
        break;
        
      case 4: 
      if (strcmp(readBuffer, MANAGER_LOGIN_ID) == 0)
            userFound = true;
      break;
      
      default: break;
    }

    if (userFound)
    {
        bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connFD, PASSWORD, strlen(PASSWORD));
        if (writeBytes == -1)
        {
            perror("Error writing PASSWORD message to client!");
            return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == 1)
        {
            perror("Error reading password from the client!");
            return false;
        }

        char hashedPassword[1000];
        strcpy(hashedPassword, crypt(readBuffer, SALT));

        switch(isAdmin)
        {
            case 1:
            if (strcmp(hashedPassword, ADMIN_PASSWORD) == 0)
                return true;
            break;
            case 2:
            if (strcmp(hashedPassword, customer.password) == 0)
            {
                *ptrToCustomerID = customer;
                return true;
            }
            break;
            case 3:
            if (strcmp(hashedPassword, employee.password) == 0)
            {
                *ptrToEmployeeID = employee;
                return true;
            }
            break;
            case 4:  
            if (strcmp(hashedPassword, MANAGER_PASSWORD) == 0)
            {
                return true;
            }
            break;
            default: break;
          }
      

        bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connFD, INVALID_PASSWORD, strlen(INVALID_PASSWORD));
    }
    else
    {
        bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connFD, INVALID_LOGIN, strlen(INVALID_LOGIN));
    }

    return false;
}

bool get_customer_details(int connFD, int customerID)
{
    ssize_t readBytes, writeBytes;            
    char readBuffer[1000], writeBuffer[10000]; 
    char tempBuffer[1000];

    struct Customer customer;
    int customerFD;
    struct flock lock = {F_RDLCK, SEEK_SET, 0, sizeof(struct Account), getpid()};

    if (customerID == -1)
    {
        writeBytes = write(connFD, GET_CUSTOMER_ID, strlen(GET_CUSTOMER_ID));
        if (writeBytes == -1)
        {
            perror("Error while writing GET_CUSTOMER_ID message to client!");
            return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error getting customer ID from client!");
            ;
            return false;
        }

        customerID = atoi(readBuffer);
    }

    customerFD = open(CUSTOMER_FILE, O_RDONLY);
    if (customerFD == -1)
    {
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, CUSTOMER_ID_DOESNT_EXIT);
        strcat(writeBuffer, "^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing CUSTOMER_ID_DOESNT_EXIT message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); 
        return false;
    }
    int offset = lseek(customerFD, customerID * sizeof(struct Customer), SEEK_SET);
    if (errno == EINVAL)
    {
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, CUSTOMER_ID_DOESNT_EXIT);
        strcat(writeBuffer, "^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing CUSTOMER_ID_DOESNT_EXIT message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        return false;
    }
    else if (offset == -1)
    {
        perror("Error while seeking to required customer record!");
        return false;
    }
    lock.l_start = offset;

    int lockingStatus = fcntl(customerFD, F_SETLKW, &lock);
    if (lockingStatus == -1)
    {
        perror("Error while obtaining read lock on the Customer file!");
        return false;
    }

    readBytes = read(customerFD, &customer, sizeof(struct Customer));
    if (readBytes == -1)
    {
        perror("Error reading customer record from file!");
        return false;
    }

    lock.l_type = F_UNLCK;
    fcntl(customerFD, F_SETLK, &lock);

    bzero(writeBuffer, sizeof(writeBuffer));
    sprintf(writeBuffer, "Customer Details - \n\tID : %d\n\tName : %s\n\tGender : %c\n\tAge: %d\n\tAccount Number : %d\n\tLoginID : %s", customer.id, customer.name, customer.gender, customer.age, customer.account, customer.login);

    strcat(writeBuffer, "\n\nYou'll now be redirected to the main menu...^");

    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error writing customer info to client!");
        return false;
    }

    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); 
    return true;
}

bool get_employee_details(int connFD, int employeeID)
{
    ssize_t readBytes, writeBytes;          
    char readBuffer[1000], writeBuffer[10000]; 
    char tempBuffer[1000];

    struct Employee employee;
    int employeeFD;
    struct flock lock = {F_RDLCK, SEEK_SET, 0, sizeof(struct Employee), getpid()};

    if (employeeID == -1)
    {
        writeBytes = write(connFD, GET_EMPLOYEE_ID, strlen(GET_EMPLOYEE_ID));
        if (writeBytes == -1)
        {
            perror("Error while writing GET_EMPLOYEE_ID message to client!");
            return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error getting employee ID from client!");
            ;
            return false;
        }

        employeeID = atoi(readBuffer);
    }

    employeeFD = open(EMPLOYEE_FILE, O_RDONLY);
    if (employeeFD == -1)
    {
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, EMPLOYEE_ID_DOESNT_EXIT);
        strcat(writeBuffer, "^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing EMPLOYEE_ID_DOESNT_EXIT message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); 
        return false;
    }
    int offset = lseek(employeeFD, employeeID * sizeof(struct Employee), SEEK_SET);
    if (errno == EINVAL)
    {
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, EMPLOYEE_ID_DOESNT_EXIT);
        strcat(writeBuffer, "^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing EMPLOYEE_ID_DOESNT_EXIT message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); 
        return false;
    }
    else if (offset == -1)
    {
        perror("Error while seeking to required employee record!");
        return false;
    }
    lock.l_start = offset;

    int lockingStatus = fcntl(employeeFD, F_SETLKW, &lock);
    if (lockingStatus == -1)
    {
        perror("Error while obtaining read lock on the Employee file!");
        return false;
    }

    readBytes = read(employeeFD, &employee, sizeof(struct Employee));
    if (readBytes == -1)
    {
        perror("Error reading employee record from file!");
        return false;
    }

    lock.l_type = F_UNLCK;
    fcntl(employeeFD, F_SETLK, &lock);

    bzero(writeBuffer, sizeof(writeBuffer));
    sprintf(writeBuffer, "Employee Details - \n\tID : %d\n\tName : %s\n\tGender : %c\n\tAge: %d\n\tEmployee Number : %d\n\tLoginID : %s", employee.id, employee.name, employee.gender, employee.age, employee.empno, employee.login);

    strcat(writeBuffer, "\n\nYou'll now be redirected to the main menu...^");

    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error writing employee info to client!");
        return false;
    }

    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); 
    return true;
}


bool get_transaction_details(int connFD, int accountNumber)
{

    ssize_t readBytes, writeBytes;                             
    char readBuffer[1000], writeBuffer[10000], tempBuffer[1000];

    struct Account account;

    if (accountNumber == -1)
    {
        writeBytes = write(connFD, GET_ACCOUNT_NUMBER, strlen(GET_ACCOUNT_NUMBER));
        if (writeBytes == -1)
        {
            perror("Error writing GET_ACCOUNT_NUMBER message to client!");
            return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error reading account number response from client!");
            return false;
        }

        account.accountNumber = atoi(readBuffer);
    }
    else
        account.accountNumber = accountNumber;

    if (get_account_details(connFD, &account))
    {
        int iter;

        struct Transaction transaction;
        struct tm transactionTime;

        bzero(writeBuffer, sizeof(readBuffer));

        int transactionFD = open(TRANSACTION_FILE, O_RDONLY);
        if (transactionFD == -1)
        {
            perror("Error while opening transaction file!");
            write(connFD, TRANSACTIONS_NOT_FOUND, strlen(TRANSACTIONS_NOT_FOUND));
            read(connFD, readBuffer, sizeof(readBuffer)); 
            return false;
        }

        for (iter = 0; iter < MAX_TRANSACTIONS && account.transactions[iter] != -1; iter++)
        {

            int offset = lseek(transactionFD, account.transactions[iter] * sizeof(struct Transaction), SEEK_SET);
            if (offset == -1)
            {
                perror("Error while seeking to required transaction record!");
                return false;
            }

            struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Transaction), getpid()};

            int lockingStatus = fcntl(transactionFD, F_SETLKW, &lock);
            if (lockingStatus == -1)
            {
                perror("Error obtaining read lock on transaction record!");
                return false;
            }

            readBytes = read(transactionFD, &transaction, sizeof(struct Transaction));
            if (readBytes == -1)
            {
                perror("Error reading transaction record from file!");
                return false;
            }

            lock.l_type = F_UNLCK;
            fcntl(transactionFD, F_SETLK, &lock);

            transactionTime = *localtime(&(transaction.transactionTime));

            bzero(tempBuffer, sizeof(tempBuffer));
            sprintf(tempBuffer, "Details of transaction %d - \n\t Date : %d:%d %d/%d/%d \n\t Operation : %s \n\t Balance - \n\t\t Before : %ld \n\t\t After : %ld \n\t\t Difference : %ld\n", (iter + 1), transactionTime.tm_hour, transactionTime.tm_min, transactionTime.tm_mday, (transactionTime.tm_mon + 1), (transactionTime.tm_year + 1900), (transaction.operation ? "Deposit" : "Withdraw"), transaction.oldBalance, transaction.newBalance, (transaction.newBalance - transaction.oldBalance));

            if (strlen(writeBuffer) == 0)
                strcpy(writeBuffer, tempBuffer);
            else
                strcat(writeBuffer, tempBuffer);
        }

        close(transactionFD);

        if (strlen(writeBuffer) == 0)
        {
            write(connFD, TRANSACTIONS_NOT_FOUND, strlen(TRANSACTIONS_NOT_FOUND));
            read(connFD, readBuffer, sizeof(readBuffer)); 
            return false;
        }
        else
        {
            strcat(writeBuffer, "^");
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            read(connFD, readBuffer, sizeof(readBuffer)); 
        }
    }
}

bool get_account_details(int connFD, struct Account *customerAccount)
{
    ssize_t readBytes, writeBytes;           
    char readBuffer[1000], writeBuffer[1000]; 
    char tempBuffer[1000];

    int accountNumber;
    struct Account account;
    int accountFD;

    if (customerAccount == NULL)
    {

        writeBytes = write(connFD, GET_ACCOUNT_NUMBER, strlen(GET_ACCOUNT_NUMBER));
        if (writeBytes == -1)
        {
            perror("Error writing GET_ACCOUNT_NUMBER message to client!");
            return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error reading account number response from client!");
            return false;
        }

        accountNumber = atoi(readBuffer);
    }
    else
        accountNumber = customerAccount->accountNumber;

    accountFD = open(ACCOUNT_FILE, O_RDONLY);
    if (accountFD == -1)
    {
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, ACCOUNT_ID_DOESNT_EXIT);
        strcat(writeBuffer, "^");
        perror("Error opening account file in get_account_details!");
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
    if (offset == -1 && errno == EINVAL)
    {
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, ACCOUNT_ID_DOESNT_EXIT);
        strcat(writeBuffer, "^");
        perror("Error seeking to account record in get_account_details!");
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
        perror("Error obtaining read lock on account record!");
        return false;
    }

    readBytes = read(accountFD, &account, sizeof(struct Account));
    if (readBytes == -1)
    {
        perror("Error reading account record from file!");
        return false;
    }

    lock.l_type = F_UNLCK;
    fcntl(accountFD, F_SETLK, &lock);

    if (customerAccount != NULL)
    {
        *customerAccount = account;
        return true;
    }

    bzero(writeBuffer, sizeof(writeBuffer));
    sprintf(writeBuffer, "Account Details - \n\tAccount Number : %d\n\tAccount Type : %s\n\tAccount Status : %s", account.accountNumber, "Regular" , (account.active) ? "Active" : "Deactived");
    if (account.active)
    {
        sprintf(tempBuffer, "\n\tAccount Balance:₹ %ld", account.balance);
        strcat(writeBuffer, tempBuffer);
    }

    sprintf(tempBuffer, "\n\tPrimary Owner ID: %d", account.owners[0]);
    strcat(writeBuffer, tempBuffer);
    strcat(writeBuffer, "\n^");

    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); 

    return true;
}

bool get_loan_details(int loanID, struct Loan *loan) {
    int loanFD = open(LOAN_FILE, O_RDONLY);
    if (loanFD == -1) {
        perror("Error opening loan file");
        return false;
    }

    off_t offset = loanID * sizeof(struct Loan);
    if (lseek(loanFD, offset, SEEK_SET) == -1) {
        perror("Error seeking to loan in file");
        close(loanFD);
        return false;
    }

    ssize_t readBytes = read(loanFD, loan, sizeof(struct Loan));
    if (readBytes == -1 || readBytes < sizeof(struct Loan)) {
        perror("Error reading loan details");
        close(loanFD);
        return false;
    }

    close(loanFD);
    return true;
}

bool update_loan_in_file(struct Loan *loan) {
    int loanFD = open(LOAN_FILE, O_WRONLY);
    if (loanFD == -1) {
        perror("Error opening loan file for updating");
        return false;
    }

    off_t offset = loan->loanID * sizeof(struct Loan);
    if (lseek(loanFD, offset, SEEK_SET) == -1) {
        perror("Error seeking to loan record in file");
        close(loanFD);
        return false;
    }

    ssize_t writeBytes = write(loanFD, loan, sizeof(struct Loan));
    if (writeBytes == -1 || writeBytes < sizeof(struct Loan)) {
        perror("Error writing loan details");
        close(loanFD);
        return false;
    }

    close(loanFD);
    return true;
}


#endif
