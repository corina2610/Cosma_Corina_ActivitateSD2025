#define _CRT_SECURE_NO_WARNINGS
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Corina Cosma -> CC -> CresCatorie

typedef struct Crescatorie Crescatorie;

typedef struct Crescatorie {
	int cod;
	char* tipAnimale;
	int pretAnimal;
	float costCrestere;
	int nrAnimale;
	float* greutate;
};

Crescatorie creeareCrescatorie(int cod, const char* denumire, int pretAnimal, float costCrestere, int nrAnimale, float* greutate) {
	Crescatorie c;
	c.cod = cod;
	c.tipAnimale = (char*)malloc(sizeof(char) * (strlen(denumire) + 1));
	strcpy_s(c.tipAnimale, (strlen(denumire) + 1), denumire);
	c.pretAnimal = pretAnimal;
	c.costCrestere = costCrestere;
	c.nrAnimale = nrAnimale;
	c.greutate = (float*)malloc(sizeof(float) * nrAnimale);
	for (int i = 0; i < nrAnimale; i++) {
		c.greutate[i] = greutate[i];
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

//citire de la tastatura a unei crescatorii

Crescatorie citireElement() {
	Crescatorie c;
	printf("Intoduceti codul crescatoriei: ");
	scanf("%d", &c.cod);

	printf("Intoduceti denumirea crescatoriei: ");
	char buffer[100];
	scanf("%s", &buffer);
	c.tipAnimale = (char*)malloc(strlen(buffer) + 1);
	strcpy(c.tipAnimale, buffer);

	printf("Intoduceti pretul per animal: ");
	scanf("%d", &c.pretAnimal);

	printf("Intoduceti costul de crestere per animal: ");
	scanf("%f", &c.costCrestere);

	printf("Intoduceti nr de animale: ");
	scanf("%d", &c.nrAnimale);

	c.greutate = (float*)malloc(sizeof(float) * c.nrAnimale);
	printf("Introduceti greutatea fiecarui animal");
	for (int i = 0; i < c.nrAnimale; i++) {
		scanf("%f", &c.greutate[i]);
	}

	return c;
}

// Sa se calculeze care este greutatea medie a unui animal

float calculGreutateMedie(Crescatorie c) {
	float suma = 0;
	for (int i = 0; i < c.nrAnimale; i++) {
		suma += c.greutate[i];
	}
	float medie = 0;
	medie = suma / c.nrAnimale;
	return medie;
}

//Sa se schimbe denumire unei ferme cu alt tip de animale
void schimareTipAnimal(Crescatorie* c, const char* noulTip) {
	if (noulTip != NULL) {
		free(c->tipAnimale);
		c->tipAnimale = (char*)malloc(sizeof(char) * (strlen(noulTip) + 1));
		strcpy(c->tipAnimale, noulTip);
	}
}




int main() {
	Crescatorie test;
	float greutati[3] = { 11, 2, 3 };
	test = creeareCrescatorie(100, "Pui", 30, 20, 3, greutati);
	afisare(test);

	Crescatorie citireTastatura2;
	//citireTastatura2 = citireElement();
	//afisare(citireTastatura2);

	printf("\nGreutatea medie a animalelor din crescatorie: %.2f", calculGreutateMedie(test));

	printf("Schimbare crescatorie animale: \n");
	afisare(test);
	schimareTipAnimal(&test, "Peste");
	afisare(test);
}


