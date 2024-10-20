#ifndef TRANSACTIONS
#define TRANSACTIONS

#include <time.h>

struct Transaction
{
    int transactionID;
    int accountNumber;
    bool operation;
    long int oldBalance;
    long int newBalance;
    time_t transactionTime;
};

#endif
