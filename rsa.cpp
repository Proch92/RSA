#include "Anumber.h"
#include "stdio.h"

struct keyPair {
	Anumber pub;
	Anumber pvt;
	Anumber n;
};

void keyGen(struct keyPair*);

int main (int argc, char **argv) {
	struct keyPair key;
	keyGen(&key);
}

void keyGen(struct keyPair* key) {
	Anumber p = newPrime();
	Anumber q = newPrime();
	Anumber n = p * q;
	
	Anumber phi = (p - 1) * (q - 1);
	
	Anumber e;
	
	do {
		e.random();
	} while(!(MCD(phi, e) == 1));
	
	Anumber d = e.invers();
	
	key->pub = e;
	key->pvt = d;
	key->n = n;
}
