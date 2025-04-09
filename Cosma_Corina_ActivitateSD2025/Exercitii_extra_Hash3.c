#define _CRT_SECURE_NO_WARNINGS
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Magazin Magazin;
typedef struct Nod Nod;
typedef struct HashTable HashTable;

struct Magazin {
	int id;
	char* nume;
	int suprafata;
	int nrAngajati;
	float vanzari;
};

struct Nod {
	Magazin info;
	Nod* next;
};

struct HashTable {
	int dim;
	Nod** vector;
};

Magazin initializare(int id, char* nume, int suprafata, int nrAngajati, float vanzari) {
	Magazin m;
	m.id = id;
	m.nume = (char*)malloc(sizeof(char) * strlen(nume) + 1);
	strcpy_s(m.nume, sizeof(char) * strlen(nume) + 1, nume);
	m.suprafata = suprafata;
	m.nrAngajati = nrAngajati;
	m.vanzari = vanzari;
	return m;
}

void afisareMagazin(Magazin m) {
	printf("Magazinul: %d, %s, %d, %d, %.2f", m.id, m.nume, m.suprafata, m.nrAngajati, m.vanzari);
}

int calculHash(HashTable tabela, int nrAngajati) {
	return nrAngajati % tabela.dim;
}

int initializareHash(HashTable tabela, Magazin m) {
	if (tabela.vector) {
		int poz = calculHash(tabela, m.nrAngajati);
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = initializare(m.id, m.nume, m.suprafata, m.nrAngajati, m.vanzari);
		nou->next = NULL;
		if (tabela.vector[poz] == NULL) {
			tabela.vector[poz] = nou;
		}
		else {
			Nod* aux = tabela.vector[poz];
			while (aux->next) {
				aux = aux->next;
			}
			aux->next = nou;
		}
		return poz;
	}
}

void afisareHash(HashTable tabela) {
	if (tabela.vector) {
		for (int i = 0; i < tabela.dim; i++) {
			Nod* aux = tabela.vector[i];
			while (aux) {
				afisareMagazin(aux->info);
				aux = aux->next;
			}
		}
	}
}

HashTable citireDinFisier(const char* numeFisier, int dim) {
	FILE* f = fopen(numeFisier, "r");
	HashTable tabela;
	tabela.dim = dim;
	tabela.vector = (Nod**)malloc(sizeof(Nod) * dim);
	for (int i = 0; i < tabela.dim; i++) {
		tabela.vector[i] = NULL;
	}
	if (f) {
		while (!feof(f)) {
			char buffer[100];
			char sep[3] = ",\n";
			fgets(buffer, 100, f);
			char* aux;
			Magazin m;
			aux = strtok(buffer, sep);
			m.id = atoi(aux);
			aux = strtok(NULL, sep);
			m.nume = (char*)malloc(strlen(aux) + 1);
			strcpy_s(m.nume, strlen(aux) + 1, aux);
			aux = strtok(NULL, sep);
			m.suprafata = atoi(aux);
			aux = strtok(NULL, sep);
			m.nrAngajati = atoi(aux);
			aux = strtok(NULL, sep);
			m.vanzari = atof(aux);

			initializareHash(tabela, m);
			free(m.nume);
		}
		fclose(f);
		return tabela;
	}
}

int main() {
	HashTable tabela = citireDinFisier("magazine.txt", 5);
	afisareHash(tabela);
}