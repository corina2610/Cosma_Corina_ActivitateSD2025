#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

//creare structura pentru un nod dintr-o lista dublu inlantuita
typedef struct Nod Nod;
typedef struct Nod {
	Masina info;
	Nod* prev;
	Nod* next;
};
//creare structura pentru Lista Dubla
typedef struct LD LD;
typedef struct LD {
	Nod* cap;
	Nod* coada;
	int nrNoduri;
};

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasiniDeLaFinal(LD lista) {
	Nod* temp = lista.coada;
	while (temp) {
		afisareMasina(temp->info);
		temp = temp->prev;
	}
}

void adaugaMasinaInLista(LD* lista, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = NULL;
	nou->prev = lista->coada;

	if (lista->cap == NULL) {
		lista->cap = nou;
	}
	else {
		lista->coada->next = nou;
	}
	lista->coada = nou;
	lista->nrNoduri++;
}

void adaugaLaInceputInLista(LD* lista, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->prev = NULL;
	nou->next = lista->cap;

	if (lista->cap == NULL) {
		lista->coada = nou;
	}
	else {
		lista->cap->prev = nou;
	}
	lista->cap = nou;
	lista->nrNoduri++;
}

void afisareListaMasinaDeLAInceput(LD lista) {
	Nod* temp = lista.cap;
	while (temp) {
		afisareMasina(temp->info);

	}
}

LD citireLDMasiniDinFisier(const char* numeFisier) {
	LD lista;
	lista.cap = NULL;
	lista.coada = NULL;
	lista.nrNoduri = 0;

	FILE* f = fopen(numeFisier, "r");
	while (!feof(f)) {
		adaugaMasinaInLista(&lista, citireMasinaDinFisier(f));
	}
	fclose(f);
	return lista;
}

void dezalocareLDMasini(LD* lista) {
	if (lista->cap) {
		Nod* temp = lista->cap;
		while (temp->next != NULL) {
			free(temp->info.numeSofer);
			free(temp->info.model);
			temp = temp->next;
			free(temp->prev);
		}
		free(temp->info.numeSofer);
		free(temp->info.model);
		free(temp);
		lista->cap = NULL;
		lista->coada = NULL;
		lista->nrNoduri = 0;
	}
}

float calculeazaPretMediu(LD lista) {
	Nod* aux = lista.cap;
	int suma = 0;
	while (aux) {
		suma += aux->info.pret;
		aux = aux->next;
	}

	return lista.nrNoduri?suma/lista.nrNoduri:0;
}

void stergeMasinaDupaID(/*lista masini*/ int id) {
	//sterge masina cu id-ul primit.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie
}

char* getNumeSoferMasinaScumpa(LD lista) {
	Nod* temp = lista.cap;
	Nod* maxim = lista.cap;
	while (temp) {
		if (temp->info.pret > maxim->info.pret) {
			maxim = temp;
		}
		temp = temp->next;
	}
	char* numeCopiat = (char*)malloc(sizeof(strlen(maxim->info.numeSofer) + 1));
	strcpy_s(numeCopiat, (strlen(maxim->info.numeSofer) + 1), maxim->info.numeSofer);

	return strcpy_s(numeCopiat, (strlen(maxim->info.numeSofer)+1), maxim->info.numeSofer);
}

int main() {
	LD lista;
	lista = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasiniDeLaFinal(lista);
	return 0;

	float pretMediu = 0;
	pretMediu = calculeazaPretMediu(lista);
	printf("Pret mediu: %.2f", pretMediu);

	char* numeReturnat = getNumeSoferMasinaScumpa(lista);
	printf("Nume returnat: %s", numeReturnat);
}