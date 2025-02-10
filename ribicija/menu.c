#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strukture.h"

igrac* players;
int CurrentUniqueId = 0; // Globalna varijabla za jedinstveni broj trenutne igre

void menu() {
    int brojIgraca = 0;
    players = (igrac*)malloc(4 * sizeof(igrac)); // Alokacija za najviše 4 igrača
    if (players == NULL) {
        perror("Failed to allocate memory");
        return;
    }

    char input[10];
    IzbornikOpcija choice;

    while (1) {
        printf("**************************************************\n");
        printf("Odaberi opciju:\n");
        printf("1. Zaigraj igru Ribicija,\n");
        printf("2. Nastavi igru,\n");
        printf("3. Pravila igre,\n");
        printf("4. Procitaj highscore,\n");
        printf("5. Izbrisi highscore,\n");
        printf("6. Izlaz.\n");
        scanf("%s", input);

        if (sscanf(input, "%d", (int*)&choice) != 1) {
            printf("Pogrešan izbor, upiši broj između 1 i 6.\n");
            continue;
        }

        switch (choice) {
        case IZBORNIK_IGRAJ:
            brojIgraca = UpisiBrojIgraca();
            printf("Unesite jedinstveni broj za ovu igru: ");
            scanf("%d", &CurrentUniqueId);
            InitGame(brojIgraca, players);
            PlayGame(players, brojIgraca);
            // Nakon završetka igre, stanje se sprema unutar PlayGame() ili SaveAndExit()
            SaveGameUnified(players, brojIgraca, CurrentUniqueId);
            break;
        case IZBORNIK_NASTAVI: {
            int uniqueId;
            printf("Unesite jedinstveni broj igre koju želite učitati: ");
            scanf("%d", &uniqueId);
            if (LoadGameUnified(players, &brojIgraca, uniqueId) != 0) {
                // Ako ne postoji spremljena igra s tim brojem, vraćamo se u glavni izbornik
                break;
            }
            CurrentUniqueId = uniqueId;
            PlayGame(players, brojIgraca);
            break;
        }
        case IZBORNIK_PRAVILA:
            NapisiPravilaIgre();
            break;
        case IZBORNIK_HIGHSCORE:
            IspisiHighscore();
            break;
        case IZBORNIK_IZBRISI_HIGHSCORE:
            IzbrisiHighscore();
            break;
        case IZBORNIK_IZLAZ:
            SaveAndExit(players, brojIgraca);
            break;
        default:
            printf("Pogrešan izbor, upiši broj između 1 i 6.\n");
            break;
        }
    }

    free(players);
    players = NULL;
}
