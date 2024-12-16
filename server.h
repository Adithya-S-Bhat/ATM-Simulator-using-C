#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>

struct user {
    char firstname[30];
    char lastname[30];
    char username[30];
    char password[30];
    double balance;
    int accno;
};

struct dbase {
    char firstname[30];
    char lastname[30];
    char username[30];
    char password[30];
    double balance;
    int accno;
};

extern struct user userData;

void clrscr();
void home();
void login(FILE *db, int size);
int selectoption(char *userName);
void createacc(FILE *db, int size);

#endif