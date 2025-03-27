#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

typedef struct Scoala Scoala;
typedef struct Nod Nod;

typedef struct Scoala {
	int id;
	char* nume;
	float cost;
	int olimpici;
	int nrClase;
	int* elevi;
};

typedef struct Nod {
	Scoala info;
	Nod* next;
};

Scoala citireScoalaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Scoala s;
	aux = strtok(buffer, sep);
	s.id = atoi(aux);
	aux = strtok(NULL, sep);
	s.nume = (char*)malloc(strlen(aux) + 1);
	strcpy_s(s.nume, (strlen(aux) + 1), aux);
	aux = strtok(NULL, sep);
	s.cost = atof(aux);
	aux = strtok(NULL, sep);
	s.olimpici = atoi(aux);
	aux = strtok(NULL, sep);
	s.nrClase = atoi(aux);
	s.elevi = (int*)malloc(sizeof(int) * s.nrClase);
	for (int i = 0; i < s.nrClase; i++) {
		s.elevi[i] = atoi(aux);
	}
	return s;
}

void afisareScoala(Scoala s) {
	printf("\n");
	printf("Id: %d \n", s.id);
	printf("Nume: %s \n", s.nume);
	printf("Cost intretinere lunara: %.2f \n", s.cost);
	printf("Nr elevi olimpici: %d \n", s.olimpici);
	printf("Nr clase de elevi: %d \n", s.nrClase);
	for (int i = 0; i < s.nrClase; i++) {
		printf("Numar de elevi in clasa: %d ", s.elevi[i]);
	}
	printf("\n");
}

void afisareListaScoli(Nod* cap) {
	while (cap != NULL) {
		afisareScoala(cap->info);
		cap = cap->next;
	}
}

void adaugaScoalaInListaLaInceput(Nod** cap, Scoala s) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = s;
	nou->next = NULL;

	if (*cap == NULL) {
		*cap = nou;
	}
	else {
		nou->next = *cap;
		*cap = nou;
	}
}

void adaugaScoalaInListaLaFinal(Nod** cap, Scoala s) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = s;
	nou->next = NULL;

	if (*cap == NULL) {
		*cap = nou;
	}
	else {
		Nod* temp = *cap;
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = nou;
	}
}

Nod* citireListaDinFisier(const char* numeFisier) {
	Nod* cap = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			adaugaScoalaInListaLaInceput(&cap, citireScoalaDinFisier(f));
		}
	}
	fclose(f);
	return cap;
}

void dezalocare(Nod* cap) {
	Nod* aux = cap;
	while (aux) {
		free(aux->info.nume);
		Nod* temp = aux->next;
		free(aux);
		aux = temp;
	}

}

int main() {
	Nod* cap = citireListaDinFisier("scoli.txt");
	afisareListaScoli(cap);
	dezalocare(cap);
}