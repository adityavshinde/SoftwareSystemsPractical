/*
============================================================================
Name : 18.c
Author : Aditya Vijay Shinde
Description : Write a program to perform Record locking.
a. Implement write lock
b. Implement read lock
Create three records in a file. Whenever you access a particular record, first lock it then modify/access to avoid race condition.
Date: 29th Aug, 2024.
============================================================================
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define RECORD_SIZE 50  // Fixed record size (in bytes)

void apply_lock(int fd, short lock_type, int record_number) {
    struct flock lock;
    lock.l_type = lock_type;             // Lock type: F_WRLCK (write) or F_RDLCK (read)
    lock.l_whence = SEEK_SET;            // Start from the beginning
    lock.l_start = record_number * RECORD_SIZE;  // Start of the specific record
    lock.l_len = RECORD_SIZE;            // Lock the size of one record

    if (fcntl(fd, F_SETLKW, &lock) == -1) {  // Use F_SETLKW to block until lock is acquired
        perror("Error applying lock");
    } else {
        printf("Lock applied on record %d.\n", record_number);
    }
}

void release_lock(int fd, int record_number) {
    struct flock lock;
    lock.l_type = F_UNLCK;               // Unlock
    lock.l_whence = SEEK_SET;
    lock.l_start = record_number * RECORD_SIZE;  // Unlock the specific record
    lock.l_len = RECORD_SIZE;            // Size of one record

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("Error releasing lock");
    } else {
        printf("Lock released on record %d.\n", record_number);
    }
}

void modify_record(int fd, int record_number, const char *new_data) {
    apply_lock(fd, F_WRLCK, record_number);  // Apply write lock

    // Move to the position of the record and modify it
    lseek(fd, record_number * RECORD_SIZE, SEEK_SET);
    write(fd, new_data, strlen(new_data));  // Write the new data to the record

    release_lock(fd, record_number);  // Release lock
}

void read_record(int fd, int record_number) {
    apply_lock(fd, F_RDLCK, record_number);  // Apply read lock

    // Move to the position of the record and read it
    char buffer[RECORD_SIZE];
    lseek(fd, record_number * RECORD_SIZE, SEEK_SET);
    read(fd, buffer, RECORD_SIZE);
    printf("Record %d: %s\n", record_number, buffer);

    release_lock(fd, record_number);  // Release lock
}

int main() {
    int fd = open("records.txt", O_RDWR | O_CREAT, 0666);  // Open file for read/write
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    // Option 1: Modify a record
    modify_record(fd, 1, "Modified Record 2\n");  // Modify the second record

    // Option 2: Read all records
    read_record(fd, 0);  // Read the first record
    read_record(fd, 1);  // Read the second record
    read_record(fd, 2);  // Read the third record

    close(fd);
    return 0;
}

