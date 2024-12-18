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
            Sleep(1500);

            clearScreen();
            adminMenu();
            clearScreen();

            return;
        }

        printf("\x1B[31mIncorrect password, please try again.\x1B[0m\n");
        Sleep(1500);
        attempts++;

        // Three failed attempts, cooldown
        if (attempts == 3) {
            printf(
                "\x1B[31mYou have entered the wrong password more than 3 "
                "times, the Admin permission is locked.\n\n");
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

        // validate the input choice
        if (c < 0 || c > 2) {
            invalidInputWarning();
            continue;
        }

        // Load user data file
        FILE* db;
        db = fopen("userdbase.dat", "ab+");
        if (db == NULL) {  // File not found
            db = fopen("userdbase.dat", "wb+");
            if (db == NULL) {
                printf(
                    "\x1B[31mError: Failed to load user data file.\n\x1B[0m");
                exit(0);
            }
        }

        switch (c) {
            case 1:
                addUser(db);
                break;

            case 2:
                printf("\x1B[31m\nLogining out, please wait...\n\x1B[0m");
                Sleep(1500);
                return;

            default:
                invalidInputWarning();
        }

        fclose(db);
    }
}

// Add new user
void addUser(FILE* db) {
    clearScreen();
    printf("\x1B[32m");
    printf(
        "\n\t\t***************************   Adding   Account   "
        "***************************\n\n\n\n");
    printf("\x1B[0m");
    register_user(db);  // db is closed
}
