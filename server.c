#include "server.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

// Initialization
Account global_accounts[MAX_USERS];
int global_usr_cnt = 0;

bool verifyPassword(Account* account, const char* inputPassword) {
    return strcmp(account->password, inputPassword) == 0;
}

void invalidInputWarning() {
    printf(
        "Error: Invalid value received, "
        "please check your input and enter a correct answer.\n");
}

void mainMenu() {
    int c = -1;

    while (true) {
        clearScreen();

        printf("\x1B[32m");
        printf(
            "\n\t\t***************************  Welcome to AVL ATM  "
            "***************************\n\n\n\n");
        printf("\x1B[0m");
        printf("\x1B[34mEnter:\n\x1B[0m");
        printf("\x1B[31m [0] - sudo\n\x1B[0m");
        printf(" [1] - Login\n [2] - Registration\n [3] - Exit\n");
        printf("-------------------------\n");
        scanf("%d", &c);

        // validate the input choice
        if (c < 0 || c > 3) {
            invalidInputWarning();
            continue;
        }

        // Load user data file
        FILE* db;
        db = fopen("userdbase.dat", "ab+");
        if (db = NULL) {  // File not found
            db = fopen("userdbase.dat", "wb+");
            if (db == NULL) {
                printf(
                    "\x1B[31mError: Failed to load user data file.\n\x1B[0m");
                exit(0);
            }
        }

        if (c == 0) {
            // SuperUser
            printf(
                "\x1B[31mBy login as SuperUser, your operations will have "
                "consequences...\n"
                "Are you sure you want to continue? [Y/n]: \n\x1B[0m");
            char ans = '\0';
            scanf("%c", &ans);
            if (ans == 'y' || ans == 'Y') {
                adminLogin();
            } else if (ans == 'n' || ans == 'N') {
                continue;
            } else {
                invalidInputWarning();
            }
        } else if (c == 1) {
            // User
            int user_idx = login();
            if (user_idx >= 0) {
                // Enter the user interface
                userMenu(&global_accounts[user_idx]);
                return;
            } else {
                // TODO
            }
        } else if (c == 2) {
            // Registration
            // TODO
        } else if (c == 3) {
            // Exit
            return;
        } else {
            invalidInputWarning();
        }
    }
}

int login() {
    char inputID[ACCOUNT_ID_LENGTH];
    char inputPassword[PASSWORD_LENGTH];
    int attempts = 0;
    time_t lastFailedAttempt = 0;  // Time of failure

    printf("Please enter your card number: ");
    scanf("%s", inputID);

    while (attempts < 3) {
        printf("Please enter your password: ");
        scanf("%s", inputPassword);

        // Verify password
        int i;
        for (i = 0; i < global_usr_cnt; i++) {
            if (strcmp(global_accounts[i].ID, inputID) == 0 &&
                verifyPassword(&global_accounts[i], inputPassword)) {
                return i;  // Return user index
            }
        }

        printf("Incorrect password, please try again.\n");
        attempts++;

        // Three failed attempts, wait
        if (attempts == 3) {
            lastFailedAttempt = time(NULL);  // Error time
            printf(
                "You have entered the wrong password more than 3 times, the "
                "account is locked for 10 seconds.\n");
            printf("Please try again later...\n");
        }

        // Check if the cooldown period has ended
        if (attempts == 3) {
            while (1) {
                time_t currentTime = time(NULL);
                if (difftime(currentTime, lastFailedAttempt) >= 10) {
                    printf(
                        "Cooldown period has ended, you can try logging in "
                        "again.\n");
                    break;
                } else {
                    printf(
                        "Waiting for cooldown... (Remaining time: %.0f "
                        "seconds)\n",
                        10 - difftime(currentTime, lastFailedAttempt));
                    Sleep(1);  // Check every second
                }
            }
            attempts = 0;  // Reset attempt count
        }
    }
}

void userMenu(Account* account) {
    int choice;
    while (1) {
        printf("\n--- User Main Menu ---\n");
        printf("1. Deposit\n");
        printf("2. Withdraw\n");
        printf("3. View account information\n");
        printf("0. Exit\n");
        printf("Please enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                double amount;
                printf("Please enter the deposit amount: ");
                scanf("%lf", &amount);
                deposit(account, amount);
                break;
            }
            case 2: {
                double amount;
                printf("Please enter the withdrawal amount: ");
                scanf("%lf", &amount);
                withdraw(account, amount);
                break;
            }
            case 3:
                queryAccountInfo(account);
                break;
            case 0:
                printf("Exiting the system.\n");
                return;
            default:
                printf("Invalid choice, please enter again.\n");
        }
    }
}

// Deposit
void deposit(Account* account, double amount) {
    if (amount <= 0) {
        printf("The deposit amount must be greater than zero!\n");
        return;
    }
    account->balance += amount;
    printf("Successfully deposited %.2f yuan, current balance: %.2f yuan\n",
           amount, account->balance);
}

// Withdrawal
void withdraw(Account* account, double amount) {
    if (amount <= 0) {
        printf("The withdrawal amount must be greater than zero!\n");
        return;
    }
    if (amount > account->balance) {
        printf("Insufficient balance, cannot withdraw!\n");
        return;
    }
    account->balance -= amount;
    printf("Successfully withdrew %.2f yuan, current balance: %.2f yuan\n",
           amount, account->balance);
}

// View account information
void queryAccountInfo(const Account* account) {
    printf("\nAccount Information:\n");
    printf("Card Number: %s\n", account->ID);
    printf("Name: %s\n", account->username);
    printf("Balance: %.2f yuan\n", account->balance);
}

void registration(FILE* db) {
    clearScreen();
    int acc_size = sizeof(Account);
    Account* ptr_usr_data = (Account*)malloc(sizeof(acc_size));
    char lastNameTemp[30], firstNameTemp[30], ID[ACCOUNT_ID_LENGTH],
        password[PASSWORD_LENGTH], usernameTemp[30];
    char confirmPassword[PASSWORD_LENGTH],
        checkSave;

    printf("\x1B[32m");
    printf(
        "\n\t\t***************************  Registration  Menu  "
        "***************************\n\n\n\n");
    printf("\x1B[0m");
    printf("\x1B[34mCreate new account:\n\x1B[0m");
    printf(
        "\x1B[31mWarning: Password Must contain less than ten(10) Alphanumeric "
        "Digits.\n\n\n\x1B[0m");
    printf("\x1B[32mEnter First Name:\x1B[0m");
    scanf(" %29[^'\n']", firstNameTemp);
    printf("\x1B[32mEnter Last Name: \x1B[0m");
    scanf(" %29[^'\n']", lastNameTemp);

    bool chk = true;  // boolean value: check the existence of account
    while (chk) {
        printf("\x1B[32mEnter Username: \x1B[0m");
        scanf(" %29[^'\n']", usernameTemp);

        // Validate the duplication
        chk = false;
        while (fread(ptr_usr_data, acc_size, 1, db) == 1) {
            if (strcmp(ptr_usr_data->username, usernameTemp) == 0) chk = true;
        }
        if (chk) {
            printf("\x1B[31mUsername already exists\n\x1B[0m");
            printf("\x1B[32mPlease enter a different username\n\x1B[0m");
        }
        rewind(db);
    }

    printf("\x1B[32mEnter Card ID: \x1B[0m");
    scanf(" %29s", ID);
    printf("\x1B[32mEnter Password: \x1B[0m");
    scanf(" %29s", password);
    printf("\x1B[32mConfirm Password: \x1B[0m");
    scanf(" %29s", confirmPassword);
}
