#define _CRT_SECURE_NO_WARNINGS
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Florarie Florarie;
typedef struct Nod Nod;
typedef struct HashTable HashTable;

struct Florarie {
	int nr;
	char* denumire;
	int nrSortimente;
	int nrCumparatori;
	float vanzari;
};

struct Nod {
	Florarie info;
	Nod* next;
};

struct HashTable {
	int dim;
	Nod** vector;
};

Florarie initializare(int nr, char* denumire, int nrSortimente, int nrCumparatori, float vanzari) {
	Florarie f;
	f.nr = nr;
	f.denumire = (char*)malloc(sizeof(char) + strlen(denumire) + 1);
	strcpy_s(f.denumire, sizeof(char) + strlen(denumire) + 1, denumire);
	f.nrSortimente = nrSortimente;
	f.nrCumparatori = nrCumparatori;
	f.vanzari = vanzari;
	return f;
}
	
void afisareFlorarie(Florarie f) {
	printf("Floraria: %d, %s, %d, %d, %.2f\n", f.nr, f.denumire, f.nrSortimente, f.nrCumparatori, f.vanzari);
}

int calculareHash(HashTable tabela, const char* denumire) {
	return strlen(denumire) % tabela.dim;
}

int inserareInTable(HashTable tabela, Florarie f) {
	int poz = 0;
	if (tabela.vector) {
		poz = calculareHash(tabela, f.denumire);
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = initializare(f.nr, f.denumire, f.nrSortimente, f.nrCumparatori, f.vanzari);
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
			printf("Pozitie: %d: \n", i);
			Nod* aux = tabela.vector[i];
			while (aux) {
				afisareFlorarie(aux->info);
				aux = aux->next;
			}
		}
	}
}

HashTable citireDinFisier(const char* numeFisier, int dim) {
	FILE* file = fopen(numeFisier, "r");
	HashTable tabela;
	tabela.dim = dim;
	tabela.vector = (Nod**)malloc(sizeof(Nod) * dim);
	for (int i = 0; i < tabela.dim; i++) {
		tabela.vector[i] = NULL;
	}
	if (file) {
		while (!feof(file)) {
		char buffer[100];
		char sep[3] = ",\n";
		fgets(buffer, 100, file);
		char* aux;
		Florarie f;
		aux = strtok(buffer,sep);
		f.nr = atoi(aux);
		aux = strtok(NULL, sep);
		f.denumire = (char*)malloc(strlen(aux) + 1);
	    strcpy_s(f.denumire, strlen(aux) + 1, aux);
		aux = strtok(NULL, sep);
		f.nrSortimente = atoi(aux);
		aux = strtok(NULL, sep);
		f.nrCumparatori = atoi(aux);
		aux = strtok(NULL, sep);
		f.vanzari = atof(aux);

		inserareInTable(tabela, f);

		free(f.denumire);
		}
		fclose(file);
		return tabela;
	}
}

int main() {
	HashTable tabela = citireDinFisier("florarie.txt",5);
	afisareHash(tabela);
}