#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include "strukture.h"

// External declarations
extern igrac* players;

// Function to validate input
int validInput(char* input) {
    for (int i = 0; input[i] != '\0'; i++) {
        if (!isdigit(input[i])) {
            return 0;  // Not a number
        }
    }
    return 1;  // Valid number
}

// Recursive dice roll function
int bacanjeKockeRekurzivno(int rollCount) {
    srand(time(NULL) + rollCount);
    if (rollCount == 1) {
        return rand() % 6 + 1;
    } else {
        return (rand() % 6 + 1) + bacanjeKockeRekurzivno(rollCount - 1);
    }
}

// Dice roll function
int bacanjeKocke() {
    return bacanjeKockeRekurzivno(1);
}

// Write game rules
void napisiPravilaIgre() {
    printf("\n - \n Dok igras portebno je pokraj sebe imati fizicku kopiju ili sliku polja igre ribicija \n kako bi znali gdje se nalazite u igri posto je ovaj program samo text :p. \n Ribicija je covjece ne ljuti se igra sa posebnim poljima gdje pecas i ovisno koju ribu \n ulovis dobivas odredenu kolicinu bodova. Ti bodovi odlucuju tko je pobjednik. \n Pobjednik nije onaj igrac koji prvi dođe do cilja vec onaj sa najvise bodova na kraju igre. \n Prvi igrac koji dode do cilja dobiva 10 nagradnih bodova. \n - \n - ");
}

// Function to fish and determine points
int loviRibu() {
    static ribe riba[17];
    int brojRibe = rand() % 16 + 1;

    // Initialize fish data
    riba[1].ime = "Pastrva"; riba[1].tezina = 2.5; riba[1].bod = 4;
    riba[2].ime = "Deverika"; riba[2].tezina = 3.7; riba[2].bod = 3;
    riba[3].ime = "Crvenperka"; riba[3].tezina = 1.2; riba[3].bod = 3;
    riba[4].ime = "Tolstolobik"; riba[4].tezina = 25.3; riba[4].bod = 4;
    riba[5].ime = "Saran"; riba[5].tezina = 13.4; riba[5].bod = 4;
    riba[6].ime = "Bolen"; riba[6].tezina = 6.6; riba[6].bod = 4;
    riba[7].ime = "Som"; riba[7].tezina = 39.5; riba[7].bod = 4;
    riba[8].ime = "Smud"; riba[8].tezina = 4.2; riba[8].bod = 6;
    riba[9].ime = "Keciga"; riba[9].tezina = 7.1; riba[9].bod = 4;
    riba[10].ime = "Cvergl"; riba[10].tezina = 1.0; riba[10].bod = -2;
    riba[11].ime = "Amur"; riba[11].tezina = 11.8; riba[11].bod = 5;
    riba[12].ime = "Mrena"; riba[12].tezina = 2.9; riba[12].bod = 3;
    riba[13].ime = "Stuka"; riba[13].tezina = 15.5; riba[13].bod = 7;
    riba[14].ime = "Grgec"; riba[14].tezina = 1.3; riba[14].bod = 3;
    riba[15].ime = "Klen"; riba[15].tezina = 1.1; riba[15].bod = 2;
    riba[16].ime = "Linjak"; riba[16].tezina = 4.7; riba[16].bod = 7;

    if (brojRibe == 10) {
        printf("Ajoj, ulovili ste Cvergla. Ta riba vam oduzima 2 boda :(\n \n");
        return -2;
    }
    printf("Ulovili ste ribu %s tezine %.1f ! Vrijednost ribe u bodovima: %d\n \n", riba[brojRibe].ime, riba[brojRibe].tezina, riba[brojRibe].bod);
    return riba[brojRibe].bod;
}

// Initialize game
void initGame(int numPlayers, igrac* players) {
    for (int i = 0; i < numPlayers; i++) {
        printf("Unesite ime %d. igraca: ", i + 1);
        scanf("%s", players[i].ime);
        players[i].polje = 0;
        players[i].bodovi = 0;
        players[i].gotov = 0;
    }
}

// Play game function
void playGame(igrac* players, int numPlayers) {
    int currentPlayer = 0;
    int posebnaPolja[17] = { 37, 41, 45, 50, 55, 61, 66, 69, 73, 77, 80, 83, 88, 93, 100, 104, 108 };
    int krajIgre = 0;
    int igraZavrsena = 0;

    while (!igraZavrsena) {
        for (int i = 0; i < numPlayers; i++) {
            currentPlayer = i;
            printf("Igrac %s je na polju %d. Zelite li baciti kocku ili izaci van iz igre? (b/i): ", players[currentPlayer].ime, players[currentPlayer].polje);
            char choice[10];
            scanf("%9s", choice);
            char action = choice[0];
            if (action == 'b') {
                int roll = bacanjeKocke();
                players[currentPlayer].polje += roll;
                printf("Bacili ste kocku i dobili broj %d. Sada ste na polju %d.\n \n", roll, players[currentPlayer].polje);

                for (int j = 0; j < numPlayers; j++) {
                    if (j != currentPlayer && players[currentPlayer].polje == players[j].polje) {
                        printf("Igrac %s je pojeo igraca %s i poslao ga na polje 0!\n", players[currentPlayer].ime, players[j].ime);
                        players[j].polje = 0;
                    }
                }

                if (players[currentPlayer].polje >= 111) {
                    printf("Cestitamo! Igrac %s je prvi dosao do cilja i dobio 10 bonus bodova!\n", players[currentPlayer].ime);
                    players[currentPlayer].bodovi += 10;
                    krajIgre = 1;
                    break;
                }

                if (players[currentPlayer].polje == 5) {
                    printf("Igrac %s je stao na posebno polje te se pomice 5 mjesta unaprijed.\n", players[currentPlayer].ime);
                    players[currentPlayer].polje += 5;
                } else if (players[currentPlayer].polje == 11) {
                    printf("Igrac %s je stao na posebno polje te se vraca 3 mjesta unazad.\n", players[currentPlayer].ime);
                    players[currentPlayer].polje -= 3;
                } else if (players[currentPlayer].polje == 21) {
                    printf("Igrac %s je stao na posebno polje te se pomice 7 mjesta unaprijed.\n", players[currentPlayer].ime);
                    players[currentPlayer].polje += 7;
                }

                for (int j = 0; j < 17; j++) {
                    if (players[currentPlayer].polje == posebnaPolja[j]) {
                        if (players[currentPlayer].polje == 66) {
                            printf("Pecali ste na zabranjenom mjestu stoga vas vraćamo na start 2!\n");
                            players[currentPlayer].polje = 33;
                        } else if (players[currentPlayer].polje == 83) {
                            printf("Igrac %s je upao/la u camac koji je otplutovao do druge strane jezera te igraca doveo nazad na polje 56!\n", players[currentPlayer].ime);
                            players[currentPlayer].polje = 56;
                        } else {
                            players[currentPlayer].bodovi += loviRibu();
                        }
                    }
                }
            } else if (action == 'i') {
                saveGame(players, numPlayers);
                printf("Igra je spremljena. Igrac %s je izasao van iz igre.\n", players[currentPlayer].ime);
                krajIgre = 0;
                return;
            }
        }

        if (krajIgre) {
            printf("Kraj igre! Rezultati:\n");
            for (int i = 0; i < numPlayers; i++) {
                printf("Igrac %s: %d bodova\n", players[i].ime, players[i].bodovi);
            }

            int maxBodovi = players[0].bodovi;
            char pobjednik[20];
            strcpy(pobjednik, players[0].ime);
            for (int i = 1; i < numPlayers; i++) {
                if (players[i].bodovi > maxBodovi) {
                    maxBodovi = players[i].bodovi;
                    strcpy(pobjednik, players[i].ime);
                }
            }
            printf("Pobjednik je %s sa %d bodova!\n", pobjednik, maxBodovi);
            upisiHighscore(pobjednik, maxBodovi);
            igraZavrsena = 1;
        }
    }
}

// Save game to file
void saveGame(igrac* players, int numPlayers) {
    FILE* file = fopen("savefile.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    for (int i = 0; i < numPlayers; i++) {
        fprintf(file, "%s %d %d %d\n", players[i].ime, players[i].polje, players[i].bodovi, players[i].gotov);
    }
    if (ferror(file)) {
        perror("Error writing to file");
    }
    fclose(file);
}

// Load game from binary file
int loadGameBinary(igrac* players, int* numPlayers, int fileIndex) {
    char filename[20];
    sprintf(filename, "savefile%d.bin", fileIndex);

    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    fread(numPlayers, sizeof(int), 1, file);
    fread(players, sizeof(igrac), *numPlayers, file);

    long currentPosition = ftell(file);
    if (currentPosition != -1) {
        printf("Trenutna pozicija pokazivača u datoteci: %ld byte-ova.\n", currentPosition);
    } else {
        perror("Greska kod poziva ftell()");
    }

    fclose(file);
    return 0;
}

// Input number of players
int upisiBrojIgraca() {
    char input[10];
    int brojIgraca = 0;
    while (1) {
        printf("Unesite broj igraca (2-4): ");
        scanf("%s", input);
        if (validInput(input)) {
            sscanf(input, "%d", &brojIgraca);
            if (brojIgraca >= 2 && brojIgraca <= 4) {
                break;
            } else {
                printf("Broj igraca mora biti izmedu 2 i 4!\n");
            }
        } else {
            printf("Pogresan unos, upisite broj izmedu 2 i 4!\n");
        }
    }
    return brojIgraca;
}

// Print high score
void ispisiHighscore() {
    FILE* file = fopen("highscore.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        printf("Nema prijasnjih igara.\n");
        return;
    }

    char ime[20];
    int bodovi;
    int maxBodovi = 0;
    char najboljiIgrac[20];
    int postojiRezultati = 0;

    printf("Pobjednici do sada:\n");
    while (fscanf(file, "%s %d", ime, &bodovi) == 2) {
        printf("Igrac: %s, Bodovi: %d\n", ime, bodovi);
        postojiRezultati = 1;
        if (bodovi > maxBodovi) {
            maxBodovi = bodovi;
            strcpy(najboljiIgrac, ime);
        }
    }

    if (postojiRezultati) {
        printf("\nNajbolji igrac do sada je %s sa %d bodova.\n", najboljiIgrac, maxBodovi);
    }

    if (ferror(file)) {
        perror("Error reading from file");
    }
    fclose(file);
}

// Save high score
void upisiHighscore(const char* ime, int bodovi) {
    FILE* file = fopen("highscore.txt", "a");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    fprintf(file, "%s %d\n", ime, bodovi);
    if (ferror(file)) {
        perror("Error writing to file");
    }
    fclose(file);
}

// Save game in binary format
void saveGameBinary(igrac* players, int numPlayers, int fileIndex) {
    char filename[20];
    sprintf(filename, "savefile%d.bin", fileIndex);

    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    fwrite(&numPlayers, sizeof(int), 1, file);
    fwrite(players, sizeof(igrac), numPlayers, file);

    fclose(file);
}

// Get available save file
int getAvailableSaveFile() {
    FILE* file;
    char filename[20];

    for (int i = 1; i <= 3; i++) {
        sprintf(filename, "savefile%d.bin", i);
        file = fopen(filename, "rb");
        if (file == NULL) {
            return i;
        }
        fclose(file);
    }

    return 3;
}

// Choose save file
int askWhichSaveFile() {
    int fileIndex;
    printf("Nastavi od kojeg savefile-a (1, 2, ili 3)? ");
    scanf("%d", &fileIndex);

    if (fileIndex < 1 || fileIndex > 3) {
        printf("Pogresan unos! Koristimo savefile1.\n");
        return 1;
    }

    return fileIndex;
}

// Delete high score
void izbrisiHighscore() {
    if (remove("highscore.txt") == 0) {
        printf("High score datoteka je uspješno izbrisana.\n");
    } else {
        perror("Greška pri brisanju high score datoteke");
    }
}

// Save and exit game
void saveAndExit(igrac* players, int brojIgraca) {
    char odgovor[3];
    printf("Da li ste sigurni da zelite zavrsiti program? (da / ne) ");
    scanf("%s", odgovor);
    if (!strcmp(odgovor, "da")) {
        int nextSaveFile = getAvailableSaveFile();
        saveGameBinary(players, brojIgraca, nextSaveFile);
        free(players);
        players = NULL;
        exit(0);
    } else if (!strcmp(odgovor, "ne")) {
        return;
    } else {
        printf("Nepoznata opcija, upisi opet\n");
    }
}

// Continue saved game
void continueGame(igrac* players, int* brojIgraca) {
    int fileIndex = askWhichSaveFile();
    if (loadGameBinary(players, brojIgraca, fileIndex) == 0) {
        playGame(players, *brojIgraca);
    }
}
