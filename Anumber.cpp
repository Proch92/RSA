#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "time.h"
#include "math.h"
#include "Anumber.h"

Anumber karatsuba(Anumber, Anumber);
Anumber schoolbook(Anumber, Anumber);
void eucEx(Anumber&, Anumber, Anumber&, Anumber&, Anumber&);

Anumber::Anumber() {
	sign = false;
	
	memset(buffer, 0, BUFLEN);
	
	srand(time(NULL));
}

Anumber::Anumber(char* init) {
	if(*init == '-') {
		sign = true;
		init++;
	}
	else sign = false;
	
	
	memset(buffer, 0, BUFLEN);
	
	int lung = strlen(init);
	
	strncpy(buffer + (BUFLEN - lung), init, lung);
	
	char *ptr;
	ptr = buffer + (BUFLEN - lung);
	int i;
	for(i=0; i!=lung; i++) {
		*ptr -= 48;
		ptr++;
	}
	
	srand(time(NULL));
}

Anumber::Anumber(char* init, int size) {
	sign = false;
	
	memset(buffer, 0, BUFLEN);
	
	int lung = size;
	
	strncpy(buffer + (BUFLEN - lung), init, lung);
	
	char *ptr;
	ptr = buffer + (BUFLEN - lung);
	int i;
	for(i=0; i!=lung; i++) {
		*ptr -= 48;
		ptr++;
	}
	
	srand(time(NULL));
}

Anumber::Anumber(int init) {
	sign = (init < 0);
	
	memset(buffer, 0, BUFLEN);
	
	char *ptr;
	ptr = buffer + (BUFLEN - 1);
	while(init != 0) {
		*ptr = init % 10;
		init /= 10;
		ptr--;
	}
	
	srand(time(NULL));
}

int Anumber::len() {
	int i=0;
	bool found = false;
	
	while(!found && i != BUFLEN) {
		if(buffer[i] != 0) found = true;
		else i++;
	}
	
	return BUFLEN - i;
}

void Anumber::shr(int shift) {
	int i;
	for(i=BUFLEN - 1; i>=0; i--) {
		if(i < shift) buffer[i] = 0;
		else buffer[i] = buffer[i - shift];
	}
}

Anumber Anumber::invers(Anumber m) {
	Anumber x(1);
	Anumber y(0);
	Anumber d;
	
	Anumber e = *this;
	
	eucEx(e, m, d, x, y);
	
	printf("DONE #########################\n");
	
	Anumber b(1);
	if ((b % d) == 0)
		return (x * (b / d)) % m;
	else {
		printf("error in key generation. [invers]\n");
		return Anumber(0);
	}
}

void eucEx(Anumber &a, Anumber b, Anumber &d, Anumber &x, Anumber &y) { //inverso moltiplicativo
	if(b == 0) {
		d = a;
		x = 1; y = 0;
	}
	else {
		eucEx(b, a%b, d, x, y);
		Anumber temp = x;
		x = y;
		y = temp - a / b * y;
	}
}

void Anumber::split(Anumber *half1, Anumber *half2, int lung) {
	int i;
	//int lung = len();
	int med = lung / 2;
	//if(lung % 2 == 1) med++;
	
	char *tmp = buffer + (BUFLEN - lung);
	char *tmp1 = half1->buffer + (BUFLEN - (lung - med));
	char *tmp2 = half2->buffer + (BUFLEN - med);
	for(i=0; i!=(lung - med); i++) {
		*tmp1 = *tmp;
		tmp++;
		tmp1++;
	}
	for(i=0; i!=med; i++) {
		*tmp2 = *tmp;
		tmp++;
		tmp2++;
	}
}

void Anumber::random() {
	//sign = rand() % 2;
	sign = false;
	
	int i;
	int len = BUFLEN / 10;
	for(i=BUFLEN - 1; i>=BUFLEN - len; i--)
		buffer[i] = rand()%10;
}

void Anumber::random(int len) {
	//sign = rand() % 2;
	sign = false;
	
	int i;
	for(i=BUFLEN - 1; i>=BUFLEN - len; i--)
		buffer[i] = rand()%10;
}

void Anumber::show() {
	if(!len()) {
		printf("0\n");
		return;
	}
	
	if(sign)
		printf("-");
	
	int i;
	//printf("len = %d\n", len());
	for(i=BUFLEN - len(); i!=BUFLEN; i++)
		printf("%d", buffer[i]);
	printf("\n");
}

void Anumber::show_all() {
	if(sign)
		printf("-");
	
	int i;
	for(i=0; i!=BUFLEN; i++)
		printf("%d", buffer[i]);
	printf("\n");
}

void Anumber::exp10(int shift) {
	if(shift == 0) return;
	int i;
	for(i = shift; i!=BUFLEN; i++) {
		buffer[i - shift] = buffer[i];
		if(i >= BUFLEN - shift) buffer[i] = 0;
	}
}

int Anumber::Alog10() { //it is supposed that the number is a 10 pow (i dont check the is10pow())
	int i;
	int returning = 0;
	bool found = false;
	
	for(i = 0; i!=BUFLEN && !found; i++)
		if(buffer[i] == 1) found = true;
	
	for(;i!=BUFLEN && found; i++)
		returning++;
	
	return returning;
}

bool Anumber::is10pow(){
	int i;
	bool found = false;
	
	for(i = 0; i!=BUFLEN && !found; i++) {
		if(buffer[i] > 1) return false;
		if(buffer[i] == 1) found = true;
	}
	
	for(;i!=BUFLEN && found; i++)
		if(buffer[i] != 0) return false;
	
	if(found) return true;
	
	return false;
}

Anumber Anumber::pow(Anumber op) {
	Anumber returning;
	Anumber base = *this;
	Anumber exp = op;
	
	int oplen = op.len();
	char* exp2 = (char*) malloc(oplen * oplen);
	
	int i=0;
	while(!(exp == 0)) {
		exp2[i] = exp % 2;
		exp /= 2;
		i++;
	}
	int len2 = i;
	
	Anumber pow;
	pow = 1;
	
	for(i=len2 - 1; i>=0; i--) {
		pow *= pow;
		if(exp2[i])
			pow *= base;
	}
	
	returning = pow;
	
	return returning;
}

Anumber Anumber::modExp(Anumber op, Anumber mod) {
	int oplen = op.len();
	char* exp2 = (char*) malloc(oplen * oplen);
	
	Anumber exp = op;
	int i=0;
	while(!(exp == 0)) {
		exp2[i] = exp % 2;
		exp /= 2;
		i++;
	}
	int len2 = i;
	Anumber base = *this;
	Anumber result(1);
	for(i=0; i!=len2; i++) {
		if(exp2[i]) result = (result * base) % mod;
		base = (base * base) % mod;
	}
	
	return result;
}

void Anumber::operator = (Anumber op) {
	//strncpy(buffer, op.buffer, BUFLEN);
	int i;
	for(i=0; i!=BUFLEN; i++)
		buffer[i] = op.buffer[i];
	
	sign = op.sign;
}

void Anumber::operator = (char op) {
	if(abs(op) < 10) {
		buffer[BUFLEN - 1] = op;
		
		sign = (op < 0);
	}
}

void Anumber::operator += (Anumber op) {
	char carry = 0;
	int i;
	
	for(i = BUFLEN - 1; i>=0; i--) {
		buffer[i] += op.buffer[i] + carry;
		if(buffer[i] > 9) {
			carry = 1;
			buffer[i] -= 10;
		}
		else carry = 0;
	}
}

void Anumber::operator -= (Anumber op) {
	char carry = 0;
	int i;
	
	for(i = BUFLEN - 1; i>=0; i--) {
		buffer[i] -= op.buffer[i] + carry;
		if(buffer[i] < 0) {
			carry = 1;
			buffer[i] += 10;
		}
		else carry = 0;
	}
	
	if(carry) sign = !sign;
}

Anumber Anumber::operator ++ () {	
	char carry = 0;
	int i;
	
	i = BUFLEN - 1;
	buffer[i] += 1;
	if(buffer[i] > 9) {
		carry = 1;
		buffer[i] -= 10;
	}
	i--;
	
	for(; i>=0; i--) {
		buffer[i] += carry;
		if(buffer[i] > 9) {
			carry = 1;
			buffer[i] -= 10;
		}
		else carry = 0;
	}
	
	return *this;
}

Anumber Anumber::operator + (Anumber op) {
	Anumber result;
	
	if(!sign && !op.sign) {
		result = *this;
		result += op;
		result.sign = false;
	}
	else if(sign && op.sign) {
		result = *this;
		result += op;
		result.sign = true;
	}
	else if(sign ^ op.sign) {
		if(sign) {
			if(Aabs(*this) > op) {
				result = *this;
				result -= op;
				result.sign = true;
			}
			else {
				result = op;
				result -= *this;
				result.sign = false;
			}
		}
		else {
			if(*this > Aabs(op)) {
				result = *this;
				result -= op;
				result.sign = false;
			}
			else {
				result = op;
				result -= *this;
				result.sign = true;
			}
		}
	}
	
	return result;
}

Anumber Anumber::operator - (Anumber op) {
	Anumber result;
	
	if(!sign && !op.sign) {
		if(*this > op) {
			result = *this;
			result -= op;
			result.sign = false;
		}
		else {
			result = op;
			result -= *this;
			result.sign = true;
		}
	}
	else if(sign && op.sign) {
		if(*this > op) {
			result = op;
			result -= *this;
			result.sign = false;
		}
		else {
			result = *this;
			result -= op;
			result.sign = true;
		}
	}
	else if(sign ^ op.sign) {
		result = *this;
		result += op;
		if(sign)
			result.sign = true;
		else
			result.sign = false;
	}
	
	return result;
}

Anumber Anumber::operator - (char op) {
	Anumber returning = *this;
	
	if(op == 1) { //only 1 admitted... dont make 0 - 1 !!!!!!!!! OVERFLOW
		char carry = 0;
		int i;
		
		i = BUFLEN - 1;
		
		returning.buffer[i] -= 1;
		if(returning.buffer[i] < 0) {
			carry = 1;
			returning.buffer[i] += 10;
		}
		i--;
		
		for(; i>=0; i--) {
			returning.buffer[i] -= carry;
			if(returning.buffer[i] < 0) {
				carry = 1;
				returning.buffer[i] += 10;
			}
			else carry = 0;
		}
		
		return returning;
	}
}

bool Anumber::operator == (int num) {
	if(num == 0)
		return !(len());
	if(num == 1 && len() == 1 && buffer[BUFLEN - 1] == 1)
		return true;
	
	return false; //it counts only 0 and 1
}

bool Anumber::operator == (Anumber op) {
	if(sign != op.sign) return false;
	
	bool found = false;
	int i = 0;
	
	while (!found && i!=BUFLEN) {
		if(buffer[i] != op.buffer[i])
			found = true;
		i++;
	}
	
	if(found) return false;
	else return true;
}

bool Anumber::operator != (int num) { //DOES NOT WORK PROPERLY
	if(num == 0) {
		int i = 0, lung = len();
		bool found = false;
		while(!found && i<lung) {
			if(buffer[i] != 0) found = true;
			i++;
		}
		
		return found;
	}
	
	return false; //it counts only 0
}

bool Anumber::operator > (Anumber op) {
	if(sign && !op.sign) return false;
	if(!sign && op.sign) return true;
	
	int i=0;
	
	while(i!=BUFLEN) {
		if(buffer[i] != op.buffer[i])
			return (buffer[i] > op.buffer[i]) ^ sign;
		i++;
	}
	
	return false;
}

bool Anumber::operator < (Anumber op) {
	if(sign && !op.sign) return true;
	if(!sign && op.sign) return false;
	
	int i=0;
	
	while(i!=BUFLEN) {
		if(buffer[i] != op.buffer[i])
			return (buffer[i] < op.buffer[i]) ^ sign;
		i++;
	}
	
	return false;
}

void Anumber::operator *= (Anumber op) {	
	Anumber result;
	
	if(op == 0 || *this == 0) {
		sign = false;
		memset(buffer, 0, BUFLEN);
	}
	else {
		/*int num10 = 0;
		Anumber op1;
		op1 = *this;
		
		if(len() % 2 == 1) {
			op1.exp10(1);
			num10++;
		}
		if(op.len() % 2 == 1) {
			op.exp10(1);
			num10++;
		}
		
		int len1 = len();
		int len2 = op.len();
		if(abs(len1 - len2)) {
			if(len1 < len2) {
				op1.exp10(len2 - len1);
				num10 += len2 - len1;
			}
			else {
				op.exp10(len1 - len2);
				num10 += len1 - len2;
			}
		}*/
		
		
		
		result = karatsuba(*this, op);
		/*printf("shr = %d\n", num10);
		result.show();
		result.shr(num10);*/
		
		int i;
		for(i=0; i!=BUFLEN; i++)
			buffer[i] = result.buffer[i];
		sign = (sign ^ op.sign);
	}
}

void Anumber::operator *= (int num) {
	if(num == 0 || *this == 0) {
		sign = false;
		memset(buffer, 0, BUFLEN);
		return;
	}
	
	float shift = log10(num);
	
	if(num % 10 == 0) {
		if(shift == 0) return;
		
		int i;
		for(i = shift; i!=BUFLEN; i++) {
			buffer[i - (int)shift] = buffer[i];
			if(i >= BUFLEN - shift) buffer[i] = 0;
		}
	}
	else {
		int i;
		Anumber result;
		for(i=0; i!=num; i++)
			result += *this;
		
		*this = result;
	}
}

Anumber Anumber::operator * (Anumber op) {
	Anumber result;
	
	result = *this;
	result *= op;
	
	return result;
}

Anumber Anumber::operator * (int num) {
	Anumber result;
	
	result = *this;
	result *= num;
	
	return result;
}

Anumber Anumber::operator / (Anumber op) {
	Anumber result;
	
	result = *this;
	result /= op;
	
	return result;
}

Anumber Anumber::operator / (char op) {
	Anumber result;
	
	result = *this;
	result /= op;
	
	return result;
}

bool smaller(Anumber num1, Anumber num2, int k, int m) {
	int i = BUFLEN - m;
	
	while(i != BUFLEN) {
		if(num1.buffer[i - k - 1] != num2.buffer[i - 1])
			return (num1.buffer[i - k - 1] < num2.buffer[i - 1]);
		else i++;
	}
	
	return (num1.buffer[i - k - 1] < num2.buffer[i - 1]);
}

void Anumber::operator /= (Anumber op) {
	/*if(op.is10pow()) {
		printf("potenza 10\n");
		int i;
		
		Anumber returning = op;
		
		int lung = op.Alog10();
		printf("lung = %d\n", lung);
		
		for(i=BUFLEN-1; i>=0; i--) {
			if(i >= lung)
				returning.buffer[i] = returning.buffer[i - lung];
			else returning.buffer[i] = 0;
		}
		return;
	}*/
	
	Anumber result;
	
	if(len() < op.len()) *this = result;
	
	int dividend_len = len();
	int divisor_len = op.len();
	
	char factor = 10 / (op.buffer[BUFLEN - divisor_len] + 1);
	Anumber reminder = *this * factor;
	Anumber divisor = op * factor;
	
	divisor_len = divisor.len();
	
	Anumber part_product;
	
	Anumber quotient;
	
	int k;
	char quotient_digit;
	for(k = 0; k <= dividend_len - divisor_len; k++) {
		int rem3 = reminder.buffer[k + (BUFLEN - dividend_len) - 1] * 100 + reminder.buffer[k + (BUFLEN - dividend_len) + 1 - 1] * 10 + reminder.buffer[k + (BUFLEN - dividend_len) + 2 - 1];
		int div2 = divisor.buffer[(BUFLEN - divisor_len)] * 10 + divisor.buffer[(BUFLEN - divisor_len) + 1];
		
		quotient_digit = ((rem3 / div2) < 9) ? (rem3 / div2) : 9;
		
		part_product = divisor * quotient_digit;
		
		//if(reminder < part_product) {
		if(smaller(reminder, part_product, (dividend_len - divisor_len) - k, divisor_len)) {
			quotient_digit = quotient_digit - 1;
			part_product = divisor * quotient_digit;
		}
		
		quotient.buffer[k] = quotient_digit;
		
		//reminder -= part_product;
		int i, borrow = 0, diff;
		for(i=0; i!=divisor_len; i++) {
			diff = reminder.buffer[BUFLEN - i - (dividend_len - divisor_len - k) - 1] - part_product.buffer[BUFLEN - i - 1] - borrow + 10;
			reminder.buffer[BUFLEN - i - (dividend_len - divisor_len - k) - 1] = diff % 10;
			borrow = 1 - (diff / 10);
		}
	}
	
	char temp;
	int i;
	for(i=0; i!=k; i++) {
		quotient.buffer[BUFLEN - k + i] = quotient.buffer[i];
		quotient.buffer[i] = 0;
	}
	
	*this = quotient;
}

void Anumber::operator /= (char op) {
	Anumber returning;
	
	char carry = 0;
	int j=0, i, temp;
	
	for(i=BUFLEN-len(); i!=BUFLEN; i++) {
		temp = carry*10 + buffer[i];
		
		returning.buffer[j] = temp / op;
		carry = temp % op;
		j++;
	}
	
	for(i=BUFLEN-1; i>=0; i--) {
		returning.buffer[i] = returning.buffer[i - (BUFLEN - j)];
		if(i < BUFLEN - j)
			returning.buffer[i] = 0;
	}
	
	*this = returning;
}

Anumber Anumber::operator % (Anumber op) {
	Anumber result;
	
	if(op.is10pow()) {
		int i;
		int lung = BUFLEN - op.Alog10();
		for(i = BUFLEN - 1; i>=lung; i--)
			result.buffer[i] = buffer[i];
		
		return result;
	}
	
	if(len() < op.len()) {
		result = *this;
		return result;
	}
	
	Anumber b = 10;
	Anumber k = op.len();
	//printf("k = "); k.show();
	
	Anumber u = b.pow(k*2) / op;
	//printf("u = "); u.show();
	
	Anumber q1 = *this / (b.pow(k-1));
	//printf("q1 = "); q1.show();
	
	Anumber q2 = q1 * u;
	//printf("q2 = "); q2.show();
	Anumber q3 = q2 / (b.pow(++k)); //now k is k + 1
	//printf("q3 = "); q3.show();
	
	Anumber r1 = *this % b.pow(k); //k è k+1!!!!
	//printf("r1 = "); r1.show();
	Anumber r2 = (q3 * op) % b.pow(k); //k è k+1!!!!
	//printf("r2 = "); r2.show();
	Anumber r = r1 - r2;
	//printf("r = "); r.show();
	
	if(r.sign) r = b.pow(k) - (r2 - r1); //l'ho tolto così ci accorgiamo se fallisce
	
	while((r > op) || (r == op)) r -= op;
	return r;
	
	//using division ---------------------------------------------------------------------------------
	
	/*printf("op = "); op.show();
	
	Anumber a = *this / op;
	printf("div = "); a.show();
	
	Anumber b = a * op;
	printf("b = "); b.show();
	
	Anumber c = *this - b;
	printf("c = "); c.show();*/
	
	//return (*this - ((*this / op) * op));
	//return c;
}

char Anumber::operator % (char op) {
	char carry = 0;
	int j=0, i, temp;
	
	for(i=BUFLEN-len(); i!=BUFLEN; i++) {
		temp = carry*10 + buffer[i];
		carry = temp % op;
		j++;
	}
	
	return carry;
}

Anumber karatsuba(Anumber op1, Anumber op2) {
	bool div1 = false, div2 = false;
	
	if(op1 == 0 || op2 == 0) {
		Anumber zero;
		return zero;
	}
	if(op1.len() < 3 || op2.len() < 3)
		return schoolbook(op1, op2);
	
	int len1 = op1.len();
	/*if(len1 % 2 == 1) {
		len1++;
		div1 = true;
	}*/
	int len2 = op2.len();
	/*if(len2 % 2 == 1) {
		len2++;
		div2 = true;
	}*/
	//char lung = (len1 > len2) ? len1 : len2;
	
	int lung = 2;
	do {
		lung *= 2;
	} while(lung < len1 || lung < len2);
	
	Anumber a;
	Anumber b;
	Anumber c;
	
	Anumber half11, half12;
	Anumber half21, half22;
	op1.split(&half11, &half12, lung);
	op2.split(&half21, &half22, lung);
	
	/*if(div1) half12.exp10(1);
	if(div2) half22.exp10(1);*/
	
	a = half11 * half21;
	b = half12 * half22;
	
	c = (((half11 + half12) * (half21 + half22)) - a) - b;
	
	Anumber returning;
	
	a.exp10(lung);
	c.exp10(lung / 2);
	returning = a + c + b;
	
	/*if(div1) returning.shr(1);
	if(div2) returning.shr(1);*/
	
	return returning;
}

/*Anumber karatsuba(Anumber op1, Anumber op2) {
	bool div1 = false, div2 = false;
	
	if(op1 == 0 || op2 == 0) {
		Anumber zero;
		return zero;
	}
	if(op1.len() < 3 || op2.len() < 3)
		return schoolbook(op1, op2);
	
	int lung = op1.len();
	
	Anumber a;
	Anumber b;
	Anumber c;
	
	Anumber half11, half12;
	Anumber half21, half22;
	op1.split(&half11, &half12);
	op2.split(&half21, &half22);
	
	a = half11 * half21;
	b = half12 * half22;
	
	c = (((half11 + half12) * (half21 + half22)) - a) - b;
	
	Anumber returning;
	
	a.exp10(lung);
	c.exp10(lung / 2);
	returning = a + c + b;
	
	return returning;
}*/

Anumber schoolbook(Anumber op1, Anumber op2) {
	Anumber result;
	int len1 = op1.len();
	int len2 = op2.len();
	int lung = (len1 > len2) ? len1 : len2;
	int i, j;
	char temp;
	
	op1.invert();
	op2.invert();
	
	for(i=0; i!=lung; i++)
		for(j=0; j!=lung; j++) {
			temp = op1.buffer[i] * op2.buffer[j];
			if(temp > 10) {
				result.buffer[i+j] += temp % 10;
				result.buffer[i+j+1] += temp / 10;
			}
			else
				result.buffer[i+j] += temp;
		}
	
	for(i=0; i!=BUFLEN; i++)
		if(result.buffer[i] > 9) {
			result.buffer[i+1] += result.buffer[i] / 10;
			result.buffer[i] = result.buffer[i] % 10;
		}
	
	result.invert();
	
	return result;
}

void Anumber::invert() {
	int i;
	char temp;
	for(i=0; i!=BUFLEN/2; i++) {
		temp = buffer[i];
		buffer[i] = buffer[BUFLEN - 1 - i];
		buffer[BUFLEN - 1 - i] = temp;
	}
}

Anumber Aabs(Anumber op) {
	op.sign = false;
	return op;
}

Anumber MCD(Anumber a, Anumber b) {
	if(b == 0) return a;
	else return MCD(b, a % b);
}

Anumber newPrime(int len) {
	Anumber p;
	
	bool found = false;
	int tentativi = 0;;
	
	srand(time(NULL));
	int i;
	
	while(!found) {
		printf("generating random number\n");
		i=BUFLEN - 1;
		p.buffer[i] = ((rand()%5) * 2) + 1;
		i--;
		for(; i>BUFLEN - len; i--)
			p.buffer[i] = rand()%10;
		p.buffer[i] = (rand()%9) + 1;
		printf("number generated = "); p.show();
		//Anumber n(len);
		printf("doing modular exponentation\n");
		Anumber a(2);
		Anumber resto = a.modExp(p - 1, p);
		printf("done, resto = "); resto.show();
		if(resto == 1) found = true;
		
		tentativi++;
		printf("%d\n", tentativi);
	}
	
	printf("tentativi = %d\n", tentativi);
	
	return p;
}

bool coprime(Anumber a, Anumber b) {
	if(MCD(a, b) == 1) return true;
	return false;
}

//sadgghsdhshtgdsfssagagseghedhgehsehfsdhshsfdhthjs
