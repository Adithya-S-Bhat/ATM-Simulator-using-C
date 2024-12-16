#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>

#include "server.h"

void adminLogin() {
    char adminPassword[PASSWORD_LENGTH];

    const char correctPassword[] = "admin123";  // Admin password set!!!

    int attempts = 0;
    time_t lastFailedAttempt = 0;  // Time of last failure

    while (attempts < 3) {
        printf("Please enter the admin password: ");
        scanf("%s", adminPassword);

        // Check password
        if (strcmp(adminPassword, correctPassword) == 0) {
            printf("Admin login successful!\n");
            Sleep(1);
            clearScreen();
            adminMenu();
            return;
        }

        printf("Incorrect password, please try again.\n");
        Sleep(500);
        clearScreen();
        attempts++;

        // Three failed attempts, cooldown
        if (attempts == 3) {
            lastFailedAttempt = time(NULL);  // Record time
            printf(
                "You have entered the wrong password more than 3 times, the "
                "account is locked for 10 seconds.\n");
            printf("Please try again later...\n");
            Sleep(1);
            clearScreen();
        }

        // Cooldown period
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
                    Sleep(1);
                    clearScreen();
                }
            }
            attempts = 0;
        }
    }
}

void adminMenu() {
    int choice;

    while (1) {
        clearScreen();
        printf("--- Admin Menu ---\n");
        printf("1. Add new user\n");
        printf("0. Exit\n");
        printf("Please enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addUser();
                break;
            case 0:
                printf("Exiting admin mode.\n");
                return;
            default:
                printf("Invalid choice, please enter again.\n");
        }
    }
}

// Add new user
void addUser() {
    if (userCount >= MAX_USERS) {
        printf("User limit reached, unable to add a new user.\n");
        return;
    }

    Account newAccount;

    // Input new user information
    printf("Please enter the new user's card number: ");
    scanf("%s", newAccount.ID);
    printf("Please enter the new user's name: ");
    scanf("%s", newAccount.name);
    printf("Please enter the new user's password: ");
    scanf("%s", newAccount.password);
    newAccount.money = 0.0;

    // Add new user information
    accounts[userCount] = newAccount;
    userCount++;

    printf("New user added successfully!\n");
}
