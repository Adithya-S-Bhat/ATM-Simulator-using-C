#include "server.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void clrscr() { system("clear"); }

void home() {
    int c, home = 1;
    while (home == 1) {
        printf("\x1B[32m");
        printf(
            "\n\t\t***************************Welcome to ATV "
            "ATM***************************\n\n\n\n");
        printf("\x1B[0m");
        printf("\x1B[34mEnter:\n\x1B[0m");
        printf(" [1]-Login\n [2]-Create Account\n [3]-Exit\n");
        printf("----------------------\n");
        scanf("%d", &c);
        FILE *db;
        db = fopen("userdbase.dat", "ab+");
        if (db == NULL) {
            db = fopen("userdbase.dat", "wb+");
            if (db == NULL) {
                printf("The File could not be opened.\n");
                exit(0);
            }
        }
        long size = sizeof(struct user);
        char ch;
        if (c == 1) {
            login(db, size);
        } else if (c == 2) {
            createacc(db, size);
        } else if (c == 3) {
            home = 0;
            fclose(db);
        }
    }
}

void login(FILE *db, int size) {
    char userName[10], passwd[8];
    char tempo[8];
    int validUserNameOption, validUserPwdOption, returnOption,
        showAdminMenu = 1, chk = 0;
    char pass[30];
    validUserNameOption = 0;
    validUserPwdOption = 0;
    while (validUserNameOption == 0) {
        clrscr();
        printf("\x1B[34mLogin:\n\n\n\x1B[0m");
        printf("\x1B[32mEnter User Name: \x1B[0m");
        scanf(" %9s", userName);
        while (fread(&userData, size, 1, db) == 1) {
            if (strcmp(userData.username, userName) == 0) {
                validUserNameOption = 1;
                chk = 1;
                while (validUserPwdOption == 0) {
                    printf("\x1B[32mEnter Your Password: \x1B[0m");
                    scanf(" %29s", pass);
                    if (strcmp(userData.password, pass)) {
                        printf(
                            "\x1B[31mYou Have Supplied a Wrong "
                            "Password.\n\x1B[0m");
                    } else {
                        clrscr();
                        fclose(db);
                        printf(
                            "\x1B[34mYou have successfully Logged In the "
                            "System.\n");
                        validUserPwdOption = 1;
                        showAdminMenu = 0;
                    }
                }
                while (showAdminMenu == 0) {
                    printf("\x1B[32mOPTIONS\n\n\x1B[0m");
                    printf("[1] Deposit\n");
                    printf("[2] Balance Statement and Account information\n");
                    printf("[3] Change Password\n");
                    printf("[4] Withdraw\n");
                    printf("[5] Close your account\n");
                    printf("[6] Log out\n");
                    printf("----------------------\n");
                    printf("\x1B[32mPlease enter your choice: \x1B[0m");
                    showAdminMenu = selectoption(userName);
                }
            }
        }
        if (chk == 0) {
            printf("\x1B[31mInvalid Username\n\x1B[0m");
            rewind(db);
        }
    }
}

int selectoption(char *userName) {
    int choice, rtrn = 0, i = 0, n = 0;
    scanf("%d", &choice);
    clrscr();
    FILE *db;
    db = fopen("userdbase.dat", "rb+");
    long size = sizeof(struct user);
    long dbsize = sizeof(struct dbase);
    while (fread(&userData, size, 1, db) == 1) n++;
    rewind(db);
    struct dbase user[n];
    fread(&user, size, n, db);
    rewind(db);
    for (i = 0; fread(&userData, size, 1, db) == 1; i++) {
        if (strcmp(userData.username, userName) == 0) {
            break;
        }
    }
    double amt;
    char newpasswd[30], confirmPassword[30], curpasswd[30];
    char confirm, pass[30];
    int validUserPwdOption = 0;
    switch (choice) {
        case 1:
            printf("\x1B[32mEnter the amount to be deposited\x1B[0m");
            scanf("%lf", &amt);
            user[i].balance += amt;
            printf("\x1B[34mAmount deposited successfully\n\x1B[0m");
            printf("Your new balance is Rs.%.2lf\n", user[i].balance);
            break;
        case 2:
            printf("\x1B[34mAccount information:\n\n\x1B[0m");
            printf("Account Number:\t%d\n", userData.accno);
            printf("First name:\t%s\n", userData.firstname);
            printf("Last name:\t%s\n", userData.lastname);
            printf("Balance:\tRs.%.2lf\n\n", userData.balance);
            break;
        case 3:
            printf("\x1B[32mEnter your current password\x1B[0m");
            scanf(" %29s", curpasswd);
            if (strcmp(curpasswd, userData.password) == 0) {
                printf("\x1B[32mEnter new password\x1B[0m");
                scanf(" %29s", newpasswd);
                printf("\x1B[32mConfirm Password: \x1B[0m");
                scanf(" %29s", confirmPassword);
                if (strcmp(newpasswd, confirmPassword)) {
                    printf("\x1B[31mPasswords do not match.\n\x1B[0m");
                } else if (strcmp(newpasswd, userData.password) == 0)
                    printf(
                        "\x1B[31mYour new password is same as old "
                        "password\n\x1B[0m");
                else {
                    strcpy(user[i].password, newpasswd);
                    printf(
                        "\x1B[34mYour password has been changed "
                        "successfully\n\x1B[0m");
                }
            } else {
                printf("\x1B[31mIncorrect password\n\x1B[0m");
            }
            break;
        case 4:
            printf("\x1B[32mEnter the amount you want to withdraw: \x1B[0m");
            scanf("%lf", &amt);
            if (userData.balance >= amt) {
                user[i].balance -= amt;
                printf("\x1B[34mPlease collect the cash\n\x1B[0m");
                printf("Your new balance is Rs.%.2lf\n", user[i].balance);
            } else
                printf("\x1B[31mInsufficient balance\n\x1B[0m");
            break;
        case 5:
            printf(
                "\x1B[31mConfirm:Do you want to delete account "
                "permanently?(Enter y to confirm)\n\x1B[0m");
            scanf(" %c", &confirm);
            if (confirm == 'y') {
                while (validUserPwdOption == 0) {
                    printf("\x1B[32mConfirm Your Password: \x1B[0m");
                    scanf(" %29s", pass);
                    if (strcmp(userData.password, pass)) {
                        printf(
                            "\x1B[31mYou Have Supplied a Wrong "
                            "Password.\n\x1B[0m");
                        printf(
                            "\x1B[32mPlease re-enter your password: \n\x1B[0m");
                    } else {
                        int temp = i;
                        clrscr();
                        fclose(db);
                        db = fopen("userdbase.dat", "wb+");
                        for (int j = 0; j < n; j++) {
                            if (j == i)
                                continue;
                            else {
                                fwrite(&user[j], dbsize, 1, db);
                            }
                        }
                        printf("%d\n", i);
                        printf(
                            "\x1B[34mYou can collect your balance amount "
                            "Rs.%.2lf\n",
                            userData.balance);
                        printf(
                            "Your account has been successfully "
                            "deleted\n\x1B[0m");
                        validUserPwdOption = 1;
                        rtrn = 1;
                    }
                }
            }
            break;
        case 6:
            rtrn = 1;
            break;
        default:
            printf("\x1B[31mInvalid input\n\x1B[0m");
    }
    rewind(db);
    if (choice != 5) fwrite(&user, dbsize, n, db);
    fclose(db);
    return rtrn;
}

void createacc(FILE *db, int size) {
    clrscr();
    char lastNameTemp[30], firstNameTemp[30], password[30], usernametemp[30];
    char confirmPassword[30], checkSave;
    printf("\x1B[34mCreate new account:\n\x1B[0m");
    printf(
        "\x1B[31mWarning: Password Must contain less than ten(10) Alphanumeric "
        "Digits.\n\n\n\x1B[0m");
    printf("\x1B[32mEnter First Name:\x1B[0m");
    scanf(" %29[^'\n']", firstNameTemp);
    printf("\x1B[32mEnter Last Name: \x1B[0m");
    scanf(" %29[^'\n']", lastNameTemp);
    int chk = 1;
    while (chk == 1) {
        printf("\x1B[32mEnter Username: \x1B[0m");
        scanf(" %29[^'\n']", usernametemp);
        chk = 0;
        while (fread(&userData, size, 1, db) == 1) {
            if (strcmp(userData.username, usernametemp) == 0) {
                chk = 1;
            }
        }
        if (chk == 1) {
            printf("\x1B[31mUsername already exists\n\x1B[0m");
            printf("\x1B[32mPlease enter a different username\n\x1B[0m");
        }
        rewind(db);
    }
    printf("\x1B[32mEnter Password: \x1B[0m");
    scanf(" %29s", password);
    printf("\x1B[32mConfirm Password: \x1B[0m");
    scanf(" %29s", confirmPassword);
    if (strcmp(password, confirmPassword)) {
        printf("\x1B[31mPasswords do not match.\n\x1B[0m");
    } else {
        printf("\x1B[31mSave User Information? (y/n): \n\x1B[0m");
        scanf(" %c", &checkSave);
        int n = 0;
        while (fread(&userData, size, 1, db) == 1) n++;
        if (checkSave == 'y') {
            strcpy(userData.firstname, firstNameTemp);
            strcpy(userData.lastname, lastNameTemp);
            strcpy(userData.username, usernametemp);
            strcpy(userData.password, password);
            userData.balance = 0.0;
            userData.accno = n + 1;
            if (userData.accno < 0) {
                printf(
                    "\x1B[31mError: too many accounts. Sorry, the max number "
                    "of accounts has been reached.\n\x1B[0m");
                fclose(db);
                return;
            }
            fclose(db);
            db = fopen("userdbase.dat", "ab+");
            fwrite(&userData, size, 1, db);
            fflush(db);
            printf("----------------------\n");
            printf(
                "\x1B[34mYour account has been created successfully and your "
                "Account number is %d\n\x1B[0m",
                userData.accno);
        }
    }
    fclose(db);
}
