#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

typedef struct Nod Nod;

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct Nod {
	Masina info;
	Nod* next;
};

//creare structura pentru un nod dintr-o lista simplu inlantuita

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

void afisareListaMasini(Nod* lista) {
	while (lista) {
		afisareMasina(lista->info);
		lista = lista->next;
	}
}

void adaugaMasinaInLista(Nod** lista, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = NULL;
	if ((*lista) != NULL) {
		(*lista) = nou;
	}
	else {
		Nod* temp = (*lista);
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = nou;
	}
}

void adaugaLaInceputInLista(Nod** lista, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = *lista;
	*lista = nou;

}

//de documentat void*
Nod* citireListaMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file = fopen(numeFisier, "r");
	Nod* lista = NULL;
	while (!feof(file)) {
		adaugaMasinaInLista(lista, citireMasinaDinFisier(file));
	}
	fclose(file);
	return lista;
}

void dezalocareListaMasini(Nod** lista) {
	while (lista) {
		Nod* p = (*lista);
		(*lista) = (*lista)->next;
		free(p->info.model);
		free(p->info.numeSofer);
		free(p);
	}
}

float calculeazaPretMediu(Nod* lista) {
	int suma = 0;
	int medie = 0;
	while (lista) {
		suma += lista->info.pret;
		medie++;
		lista = lista->next;
	}
	
	return medie;
}

void stergeMasiniDinSeria(Nod** lista, char serieCautata) {
	while ((*lista) != NULL && (*lista)->info.serie == serieCautata) {
		Nod* temp = (*lista);
		(*lista) = (*lista)->next;
		free(temp->info.model);
		free(temp->info.numeSofer);
		free(temp);
	}
	Nod* p = (*lista);
	while (p != NULL) {
		while (p->next && p->next->info.serie != serieCautata) {
			p = p->next;
		}

		if (p->next != NULL) {
			Nod* temp = p->next;
			p->next = temp->next;
			free(temp->info.model);
			free(temp->info.numeSofer);
			free(temp);
		}
		else {
			p = p->next;
		}
	}
}

float calculeazaPretulMasinilorUnuiSofer(Nod* lista, const char* numeSofer) {
	float suma = 0;
	while (lista) {
		if (strcmp(lista->info.numeSofer, numeSofer) == 0) {
			suma += lista->info.pret;
		}
		lista = lista->next;
	}
	return suma;
}

int main() {
	Nod* lista = citireListaMasiniDinFisier("masini.txt");
	afisareListaMasini(lista);
	stergeMasiniDinSeria(lista, "A");
	stergeMasiniDinSeria(lista, "B");
	afisareListaMasini(lista);
	dezalocareListaMasini(lista);
	afisareListaMasini(lista);
	return 0;
}