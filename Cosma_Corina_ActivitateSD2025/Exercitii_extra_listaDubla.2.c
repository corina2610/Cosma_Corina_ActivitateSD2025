#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<malloc.h>
#include<string.h>

typedef struct Client Client;
typedef struct Nod Nod;
typedef struct LD LD;

typedef struct Client {
	unsigned int id;
	char* nume;
	int nrProduse;
	float* pret;
};

typedef struct Nod {
	Nod* prev;
	Client info;
	Nod* next;
};

typedef struct LD {
	Nod* prim;
	Nod* ultim;
};

Client initializareClient(unsigned int id, const char* nume, int nrProduse, float* pret) {
	Client c;
	c.id = id;
	c.nume = (char*)malloc(sizeof(nume) * (strlen(nume) + 1));
	strcpy_s(c.nume, (strlen(nume) + 1), nume);
	c.nrProduse = nrProduse;
	c.pret = (float*)malloc(sizeof(float) * nrProduse);
	for (int i = 0; i < nrProduse; i++) {
		c.pret[i] = pret[i];
	}
	return c;
}

//inserareLaInceput
LD inserareLaInceput(LD lista, Client c) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = initializareClient(c.id, c.nume, c.nrProduse, c.pret);
	nou->next = NULL;

	if (lista.prim == NULL) {
		lista.prim = lista.ultim = nou;
	}
	else {
		nou->next = lista.prim;
		lista.prim->prev = nou;
		lista.prim = nou;
	}
	return lista;
}

LD inserareLaFinal(LD lista, Client c) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = initializareClient(c.id, c.nume, c.nrProduse, c.pret);
	nou->next = NULL;

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

void traversareListaInceput(LD lista) {
	Nod* aux = lista.prim;
	while (aux) {
		printf("%u %s %d ", aux->info.id, aux->info.nume, aux->info.nrProduse);
		for (int i = 0; i < aux->info.nrProduse; i++) {
			printf("%5.2f ", aux->info.pret[i]);
		}
		printf("\n");
		aux = aux->next;
	}
}

void traversareListaFinal(LD lista) {
	Nod* aux = lista.ultim;
	while (aux) {
		printf("%u %s %d ", aux->info.id, aux->info.nume, aux->info.nrProduse);
		for (int i = 0; i < aux->info.nrProduse; i++) {
			printf("%5.2f ", aux->info.pret[i]);
		}
		printf("\n");
		aux = aux->prev;
	}
}

void dezalocare(LD lista) {
	Nod* aux = lista.prim;
	while (aux) {
		free(aux->info.nume);
		free(aux->info.pret);
		Nod* temp = aux->next;
		free(aux);
		aux = temp;
	}
}

void main() {
	int nr;
	Client c;
	char buffer[50];
	LD lista;
	lista.prim = NULL;
	lista.ultim = NULL;

	FILE* pfile = fopen("fisier.txt", "r");
	fscanf(pfile, "%d", &nr);
	for (int i = 0; i < nr; i++) {
		fscanf(pfile, "%u", &c.id);

		fscanf(pfile, "%s", buffer); 
		c.nume = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
		strcpy(c.nume, buffer);

		fscanf(pfile, "%d", &c.nrProduse);
		c.pret = (float*)malloc(sizeof(float) * c.nrProduse);
		for (int i = 0; i < c.nrProduse; i++) {
			fscanf(pfile, "%f", &c.pret[i]);
		}
		lista = inserareLaInceput(lista, c);

	}

	fclose(pfile);

	traversareListaInceput(lista);
}
