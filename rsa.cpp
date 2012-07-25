#include "Anumber.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "string.h"

struct keyPair {
	Anumber pub;
	Anumber pvt;
	Anumber n;
};

void readKeys(struct keyPair*);
void keyGen(struct keyPair*);
void encrypt(struct keyPair*);
void encodeChunk(unsigned char*, unsigned char*, int);
void decrypt(struct keyPair*);
void decodeChunk(unsigned char*, unsigned char*, int);

int numero_chunk;

int main (int argc, char **argv) {
	struct keyPair key;
	//keyGen(&key);
	readKeys(&key);
	
	key.pvt.show();
	key.pub.show();
	key.n.show();
	
	long start = time(0);
	
	encrypt(&key);
	printf("encrypted\n\n");
	
	decrypt(&key);
	printf("decrypted\n\n");
	
	long end = time(0);
	long tempo = end - start;
	printf("il tempo di esecuzione è: %ds\n", tempo);
	
	return 1;
}

void encrypt(struct keyPair* key) {
	FILE* fmessage = fopen("file/messaggio.txt", "r");
	FILE* fencrypted = fopen("file/cifrato.txt", "w");
	
	fseek(fmessage, 0, SEEK_END);
	int length = ftell(fmessage) - 1;
	fseek(fmessage, 0, SEEK_SET);
	
	int chunk_size = ((key->n).len() - 2) / 3;
	int chunk_num = length / chunk_size;
	numero_chunk = chunk_num;
	
	unsigned char* chunk_encoded = (unsigned char*) malloc((chunk_size * 3) + 1);
	unsigned char* chunk = (unsigned char*) malloc(chunk_size);
	
	int i;
	for(i=0; i!=chunk_num; i++) {
		printf("(%d/%d)\n", i, chunk_num);
		fread(chunk, 1, chunk_size, fmessage);
		encodeChunk(chunk, chunk_encoded, chunk_size);
		
		Anumber message((char*) chunk_encoded);
		
		Anumber crypted(message.modExp(key->pub, key->n));
		
		int cryplen = crypted.len();
		fwrite(crypted.buffer + BUFLEN - cryplen, 1, cryplen, fencrypted);
		fputc(32, fencrypted);
	}
	
	if(length % chunk_size != 0) {
		int last_size = length % chunk_size;
		
		free(chunk_encoded);
		free(chunk);
		chunk_encoded = (unsigned char*) malloc((last_size * 3) + 2);
		chunk = (unsigned char*) malloc(last_size);
		
		fread(chunk, 1, last_size, fmessage);
		encodeChunk(chunk, chunk_encoded, last_size);
		
		Anumber message((char *) chunk_encoded);
		
		Anumber crypted(message.modExp(key->pub, key->n));
		
		int cryplen = crypted.len();
		fwrite(crypted.buffer + BUFLEN - cryplen, 1, cryplen, fencrypted);
		
		fputc(32, fencrypted);
	}
	free(chunk_encoded);
	free(chunk);
	
	fclose(fmessage);
	fclose(fencrypted);
}

void encodeChunk(unsigned char* chunk, unsigned char* chunk_encoded, int chunk_size) {
	int i;
	unsigned char* ptr = chunk_encoded;
	*ptr = '1';
	ptr++;
	for(i=0; i!=chunk_size; i++) {
		*ptr = (chunk[i] % 10) + '0';
		ptr++;
		chunk[i] /= 10;
		*ptr = (chunk[i] % 10) + '0';
		ptr++;
		*ptr = (chunk[i] / 10) + '0';
		ptr++;
	}
}

void decrypt(struct keyPair* key) {
	FILE* fencrypted = fopen("file/cifrato.txt", "r");
	FILE* fdecrypted = fopen("file/org_messaggio.txt", "w");
	
	unsigned char* chunk_encoded = NULL;
	unsigned char* chunk = NULL;
	unsigned char* ptr;
	int size;
	
	const int CHUNK_SIZE = 5000;
	
	chunk_encoded = (unsigned char*) malloc(CHUNK_SIZE);
	ptr = chunk_encoded;
	unsigned char digit;
	size = 0;
	int j = 0;
	while(!feof(fencrypted)) {
		digit = fgetc(fencrypted);
		if(digit != ' ') {
			*ptr = digit;
			ptr++;
			size++;
		}
		else {
			j++;
			printf("(%d/%d)\n", j, numero_chunk);
			
			int k;
			for(k=0; k!=size; k++) chunk_encoded[k] += 48;
			
			Anumber crypted((char *)chunk_encoded, size);
			
			Anumber message(crypted.modExp(key->pvt, key->n));
			
			chunk = (unsigned char*) malloc((message.len() - 1) / 3);
			decodeChunk(chunk, (unsigned char*) message.buffer + BUFLEN - message.len(), message.len());
			
			fwrite(chunk, 1, (message.len() - 1) / 3, fdecrypted);
			
			size = 0;
			ptr = chunk_encoded;
			free(chunk);
		}
	}
	
	free(chunk_encoded);
	
	fclose(fencrypted);
	fclose(fdecrypted);
}

void decodeChunk(unsigned char* chunk, unsigned char* chunk_encoded, int size) {
	unsigned char *ptr = chunk;
	
	int i = 1; //bisogna scartare l'1 che avevamo messo encriptandolo
	
	while(i != size) {
		*ptr = 100 * chunk_encoded[i+2];
		*ptr += 10 * chunk_encoded[i+1];
		*ptr += chunk_encoded[i];
		i += 3;
		ptr++;
	}
}

void keyGen(struct keyPair* key) {
	Anumber p(newPrime(10));
	Anumber q(newPrime(10));
	Anumber n(p * q);
	
	Anumber phi((p - 1) * (q - 1));
	
	Anumber e(0);
	
	Anumber d;
	do {
		do {
			e.random(6);
		} while(!(MCD(phi, e) == 1));
		
		d = e.invers(n);
	} while(d == 0);
	
	key->pub = e;
	key->pvt = d;
	key->n = n;
}

void readKeys(struct keyPair* key) {
	FILE* chiavePubblica = fopen("file/chiavePubblica.txt", "r");
	FILE* chiavePrivata = fopen("file/chiavePrivata.txt", "r");
	
	char buffer[200];
	
	fgets(buffer, 200, chiavePubblica);
	Anumber n(buffer, strlen(buffer) - 2);
	fgets(buffer, 200, chiavePubblica);
	Anumber e(buffer, strlen(buffer));
	
	fgets(buffer, 200, chiavePrivata);
	fgets(buffer, 200, chiavePrivata);
	Anumber d(buffer, strlen(buffer));
	
	key->pub = e;
	key->pvt = d;
	key->n = n;
	
	fclose(chiavePubblica);
	fclose(chiavePrivata);
}
