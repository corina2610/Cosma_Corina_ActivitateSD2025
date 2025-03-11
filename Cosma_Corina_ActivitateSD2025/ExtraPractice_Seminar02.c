#include<stdio.h>
#include<malloc.h>

struct Masina {
	int id;
	char* model;
	float tonaj;
	char serie;
};

//sa avem grija sa punem const la pointer
struct Masina initializare(int id, const char* model, float tonaj, char serie) {
	struct Masina m;
	m.id = id;
	m.model = malloc(sizeof(char) * (strlen(model) + 1));
	strcpy_s(m.model, strlen(model) + 1, model);
	m.tonaj = tonaj;
	m.serie = serie;
	return m;
}

void afisare(struct Masina m) {
	printf("Id: %d\n", m.id);
	printf("Model: %s\n", m.model);
	printf("Tonaj: %.2f\n", m.tonaj);
	printf("Serie: %c\n", m.serie);

}

void afisareVector(struct Masina* vector, int nrElemente) {
	for (int i = 0; i < nrElemente; i++) {
		afisare(vector[i]);
	}
}

struct Masina* copiazaPrimeleMasini(struct Masina* vector, int nrElemente, int nrElementeCopiate) {
	//copiem intr-un vector nou pe care il vom returna primele nrElementeCopiate

	struct Masina* vectorNou=malloc(sizeof(struct Masina) * nrElementeCopiate);
	for (int i = 0; i < nrElementeCopiate; i++) {
		vectorNou[i] = vector[i];
		vectorNou[i].model = malloc(sizeof(char*) * (strlen(vector[i].model) + 1));
		strcpy_s(vectorNou[i].model, strlen(vector[i].model) + 1, vector[i].model);
	}

	return vectorNou;

}

void dezalocare(struct Masina** vector, int* nrElemente) {
	for (int i = 0; i < *nrElemente; i++) {
		free((*vector)[i].model);
	}
	free(*vector);
	*vector = NULL;
	*nrElemente = 0;
}

void copiazaAnumiteElemente(struct Masina* vector, int nrElemente, float prag, struct Masina** vectorNou, int* dimensiune) {
	//parametrul prag poate fi modificat in functie de 
	// tipul atributului ales pentru a indeplini o conditie
	//este creat un nou vector cu elementele care indeplinesc acea conditie
	(*dimensiune) = 0;
	int k = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].tonaj < prag) {
			(*dimensiune)++;
		}
	}
	(*vectorNou) = malloc(sizeof(struct Masina) * (*dimensiune));
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].tonaj < prag) {
			(*vectorNou)[k] = vector[i];
			(*vectorNou)[k].model = malloc(sizeof(char*) * (strlen(vector[i].model) + 1));
			strcpy_s((*vectorNou)[k].model, strlen(vector[i].model) + 1, vector[i].model);
			k++;
		}
	}
}

struct Masina getPrimulElementConditionat(struct Masina* vector, int nrElemente, const char* conditie) {
	//trebuie cautat elementul care indeplineste o conditie
	//dupa atributul de tip char*. Acesta este returnat.
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].model != NULL && strcmp(vector[i].model, conditie) == 0) {
			return vector[i];
		}
	}
	struct Masina s;
	s.id = 1;

	return s;
}

//Exercitii extra 

// 1.
// Sa se stocheze intr-un vector doar elementele care au un tonaj cuprins intre o anumita valoare primita ca parametru si
// contin un anumit char*. 

struct Masina* adaugaAnumiteElemente(struct Masina* vector, int nrElemente, float pragInferior, float pragSuperior, const char* conditie, int* nrElementeNou) {
	*nrElementeNou = 0;

	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].tonaj > pragInferior && vector[i].tonaj < pragSuperior && strcmp(vector[i].model, conditie) == 0) {
			(*nrElementeNou)++;
		}
	}
	struct Masina* vectorNou = malloc(sizeof(struct Masina) * (*nrElementeNou));

	int k = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].tonaj > pragInferior && vector[i].tonaj < pragSuperior && strcmp(vector[i].model, conditie) == 0) {
			vectorNou[k] = vector[i];
			vectorNou[k].model = malloc(sizeof(char) * (strlen(vector[i].model) + 1));
			strcpy_s(vectorNou[k].model, strlen(vector[i].model) + 1, vector[i].model);
			k++;
		}
	}
	return vectorNou;
}
	


int main() {
	struct Masina m;
	m = initializare(2, "Audi", 3.4, 'a');
	afisare(m);

	struct Masina* vectorMasini;
	int nrMasini = 3;
	vectorMasini = malloc(sizeof(struct Masina) * nrMasini);
	vectorMasini[0] = initializare(3, "BMW", 5.6, 'q');
	vectorMasini[1] = initializare(3, "Mercedes", 4.6, 'r');
	vectorMasini[2] = initializare(3, "Opel", 8.9, 'f');

	afisareVector(vectorMasini, nrMasini);

	struct Masina* vectorMasiniCopiate;
	int nrMasiniCopiate = 2;
	vectorMasiniCopiate = copiazaPrimeleMasini(vectorMasini, nrMasini, nrMasiniCopiate);
	printf("Masini copiate\n");
	afisareVector(vectorMasiniCopiate, nrMasiniCopiate);

	printf("=-----------------------------------------------=\n");
	printf("Exercitiu extra\n");

	int nrElementeNou;
	struct Masina* vectorConditionat = adaugaAnumiteElemente(vectorMasini, nrMasini, 4.6, 8.9, "BMW", &nrElementeNou);
	afisareVector(vectorConditionat, nrElementeNou);

	printf("=-----------------------------------------------=\n");
	
	struct Masina* vectorMasiniUsoare;
	copiazaAnumiteElemente(vectorMasini, nrMasini, 7.5, &vectorMasiniUsoare, &nrMasiniCopiate);
	afisareVector(vectorMasiniUsoare, nrMasiniCopiate);

	printf("\nMasina gasita\n");
	struct Masina masinaGasita = getPrimulElementConditionat(vectorMasiniUsoare, nrMasini, "BMW");
	afisare(masinaGasita);


	dezalocare(&vectorMasiniCopiate, &nrMasiniCopiate);

	return 0;
}