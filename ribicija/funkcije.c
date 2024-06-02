#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include "strukture.h"

// Funkcija za validaciju unosa
int valid_input(char* input) {
	for (int i = 0; input[i] != '\0'; i++) {
		if (!isdigit(input[i])) {
			return 0; // false, not a number
		}
	}
	return 1; // true, it is a number
}

// Rekurzivna funkcija za bacanje kocke s nasumičnim seed-om
int bacanjekocke_rekurzivno(int roll_count) {
	srand(time(NULL) + roll_count);
	if (roll_count == 1) {
		return rand() % 6 + 1;
	}
	else {
		return (rand() % 6 + 1) + bacanjekocke_rekurzivno(roll_count - 1);
	}
}

// Funkcija za bacanje kocke
int bacanjekocke() {
	return bacanjekocke_rekurzivno(1);
}

void NapisiPravilaIgre() {
	printf("\n - \n Dok igras portebno je pokraj sebe imati fizicku kopiju ili sliku polja igre ribicija \n kako bi znali gdje se nalazite u igri posto je ovaj program samo text :p. \n Ribicija je covjece ne ljuti se igra sa posebnim poljima gdje pecas i ovisno koju ribu \n ulovis dobivas odredenu kolicinu bodova. Ti bodovi odlucuju tko je pobjednik. \n Pobjednik nije onaj igrac koji prvi dođe do cilja vec onaj sa najvise bodova na kraju igre. \n Prvi igrac koji dode do cilja dobiva 10 nagradnih bodova. \n - \n - ");
}

int loviribu() {
	static ribe riba[17];
	int brojribe = rand() % 16 + 1;

	riba[1].ime = "Pastrva";
	riba[1].tezina = 2;
	riba[1].bod = 4;

	riba[2].ime = "Deverika";
	riba[2].tezina = 3;
	riba[2].bod = 3;

	riba[3].ime = "Crvenperka";
	riba[3].tezina = 1;
	riba[3].bod = 3;

	riba[4].ime = "Tolstolobik";
	riba[4].tezina = 25;
	riba[4].bod = 4;

	riba[5].ime = "Saran";
	riba[5].tezina = 13;
	riba[5].bod = 4;

	riba[6].ime = "Bolen";
	riba[6].tezina = 6;
	riba[6].bod = 4;

	riba[7].ime = "Som";
	riba[7].tezina = 39;
	riba[7].bod = 4;

	riba[8].ime = "Smud";
	riba[8].tezina = 4;
	riba[8].bod = 6;

	riba[9].ime = "Keciga";
	riba[9].tezina = 7;
	riba[9].bod = 4;

	riba[10].ime = "Cvergl";
	riba[10].tezina = 1;
	riba[10].bod = -2;

	riba[11].ime = "Amur";
	riba[11].tezina = 11;
	riba[11].bod = 5;

	riba[12].ime = "Mrena";
	riba[12].tezina = 2;
	riba[12].bod = 3;

	riba[13].ime = "Stuka";
	riba[13].tezina = 15;
	riba[13].bod = 7;

	riba[14].ime = "Grgec";
	riba[14].tezina = 1;
	riba[14].bod = 3;

	riba[15].ime = "Klen";
	riba[15].tezina = 1;
	riba[15].bod = 2;

	riba[16].ime = "Linjak";
	riba[16].tezina = 4;
	riba[16].bod = 7;

	if (brojribe == 10) {
		printf("Ajoj, ulovili ste Cvergla. Ta riba vam oduzima 2 boda :(\n \n");
		return -2;
	}
	printf("Ulovili ste ribu %s tezine %d ! Vrijednost ribe u bodovima: %d\n \n", riba[brojribe].ime, riba[brojribe].tezina, riba[brojribe].bod);
	return riba[brojribe].bod;
}

void init_game(int num_players, igrac players[]) {
	for (int i = 0; i < num_players; i++) {
		printf("Unesite ime %d. igraca: ", i + 1);
		scanf("%s", players[i].ime);
		players[i].polje = 0;
		players[i].bodovi = 0;
		players[i].gotov = 0;
	}
}

void play_game(igrac players[], int num_players) {
	int current_player = 0;
	int posebna_polja[17] = { 37, 41, 45, 50, 55, 61, 66, 69, 73, 77, 80, 83, 88, 93, 100, 104, 108 };
	int kraj_igre = 0;
	int igra_zavrsena = 0;

	while (!igra_zavrsena) {
		for (int i = 0; i < num_players; i++) {
			current_player = i;
			printf("Igrac %s je na polju %d. Zelite li baciti kocku ili izaci van iz igre? (b/i): ", players[current_player].ime, players[current_player].polje);
			char choice[10];
			scanf("%9s", choice);  // Read up to 9 characters to prevent buffer overflow
			char action = choice[0];  // Only use the first character
			if (action == 'b') {
				int roll = bacanjekocke();
				players[current_player].polje += roll;
				printf("Bacili ste kocku i dobili broj %d. Sada ste na polju %d.\n \n", roll, players[current_player].polje);

				for (int j = 0; j < num_players; j++) {
					if (j != current_player && players[current_player].polje == players[j].polje) {
						printf("Igrac %s je pojeo igraca %s i poslao ga na polje 0!\n", players[current_player].ime, players[j].ime);
						players[j].polje = 0;
					}
				}

				if (players[current_player].polje >= 111) {
					printf("Cestitamo! Igrac %s je prvi dosao do cilja i dobio 10 bonus bodova!\n", players[current_player].ime);
					players[current_player].bodovi += 10;
					kraj_igre = 1;
					break;
				}

				// Posebna polja 5, 11, 21
				if (players[current_player].polje == 5) {
					printf("Igrac %s je stao na posebno polje te se pomice 5 mjesta unaprijed.\n", players[current_player].ime);
					players[current_player].polje += 5;
				}
				else if (players[current_player].polje == 11) {
					printf("Igrac %s je stao na posebno polje te se vraca 3 mjesta unazad.\n", players[current_player].ime);
					players[current_player].polje -= 3;
				}
				else if (players[current_player].polje == 21) {
					printf("Igrac %s je stao na posebno polje te se pomice 7 mjesta unaprijed.\n", players[current_player].ime);
					players[current_player].polje += 7;
				}

				for (int j = 0; j < 17; j++) {
					if (players[current_player].polje == posebna_polja[j]) {
						if (players[current_player].polje == 66) {
							printf("Pecali ste na zabranjenom mjestu stoga vas vraćamo na start 2!\n");
							players[current_player].polje = 33;
						}
						else if (players[current_player].polje == 83) {
							printf("Igrac %s je upao/la u camac koji je otplutovao do druge strane jezera te igraca doveo nazad na polje 56!\n", players[current_player].ime);
							players[current_player].polje = 56;
						}
						else {
							players[current_player].bodovi += loviribu();
						}
					}
				}
			}
			else if (action == 'i') {
				save_game(players, num_players);
				printf("Igra je spremljena. Igrac %s je izasao van iz igre.\n", players[current_player].ime);
				kraj_igre = 0;
				return;
			}
		}

		if (kraj_igre) {
			printf("Kraj igre! Rezultati:\n");
			for (int i = 0; i < num_players; i++) {
				printf("Igrac %s: %d bodova\n", players[i].ime, players[i].bodovi);
			}

			int max_bodovi = players[0].bodovi;
			char pobjednik[20];
			strcpy(pobjednik, players[0].ime);
			for (int i = 1; i < num_players; i++) {
				if (players[i].bodovi > max_bodovi) {
					max_bodovi = players[i].bodovi;
					strcpy(pobjednik, players[i].ime);
				}
			}
			printf("Pobjednik je %s sa %d bodova!\n", pobjednik, max_bodovi);
			upisiHighscore(pobjednik, max_bodovi);
			igra_zavrsena = 1;
		}
	}
}

void save_game(igrac players[], int num_players) {
	FILE* file = fopen("savefile.txt", "w");
	if (file == NULL) {
		perror("Error opening file");
		return;
	}
	for (int i = 0; i < num_players; i++) {
		fprintf(file, "%s %d %d %d\n", players[i].ime, players[i].polje, players[i].bodovi, players[i].gotov);
	}
	if (ferror(file)) {
		perror("Error writing to file");
	}
	fclose(file);
}

int load_game(igrac players[], int* num_players) {
	FILE* file = fopen("savefile.txt", "r");
	if (file == NULL) {
		perror("Error opening file");
		return -1;
	}
	*num_players = 0;
	while (fscanf(file, "%s %d %d %d", players[*num_players].ime, &players[*num_players].polje, &players[*num_players].bodovi, &players[*num_players].gotov) == 4) {
		(*num_players)++;
	}
	if (feof(file)) {
		printf("End of file reached.\n");
	}
	if (ferror(file)) {
		perror("Error reading from file");
	}
	fclose(file);
	return 0;
}

void ispisiHighscore() {
	FILE* file = fopen("highscore.txt", "r");
	if (file == NULL) {
		perror("Error opening file");
		printf("Nema prijasnjih igara.\n");
		return;
	}

	char ime[20];
	int bodovi;
	int max_bodovi = 0;
	char najbolji_igrac[20];
	int postoji_rezultati = 0;

	printf("Pobjednici do sada:\n");
	while (fscanf(file, "%s %d", ime, &bodovi) == 2) {
		printf("Igrac: %s, Bodovi: %d\n", ime, bodovi);
		postoji_rezultati = 1;
		if (bodovi > max_bodovi) {
			max_bodovi = bodovi;
			strcpy(najbolji_igrac, ime);
		}
	}

	if (postoji_rezultati) {
		printf("\nNajbolji igrac do sada je %s sa %d bodova.\n", najbolji_igrac, max_bodovi);
	}

	if (ferror(file)) {
		perror("Error reading from file");
	}
	fclose(file);
}

void izbrisiHighscore() {
	FILE* file = fopen("highscore.txt", "w");
	if (file == NULL) {
		perror("Error opening file");
		return;
	}
	fclose(file);
}

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

int upisiBrojIgraca() {
	char input[10];  // Buffer
	int broj_Igraca = 0;
	while (1) {
		printf("Unesite broj igraca (2-4): ");
		scanf("%s", input);
		if (valid_input(input)) {
			sscanf(input, "%d", &broj_Igraca);
			if (broj_Igraca >= 2 && broj_Igraca <= 4) {
				break;
			}
			else {
				printf("Broj igraca mora biti izmedu 2 i 4!\n");
			}
		}
		else {
			printf("Pogresan unos, upisite broj izmedu 2 i 4!\n");
		}
	}
	return broj_Igraca;
}