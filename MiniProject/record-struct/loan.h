#ifndef LOAN
#define LOAN

struct Loan {
    int loanID;
    int customerID;
    int employeeID;
    long int amount;
    int status;  
    char loanType[20];
    int approvedAmount;
};

enum LoanStatus {
    PENDING = 0,
    PROCESSED = 1,
    APPROVED = 2,
    REJECTED = 3
};

#endif

