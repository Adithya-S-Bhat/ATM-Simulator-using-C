#include <stdio.h>
#include <string.h>
#include <windows.h>

#include "server.h"

void clearScreen() { system("cls"); }

void invalidInputWarning() {
    printf(
        "\x1B[31mError: Invalid value received, "
        "please check your input and enter a correct answer.\n\x1B[0m");
}

int main(void) {
    /* Test for Encryption and Decryption
    demo();
    */

    mainMenu();
    return 0;
}
