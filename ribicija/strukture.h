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
	int tezina;
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

void init_game(int num_players, igrac players[]);
void play_game(igrac players[], int num_players);
void save_game(igrac players[], int num_players);
int load_game(igrac players[], int* num_players);
void NapisiPravilaIgre();
int loviribu();
int bacanjekocke();
int bacanjekocke_rekurzivno(int roll_count);
void ispisiHighscore();
void izbrisiHighscore();
void upisiHighscore(const char* ime, int bodovi);
int upisiBrojIgraca();
int valid_input(char* input);  // dodali smo ovu liniju

#endif