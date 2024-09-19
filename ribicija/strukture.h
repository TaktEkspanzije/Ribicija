#pragma once

#ifndef STRUKTURE_H
#define STRUKTURE_H

// Structure for players
typedef struct {
    char ime[20];
    int polje;
    int bodovi;
    int gotov;
} igrac;

// Structure for fish
typedef struct {
    const char* ime;
    float tezina;
    int bod;
} ribe;

// Enumeration for menu options
typedef enum {
    IZBORNIK_IGRAJ = 1,
    IZBORNIK_NASTAVI,
    IZBORNIK_PRAVILA,
    IZBORNIK_HIGHSCORE,
    IZBORNIK_IZBRISI_HIGHSCORE,
    IZBORNIK_IZLAZ
} IzbornikOpcija;

// Function prototypes
void initGame(int numPlayers, igrac* players);
void playGame(igrac* players, int numPlayers);
void saveGame(igrac* players, int numPlayers);
int loadGame(igrac* players, int* numPlayers);
void napisiPravilaIgre();
int loviRibu();
int bacanjeKocke();
int bacanjeKockeRekurzivno(int rollCount);
void ispisiHighscore();
void izbrisiHighscore();
void upisiHighscore(const char* ime, int bodovi);
int upisiBrojIgraca();
int validInput(char* input);
void saveAndExit(igrac* players, int brojIgraca);
void continueGame(igrac* players, int* brojIgraca);
void saveGameBinary(igrac* players, int numPlayers, int fileIndex);
int askWhichSaveFile();
int loadGameBinary(igrac* players, int* numPlayers, int fileIndex);

#endif  // STRUKTURE_H
