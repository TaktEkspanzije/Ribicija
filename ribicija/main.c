#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "strukture.h"  // Include the header to access necessary structures

extern void menu();  // Declaration of the menu function, assumed to be defined in menu.c

int main() {
    menu();  // Call to the menu function
    return 0;
}
