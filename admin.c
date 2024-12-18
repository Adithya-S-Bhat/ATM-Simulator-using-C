#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#include "server.h"

void adminLogin() {
    char adminPassword[PASSWORD_LENGTH];

    const char correctPassword[] = "******";  // Admin password

    int attempts = 0;

    while (true) {
        clearScreen();
        printf("-------------------------\n");
        printf("\x1B[32mPlease enter the admin password: \x1B[0m");
        scanf(" %s", adminPassword);

        // Check password
        if (strcmp(adminPassword, correctPassword) == 0) {
            printf("\x1B[32m\nAdmin login successful!\n\x1B[0m");
            Sleep(1000);
            clearScreen();
            adminMenu();
            return;
        }

        printf("\x1B[31mIncorrect password, please try again.\x1B[0m\n");
        Sleep(1000);
        attempts++;

        // Three failed attempts, cooldown
        if (attempts == 3) {
            printf(
                "\x1B[31mYou have entered the wrong password more than 3 "
                "times, the account is locked.\n\n");
            printf("Please try again later...\n\x1B[0m");
            attempts = 0;
            Sleep(10000);
            clearScreen();
        }
    }
}

void adminMenu() {
    int c = -1;

    while (true) {
        clearScreen();
        printf("\x1B[32m");
        printf(
            "\n\t\t***************************    Welcome, Dr.Ji    "
            "***************************\n\n\n\n");
        printf("\x1B[0m");
        printf("\x1B[34mEnter:\n\x1B[0m");
        printf(" [1] - Add users\n [2] - Exit\n");
        printf("-------------------------\n");
        scanf(" %d", &c);

        switch (c) {
            case 1:
                addUser();
                break;
            case 2:
                printf("Exiting admin mode.\n");
                return;
            default:
                printf("Invalid choice, please enter again.\n");
        }
    }
}

// Add new user
void addUser() {
    if (global_usr_cnt >= MAX_USERS) {
        printf("User limit reached, unable to add a new user.\n");
        return;
    }

    Account newAccount;

    // Input new user information
    printf("Please enter the new user's card number: ");
    scanf(" %s", newAccount.card_id);
    printf("Please enter the new user's password: ");
    scanf(" %s", newAccount.password);
    newAccount.balance = 0.0;

    // Add new user information
    global_accounts[global_usr_cnt] = newAccount;
    global_usr_cnt++;

    printf("New user added successfully!\n");
}
