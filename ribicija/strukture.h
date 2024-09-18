#pragma once

#ifndef STRUKTURE_H
#define STRUKTURE_H

typedef struct {
    char ime[20];
    int polje;
    int bodovi;
    int gotov;
} igrac;

typedef struct {
    const char* ime;
    float tezina;
    int bod;
} ribe;

typedef enum {
    IZBORNIK_IGRAJ = 1,
    IZBORNIK_NASTAVI,
    IZBORNIK_PRAVILA,
    IZBORNIK_HIGHSCORE,
    IZBORNIK_IZBRISI_HIGHSCORE,
    IZBORNIK_IZLAZ
} IzbornikOpcija;

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
void izlazIzPrograma(igrac* players, int brojIgraca); 

void saveAndExit(igrac* players, int brojIgraca);
void continueGame(igrac* players, int* brojIgraca);
void saveGameBinary(igrac* players, int numPlayers, int fileIndex);
int askWhichSaveFile();
int loadGameBinary(igrac* players, int* numPlayers, int fileIndex);

#endif
