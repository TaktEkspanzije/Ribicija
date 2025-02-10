#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <ctype.h>
#include "strukture.h"

// Globalna varijabla definirana u main.c
extern igrac* players;
// Globalna varijabla za jedinstveni broj igre (definirana i u main.c)
extern int CurrentUniqueId;

//------------------------------------------------------------------------------
// NOVO: Funkcija CompareGameSaves – koristi se kao usporedna funkcija za qsort() i bsearch()
int CompareGameSaves(const void* a, const void* b) {
    const GameSave* gs1 = (const GameSave*)a;
    const GameSave* gs2 = (const GameSave*)b;
    return gs1->UniqueId - gs2->UniqueId;
}

//------------------------------------------------------------------------------
// NOVO: SaveGameUnified – sprema trenutno stanje igre u jedinstveni file "saves.txt".
// Ako igra s istim UniqueId-om već postoji, zapis se ažurira; inače se dodaje novi.
void SaveGameUnified(igrac* players, int numPlayers, int uniqueId) {
    FILE* file = fopen("saves.txt", "r");
    int count = 0;
    GameSave* saves = NULL;
    if (file != NULL) {
        while (!feof(file)) {
            GameSave temp;
            if (fscanf(file, "%d %d", &temp.UniqueId, &temp.NumPlayers) == 2) {
                for (int i = 0; i < temp.NumPlayers; i++) {
                    fscanf(file, "%s %d %d %d", temp.Players[i].ime,
                        &temp.Players[i].polje,
                        &temp.Players[i].bodovi,
                        &temp.Players[i].gotov);
                }
                saves = (GameSave*)realloc(saves, (count + 1) * sizeof(GameSave));
                if (saves == NULL) {
                    perror("Memory allocation error");
                    fclose(file);
                    return;
                }
                saves[count] = temp;
                count++;
            }
        }
        fclose(file);
    }

    if (count > 0)
        qsort(saves, count, sizeof(GameSave), CompareGameSaves);

    GameSave key;
    key.UniqueId = uniqueId;
    GameSave* found = (GameSave*)bsearch(&key, saves, count, sizeof(GameSave), CompareGameSaves);
    if (found != NULL) {
        // Ažuriramo postojeći zapis
        found->NumPlayers = numPlayers;
        for (int i = 0; i < numPlayers; i++) {
            found->Players[i] = players[i];
        }
    }
    else {
        // Dodajemo novi zapis
        saves = (GameSave*)realloc(saves, (count + 1) * sizeof(GameSave));
        if (saves == NULL) {
            perror("Memory allocation error");
            return;
        }
        saves[count].UniqueId = uniqueId;
        saves[count].NumPlayers = numPlayers;
        for (int i = 0; i < numPlayers; i++) {
            saves[count].Players[i] = players[i];
        }
        count++;
    }

    file = fopen("saves.txt", "w");
    if (file == NULL) {
        perror("Error opening saves file for writing");
        free(saves);
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d %d ", saves[i].UniqueId, saves[i].NumPlayers);
        for (int j = 0; j < saves[i].NumPlayers; j++) {
            fprintf(file, "%s %d %d %d ", saves[i].Players[j].ime,
                saves[i].Players[j].polje,
                saves[i].Players[j].bodovi,
                saves[i].Players[j].gotov);
        }
        fprintf(file, "\n");
    }

    // Umetnuti ftell() - ispis trenutne pozicije pokazivača u datoteci
    long filePosition = ftell(file);
    if (filePosition == -1) {
        perror("ftell error");
    }
    else {
        printf("Trenutna pozicija u datoteci (ftell): %ld byte-ova.\n", filePosition);
    }
    // Kraj umetanja ftell()

    fclose(file);
    free(saves);
}

//------------------------------------------------------------------------------
// NOVO: LoadGameUnified – učitava spremljenu igru s danim jedinstvenim brojem.
// Ako zapis ne postoji, obavještava korisnika i vraća -1.
int LoadGameUnified(igrac* players, int* numPlayers, int uniqueId) {
    FILE* file = fopen("saves.txt", "r");
    if (file == NULL) {
        perror("Error opening saves file for reading");
        return -1;
    }
    int count = 0;
    GameSave* saves = NULL;
    while (!feof(file)) {
        GameSave temp;
        if (fscanf(file, "%d %d", &temp.UniqueId, &temp.NumPlayers) == 2) {
            for (int i = 0; i < temp.NumPlayers; i++) {
                fscanf(file, "%s %d %d %d", temp.Players[i].ime,
                    &temp.Players[i].polje,
                    &temp.Players[i].bodovi,
                    &temp.Players[i].gotov);
            }
            saves = (GameSave*)realloc(saves, (count + 1) * sizeof(GameSave));
            if (saves == NULL) {
                perror("Memory allocation error");
                fclose(file);
                return -1;
            }
            saves[count] = temp;
            count++;
        }
    }
    fclose(file);

    if (count > 0)
        qsort(saves, count, sizeof(GameSave), CompareGameSaves);

    GameSave key;
    key.UniqueId = uniqueId;
    GameSave* found = (GameSave*)bsearch(&key, saves, count, sizeof(GameSave), CompareGameSaves);
    if (found == NULL) {
        printf("Nema spremljene igre s tim jedinstvenim brojem.\n");
        free(saves);
        return -1;
    }
    else {
        *numPlayers = found->NumPlayers;
        for (int i = 0; i < found->NumPlayers; i++) {
            players[i] = found->Players[i];
        }
    }
    free(saves);
    return 0;
}

//------------------------------------------------------------------------------
// Ostatak postojećih funkcija

int ValidInput(char* input) {
    for (int i = 0; input[i] != '\0'; i++) {
        if (!isdigit(input[i])) {
            return 0;
        }
    }
    return 1;
}

int BacanjeKockeRekurzivno(int rollCount) {
    srand((unsigned int)(time(NULL) + rollCount));
    if (rollCount == 1) {
        return rand() % 6 + 1;
    }
    else {
        return (rand() % 6 + 1) + BacanjeKockeRekurzivno(rollCount - 1);
    }
}

int BacanjeKocke() {
    return BacanjeKockeRekurzivno(1);
}

void NapisiPravilaIgre() {
    printf("\n - \n Dok igras, pokraj sebe imaj fizicku kopiju ili sliku polja igre Ribicija kako bi znao/la gdje se nalaziš u igri, budući da je ovaj program tekstualan. \n Ribicija je igra u kojoj pecate ribu na posebnim poljima – ovisno koju ribu ulovite, dobivate određeni broj bodova. Bodovi određuju pobjednika, a prvi igrač koji dođe do cilja dobiva 10 bonus bodova. \n - \n - ");
}

int LoviRibu() {
    static ribe riba[17];
    int brojRibe = rand() % 16 + 1;

    riba[1].ime = "Pastrva";
    riba[1].tezina = 2.5;
    riba[1].bod = 4;

    riba[2].ime = "Deverika";
    riba[2].tezina = 3.7;
    riba[2].bod = 3;

    riba[3].ime = "Crvenperka";
    riba[3].tezina = 1.2;
    riba[3].bod = 3;

    riba[4].ime = "Tolstolobik";
    riba[4].tezina = 25.3;
    riba[4].bod = 4;

    riba[5].ime = "Saran";
    riba[5].tezina = 13.4;
    riba[5].bod = 4;

    riba[6].ime = "Bolen";
    riba[6].tezina = 6.6;
    riba[6].bod = 4;

    riba[7].ime = "Som";
    riba[7].tezina = 39.5;
    riba[7].bod = 4;

    riba[8].ime = "Smud";
    riba[8].tezina = 4.2;
    riba[8].bod = 6;

    riba[9].ime = "Keciga";
    riba[9].tezina = 7.1;
    riba[9].bod = 4;

    riba[10].ime = "Cvergl";
    riba[10].tezina = 1.0;
    riba[10].bod = -2;

    riba[11].ime = "Amur";
    riba[11].tezina = 11.8;
    riba[11].bod = 5;

    riba[12].ime = "Mrena";
    riba[12].tezina = 2.9;
    riba[12].bod = 3;

    riba[13].ime = "Stuka";
    riba[13].tezina = 15.5;
    riba[13].bod = 7;

    riba[14].ime = "Grgec";
    riba[14].tezina = 1.3;
    riba[14].bod = 3;

    riba[15].ime = "Klen";
    riba[15].tezina = 1.1;
    riba[15].bod = 2;

    riba[16].ime = "Linjak";
    riba[16].tezina = 4.7;
    riba[16].bod = 7;

    if (brojRibe == 10) {
        printf("Ajoj, ulovili ste Cvergla. Ta riba vam oduzima 2 boda :(\n \n");
        return -2;
    }
    printf("Ulovili ste ribu %s težine %.1f! Vrijednost ribe u bodovima: %d\n \n", riba[brojRibe].ime, riba[brojRibe].tezina, riba[brojRibe].bod);
    return riba[brojRibe].bod;
}

void InitGame(int numPlayers, igrac* players) {
    for (int i = 0; i < numPlayers; i++) {
        printf("Unesite ime %d. igrača: ", i + 1);
        scanf("%s", players[i].ime);
        players[i].polje = 0;
        players[i].bodovi = 0;
        players[i].gotov = 0;
    }
}

void PlayGame(igrac* players, int numPlayers) {
    int currentPlayer = 0;
    int posebnaPolja[17] = { 37, 41, 45, 50, 55, 61, 66, 69, 73, 77, 80, 83, 88, 93, 100, 104, 108 };
    int krajIgre = 0;
    int igraZavrsena = 0;

    while (!igraZavrsena) {
        for (int i = 0; i < numPlayers; i++) {
            currentPlayer = i;
            printf("Igrač %s je na polju %d. Želite li baciti kocku ili izaći iz igre? (b/i): ",
                players[currentPlayer].ime, players[currentPlayer].polje);
            char choice[10];
            scanf("%9s", choice);
            char action = choice[0];
            if (action == 'b') {
                int roll = BacanjeKocke();
                players[currentPlayer].polje += roll;
                printf("Bacili ste kocku i dobili broj %d. Sada ste na polju %d.\n \n",
                    roll, players[currentPlayer].polje);

                for (int j = 0; j < numPlayers; j++) {
                    if (j != currentPlayer && players[currentPlayer].polje == players[j].polje) {
                        printf("Igrač %s je pojeo igrača %s i poslao ga na polje 0!\n",
                            players[currentPlayer].ime, players[j].ime);
                        players[j].polje = 0;
                    }
                }

                if (players[currentPlayer].polje >= 111) {
                    printf("Čestitamo! Igrač %s je prvi došao do cilja i dobio 10 bonus bodova!\n",
                        players[currentPlayer].ime);
                    players[currentPlayer].bodovi += 10;
                    krajIgre = 1;
                    break;
                }

                if (players[currentPlayer].polje == 5) {
                    printf("Igrač %s je stao na posebno polje te se pomiče 5 mjesta unaprijed.\n",
                        players[currentPlayer].ime);
                    players[currentPlayer].polje += 5;
                }
                else if (players[currentPlayer].polje == 11) {
                    printf("Igrač %s je stao na posebno polje te se vraća 3 mjesta unazad.\n",
                        players[currentPlayer].ime);
                    players[currentPlayer].polje -= 3;
                }
                else if (players[currentPlayer].polje == 21) {
                    printf("Igrač %s je stao na posebno polje te se pomiče 7 mjesta unaprijed.\n",
                        players[currentPlayer].ime);
                    players[currentPlayer].polje += 7;
                }

                for (int j = 0; j < 17; j++) {
                    if (players[currentPlayer].polje == posebnaPolja[j]) {
                        if (players[currentPlayer].polje == 66) {
                            printf("Pecali ste na zabranjenom mjestu stoga vas vraćamo na start 2!\n");
                            players[currentPlayer].polje = 33;
                        }
                        else if (players[currentPlayer].polje == 83) {
                            printf("Igrač %s je upao/la u čamac koji je otplutovao te ga vratio na polje 56!\n",
                                players[currentPlayer].ime);
                            players[currentPlayer].polje = 56;
                        }
                        else {
                            players[currentPlayer].bodovi += LoviRibu();
                        }
                    }
                }
            }
            else if (action == 'i') {
                // Pri izlasku iz igre spremamo trenutnu igru
                SaveGameUnified(players, numPlayers, CurrentUniqueId);
                printf("Igra je spremljena. Igrač %s je izašao iz igre.\n", players[currentPlayer].ime);
                krajIgre = 0;
                return;
            }
        }

        if (krajIgre) {
            printf("Kraj igre! Rezultati:\n");
            for (int i = 0; i < numPlayers; i++) {
                printf("Igrač %s: %d bodova\n", players[i].ime, players[i].bodovi);
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
            UpisiHighscore(pobjednik, maxBodovi);
            igraZavrsena = 1;
        }
    }
}

void IspisiHighscore(void) {
    FILE* file = fopen("highscore.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        printf("Nema prijašnjih igara.\n");
        return;
    }
    char ime[20];
    int bodovi;
    int maxBodovi = 0;
    char najboljiIgrac[20];
    int postojiRezultati = 0;
    printf("Pobjednici do sada:\n");
    while (fscanf(file, "%s %d", ime, &bodovi) == 2) {
        printf("Igrač: %s, Bodovi: %d\n", ime, bodovi);
        postojiRezultati = 1;
        if (bodovi > maxBodovi) {
            maxBodovi = bodovi;
            strcpy(najboljiIgrac, ime);
        }
    }
    if (postojiRezultati) {
        printf("\nNajbolji igrač do sada je %s sa %d bodova.\n", najboljiIgrac, maxBodovi);
    }
    if (ferror(file)) {
        perror("Error reading from file");
    }
    fclose(file);
}

void UpisiHighscore(const char* ime, int bodovi) {
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

//------------------------------------------------------------------------------
// Funkcije koje su prije bile nedostajuće:

int UpisiBrojIgraca(void) {
    char input[10];
    int brojIgraca = 0;
    while (1) {
        printf("Unesite broj igrača (2-4): ");
        scanf("%s", input);
        if (ValidInput(input)) {
            sscanf(input, "%d", &brojIgraca);
            if (brojIgraca >= 2 && brojIgraca <= 4) {
                break;
            }
            else {
                printf("Broj igrača mora biti između 2 i 4!\n");
            }
        }
        else {
            printf("Pogrešan unos, upišite broj između 2 i 4!\n");
        }
    }
    return brojIgraca;
}

void IzbrisiHighscore(void) {
    if (remove("highscore.txt") == 0) {
        printf("High score datoteka je uspješno izbrisana.\n");
    }
    else {
        perror("Greška pri brisanju highscore datoteke");
    }
}

void SaveAndExit(igrac* players, int brojIgraca) {
    char odgovor[3];
    printf("Da li ste sigurni da želite završiti program? (da / ne) ");
    scanf("%s", odgovor);
    if (strcmp(odgovor, "da") == 0) {
        SaveGameUnified(players, brojIgraca, CurrentUniqueId);
        free(players);
        players = NULL;
        exit(0);
    }
    else if (strcmp(odgovor, "ne") == 0) {
        return;
    }
    else {
        printf("Nepoznata opcija, upiši opet\n");
    }
}
