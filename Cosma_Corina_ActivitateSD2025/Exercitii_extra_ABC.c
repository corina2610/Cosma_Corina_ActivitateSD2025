#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

typedef struct Fotografie Fotografie;
struct Fotografie {
	unsigned int id;
	char* oras;
	char* data;
	float rezolutie;
};

Fotografie initializareFotografie(unsigned int id, char* oras, char* data, float rezolutie){
	Fotografie f;
	f.id = id;
	f.oras = (char*)malloc(sizeof(char) * strlen(oras)+1);
	strcpy_s(f.oras, sizeof(char) * strlen(oras) + 1, oras);
	f.data = (char*)malloc(sizeof(char) * strlen(data) + 1);
	strcpy_s(f.data, sizeof(char) * strlen(data) + 1, data);
	f.rezolutie = rezolutie;
	return f;
}

void afisare(Fotografie f) {
	printf("Fotografia: %u, %s, %s, %.2f\n", f.id, f.oras, f.data, f.rezolutie);
}

typedef struct NodArbore NodArbore;
struct NodArbore {
	Fotografie info;
	NodArbore* stanga;
	NodArbore* dreapta;
};

NodArbore* creeazaArbore(Fotografie f, NodArbore* stanga, NodArbore* dreapta) {
	NodArbore* nou = (NodArbore*)malloc(sizeof(NodArbore));
	nou->info = initializareFotografie(f.id, f.oras, f.data, f.rezolutie);
	nou->stanga = stanga;
	nou->dreapta = dreapta;
	return nou;
}

NodArbore* insereazaNodArbore(NodArbore* radacina, Fotografie f) {
	if (radacina) {
		if (f.id < radacina->info.id) {
			radacina->stanga = insereazaNodArbore(radacina->stanga, f);
			return radacina;
		}
		else if (f.id > radacina->info.id) {
			radacina->dreapta = insereazaNodArbore(radacina->dreapta, f);
			return radacina;
		}
		else {
			return radacina;
		}
	}
	else {
		return creeazaArbore(f, NULL, NULL);
	}
}

void preordine(NodArbore* rad) {
	if (rad) {
		printf("Fotografia: %u, %s, %s, %.2f\n", rad->info.id, rad->info.oras, rad->info.data, rad->info.rezolutie);
		preordine(rad->stanga);
		preordine(rad->dreapta);
	}
}

void inordine(NodArbore* rad) {
	if (rad) {
		inordine(rad->stanga);
		printf("Fotografia: %u, %s, %s, %.2f\n", rad->info.id, rad->info.oras, rad->info.data, rad->info.rezolutie);
		inordine(rad->dreapta);
	}
}

void postordine(NodArbore* rad) {
	if (rad) {
		postordine(rad->stanga);
		postordine(rad->dreapta);
		printf("Fotografia: %u, %s, %s, %.2f\n", rad->info.id, rad->info.oras, rad->info.data, rad->info.rezolutie);
	}
}

void dezalocareArbore(NodArbore* rad) {
	if (rad) {
		NodArbore* stanga = rad->stanga;
		NodArbore* dreapta = rad->dreapta;

		free(rad->info.oras);
		free(rad->info.data);
		free(rad);
		dezalocareArbore(stanga);
		dezalocareArbore(dreapta);
	}
}

void modificaData(NodArbore* rad, char* datainit, char* datanoua) {
	if (rad == NULL) {
		return 0;
	}

	if (strcmp(datainit, rad->info.data) == 0) {
		rad->info.data = (char*)malloc(sizeof(char*) * strlen(datanoua) + 1);
		strcpy_s(rad->info.data, sizeof(char*) * strlen(datanoua) + 1, datanoua);
	}

	modificaData(rad->dreapta, datainit, datanoua);
	modificaData(rad->stanga, datainit, datanoua);
}

int nrPozeInAcelasiOras(const char* numeOras, NodArbore* rad) {
	if (rad == NULL) {
		return 0;
	}
		int count = 0;

		if (strcmp(numeOras, rad->info.oras) == 0) {
			count++;
		}
		count += nrPozeInAcelasiOras(numeOras, rad->stanga);
		count += nrPozeInAcelasiOras(numeOras, rad->dreapta);
		return count;
}

NodArbore* citireFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	NodArbore* rad = NULL;
	if (f) {
		while (!feof(f)) {
			char buffer[500];
			char sep[3] = ",\n";
			fgets(buffer, 500, f);
			char* aux;
			Fotografie f;
			aux = strtok(buffer, sep);
			f.id = atoi(aux);
			aux = strtok(NULL, sep);
			f.oras = (char*)malloc(strlen(aux) + 1);
			strcpy_s(f.oras, strlen(aux) + 1, aux);
			aux = strtok(NULL, sep);
			f.data = (char*)malloc(strlen(aux) + 1);
			strcpy_s(f.data, strlen(aux) + 1, aux);
			aux = strtok(NULL, sep);
			f.rezolutie = atof(aux);

			rad = insereazaNodArbore(rad, f);
			free(f.oras);
			free(f.data);
		}
		fclose(f);
		return rad;
	}
}

int main() {
	NodArbore* rad = citireFisier("fotografii.txt");
	printf("Inordine: \n");
	inordine(rad);
	printf("Preordine: \n");
	preordine(rad);
	printf("Postordine: \n");
	postordine(rad);

	int nrFotografii = nrPozeInAcelasiOras("Timisoara", rad);
	printf("Numarul de poze din Timisoara: %d\n", nrFotografii);

	printf("Modifiare data: \n");
	modificaData(rad, "12/01/2020", "26/10/2003");
	inordine(rad);
}