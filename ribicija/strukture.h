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


typedef struct {
    int UniqueId;      
    int NumPlayers;     
    igrac Players[4];    
} GameSave;


void InitGame(int numPlayers, igrac* players);
void PlayGame(igrac* players, int numPlayers);
void NapisiPravilaIgre(void);
int LoviRibu(void);
int BacanjeKocke(void);
int BacanjeKockeRekurzivno(int rollCount);
int ValidInput(char* input);
int UpisiBrojIgraca(void);
void IspisiHighscore(void);
void IzbrisiHighscore(void);
void UpisiHighscore(const char* ime, int bodovi);
void SaveAndExit(igrac* players, int brojIgraca);

// Prototipi novih funkcija za unificirani save/učitavanje
void SaveGameUnified(igrac* players, int numPlayers, int uniqueId);
int LoadGameUnified(igrac* players, int* numPlayers, int uniqueId);
int CompareGameSaves(const void* a, const void* b);

#endif
