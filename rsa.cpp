#include "Anumber.h"
#include "stdio.h"
#include "stdlib.h"

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

int main (int argc, char **argv) {
	struct keyPair key;
	//keyGen(&key);
	readKeys(&key);
	
	key.pvt.show();
	key.pub.show();
	key.n.show();
	
	encrypt(&key);
	
	printf("\n\n");
	
	decrypt(&key);
	
	return 1;
}

void encrypt(struct keyPair* key) {
	FILE* fmessage = fopen("message.txt", "r");
	FILE* fencrypted = fopen("encryptded", "w");
	
	fseek(fmessage, 0, SEEK_END);
	int length = ftell(fmessage) - 1;
	printf("message length = %d\n", length);
	fseek(fmessage, 0, SEEK_SET);
	
	int chunk_size = ((key->n).len() - 2) / 3;
	printf("chunk size = %d\n", chunk_size);
	int chunk_num = length / chunk_size;
	printf("chunk num = %d\n", chunk_num);
	
	unsigned char* chunk_encoded = (unsigned char*) malloc((chunk_size * 3) + 1);
	unsigned char* chunk = (unsigned char*) malloc(chunk_size);
	
	int i;
	for(i=0; i!=chunk_num; i++) {
		printf("chunk %d\n", i);
		fread(chunk, 1, chunk_size, fmessage);
		encodeChunk(chunk, chunk_encoded, chunk_size);
		
		Anumber message((char*) chunk_encoded);
		printf("message encoded = "); message.show();
		Anumber crypted = message.modExp(key->pub, key->n);
		printf("crypted = "); crypted.show();
		
		fwrite(crypted.buffer + BUFLEN - crypted.len(), 1, crypted.len(), fencrypted);
		fputc(32, fencrypted);
	}
	
	if(length % chunk_size != 0) {
		printf("chunk %d\n", i);
		int last_size = length % chunk_size;
		printf("last size = %d\n", last_size);
		
		free(chunk_encoded);
		free(chunk);
		chunk_encoded = (unsigned char*) malloc((last_size * 3) + 2);
		chunk = (unsigned char*) malloc(last_size);
		
		fread(chunk, 1, last_size, fmessage);
		encodeChunk(chunk, chunk_encoded, last_size);
		
		Anumber message((char *) chunk_encoded);
		printf("message encoded = "); message.show();
		Anumber crypted = message.modExp(key->pub, key->n);
		printf("crypted = "); crypted.show();
		printf("len crypted = %d\n", crypted.len());
		
		fwrite(crypted.buffer + BUFLEN - crypted.len(), 1, crypted.len(), fencrypted);
		
		fputc(32, fencrypted);
		
		printf("encrypted message written\n");
	}
	fclose(fmessage);
	fclose(fencrypted);
}

void encodeChunk(unsigned char* chunk, unsigned char* chunk_encoded, int chunk_size) {
	int i;
	unsigned char* ptr = chunk_encoded;
	*ptr = '1';
	ptr++;
	for(i=0; i!=chunk_size; i++) {
		printf("%d, %c\n", chunk[i], chunk[i]);
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
	FILE* fencrypted = fopen("encryptded", "r");
	FILE* fdecrypted = fopen("decrypted", "w");
	
	unsigned char* chunk_encoded;
	unsigned char* chunk;
	unsigned char* ptr;
	int size;
	int chunk_num;
	//bool quit = false;
	
	const int CHUNK_SIZE = 5000;
	
	chunk_encoded = (unsigned char*) malloc(CHUNK_SIZE);
	ptr = chunk_encoded;
	unsigned char digit;
	size = 0;
	while(!feof(fencrypted)) {
		digit = fgetc(fencrypted);
		if(digit != ' ') {
			*ptr = digit;
			ptr++;
			size++;
		}
		else {
			int k;
			for(k=0; k!=size; k++) chunk_encoded[k] += 48;
			
			Anumber crypted((char *)chunk_encoded, size);
			printf("crypted = "); crypted.show();
			Anumber message = crypted.modExp(key->pvt, key->n);
			printf("message decrypted encoded = "); message.show();
			
			chunk = (unsigned char*) malloc((message.len() - 1) / 3);
			decodeChunk(chunk, (unsigned char*) message.buffer + BUFLEN - message.len(), message.len());
			
			printf("decoded\n");
			fwrite(chunk, 1, (message.len() - 1) / 3, fdecrypted);
			
			size = 0;
			ptr = chunk_encoded;
			free(chunk);
		}
	}
	
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
	Anumber p = newPrime(10);
	Anumber q = newPrime(10);
	Anumber n = p * q;
	
	Anumber phi = (p - 1) * (q - 1);
	
	Anumber e;
	
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
	Anumber e("65537");
	Anumber d("1566313612777345907799055755564377114598597777438697569779477424564149640433480506656444659315440547759266794017002517360017056024639792557302664815286907137");
	Anumber n("3399506399542618848835167474248992679806871855212608379607816001247207411083952891013102265076718301662808077136006733812506861100459986200366033132099547053");
	//Anumber d("17661953");
	//Anumber n("169909673");
	
	key->pub = e;
	key->pvt = d;
	key->n = n;
}

//sdfasgdfgdsgrgdfgsrgsgafgegagagregaezfasgeagdgsdfgshgvisabgisdhbguidsbugfdsugaegdghdsfhsth
