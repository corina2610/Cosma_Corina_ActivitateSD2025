#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

typedef struct student {
	unsigned int id;
	char* nume;
	float medie;
} student;

typedef struct nod {
	student info;
	struct nod* next;
}nod;

student creeareStudent(unsigned int id, char* nume, float medie) {
	student s;
	s.id = id;
	s.nume = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
	strcpy(s.nume, nume);
	s.medie = medie;
	return s;
}

nod* inserareInceput(nod* cap, student s) {
	//formam nodul
	//alocam memorie nodului
	nod* nou = (nod*)malloc(sizeof(nod));
	//shallow copy 
	nou->info = s;
	//deepcopy 
	//nou->info = creeareStudent(s.id, s.nume, s.medie);
	nou->next = NULL;
	if (cap == NULL) {
		cap = nou;
	}
	else {
		nou->next = cap;
		cap = nou;

	}

	return cap;
}


void inserareLaFinal(nod** cap, student s) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = s;
	nou->next = NULL;

	if (*cap == NULL) {
		*cap = nou;
	}
	else {
		nod* aux = *cap;
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = nou;

	}
}
//afisare

void afisare(nod* cap) {
	nod* aux = cap;
	while (aux) {
		printf("%u, %s, %5.2f \n", aux->info.id, aux->info.nume, aux->info.medie);
		aux = aux->next;
	}
}

//dezalocare
void dezalocare(nod* cap) {
	nod* aux = cap;
	while (aux) {
		free(aux->info.nume);
		nod* temp = aux->next;
		free(aux);
		aux = temp;
	}
}


void main() {
	student s = creeareStudent(1, "Maria", 9.1);
	student s1 = creeareStudent(2, "Ana", 10);
	student s2 = creeareStudent(3, "Marius", 9.5);
	student s3 = creeareStudent(4, "Alina", 8.2);

	nod* cap = NULL;

	cap = inserareInceput(cap, s);
	cap = inserareInceput(cap, s1);
	cap = inserareInceput(cap, s2);
	cap = inserareInceput(cap, s3);

	inserareLaFinal(&cap, s);

	afisare(cap);


}