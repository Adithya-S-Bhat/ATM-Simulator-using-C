#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#include "server.h"

void adminLogin() {
    unsigned char adminPassword[PASSWORD_LENGTH];

    const unsigned char correctPassword[] = "******";  // Admin password

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
        printf(" [1] - Add Users\n [2] - List All User Info\n [3] - Exit\n");
        printf("-------------------------\n");
        scanf(" %d", &c);

        // validate the input choice
        if (c < 1 || c > 3) {
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
                listAllUserInfo(db);
                break;

            case 3:
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

// List All User Info
void listAllUserInfo(FILE* db) {
    int acc_size = sizeof(Account), n = 0;
    Account* ptr_usr_data = (Account*)malloc(acc_size);
    _init_acc_transinfo(ptr_usr_data);

    // Calculate user count
    while (fread(ptr_usr_data, acc_size, 1, db) == 1) n++;
    rewind(db);
    free(ptr_usr_data);
    ptr_usr_data = NULL;

    // Load all user data
    Account* ptr_usr_dlst = (Account*)malloc(n * acc_size);
    fread(ptr_usr_dlst, acc_size, n, db);
    rewind(db);

    printf(
        "___________________________________________________________________________________________________________\n");
    printf(
        "|\x1B[33m   -----------------------------------   User  Information  List   -----------------------------------   \x1B[0m|\n");
    printf(
        "|---------------------------------------------------------------------------------------------------------|\n");
    printf(
        "|Idx| Card No. |        Card   Cipher(HEX)        | Password |      Password   Cipher(HEX)      | Balance |\n");
    printf(
        "|---------------------------------------------------------------------------------------------------------|\n");
    for (int i = 0; i < n; i++) {
        unsigned char target_card_id[ACCOUNT_ID_LENGTH];
        decryptor((ptr_usr_dlst + i)->card_id_cipher, target_card_id);
        printf("|%03d| %-8s | ", i + 1, target_card_id);
        for (int j = 0; j < CIPHER_SIZE; j++)
            printf("%02x", (ptr_usr_dlst + i)->card_id_cipher[j]);
        unsigned char target_pwd[PASSWORD_LENGTH];
        decryptor((ptr_usr_dlst + i)->password_cipher, target_pwd);
        printf(" | %-8s | ", target_pwd);
        for (int j = 0; j < CIPHER_SIZE; j++)
            printf("%02x", (ptr_usr_dlst + i)->password_cipher[j]);
        printf(" | %07.2f |\n", (ptr_usr_dlst + i)->balance);
    }
    printf(
        "|\x1B[33m --------------------------------------------   THE   END   -------------------------------------------- \x1B[0m|\n");
    printf(
        "|_________________________________________________________________________________________________________|\n\n");

    unsigned char confirm;
    printf("\x1B[32mEnter a character to continue: \n\x1B[0m");
    scanf(" %c", &confirm);
    confirm = '\0';
}
