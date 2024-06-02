#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include "strukture.h"

int main() {
	int brojIgraca;
	igrac players[4];
	char odgovor[3];
	char input[10];  // Buffer to store user input
	IzbornikOpcija choice;

	printf("\n**************************************************\n");
	printf("\n***!****************RIBICIJA******************!***\n");
	printf("\n**************************************************\n");

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
			continue;  // Ask for input again
		}

		switch (choice) {
		case IZBORNIK_IGRAJ:
			brojIgraca = upisiBrojIgraca();
			init_game(brojIgraca, players);
			play_game(players, brojIgraca);
			break;

		case IZBORNIK_NASTAVI:
			if (load_game(players, &brojIgraca) == 0) {
				play_game(players, brojIgraca);
			}
			break;

		case IZBORNIK_PRAVILA:
			NapisiPravilaIgre();
			break;

		case IZBORNIK_HIGHSCORE:
			ispisiHighscore();
			break;

		case IZBORNIK_IZBRISI_HIGHSCORE:
			izbrisiHighscore();
			break;

		case IZBORNIK_IZLAZ:
			printf("Da li ste sigurni da zelite zavrsiti program? (da / ne)");
			scanf("%s", odgovor);
			if (!strcmp(odgovor, "da")) {
				exit(0);
			}
			else if (!strcmp(odgovor, "ne")) {
				break;
			}
			else {
				printf("Nepoznata opcija, upisi opet\n");
			}
			break;

		default:
			printf("Pogresan izbor, upisi broj izmedu 1 i 6.\n");
			break;
		}
	}
	return 0;
}