#define _CRT_SECURE_NO_WARNINGS
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Comanda Comanda;
typedef struct Nod Nod;
typedef struct LD LD;

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
	Nod* prev;
};

struct LD {
	Nod* prim;
	Nod* ultim;
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

void afisareComanda(Comanda c) {
	printf("Comanda: %d, %s, %s, %.2f, %d ", c.id, c.numeCumparator, c.categorie, c.greutate, c.nrProduse);
	for (int i = 0; i < c.nrProduse; i++) {
	printf(", %.2f", c.preturi[i]);
	}
	printf("\n");
}

LD inserareLaInceput(LD lista, Comanda c) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = creeareComanda(c.id, c.numeCumparator, c.categorie, c.greutate, c.nrProduse, c.preturi);
	nou->next = NULL;
	nou->prev = NULL;

	if (lista.prim == NULL) {
		lista.prim = lista.ultim = nou;
	}
	else {
		lista.prim->prev = nou;
		nou->next = lista.prim;
		lista.prim = nou;
	}
	return lista;
}

LD inserareLaFinal(LD lista, Comanda c) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = creeareComanda(c.id, c.numeCumparator, c.categorie, c.greutate, c.nrProduse, c.preturi);
	nou->next = NULL;
	nou->prev = NULL;

	if (lista.prim == NULL) {
		lista.prim = lista.ultim = nou;
	}
	else {
		lista.ultim->next = nou;
		nou->prev = lista.ultim;
		lista.ultim = nou;
	}
	return lista;
}

void afisareLista(LD lista) {
	Nod* aux = lista.prim;
	while (aux) {
		afisareComanda(aux->info);
		aux = aux->next;
	}
}

LD citireDinFisier(const char* numeFisier) {
	LD lista;
	lista.prim = NULL;
	lista.ultim = NULL;
	FILE* f = fopen(numeFisier, "r");
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
			aux = strtok(NULL, sep);
			for (int i = 0; i < c.nrProduse; i++) {
				c.preturi[i] = atof(aux);
			}

			lista = inserareLaFinal(lista, c);
		}
		fclose(f);
		return lista;
	}
}

void dezalocareLista(LD lista) {
	Nod* aux = lista.prim;
	while (aux) {
		free(aux->info.numeCumparator);
		free(aux->info.categorie);
		Nod* temp = aux->next;
		free(aux);
		aux = temp;
	}
}

int main() {
	LD lista = citireDinFisier("comenziSimpla.txt");
	afisareLista(lista);
}
