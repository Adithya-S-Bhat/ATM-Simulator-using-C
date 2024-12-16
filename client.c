#include <stdio.h>
#include <string.h>
#include <windows.h>

#include "server.h"

void clearScreen() { system("cls"); }

int main() {
    int choice;
    int attempts = 0;

    clearScreen();

    // Select system
    while (attempts < 3) {
        printf("Please select an operation mode:\n");
        printf("1. User login\n");
        printf("2. Admin login\n");
        printf("Please enter your choice: ");
        scanf("%d", &choice);

        clearScreen();

        if (choice == 1) {
            // User
            int userIndex = login();
            if (userIndex >= 0) {
                mainMenu(&accounts[userIndex]);
                return 0;
            }
        } else if (choice == 2) {
            // Admin
            adminLogin();
            return 0;
        } else {
            printf("Invalid choice, please enter a correct number.\n");
            attempts++;  // Increase attempt count
        }

        if (attempts == 3) {
            printf(
                "You have entered the wrong choice more than 3 times, the "
                "program will exit.\n");
            Sleep(1);
            return 0;
        }
    }

    return 0;
}
