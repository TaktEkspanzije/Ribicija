#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "strukture.h"

// Deklaracije funkcija - Ignoriraj :I
void saveGameBinary(igrac* players, int numPlayers, int fileIndex);
int askWhichSaveFile();
int loadGameBinary(igrac* players, int* numPlayers, int fileIndex);
int getAvailableSaveFile();

igrac* players;

// Funkcija za izlaz iz igre
void izlazIzIgre(igrac* players, int brojIgraca) {
    char odgovor[3];
    printf("Da li ste sigurni da zelite zavrsiti program? (da / ne)");
    scanf("%s", odgovor);
    if (!strcmp(odgovor, "da")) {
        int nextSaveFile = getAvailableSaveFile();
        saveGameBinary(players, brojIgraca, nextSaveFile);
        free(players);
        players = NULL; // Postavi pokazivač na NULL nakon oslobadanja memorije
        exit(0);
    } else if (!strcmp(odgovor, "ne")) {
        return;
    } else {
        printf("Nepoznata opcija, upisi opet\n");
    }
}

// Funkcija za nastavak igre
void nastaviIgru(igrac* players, int* brojIgraca) {
    int fileIndex = askWhichSaveFile();
    if (loadGameBinary(players, brojIgraca, fileIndex) == 0) {
        playGame(players, *brojIgraca);
    }
}

int main() {
    int brojIgraca = 0;
    players = (igrac*)malloc(4 * sizeof(igrac)); // Dinamička memorija za igrače
    if (players == NULL) {
        perror("Failed to allocate memory");
        return 1;
    }

    char input[10];  // Buffer to store user input
    IzbornikOpcija choice;
    int saveFileIndex = 1;  // Default save file index

    while (1) {
        printf("**************************************************\n");
        printf("Odaberi opciju:\n");
        printf("1. Zaigraj igru Ribicija,\n");
        printf("2. Nastavi igru,\n");
        printf("3. Pravila igre,\n");
        printf("4. Procitaj highscore,\n");
        printf("5. Izbrisi highscore,\n");
        printf("6. Izlaz.\n");
        scanf("%s", input);  // Read user input as string

        if (sscanf(input, "%d", (int*)&choice) != 1) {
            printf("Pogresan izbor, upisi broj izmedu 1 i 6.\n");
            continue;
        }

        switch (choice) {
        case IZBORNIK_IGRAJ:
            brojIgraca = upisiBrojIgraca();

            printf("Odaberite save file (1, 2, ili 3) za ovu igru: ");
            scanf("%d", &saveFileIndex);

            if (saveFileIndex < 1 || saveFileIndex > 3) {
                printf("Neispravan odabir. Spremit ćemo u savefile1.\n");
                saveFileIndex = 1;
            }

            initGame(brojIgraca, players);
            playGame(players, brojIgraca);

            // Spremi igru u odredeni file
            saveGameBinary(players, brojIgraca, saveFileIndex);
            break;

        case IZBORNIK_NASTAVI:
            nastaviIgru(players, &brojIgraca); // Poziv nove funkcije
            break;

        case IZBORNIK_PRAVILA:
            napisiPravilaIgre();
            break;

        case IZBORNIK_HIGHSCORE:
            ispisiHighscore();
            break;

        case IZBORNIK_IZBRISI_HIGHSCORE:
            izbrisiHighscore();
            break;

        case IZBORNIK_IZLAZ:
            izlazIzIgre(players, brojIgraca); // Poziv nove funkcije za izlaz
            break;

        default:
            printf("Pogresan izbor, upisi broj izmedu 1 i 6.\n");
            break;
        }
    }

    free(players); // Oslobodi memoriju
    players = NULL; // Postavi pokazivač na NULL nakon oslobadanja memorije
    return 0;
}
