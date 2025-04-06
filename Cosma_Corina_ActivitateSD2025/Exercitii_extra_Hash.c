#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Comanda Comanda;
typedef struct Nod Nod;
typedef struct HashTable HashTable;

struct Comanda {
	unsigned int id;
	char* numeClient;
	char* categorieProduse;
	float valoare;
	int nrProduse;
};

struct Nod {
	Comanda info;
	Nod* next;
};

struct HashTable {
	int dim;
	Nod** vector;
};

Comanda initializareComanda(unsigned int id, char* numeClient, char* categorieProduse, float valoare, int nrProduse) {
	Comanda c;
	c.id = id;
	c.numeClient = (char*)malloc(sizeof(char) * strlen(numeClient) + 1);
	strcpy_s(c.numeClient, sizeof(char) * strlen(numeClient) + 1, numeClient);
	c.categorieProduse = (char*)malloc(sizeof(char) * strlen(categorieProduse) + 1);
	strcpy_s(c.categorieProduse, sizeof(char) * strlen(categorieProduse) + 1, categorieProduse);
	c.valoare = valoare;
	c.nrProduse = nrProduse;

	return c;
}

void afisareComanda(Comanda c) {
	printf("Comanda: %u, %s. %s. %.2f, %d", c.id, c.numeClient, c.categorieProduse, c.valoare, c.nrProduse);
	printf("\n");
}

int calculareHash(HashTable tabela, const char* numeClient) {
	return strlen(numeClient) % tabela.dim;
}

int inserareTabela(HashTable tabela, Comanda c) {
	int poz = 0;
	if (tabela.vector) {
		poz = calculareHash(tabela, c.numeClient);
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = initializareComanda(c.id, c.numeClient, c.categorieProduse, c.valoare, c.nrProduse);
		nou->next = NULL;
		if (tabela.vector[poz] == NULL) {
			tabela.vector[poz] = nou;
		}
		else {
			Nod* aux = tabela.vector[poz];
			while (aux->next != NULL) {
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
			printf("Pozitie: %d\n", i);
			if (tabela.vector[i]) {
				Nod* aux = tabela.vector[i];
				while (aux) {
					afisareComanda(aux->info);
					aux = aux->next;
				}
			}
		}
	}
}


HashTable citireComandaDinFisier(const char* numeFisier, int dim) {
	FILE* f = fopen(numeFisier, "r");
	HashTable ht;
	ht.dim = dim;
	ht.vector = (Nod**)malloc(sizeof(Nod) * dim);
	for (int i = 0; i < dim; i++) {
		ht.vector[i] = NULL;
	}
	while (!feof(f)) {
		char buffer[100];
		char sep[3] = ",\n";
		fgets(buffer, 100, f);
		char* aux;
		Comanda c;
		aux = strtok(buffer, sep);
		c.id = atoi(aux);
		aux = strtok(NULL, sep);
		c.numeClient = (char*)malloc(strlen(aux) + 1);
		strcpy_s(c.numeClient, strlen(aux) + 1, aux);
		aux = strtok(NULL, sep);
		c.categorieProduse = (char*)malloc(strlen(aux) + 1);
		strcpy_s(c.categorieProduse, strlen(aux) + 1, aux);
		aux = strtok(NULL, sep);
		c.valoare = atof(aux);
		aux = strtok(NULL, sep);
		c.nrProduse = atoi(aux);
		inserareTabela(ht, c);

		free(c.numeClient);
		free(c.categorieProduse);
	}
	fclose(f);
	return ht;
}

void dezalocare(HashTable hash) {
	if (hash.vector) {
		for (int i = 0; i < hash.dim; i++) {
			Nod* aux = hash.vector[i];
			while (aux) {
				free(aux->info.numeClient);
				free(aux->info.categorieProduse);
				Nod* temp = aux->next;
				free(aux);
				aux = temp;
			}
		}
		free(hash.vector);
	}
}

int nrComenziLansateDeAcelasiClient(HashTable tabela, const char* numeClient) {
	if (tabela.vector) {
		int count = 0;
		for (int i = 0; i < tabela.dim; i++) {
			Nod* aux = tabela.vector[i];
			while (aux) {
				if (strcmp(aux->info.numeClient, numeClient) == 0) {
					count++;
				}
				aux = aux->next;
			}
		}
		return count;
	}
}

void stergereComenziCategorie(HashTable tabela, const char* numeCategorie) {
	if (tabela.vector) {
		for (int i = 0; i < tabela.dim; i++) {
			Nod* aux = tabela.vector[i];
			Nod* prec = NULL;
			while (aux) {
				if (strcmp(aux->info.categorieProduse, numeCategorie) == 0) {
					Nod* temp = aux->next;
					free(aux->info.categorieProduse);
					free(aux->info.numeClient);
					free(aux);
					if (prec == NULL) {
						tabela.vector[i] = temp;
					}
					else {
						prec->next = temp;
					}
					aux = temp;
				}
				aux = aux->next;
			}
		}
	}
}

float* calculeazaPreturiMediiPerCluster(HashTable tabela, int* nrClustere) {
	float* medii = (float*)malloc(sizeof(float)*tabela.dim);
	*nrClustere = 0;
	if (tabela.vector) {
		for (int i = 0; i < tabela.dim; i++) {
			Nod* aux = tabela.vector[i];
			if (aux != NULL) {
				int count = 0;
				float suma = 0;
				while (aux) {
					suma += aux->info.valoare;
					count++;
					aux = aux->next;
				}
				medii[*nrClustere] = suma / count;
				(*nrClustere)++;
			}
		}
		return medii;
	}
}


void main() {
	HashTable tabela = citireComandaDinFisier("comenzi.txt", 10);
	afisareHash(tabela);
	int nrComeneziLansate = nrComenziLansateDeAcelasiClient(tabela, "Ana");
	printf("Nr de comenzi lansate de Ana este de: %d", nrComeneziLansate);
	printf("\nStergere pentru o anumita categorie\n");
	stergereComenziCategorie(tabela, "Legume");
	afisareHash(tabela);
	int nrClustere = 0;
	float* medii = calculeazaPreturiMediiPerCluster(tabela, &nrClustere);
	printf("Numarul de clustere este: %d\n", nrClustere);
	for (int i = 0; i < nrClustere; i++) {
		printf("Clusterul %d are valoarea medie %.2f\n", i + 1, medii[i]);
	}

	//dezalocare(tabela);
}
