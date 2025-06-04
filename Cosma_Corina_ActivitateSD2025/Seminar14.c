#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

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
	printf("Serie: %c\n", masina.serie);
}

typedef struct NodLP NodLP;
typedef struct NodLS NodLS;

struct NodLP {
	Masina info;
	NodLP* next;
	NodLS* vecini;
};

struct NodLS {
	NodLS* next;
	NodLP* nodInfo;
};

//2.
//functii de inserare in liste
//si in principala si in secundara

void inserareLP(NodLP** cap, Masina masina) {
	NodLP* nou = (NodLP*)malloc(sizeof(NodLP));
	nou->next = NULL;
	nou->info = masina;
	nou->vecini = NULL;
	if (*cap) {
		NodLP* p = *cap;
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
	else {
		*cap = nou;
	}
}

void inserareLS(NodLS** cap, NodLP* nodInfo) {
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
	nou->next = *cap;
	nou->nodInfo = nodInfo;
	*cap = nou;
}

NodLP* cautaNodDupaID(NodLP* cap, int id) {
	while (cap && cap->info.id != id) {
		cap = cap->next;
	}
	return cap;
}

void inserareMuchie(NodLP* cap, int idStart, int idStop) {
	NodLP* nodStart = cautaNodDupaID(cap, idStart);
	NodLP* nodStop = cautaNodDupaID(cap, idStop);
	if (nodStart && nodStop) {
		inserareLS(&(nodStart->vecini), nodStop);
		inserareLS(&(nodStop->vecini), nodStart);
	}
}

NodLP* citireNoduriMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	NodLP* graf = NULL;
	if (f) {
		while (!feof(f)) {
			inserareLP(&graf, citireMasinaDinFisier(f));
		}
	}
	fclose(f);
	return graf;
}

void citireMuchiiDinFisier(const char* numeFisier, NodLP* graf) {
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			int idStart;
			int idStop;
			fscanf(f, "%d %d", &idStart, &idStop);
			inserareMuchie(graf, idStart, idStop);
		}
		fclose(f);
	}
}

void stergereLS(NodLS** cap) {
	while (*cap) {
		NodLS* aux = (*cap)->next;
		free(*cap);
		*cap = aux;
	}
}

void dezalocareNoduriGraf(NodLP** cap) {
	while (*cap) {
		NodLP* aux = (*cap)->next;
		stergereLS(&((*cap)->vecini));
		if ((*cap)->info.numeSofer) {
			free((*cap)->info.numeSofer);
		}
		if ((*cap)->info.model) {
			free((*cap)->info.model);
		}
		free(*cap);
		*cap = aux;
	}
}

NodLP* citireGrafDinFisiere(const char* fisierNoduri, const char* fisierMuchii) {
	NodLP* graf = citireNoduriMasiniDinFisier(fisierNoduri);
	citireMuchiiDinFisier(fisierMuchii, graf);

	return graf;
}

void afisareGraf(NodLP* graf) {
	while (graf) {
		printf("In nodul %d avem masina:\n", graf->info.id);
		afisareMasina(graf->info);
		printf("aceasta are ca vecini nodurile urmatoare: ");
		NodLS* p = graf->vecini;
		while (p) {
			printf("%d, ", p->nodInfo->info.id);
			p = p->next;
		}
		printf("\n\n");
		graf = graf->next;
	}
}

//Parcurgere in adancime
//creare structura Stiva

typedef struct NodStiva NodStiva;

struct NodStiva {
	int id;
	NodStiva* next;
};

void push(NodStiva** cap, int id) {
	NodStiva* nou = (NodStiva*)malloc(sizeof(NodStiva));
	nou->id = id;
	nou->next = (*cap);
	(*cap) = nou;
}

int pop(NodStiva** cap) {
	if (*cap) {
		NodStiva* aux = (*cap);
		(*cap) = (*cap)->next;
		int id = aux->id;
		free(aux);
		return id;
	}
	else {
		return -1;
	}
}

int calculeazaNrNoduriGraf(NodLP* graf) {
	int counter = 0;
	while (graf) {
		counter++;
		graf = graf->next;
	}
	return counter;
}

void afisareGrafInAdancime(NodLP* listaPrincipala, int idPlecare) {
	int nrNoduri = calculeazaNrNoduriGraf(listaPrincipala);
	int* vizitate = (int*)malloc(nrNoduri*sizeof(int));
	NodStiva* stiva = NULL;
	for (int i = 0; i < nrNoduri; i++) {
		vizitate[i] = 0;
	}
	vizitate[idPlecare - 1] = 1;
	push(&stiva, idPlecare);
	while (stiva) {
		int id = pop(&stiva);
		NodLP* nodCautat = cautaNodDupaID(listaPrincipala, id);
		afisareMasina(nodCautat->info);
		NodLS* aux = nodCautat->vecini;
		while (aux) {
			if (vizitate[aux->nodInfo->info.id-1]=0) {
				vizitate[aux->nodInfo->info.id - 1] = 1;
				push(&stiva, aux->nodInfo->info.id);

			}
			aux - aux->next;
			//deplasare
		}
	}
}


//Parcurgere in latime



//creare structura coada
//enqueue si dequeue pentru coada
typedef struct NodStiva NodCoada;
void enqueue(NodCoada** cap, int id) {
	NodCoada* nou = (NodCoada*)malloc(sizeof(NodCoada));
	nou->id = id;
	nou->next = NULL;
	if (*cap) {
		NodCoada* p = *cap;
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
	else {
		*cap = nou;
	}
}

int dequeue(NodCoada** cap) {
	return pop(cap);
}

void afisareGrafInLatime(NodLP* listaPrincipala, int idPlecare) {
	int nrNoduri = calculeazaNrNoduriGraf(listaPrincipala);
	int* vizitate = (int*)malloc(nrNoduri * sizeof(int));
	NodCoada* coada = NULL;
	for (int i = 0; i < nrNoduri; i++) {
		vizitate[i] = 0;
	}
	vizitate[idPlecare - 1] = 1;
	enqueue(&coada, idPlecare);
	while (coada) {
		int id = dequeue(&coada);
		NodLP* nodCautat = cautaNodDupaID(listaPrincipala, id);
		afisareMasina(nodCautat->info);
		NodLS* aux = nodCautat->vecini;
		while (aux) {
			if (vizitate[aux->nodInfo->info.id - 1] = 0) {
				vizitate[aux->nodInfo->info.id - 1] = 1;
				enqueue(&coada, aux->nodInfo->info.id);

			}
			aux - aux->next;
			//deplasare
		}
	}
}


int main() {
	NodLP* graf = citireGrafDinFisiere("masini.txt", "muchii.txt");
	afisareGraf(graf);

	afisareGrafInAdancime(graf, 1);

	printf("\n Latime: \n");
	afisareGrafInLatime(graf,1);

	dezalocareNoduriGraf(&graf);

	return 0;
}