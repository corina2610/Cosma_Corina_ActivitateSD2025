#define _CRT_SECURE_NO_WARNINGS
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct Comanda Comanda;
struct Comanda {
	unsigned int id;
	char* nume;
	int nrProduse;
	float pret;
};

Comanda initComanda(unsigned int id, char* nume, int nrProduse, float pret) {
	Comanda c;
	c.id = id;
	c.nume = (char*)malloc(sizeof(char) * strlen(nume) + 1);
	strcpy_s(c.nume, sizeof(char) * strlen(nume) + 1, nume);
	c.nrProduse = nrProduse;
	c.pret = pret;
	return c;
}

void afisareComanda(Comanda c) {
	printf("Comanda: %u, %s, %d, %.2f\n", c.id, c.nume, c.nrProduse, c.pret);
}

typedef struct NodArbore NodArbore;
struct NodArbore {
	Comanda info;
	NodArbore* stanga;
	NodArbore* dreapta;
	int grad;
};

NodArbore* initializareNodArbore(Comanda c, NodArbore* stanga, NodArbore* dreapta) {
	NodArbore* nou = (NodArbore*)malloc(sizeof(NodArbore));
	nou->info = initComanda(c.id, c.nume, c.nrProduse, c.pret);
	nou->stanga = stanga;
	nou->dreapta = dreapta;
	return nou;
}

NodArbore* inserareNodArbore(NodArbore* radacina, Comanda c) {
	if (radacina) {
		if (c.id < radacina->info.id) {
			radacina->stanga = inserareNodArbore(radacina->stanga, c);
			return radacina;
		}
		else if (c.id > radacina->info.id) {
			radacina->dreapta = inserareNodArbore(radacina->dreapta, c);
			return radacina;
		}
		else {
			return radacina;
		}
	}
	else {
		return initializareNodArbore(c, NULL, NULL);
	}
}

void preordine(NodArbore* rad) {
	if (rad) {
		printf("Comanda: %u, %s, %d, %.2f\n", rad->info.id, rad->info.nume, rad->info.nrProduse, rad->info.pret);
		preordine(rad->stanga);
		preordine(rad->dreapta);
	}
}

void inordine(NodArbore* rad) {
	if (rad) {
		inordine(rad->stanga);
		printf("Comanda: %u, %s, %d, %.2f\n", rad->info.id, rad->info.nume, rad->info.nrProduse, rad->info.pret);
		inordine(rad->dreapta);
	}
}

void postordine(NodArbore* rad) {
	if (rad) {
		postordine(rad->stanga);
		postordine(rad->dreapta);
		printf("Comanda: %u, %s, %d, %.2f\n", rad->info.id, rad->info.nume, rad->info.nrProduse, rad->info.pret);
	}
}

NodArbore* citireDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	NodArbore* radacina = NULL;
	if (f) {
		while (!feof(f)) {
			char buffer[100];
			char sep[3] = ",\n";
			fgets(buffer, 100, f);
			char* aux;
			Comanda c;
			aux = strtok(buffer, sep);
			c.id = atoi(aux);
			aux = strtok(NULL, sep);
			c.nume = (char*)malloc(strlen(aux) + 1);
			strcpy_s(c.nume, strlen(aux) + 1, aux);
			aux = strtok(NULL, sep);
			c.nrProduse = atoi(aux);
			aux = strtok(NULL, sep);
			c.pret = atof(aux);

			radacina = inserareNodArbore(radacina, c);

		}
		fclose(f);
		return radacina;
	}
}

void dezalocareArbore(NodArbore* rad) {
	if (rad) {
		NodArbore* arboreStg = rad->stanga;
		NodArbore* arboreDr = rad->dreapta;

		free(rad->info.nume);
		free(rad);

		dezalocareArbore(arboreStg);
		dezalocareArbore(arboreDr);
	}
}

int maxim(int a, int b) {
	if (a > b) {
		return a;
	}
	else {
		return b;
	}
}

int nrNiveluriArbore(NodArbore* rad) {
	if (rad == NULL) {
		return 0;
	}

	if (rad) {
		int nrNivelStanga = nrNiveluriArbore(rad->stanga);
		int nrNivelDreapta = nrNiveluriArbore(rad->dreapta);

		return 1 + maxim(nrNivelStanga, nrNivelDreapta);
	}
}

void calculGrad(NodArbore* radacina) {
	radacina->grad = (nrNiveluriArbore(radacina->stanga) - nrNiveluriArbore(radacina->dreapta));
	calculGrad(radacina->stanga);
	calculGrad(radacina->dreapta);
}

NodArbore* rotatieDreapta(NodArbore* rad) {
	NodArbore* x = rad->stanga;
	rad->stanga = x->dreapta;
	x->dreapta = rad;
	return x;
}

NodArbore* rotatieStanga(NodArbore* rad) {
	NodArbore* x = rad->dreapta;
	rad->dreapta = x->stanga;
	x->stanga = rad;
	return x;
}

NodArbore* rotatieStangaDreapta(NodArbore* rad) {
	rad->stanga = rotatieStanga(rad->stanga);
	return rotatieDreapta(rad);
}

///////////// stiva
typedef struct Nod Nod;
struct Nod {
	Comanda info;
	Nod* next;
};
//stiva LIFO
void push(Nod** cap, Comanda c) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = initComanda(c.id, c.nume, c.nrProduse, c.pret);
	nou->next = *cap;
	*cap = nou;
}

//FIFO
void put(Nod** cap, Comanda c) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = initComanda(c.id, c.nume, c.nrProduse, c.pret);
	nou->next = NULL;

	if ((*cap) == NULL) {
		*cap = nou;
	}
	else {
		Nod* it = *cap;
		while (it->next) {
			it = it->next;
		}
		it->next = nou;
	}
}

Comanda pop(Nod** varf) {
	Comanda c;
	c.nume = NULL;
	if (*varf) {
		c = (*varf)->info;
		Nod* copie = (*varf);
		(*varf) = (*varf)->next;
		free(copie);
	}
	return c;
}

//Heap
typedef struct Heap Heap;
struct Heap {
	Comanda* vector;
	int dim;
};

void filtrare(Heap h , int index) {
	int indexRadacina = index;
	int indexStg = 2 * index + 1;
	int indexDr = 2 * index + 2;
	if (indexStg < h.dim && h.vector[indexStg].id < h.vector[indexRadacina].id) {
		indexRadacina = indexStg;
	}

	if (indexDr < h.dim && h.vector[indexDr].id > h.vector[indexRadacina].id) {
		indexRadacina = indexDr;
	}

	if (index != indexRadacina) {

		Comanda aux = h.vector[index];
		h.vector[index] = h.vector[indexRadacina];
		h.vector[indexRadacina] = aux;
		filtrare(h, indexRadacina);
	}

}



int main() {
	NodArbore* radacina = citireDinFisier("comenziii.txt");
	preordine(radacina);
}