#define _CRT_SECURE_NO_WARNINGS
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Corina Cosma -> CC -> CresCatorie

typedef struct Crescatorie Crescatorie;
typedef struct Nod Nod;
typedef struct LD LD;

typedef struct Crescatorie {
	int cod;
	char* tipAnimale;
	int pretAnimal;
	float costCrestere;
	int nrAnimale;
	float* greutate;
};

typedef struct Nod {
	Nod* prev;
	Crescatorie info;
	Nod* next;
};

typedef struct LD {
	Nod* prim;
	Nod* ultim;
};
Crescatorie creearecrescatorie(int cod, char* tipAnimale, int pretAnimal, float costCrestere, int nrAnimale, float* greutate) {
	Crescatorie c;
	c.cod = cod;
	c.tipAnimale = (char*)malloc(sizeof(char) * strlen(tipAnimale) + 1);
	strcpy_s(c.tipAnimale, (strlen(tipAnimale) + 1), tipAnimale);
	c.pretAnimal = pretAnimal;
	c.costCrestere = costCrestere;
	c.nrAnimale = nrAnimale;
	c.greutate = (float*)malloc(sizeof(float) * c.nrAnimale);
	for (int i = 0; i < c.nrAnimale; i++) {
		c.greutate[i] = greutate[i];
	}
	return c;
}

Crescatorie citireAnimalDinFisier(FILE* f) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, f);
	char* aux;
	Crescatorie c;
	aux = strtok(buffer, sep);
	c.cod = atoi(aux);
	aux = strtok(NULL, sep);
	c.tipAnimale = (char*)malloc(strlen(aux) + 1);
	strcpy_s(c.tipAnimale, (strlen(aux) + 1), aux);
	aux = strtok(NULL, sep);
	c.pretAnimal = atoi(aux);
	aux = strtok(NULL, sep);
	c.costCrestere = atof(aux);
	aux = strtok(NULL, sep);
	c.nrAnimale = atoi(aux);
	aux = strtok(NULL, sep);
	c.greutate = (float*)malloc(sizeof(int) * c.nrAnimale);
	for (int i = 0; i < c.nrAnimale; i++) {
		c.greutate[i] = atoi(aux);
	}

	return c;
}

void afisare(Crescatorie c) {
		printf("Crescatorie - Cod: %d, Denumire: %s, Pret/Animal: %d, Cost crestere: %.2f, Nr Animale: %d, ", c.cod, c.tipAnimale, c.pretAnimal, c.costCrestere, c.nrAnimale);
		printf(" Greutate animal: ");
		for (int i = 0; i < c.nrAnimale; i++) {
			printf(" %.2f ", c.greutate[i]);
		}
		printf("\n");
}

void adaugaLaInceput(LD* lista, Crescatorie c) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = creearecrescatorie(c.cod, c.tipAnimale, c.pretAnimal, c.costCrestere, c.nrAnimale, c.greutate);
	nou->next = NULL;
	nou->prev = NULL;

	if (lista->prim == NULL) {
		lista->prim = lista->ultim = nou;
	}
	else {
		nou->next = lista->prim;
		lista->prim->prev = nou;
		lista->prim = nou;
	}
}

void adaugaLaSfarsit(LD* lista, Crescatorie c) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = creearecrescatorie(c.cod, c.tipAnimale, c.pretAnimal, c.costCrestere, c.nrAnimale, c.greutate);
	nou->next = NULL;
	nou->prev = NULL;

	if (lista == NULL) {
		lista->prim = lista->ultim = nou;
	}
	else {
		lista->ultim->next = nou;
		nou->prev = lista->ultim;
		lista->ultim = nou;
	}
}

LD citireListaDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	LD lista;
	lista.prim = NULL;
	lista.ultim = NULL;
	if (f) {
		while (!feof(f)) {
			adaugaLaInceput(&lista, citireAnimalDinFisier(f));
		}
	}
	fclose(f);
	return lista;
}

void afisareListaI(LD lista) {
	Nod* l = lista.prim;
	while (l) {
		afisare(l->info);
		l = l->next;
	}
}

void afisareListaF(LD lista) {
	Nod* l = lista.ultim;
	while (l) {
		afisare(l->info);
		l = l->prev;
	}
}

int main() {
	LD lista = citireListaDinFisier("crescatorie.txt");
	afisareListaI(lista);
}