#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include<string.h>


typedef struct Mail Mail;
struct Mail {
	char* expeditor;
	int prioritate;
};

Mail initializareMail(char* expeditor, int prioritate) {
	Mail m;
	m.expeditor = (char*)malloc(sizeof(char) * (strlen(expeditor) + 1));
	strcpy(m.expeditor, expeditor);
	m.prioritate = prioritate;
	return m;
}

void afisareMail(Mail m) {
	printf("Expeditor: %s | Prioritate: %d \n", m.expeditor, m.prioritate);
}

typedef struct Heap Heap;
struct Heap {
	Mail* vector;
	int dim;
};

void filtrare(Heap h, int index) {
	int indexRadacina = index;
	int indexStg = 2 * index + 1;
	int indexDrp = 2 * index + 2;

	if (indexStg < h.dim &&
		h.vector[indexStg].prioritate < h.vector[indexRadacina].prioritate) {
		indexRadacina = indexStg;
	}

	if (indexDrp < h.dim &&
		h.vector[indexDrp].prioritate < h.vector[indexRadacina].prioritate) {
		indexRadacina = indexDrp;
	}

	if (index != indexRadacina) {
		Mail aux = h.vector[index];
		h.vector[index] = h.vector[indexRadacina];
		h.vector[indexRadacina] = aux;

		filtrare(h, index);
	}
}

Heap inserare(Heap h, Mail nou) {
	Mail* copie = (Mail*)malloc(sizeof(Mail) * (h.dim + 1));
	for (int i = 0; i < h.dim; i++) {
		copie[i] = h.vector[i];
	}
	copie[h.dim] = nou;
	free(h.vector);
	h.dim++;
	h.vector = copie;
	for (int i = ((h.dim) - 1) / 2; i > 0; i--) {
		filtrare(h, i);
	}
	return h;
}

Heap extragere(Heap h, Mail* m) {
	(*m) = initializareMail(h.vector[h.dim - 1].expeditor,
		h.vector[h.dim - 1].prioritate);

	Mail* copie = (Mail*)malloc(sizeof(Mail) * (h.dim - 1));
	for (int i = 0; i < h.dim - 1; i++) {
		copie[i] = h.vector[i];
	}
	free(h.vector);
	h.dim--;
	h.vector = copie;
	for (int i = ((h.dim) - 1) / 2; i > 0; i--) {
		filtrare(h, i);
	}
	return h;
}

void main() {
	Heap h;
	h.dim = 0;
	h.vector = NULL;

	//h.dim = 3; 
	//h.vector = (Mail*)malloc(sizeof(Mail) * (h.dim));

	Mail m1 = initializareMail("Marcu", 9);
	Mail m2 = initializareMail("Loredana", 5);
	Mail m3 = initializareMail("Sorina", 6);

	h = inserare(h, m1);
	h = inserare(h, m2);
	h = inserare(h, m3);

	for (int i = 0; i < h.dim; i++) {
		afisareMail(h.vector[i]);
	}

	Mail m;

}