#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

typedef struct StructuraMasina Masina;

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};

void afisareMasina(Masina masina) {
	printf("Id : %d\n", masina.id);
	printf("NrUsi : %d\n", masina.nrUsi);
	printf("Pret : %.2f\n", masina.pret);
	printf("Model : %s\n", masina.model);
	printf("Nume Sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n", masina.serie);
}

void afisareVectorMasini(Masina* masini, int nrMasini) {
	for (int i = 0; i < nrMasini; i++) {
		afisareMasina(masini[i]);
		printf("\n");
	}
}

void adaugaMasinaInVector(Masina** masini, int* nrMasini, Masina masinaNoua) {
	//adauga in vectorul primit o noua masina pe care o primim ca parametru
	//ATENTIE - se modifica numarul de masini din vector;
	Masina* aux = (Masina*)malloc(sizeof(Masina) * ((*nrMasini) + 1));

	for (int i = 0; i < (*nrMasini); i++) {
		aux[i] = (*masini)[i];
	}
	aux[(*nrMasini)] = masinaNoua;
	free(*masini);
	(*masini) = aux;
	(*nrMasini)++;

	
}

Masina citireMasinaFisier(FILE* file) {
	//functia citeste o masina dintr-un strceam deja deschis
	//masina citita este returnata;

	char buffer[100];
	char sep[3] = ",\n";
	char* aux;

	fgets(buffer, 100, file);

	Masina m;
	aux = strtok(buffer, sep);
	m.id = atoi(aux);
	m.nrUsi = atoi(strtok(NULL, sep));
	m.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m.model = malloc(strlen(aux) + 1);
	strcpy_s(m.model, strlen(aux) + 1, aux);
	aux = strtok(NULL, sep);
	m.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m.numeSofer, strlen(aux) + 1, aux);
	m.serie = *strtok(NULL, sep);

	return m;

}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaFisier()
	//numarul de masini este determinat prin numarul de citiri din fisier
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file = fopen(numeFisier, "r");
	Masina* masini = NULL;

	while (!feof(file)) {
		Masina m = citireMasinaFisier(file);
		adaugaMasinaInVector(&masini, nrMasiniCitite, m);
	}
	fclose(file);
	return masini;

}



void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	for (int i = 0; i < (*nrMasini); i++) {
		free((*vector)[i].model);
		free((*vector)[i].numeSofer);

	}
	free(*vector);
	(*vector) = NULL;
	(*nrMasini) = 0;
}

float calculeazaPretMediuDupaNrUsi(Masina* vector, int nrMasini, int nrUsi) {
	float sum = 0;
	int index = 0;
	for (int i = 0; i < nrMasini; i++) {
		if (vector[i].nrUsi == nrUsi) {
			sum += vector[i].pret;
			index++;
		}
	}
	if (index > 0) {
		return sum / index;
	}
	return 0;
}

int main() {

	Masina* masini = NULL;
	int nrMasini = 0;

	masini = citireVectorMasiniFisier("masini.txt", &nrMasini);
	afisareVectorMasini(masini, nrMasini);

	float medie = calculeazaPretMediuDupaNrUsi("masini.txt", nrMasini, 4);
	printf("Pretul mediu este: %.2f", medie);

	dezalocareVectorMasini(masini, nrMasini);
	return 0;
}