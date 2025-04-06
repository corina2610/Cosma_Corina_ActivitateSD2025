#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

typedef struct Nod Nod;
typedef struct Comanda Comanda;

struct Comanda {
	int id;
	char* numeCumparator;
	char* categorie;
	float greutate;
	int nrProduse;
	float* preturi;
};

struct Nod {
	Comanda info;
	Nod* next;
};

Comanda creeareComanda(int id, char* numeCumparator, char* categorie, float greutate, int nrProduse, float* preturi) {
	Comanda c;
	c.id = id;
	c.numeCumparator = (char*)malloc(sizeof(char) * strlen(numeCumparator) + 1);
	strcpy_s(c.numeCumparator, sizeof(char) * strlen(numeCumparator) + 1, numeCumparator);
	c.categorie = (char*)malloc(sizeof(char) * strlen(categorie) + 1);
	strcpy_s(c.categorie, sizeof(char) * strlen(categorie) + 1, categorie);
	c.greutate = greutate;
	c.nrProduse = nrProduse;
	c.preturi = (float*)malloc(sizeof(float) * c.nrProduse);
	for (int i = 0; i < c.nrProduse; i++) {
		c.preturi[i] = preturi[i];
	}
	return c;
}

void afisareComada(Comanda c) {
	printf("Comanda: %d, %s, %s, %.2f, %d ", c.id, c.numeCumparator, c.categorie, c.greutate, c.nrProduse);
	for (int i = 0; i < c.nrProduse; i++) {
		printf(", %.2f", c.preturi[i]);
	}
	printf("\n");
}

Nod* inserareListaInceput(Nod* cap, Comanda c) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = creeareComanda(c.id, c.numeCumparator, c.categorie, c.greutate, c.nrProduse, c.preturi);
	nou->next = NULL;

	if (cap == NULL) {
		cap = nou;
	}
	else {
		nou->next = cap;
		cap = nou;
	}
	return cap;
}

Nod* inserareListaSfarsit(Nod* cap, Comanda c) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = creeareComanda(c.id, c.numeCumparator, c.categorie, c.greutate, c.nrProduse, c.preturi);
	nou->next = NULL;

	if (cap == NULL) {
		cap = nou;
	}
	else {
		Nod* temp = cap;
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = nou;
	}
	return cap;
}

void afisareLista(Nod* cap) {
	if (cap) {
		while (cap) {
			afisareComada(cap->info);
			cap = cap->next;
		}
	}
}

Nod* citireListaFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* cap = NULL;
	if (f) {
		while (!feof(f)) {
			char buffer[100];
			char sep[3] = ",\n";
			fgets(buffer, 100, f);
			char* aux;
			Comanda c;
			aux = strtok(buffer, sep);
			c.id = atoi(aux);
			aux = strtok(NULL, sep);
			c.numeCumparator = (char*)malloc(strlen(aux) + 1);
			strcpy_s(c.numeCumparator, strlen(aux) + 1, aux);
			aux = strtok(NULL, sep);
			c.categorie = (char*)malloc(strlen(aux) + 1);
			strcpy_s(c.categorie, strlen(aux) + 1, aux);
			aux = strtok(NULL, sep);
			c.greutate = atof(aux);
			aux = strtok(NULL, sep);
			c.nrProduse = atoi(aux);
			aux = strtok(NULL, sep);
			c.preturi = (float*)malloc(sizeof(float) * c.nrProduse);
			for (int i = 0; i < c.nrProduse; i++) {
				c.preturi[i] = aux[i];
				aux = strtok(NULL, sep);
			}

			cap = inserareListaSfarsit(cap, c);
		}
	}
	fclose(f);
	return cap;
}

float calculPretCumparaturiPerCumparator(Nod* cap, const char* numeCumparator) {
	if (cap) {
		float suma = 0;
		int count = 0;
		while (cap) {
			if (strcmp(cap->info.numeCumparator, numeCumparator) == 0) {
				count++;
			}
		
			if (strcmp(cap->info.numeCumparator, numeCumparator) == 0) {
				for (int i = 0; i < cap->info.nrProduse; i++) {
					suma += cap->info.preturi[i];
				}
			}
			cap = cap->next;
		}

		float medie = suma / count;
		return medie;
	}
}

void main() {
	Nod* cap = citireListaFisier("comenziSimpla.txt");
	afisareLista(cap);
	float mediePretCumparaturi = calculPretCumparaturiPerCumparator(cap, "Maria");
	printf("In medie, cumparaturile lui Maria au costat: %.2f\n", mediePretCumparaturi);
}