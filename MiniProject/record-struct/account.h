#ifndef ACCOUNT_RECORD
#define ACCOUNT_RECORD

#define MAX_TRANSACTIONS 10

struct Account
{
    int accountNumber;     
    int owners[2];     
    bool isRegularAccount; 
    bool active;        
    long int balance;     
    int transactions[MAX_TRANSACTIONS];  
};

#endif
