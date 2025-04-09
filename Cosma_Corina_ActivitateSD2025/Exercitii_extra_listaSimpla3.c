//#define _CRT_SECURE_NO_WARNINGS
//#include <stdlib.h>
//#include <malloc.h>
//#include <stdio.h>
//#include <string.h>
//
//typedef struct Florarie Florarie;
//typedef struct Nod Nod;
//
//struct Florarie {
//	int nr;
//	char* denumire;
//	int nrSortimente;
//	int nrCumparatori;
//	float vanzari;
//};
//
//struct Nod{
//	Florarie info;
//	Nod* next;
//};
//
//Florarie initializare(int nr, char* denumire, int nrSortimente, int nrCumparatori, float vanzari) {
//	Florarie f;
//	f.nr = nr;
//	f.denumire = (char*)malloc(sizeof(char) + strlen(denumire) + 1);
//	strcpy_s(f.denumire, sizeof(char) + strlen(denumire) + 1, denumire);
//	f.nrSortimente = nrSortimente;
//	f.nrCumparatori = nrCumparatori;
//	f.vanzari = vanzari;
//	return f;
//}
//
//void afisareFlorarie(Florarie f) {
//	printf("Floraria: %d, %s, %d, %d, %.2f\n", f.nr, f.denumire, f.nrSortimente, f.nrCumparatori, f.vanzari);
//}
//
//Nod* inserareLaInceput(Nod* cap, Florarie f) {
//	Nod* nou = (Nod*)malloc(sizeof(Nod));
//	nou->info = initializare(f.nr, f.denumire, f.nrSortimente, f.nrCumparatori, f.vanzari);
//	nou->next = NULL;
//
//	if (cap == NULL) {
//		cap = nou;
//	}
//	else {
//		nou->next = cap;
//		cap = nou;
//	}
//}
//
//Nod* inserareLaFinal(Nod* cap, Florarie f) {
//	Nod* nou = (Nod*)malloc(sizeof(Nod));
//	nou->info = initializare(f.nr, f.denumire, f.nrSortimente, f.nrCumparatori, f.vanzari);
//	nou->next = NULL;
//
//	if (cap == NULL) {
//		cap = nou;
//	}
//	else {
//		Nod* aux = cap;
//		while (aux->next) {
//			aux = aux->next;
//		}
//		aux->next = nou;
//	}
//	return cap;
//}
//
//void afisareLista(Nod* cap) {
//	if (cap) {
//		while (cap) {
//			afisareFlorarie(cap->info);
//			cap = cap->next;
//		}
//	}
//}
//
//Nod* citireDinFisier(const char* numeFisier) {
//	Nod* cap = NULL;
//	FILE* f = fopen(numeFisier, "r");
//	if (f) {
//		while (!feof(f)) {
//			char buffer[100];
//			char sep[3] = ",\n";
//			fgets(buffer, 100, f);
//			char* aux;
//			Florarie f;
//			aux = strtok(buffer, sep);
//			f.nr = atoi(aux);
//			aux = strtok(NULL, sep);
//			f.denumire = (char*)malloc(strlen(aux) + 1);
//			strcpy_s(f.denumire, strlen(aux) + 1, aux);
//			aux = strtok(NULL, sep);
//			f.nrSortimente = atoi(aux);
//			aux = strtok(NULL, sep);
//			f.nrCumparatori = atoi(aux);
//			aux = strtok(NULL, sep);
//			f.vanzari = atof(aux);
//
//			cap = inserareLaFinal(cap, f);
//
//			free(f.denumire);
//		}
//		fclose(f);
//		return cap;
//	}
//}
//
//void dezalocare(Nod* cap) {
//	if (cap) {
//		Nod* aux = cap;
//		while (aux) {
//			free(aux->info.denumire);
//			Nod* temp = aux->next;
//			free(aux);
//			aux = temp;
//		}
//	}
//}
//
//void stergereElementDinListaDupaCriteriu(Nod** cap, const char* denumire) {
//	if (*cap) {
//		Nod* aux = *cap;
//		Nod* prev = NULL;
//		while (aux) {
//			if (strcmp(aux->info.denumire, denumire) == 0) {
//				Nod* deStres = aux;
//				aux = aux->next;
//				if (prev == NULL) {
//					*cap = aux;
//				}
//				else {
//					prev->next = aux;
//				}
//				free(deStres->info.denumire);
//				free(deStres);
//			}
//			else {
//				prev = aux;
//				aux = aux->next;
//			}
//		}
//	}
//}
//
//int main() {
//	Nod* cap = citireDinFisier("florarie.txt");
//	afisareLista(cap);
//	stergereElementDinListaDupaCriteriu(&cap,"Margareta");
//	afisareLista(cap);
//}