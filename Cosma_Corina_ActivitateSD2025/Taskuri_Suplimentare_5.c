#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef struct Masina Masina;
typedef struct Nod Nod;

struct Masina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};

struct Nod {
	Masina info;
	Nod* next;
};

Masina creareMasina(int id, int nrUsi, float pret, char* model, char* numeSofer, unsigned char serie) {
	Masina m;
	m.id = id;
	m.nrUsi = nrUsi;
	m.pret = pret;
	m.model = (char*)malloc(sizeof(char) * strlen(model) + 1);
	strcpy_s(m.model, (strlen(model) + 1), model);
	m.numeSofer = (char*)malloc(sizeof(char) * strlen(numeSofer) + 1);
	strcpy_s(m.numeSofer, (strlen(numeSofer) + 1), numeSofer);
	m.serie = serie;

	return m;
}

Masina citireMasinaDinFisier(FILE* f) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, f);
	char* aux;
	Masina m;
	aux = strtok(buffer, sep);
	m.id = atoi(aux);
	aux = strtok(NULL, sep);
	m.nrUsi = atoi(aux);
	m.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m.model = malloc(strlen(aux) + 1);
	strcpy_s(m.model, (strlen(aux) + 1), aux);
	aux = strtok(NULL, sep);
	m.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m.numeSofer, (strlen(aux) + 1), aux);
	m.serie = *strtok(NULL, sep);
	return m;
}

void afisareMasina(Masina m) {
	printf("Masina: %d, %d, %.2f, %s, %s, %c\n", m.id, m.nrUsi, m.pret, m.model, m.numeSofer, m.serie);
}

void adaugaMasinaInListaLaInceput(Nod** cap, Masina m) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = creareMasina(m.id, m.nrUsi, m.pret, m.model, m.numeSofer, m.serie);
	nou->next = NULL;

	if (*cap == NULL) {
		*cap = nou;
	}
	else {
		nou->next = *cap;
		*cap = nou;
	}
}

void adaugaMasinaInListaLaSfarsit(Nod** cap, Masina m) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = creareMasina(m.id, m.nrUsi, m.pret, m.model, m.numeSofer, m.serie);
	nou->next = NULL;

	if ((*cap) == NULL) {
		(*cap) = nou;
	}
	else {
		Nod* temp = (*cap);
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = nou;
	}
}

Nod* citireListaMasiniDinFisier(const char* numeFisier) {
	Nod* cap = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			adaugaMasinaInListaLaInceput(&cap, citireMasinaDinFisier(f));
		}
	}
	fclose(f);
	return cap;
}

void afisareLista(Nod* cap) {
	while (cap != NULL) {
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

void dezalocare(Nod* cap) {
	Nod* aux = cap;
	while (aux->next) {
		free(aux->info.model);
		free(aux->info.numeSofer);
		Nod* temp = aux->next;
		free(aux);
		aux = temp;
	}
}

float calcularePretMediu(Nod* cap) {
	float suma = 0;
	int count = 0;
	while (cap) {
		suma += cap->info.pret;
		count++;
		cap = cap->next;
	}
	if (count > 0) {
		float medie = suma / count;
		return medie;
	}
	return 0;
	
}

float calculeazaPretulMasinilorUnuiSofer(Nod* cap, const char* numeSofer) {
	float suma = 0;
	while (cap) {
		if (strcmp(cap->info.numeSofer, numeSofer) == 0) {
			suma += cap->info.pret;
		}
		cap = cap->next;
	}
	return suma;
}

void main() {
	Nod* cap = citireListaMasiniDinFisier("masini.txt");
	afisareLista(cap);
	float pretMediu = calcularePretMediu(cap);
	printf("Pretul mediu: %.2f\n", pretMediu);
	printf("Pretul masilior lui Gigel este de %.2f\n", calculeazaPretulMasinilorUnuiSofer(cap, "Gigel"));
	dezalocare(cap);
}


