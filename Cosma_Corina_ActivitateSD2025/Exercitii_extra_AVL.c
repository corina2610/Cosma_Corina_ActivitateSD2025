#define _CRT_SECURE_NO_WARNINGS
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Rezervare Rezervare;
typedef struct Nod Nod;

struct Rezervare {
	unsigned int id;
	char* hotel;
	unsigned char nrCamere;
	char* client;
	float suma;
};

Rezervare creeareRezervare(unsigned int id, char* hotel, unsigned char nrCamere, char* client, float suma) {
	Rezervare r;
	r.id = id;
	r.hotel = (char*)malloc(sizeof(char) * strlen(hotel) + 1);
	strcpy_s(r.hotel, sizeof(char) * strlen(hotel) + 1, hotel);
	r.nrCamere = nrCamere;
	r.client = (char*)malloc(sizeof(char) * strlen(client) + 1);
	strcpy_s(r.client, sizeof(char) * strlen(client) + 1, client);
	r.suma = suma;
	return r;
}

struct Nod {
	Rezervare info;
	Nod* stanga;
	Nod* dreapta;
};

Nod* creeareNod(Rezervare r, Nod* stanga, Nod* dreapta) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = creeareRezervare(r.id, r.hotel, r.nrCamere, r.client, r.suma);
	nou->stanga = stanga;
	nou->dreapta = dreapta;

	return nou;
}

Nod* inserareArb(Nod* radacina, Rezervare r) {
	if (radacina) {
		if (r.id < radacina->info.id) {
			radacina->stanga = inserareArb(radacina->stanga, r);
			return radacina;
		}
		if (r.id > radacina->info.id) {
			radacina->dreapta = inserareArb(radacina->dreapta, r);
			return radacina;
		}
		else return radacina;
	}
	else {
		return creeareNod(r, NULL, NULL);
	}
}

//preordine - RSD

void preordine(Nod* rad) {
	if (rad) {
		printf("%u, %s, %u, %s, %.2f\n", rad->info.id, rad->info.hotel, rad->info.nrCamere, rad->info.client, rad->info.suma);
		preordine(rad->stanga);
		preordine(rad->dreapta);
	}
}

//inordine -> SRD
void inordine(Nod* rad) {
	if (rad) {
		inordine(rad->stanga);
		printf("%u, %s, %u, %s, %.2f\n", rad->info.id, rad->info.hotel, rad->info.nrCamere, rad->info.client, rad->info.suma);
		inordine(rad->dreapta);
	}
}

//postordine -> SDR
void postordine(Nod* rad) {
	if (rad) {
		postordine(rad->dreapta);
		postordine(rad->stanga);
		printf("%u, %s, %u, %s, %.2f\n", rad->info.id, rad->info.hotel, rad->info.nrCamere, rad->info.client, rad->info.suma);
	}
}

void dezalocare(Nod* rad) {
	if (rad) {
		Nod* arbst = rad->stanga;
		Nod* arbdr = rad->dreapta;
		free(rad->info.hotel);
		free(rad->info.client);
		free(rad);
		dezalocare(arbst);
		dezalocare(arbdr);
	}
}

int maxim(int a, int b) {
	return a > b ? a : b;
}

int nrNiveluri(Nod* rad) {
	if (rad) {
		return 1 + maxim(nrNiveluri(rad->stanga), nrNiveluri(rad->dreapta));
	}
}

void main() {
	Rezervare r;
	Nod* rad = NULL;

	FILE* f = fopen("rezervari.txt", "r");
	if (f) {
		while (!feof(f)) {
			char linieBuffer[100];
			char sep[3] = ",\n";
			fgets(linieBuffer, 100, f);
			char* aux;
			aux = strtok(linieBuffer, sep);
			r.id = atoi(aux);
			aux = strtok(NULL, sep);
			r.hotel = (char*)malloc(strlen(aux) + 1);
			strcpy_s(r.hotel, strlen(aux) + 1, aux);
			aux = strtok(NULL, sep);
			r.nrCamere = atoi(aux);
			aux = strtok(NULL, sep);
			r.client = (char*)malloc(strlen(aux) + 1);
			strcpy_s(r.client, strlen(aux) + 1, aux);
			aux = strtok(NULL, sep);
			r.suma = atof(aux);

			rad = inserareArb(rad, r);
			free(r.client);
			free(r.hotel);
		}
	}
	fclose(f);
	inordine(rad);

	int nr = nrNiveluri(rad);
	printf("\n%d\n", nr);
}
