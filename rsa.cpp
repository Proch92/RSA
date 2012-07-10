#include "Anumber.h"
#include "stdio.h"

struct keyPair {
	Anumber pub;
	Anumber pvt;
	Anumber n;
};

void keyGen(struct keyPair*);
void encrypt(strcut keyPair*);
void encodeChunk(char*, char*, int);
void decrypt(strcut keyPair*);
void decodeChunk(char*, char*, int);

int main (int argc, char **argv) {
	struct keyPair key;
	keyGen(&key);
	
	
}

void encrypt(strcut keyPair* key) {
	FILE* fmessage = fopen("message", r);
	FILE* fencrypted = fopen("encryptded", w);
	
	fseek(fmessage, 0, SEEK_END);
	int length = ftell(fmessage);
	fseek(fmessage, 0, SEEK_SET);
	
	int chunk_size = ((key->n).len() - 2) / 3;
	int chunk_num = length / chunk_size;
	
	char* chunk_encoded = (char*) malloc((chunk_size * 3) + 1);
	char* chunk = (char*) malloc(chunk_size);
	
	int i;
	for(i=0; i!=chunk_num; i++) {
		fread(chunk, 1, chunk_size, fmessage);
		encode(chunk, chunk_encoded, chunk_size);
		
		Anumber message(chunk_encoded);
		Anumber crypted = message.pow(key->pub) % key->n;
		
		fwrite(crypted.buffer, 1, (chunk_size * 3) + 1, fencrypted);
		fputc(32, fencrypted);
	}
	
	if(length % chunk_size != 0) {		
		int last_size = length % chunk_size;
		
		free(chunk_encoded);
		free(chunk);
		char* chunk_encoded = (char*) malloc((last_size * 3) + 1);
		char* chunk = (char*) malloc(last_size);
		
		fread(chunk, 1, last_size, fmessage);
		encode(chunk, chunk_encoded, last_size);
		
		Anumber message(chunk_encoded);
		Anumber crypted = message.pow(key->pub) % key->n;
		
		fwrite(crypted.buffer, 1, (last_size * 3) + 1, fencrypted);
		
		fputc(32, fencrypted);
	}
}

void encodeChunk(char* chunk, char* chunk_encoded, int chunk_size) {
	int i;
	char* ptr = chunk_encoded;
	for(i=0; i!=chunk_size; i++) {
		*ptr = (chunk[i] % 10) + '0';
		ptr++;
		chunk[i] /= 10;
		*ptr = (chunk[i] % 10) + '0';
		ptr++;
		*ptr = (chunk[i] / 10) + '0';
		ptr++;
	}
	chunk_encoded[0] = '1';
}

void decrypt(strcut keyPair* key) {
	FILE* fencrypted = fopen("encryptded", r);
	FILE* fdecrypted = fopen("decrypted", w);
	
	char* chunk_encoded;
	char* chunk;
	char* ptr;
	char digit;
	int size;
	int chunk_num;
	
	size = 0;
	do {
		digit = fgetc(fencrypted);
		size++;
	} while(digit != ' ');
	fseek(fmessage, 0, SEEK_SET);
	
	fseek(fmessage, 0, SEEK_END);
	int length = ftell(fmessage);
	fseek(fmessage, 0, SEEK_SET);
	
	chunk_num = length / size;
	
	chunk_encoded = (char*) malloc(size - 1);
	
	int chunk_size = (size - 2) / 3;
	chunk = (char*) malloc(chunk_size);
	
	int i;
	for(i=0; i!=chunk_num; i++) {
		fread(chunk_encoded, 1, size - 1, fencrypted);
		
		Anumber crypted(chunk_encoded);
		Anumber message = crypted.pow(key->pvt) % key->n;
		
		decodeChunk(chunk, chunk_encoded, size - 1);
		
		fwrite(chunk, 1, chunk_size, fdecrypted);
		
		fgetc(fencrypted);
	}
	
	if(length % size != 0) {
		int last_size = length % size;
		
		free(chunk_encoded);
		free(chunk);
		char* chunk_encoded = (char*) malloc(last_size - 1);
		char* chunk = (char*) malloc((last_size - 2) / 3);
		
		fread(chunk_encoded, 1, (last_size - 2) / 3, fencrypted);
		
		Anumber crypted(chunk_encoded);
		Anumber message = crypted.pow(key->pvt) % key->n;
		
		decodeChunk(chunk, chunk_encoded, last_size - 1);
		
		fwrite(chunk, 1, (last_size - 2) / 3, fdecrypted);
	}
}

void decodeChunk(char* chunk, char* chunk_encoded, int size) {
	char *ptr = chunk;
	
	int i = 0;
	
	while(i != size) {
		*ptr = 100 * chunk_encoded[i];
		*ptr += 10 * chunk_encoded[i + 1];
		*ptr += chunk_encoded[i + 2];
		
		i += 3;
		ptr++;
	}
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
