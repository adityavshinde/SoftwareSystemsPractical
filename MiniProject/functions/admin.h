#ifndef ADMIN_FUNCTIONS
#define ADMIN_FUNCTIONS

#include "./common.h"

// Function Declarations

bool admin_operation_handler(int connFD);
int add_employee(int connFD);
bool delete_employee(int connFD);
bool modify_customer_info(int connFD);
bool modify_employee_info(int connFD);


// Function Definitions

bool admin_operation_handler(int connFD)
{

    if (login_handler(1, connFD, NULL, NULL))
    {
        ssize_t writeBytes, readBytes;            
        char readBuffer[1000], writeBuffer[1000]; 
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, ADMIN_LOGIN_SUCCESS);
        while (1)
        {
            strcat(writeBuffer, "\n");
            strcat(writeBuffer, ADMIN_MENU);
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1)
            {
                perror("Error while writing ADMIN_MENU to client!");
                return false;
            }
            bzero(writeBuffer, sizeof(writeBuffer));

            readBytes = read(connFD, readBuffer, sizeof(readBuffer));
            if (readBytes == -1)
            {
                perror("Error while reading client's choice for ADMIN_MENU");
                return false;
            }

            int choice = atoi(readBuffer);
            switch (choice)
            {
            case 1:
                add_employee(connFD);
                break;
            case 2:
                modify_employee_info(connFD);
                break;
            case 3: 
                modify_customer_info(connFD);
                break;
            case 4:
                delete_employee(connFD);
                break;
            default:
                writeBytes = write(connFD, ADMIN_LOGOUT, strlen(ADMIN_LOGOUT));
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
int add_employee(int connFD) {
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];
    struct Employee newEmployee, previousEmployee;

    int employeeFD = open(EMPLOYEE_FILE, O_RDONLY);
    
    if (employeeFD == -1 && errno == ENOENT) {
        newEmployee.id = 0;
    } 
    else if (employeeFD == -1) {
        perror("Error while opening employee file");
        return false;
    } 
    else {
        int offset = lseek(employeeFD, -sizeof(struct Employee), SEEK_END);
        if (offset == -1) {
            perror("Error seeking to last Employee record!");
            return false;
        }

        struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Employee), getpid()};
        int lockingStatus = fcntl(employeeFD, F_SETLKW, &lock);
        if (lockingStatus == -1) {
            perror("Error obtaining read lock on Employee record!");
            return false;
        }

        readBytes = read(employeeFD, &previousEmployee, sizeof(struct Employee));
        if (readBytes == -1) {
            perror("Error while reading Employee record from file!");
            return false;
        }

        lock.l_type = F_UNLCK;
        fcntl(employeeFD, F_SETLK, &lock);
        close(employeeFD);

        newEmployee.id = previousEmployee.id + 1;
    }

    sprintf(writeBuffer, "%s%s", ADMIN_ADD_EMPLOYEE, ADMIN_ADD_EMPLOYEE_NAME);
    
    writeBytes = write(connFD, writeBuffer, sizeof(writeBuffer));
    if (writeBytes == -1) {
        perror("Error writing ADMIN_ADD_EMPLOYEE_NAME message to client");
        return false;
    }

    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1) {
        perror("Error reading employee name from client");
        return false;
    }
    strcpy(newEmployee.name, readBuffer);

    writeBytes = write(connFD, ADMIN_ADD_EMPLOYEE_GENDER, strlen(ADMIN_ADD_EMPLOYEE_GENDER));
    if (writeBytes == -1) {
        perror("Error writing ADMIN_ADD_EMPLOYEE_GENDER message to client!");
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1) {
        perror("Error reading employee gender response from client!");
        return false;
    }

    if (readBuffer[0] == 'M' || readBuffer[0] == 'F' || readBuffer[0] == 'O') {
        newEmployee.gender = readBuffer[0];
    } else {
        writeBytes = write(connFD, ADMIN_ADD_EMPLOYEE_WRONG_GENDER, strlen(ADMIN_ADD_EMPLOYEE_WRONG_GENDER));
        return false;
    }

    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, ADMIN_ADD_EMPLOYEE_AGE);
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1) {
        perror("Error writing ADMIN_ADD_EMPLOYEE_AGE message to client!");
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1) {
        perror("Error reading employee age response from client!");
        return false;
    }

    newEmployee.age = atoi(readBuffer);
    if (newEmployee.age <= 0) {
        writeBytes = write(connFD, ERRON_INPUT_FOR_NUMBER , strlen(ERRON_INPUT_FOR_NUMBER ));
        return false;
    }

    strcpy(newEmployee.login, newEmployee.name);
    strcat(newEmployee.login, "-");
    char idBuffer[10];
    sprintf(idBuffer, "%d", newEmployee.id);
    strcat(newEmployee.login, idBuffer);

    newEmployee.empno = newEmployee.id + 1000;
    
    char hashedPassword[1000];
    strcpy(hashedPassword, crypt(AUTOGEN_PASSWORD, SALT));
    strcpy(newEmployee.password, hashedPassword);

    employeeFD = open(EMPLOYEE_FILE, O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);
    if (employeeFD == -1) {
        perror("Error while creating / opening employee file");
        return false;
    }

    writeBytes = write(employeeFD, &newEmployee, sizeof(newEmployee));
    if (writeBytes == -1) {
        perror("Error while writing Employee record to file");
        return false;
    }

    close(employeeFD);

    bzero(writeBuffer, sizeof(writeBuffer));
    sprintf(writeBuffer, "Employee Login: %s\nPassword: %s\n", newEmployee.login, AUTOGEN_PASSWORD);
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1) {
        perror("Error sending login credentials");
        return false;
    }
    sprintf(writeBuffer, "%s%s-%d\n%s%s", ADMIN_ADD_EMPLOYEE_AUTOGEN_LOGIN, newEmployee.name, newEmployee.id, ADMIN_ADD_EMPLOYEE_AUTOGEN_PASSWORD, AUTOGEN_PASSWORD);
    strcat(writeBuffer, "^");
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error sending employee loginID and password to the client!");
        return false;
    }
    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); 

    return newEmployee.id;
}

bool delete_employee(int connFD) {
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];
    int employeeID;
    struct Employee employee;

    writeBytes = write(connFD, ADMIN_DEL_EMPLOYEE_NO, strlen(ADMIN_DEL_EMPLOYEE_NO));
    if (writeBytes == -1) {
        perror("Error writing ADMIN_DEL_EMPLOYEE_NO to client");
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1) {
        perror("Error reading employee ID from the client");
        return false;
    }

    employeeID = atoi(readBuffer);
    if (employeeID < 0) {
        writeBytes = write(connFD, EMPLOYEE_ID_DOESNT_EXIT, strlen(EMPLOYEE_ID_DOESNT_EXIT));
        return false;
    }

    int employeeFD = open(EMPLOYEE_FILE, O_RDWR);
    if (employeeFD == -1) {
        perror("Error opening employee file");
        return false;
    }

    bool employeeFound = false;

    while ((readBytes = read(employeeFD, &employee, sizeof(struct Employee))) > 0) {
        if (employee.id == employeeID) {
            employeeFound = true;
            break;
        }
    }

    if (!employeeFound) {
        writeBytes = write(connFD, "Employee not found or already deleted", strlen("Employee not found or already deleted"));
        close(employeeFD);
        return false;
    }


    int offset = lseek(employeeFD, -sizeof(struct Employee), SEEK_CUR);
    if (offset == -1) {
        perror("Error seeking to employee record");
        close(employeeFD);
        return false;
    }

    struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(struct Employee), getpid()};
    int lockingStatus = fcntl(employeeFD, F_SETLKW, &lock);
    if (lockingStatus == -1) {
        perror("Error obtaining write lock on Employee record");
        close(employeeFD);
        return false;
    }

    writeBytes = write(employeeFD, &employee, sizeof(struct Employee));
    if (writeBytes == -1) {
        perror("Error updating Employee record");
        close(employeeFD);
        return false;
    }

    lock.l_type = F_UNLCK;
    fcntl(employeeFD, F_SETLK, &lock);

    close(employeeFD);

    writeBytes = write(connFD, "Employee deleted successfully", strlen("Employee deleted successfully"));
    if (writeBytes == -1) {
        perror("Error sending success message");
        return false;
    }

    return true;
}

bool modify_customer_info(int connFD)
{
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];

    struct Customer customer;

    int customerID;

    off_t offset;
    int lockingStatus;

    writeBytes = write(connFD, ADMIN_MOD_CUSTOMER_ID, strlen(ADMIN_MOD_CUSTOMER_ID));
    if (writeBytes == -1)
    {
        perror("Error while writing ADMIN_MOD_CUSTOMER_ID message to client!");
        return false;
    }
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error while reading customer ID from client!");
        return false;
    }

    customerID = atoi(readBuffer);

    int customerFD = open(CUSTOMER_FILE, O_RDONLY);
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
    
    offset = lseek(customerFD, customerID * sizeof(struct Customer), SEEK_SET);
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

    struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Customer), getpid()};

    lockingStatus = fcntl(customerFD, F_SETLKW, &lock);
    if (lockingStatus == -1)
    {
        perror("Couldn't obtain lock on customer record!");
        return false;
    }

    readBytes = read(customerFD, &customer, sizeof(struct Customer));
    if (readBytes == -1)
    {
        perror("Error while reading customer record from the file!");
        return false;
    }

    lock.l_type = F_UNLCK;
    fcntl(customerFD, F_SETLK, &lock);

    close(customerFD);

    writeBytes = write(connFD, ADMIN_MOD_CUSTOMER_MENU, strlen(ADMIN_MOD_CUSTOMER_MENU));
    if (writeBytes == -1)
    {
        perror("Error while writing ADMIN_MOD_CUSTOMER_MENU message to client!");
        return false;
    }
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error while getting customer modification menu choice from client!");
        return false;
    }

    int choice = atoi(readBuffer);
    if (choice == 0)
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

    bzero(readBuffer, sizeof(readBuffer));
    switch (choice)
    {
    case 1:
        writeBytes = write(connFD, ADMIN_MOD_CUSTOMER_NEW_NAME, strlen(ADMIN_MOD_CUSTOMER_NEW_NAME));
        if (writeBytes == -1)
        {
            perror("Error while writing ADMIN_MOD_CUSTOMER_NEW_NAME message to client!");
            return false;
        }
        readBytes = read(connFD, &readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error while getting response for customer's new name from client!");
            return false;
        }
        strcpy(customer.name, readBuffer);
        break;
    case 2:
        writeBytes = write(connFD, ADMIN_MOD_CUSTOMER_NEW_AGE, strlen(ADMIN_MOD_CUSTOMER_NEW_AGE));
        if (writeBytes == -1)
        {
            perror("Error while writing ADMIN_MOD_CUSTOMER_NEW_AGE message to client!");
            return false;
        }
        readBytes = read(connFD, &readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error while getting response for customer's new age from client!");
            return false;
        }
        int updatedAge = atoi(readBuffer);
        if (updatedAge == 0)
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
        customer.age = updatedAge;
        break;
    case 3:
        writeBytes = write(connFD, ADMIN_MOD_CUSTOMER_NEW_GENDER, strlen(ADMIN_MOD_CUSTOMER_NEW_GENDER));
        if (writeBytes == -1)
        {
            perror("Error while writing ADMIN_MOD_CUSTOMER_NEW_GENDER message to client!");
            return false;
        }
        readBytes = read(connFD, &readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error while getting response for customer's new gender from client!");
            return false;
        }
        customer.gender = readBuffer[0];
        break;
    default:
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, INVALID_MENU_CHOICE);
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing INVALID_MENU_CHOICE message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); 
        return false;
    }

    customerFD = open(CUSTOMER_FILE, O_WRONLY);
    if (customerFD == -1)
    {
        perror("Error while opening customer file");
        return false;
    }
    offset = lseek(customerFD, customerID * sizeof(struct Customer), SEEK_SET);
    if (offset == -1)
    {
        perror("Error while seeking to required customer record!");
        return false;
    }

    lock.l_type = F_WRLCK;
    lock.l_start = offset;
    lockingStatus = fcntl(customerFD, F_SETLKW, &lock);
    if (lockingStatus == -1)
    {
        perror("Error while obtaining write lock on customer record!");
        return false;
    }

    writeBytes = write(customerFD, &customer, sizeof(struct Customer));
    if (writeBytes == -1)
    {
        perror("Error while writing update customer info into file");
    }

    lock.l_type = F_UNLCK;
    fcntl(customerFD, F_SETLKW, &lock);

    close(customerFD);

    writeBytes = write(connFD, ADMIN_MOD_CUSTOMER_SUCCESS, strlen(ADMIN_MOD_CUSTOMER_SUCCESS));
    if (writeBytes == -1)
    {
        perror("Error while writing ADMIN_MOD_CUSTOMER_SUCCESS message to client!");
        return false;
    }
    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); 

    return true;
}

bool modify_employee_info(int connFD)
{
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];

    struct Employee employee;

    int employeeID;

    off_t offset;
    int lockingStatus;

    writeBytes = write(connFD, ADMIN_MOD_EMPLOYEE_ID, strlen(ADMIN_MOD_EMPLOYEE_ID));
    if (writeBytes == -1)
    {
        perror("Error while writing ADMIN_MOD_EMPLOYEE_ID message to client!");
        return false;
    }
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error while reading employee ID from client!");
        return false;
    }

    employeeID = atoi(readBuffer);

    int employeeFD = open(EMPLOYEE_FILE, O_RDONLY);
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
    
    offset = lseek(employeeFD, employeeID * sizeof(struct Employee), SEEK_SET);
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

    struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Employee), getpid()};

    lockingStatus = fcntl(employeeFD, F_SETLKW, &lock);
    if (lockingStatus == -1)
    {
        perror("Couldn't obtain lock on employee record!");
        return false;
    }

    readBytes = read(employeeFD, &employee, sizeof(struct Employee));
    if (readBytes == -1)
    {
        perror("Error while reading employee record from the file!");
        return false;
    }

    lock.l_type = F_UNLCK;
    fcntl(employeeFD, F_SETLK, &lock);

    close(employeeFD);

    writeBytes = write(connFD, ADMIN_MOD_EMPLOYEE_MENU, strlen(ADMIN_MOD_EMPLOYEE_MENU));
    if (writeBytes == -1)
    {
        perror("Error while writing ADMIN_MOD_EMPLOYEE_MENU message to client!");
        return false;
    }
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error while getting employee modification menu choice from client!");
        return false;
    }

    int choice = atoi(readBuffer);
    if (choice == 0)
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

    bzero(readBuffer, sizeof(readBuffer));
    switch (choice)
    {
    case 1:
        writeBytes = write(connFD, ADMIN_MOD_EMPLOYEE_NEW_NAME, strlen(ADMIN_MOD_EMPLOYEE_NEW_NAME));
        if (writeBytes == -1)
        {
            perror("Error while writing ADMIN_MOD_EMPLOYEE_NEW_NAME message to client!");
            return false;
        }
        readBytes = read(connFD, &readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error while getting response for employee's new name from client!");
            return false;
        }
        strcpy(employee.name, readBuffer);
        break;
    case 2:
        writeBytes = write(connFD, ADMIN_MOD_EMPLOYEE_NEW_AGE, strlen(ADMIN_MOD_EMPLOYEE_NEW_AGE));
        if (writeBytes == -1)
        {
            perror("Error while writing ADMIN_MOD_EMPLOYEE_NEW_AGE message to client!");
            return false;
        }
        readBytes = read(connFD, &readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error while getting response for employee's new age from client!");
            return false;
        }
        int updatedAge = atoi(readBuffer);
        if (updatedAge == 0)
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
        employee.age = updatedAge;
        break;
    case 3:
        writeBytes = write(connFD, ADMIN_MOD_EMPLOYEE_NEW_GENDER, strlen(ADMIN_MOD_EMPLOYEE_NEW_GENDER));
        if (writeBytes == -1)
        {
            perror("Error while writing ADMIN_MOD_EMPLOYEE_NEW_GENDER message to client!");
            return false;
        }
        readBytes = read(connFD, &readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error while getting response for employee's new gender from client!");
            return false;
        }
        employee.gender = readBuffer[0];
        break;
    default:
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, INVALID_MENU_CHOICE);
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing INVALID_MENU_CHOICE message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); 
        return false;
    }

    employeeFD = open(EMPLOYEE_FILE, O_WRONLY);
    if (employeeFD == -1)
    {
        perror("Error while opening employee file");
        return false;
    }
    offset = lseek(employeeFD, employeeID * sizeof(struct Employee), SEEK_SET);
    if (offset == -1)
    {
        perror("Error while seeking to required employee record!");
        return false;
    }

    lock.l_type = F_WRLCK;
    lock.l_start = offset;
    lockingStatus = fcntl(employeeFD, F_SETLKW, &lock);
    if (lockingStatus == -1)
    {
        perror("Error while obtaining write lock on employee record!");
        return false;
    }

    writeBytes = write(employeeFD, &employee, sizeof(struct Employee));
    if (writeBytes == -1)
    {
        perror("Error while writing update employee info into file");
    }

    lock.l_type = F_UNLCK;
    fcntl(employeeFD, F_SETLKW, &lock);

    close(employeeFD);

    writeBytes = write(connFD, ADMIN_MOD_EMPLOYEE_SUCCESS, strlen(ADMIN_MOD_EMPLOYEE_SUCCESS));
    if (writeBytes == -1)
    {
        perror("Error while writing ADMIN_MOD_EMPLOYEE_SUCCESS message to client!");
        return false;
    }
    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); 

    return true;
}
#endif

