#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "time.h"
#include "math.h"
#include "Anumber.h"

Anumber karatsuba(Anumber, Anumber);
Anumber schoolbook(Anumber, Anumber);
void eucEx(Anumber, Anumber, Anumber*, Anumber*);

Anumber::Anumber() {
	sign = false;
	
	memset(buffer, 0, BUFLEN);
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

Anumber Anumber::invers(Anumber m) {
	//euclide esteso
	Anumber x(1);
	Anumber y(0);
	
	eucEx(*this, m, &x, &y);
	
	x.show();
	y.show();
}

void eucEx(Anumber a, Anumber b, Anumber* x, Anumber* y) {
	if(b == 0) {
		*x = 1; *y = 0;
		return;
	}
	else {
		eucEx(b, a%b, x, y);
		Anumber temp = *x;
		*x = *y;
		*y = (temp-a)/(b*(*y));
		return;
	}
}

void Anumber::split(Anumber *half1, Anumber *half2) {
	int i;
	int lung = len();
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
	srand(time(NULL));
	
	sign = rand() % 2;
	
	int i;
	int len = BUFLEN / 10;
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
	
	for(i = 0; i!=BUFLEN && !found; i++)
		if(buffer[i] == 1) found = true;
	
	for(;i!=BUFLEN && found; i++)
		if(buffer[i] != 0) return false;
	
	if(found) return true;
	
	return false;
}

Anumber Anumber::pow(Anumber op) { //does not work
	/*Anumber result;
	
	result = *this;
	
	Anumber i;
	while(!(i == op)) {
		result *= *this;
		++i;
	}
	
	*this = result;*/
	
	printf("doing pow...\n");
	
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
	
	printf("...done\n");
	
	return returning;
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
	result = karatsuba(*this, op);
	
	int i;
	for(i=0; i!=BUFLEN; i++)
		buffer[i] = result.buffer[i];
	sign = (sign ^ op.sign);
}

void Anumber::operator *= (int num) {
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

void Anumber::operator /= (Anumber op) {
	printf("doing division...\n");
	if(op.is10pow()) {
		int i;
		
		Anumber returning = op;
		
		int lung = op.Alog10();
		
		for(i=BUFLEN-1; i>=0; i--) {
			if(i >= lung)
				returning.buffer[i] = returning.buffer[i - lung];
			else returning.buffer[i] = 0;
		}
		
		return;
	}
	
	/*Anumber dividend, quotient;
	dividend = *this;
	
	while(dividend > op) {
		dividend -= op;
		++quotient;
	}
	
	*this = quotient;
	printf("...done\n");*/
	
	Anumber result;
	
	if(len() < op.len()) *this = result;
	
	int dividend_len = len();
	int divisor_len = op.len();
	
	char factor = 10 / (op.buffer[BUFLEN - divisor_len] + 1);
	Anumber reminder = *this * factor;
	Anumber divisor = op * factor;
	
	
	printf("reminder ");
	reminder.show();
	printf("divisor ");
	divisor.show();
	
	dividend_len = reminder.len();
	divisor_len = divisor.len();
	
	Anumber part_product;
	
	Anumber quotient;
	
	int k;
	char quotient_digit;
	for(k = 0; k != dividend_len - divisor_len; k++) {
		printf("reminder "); reminder.show();
		int rem3 = reminder.buffer[k + (BUFLEN - dividend_len)] * 100 + reminder.buffer[k + (BUFLEN - dividend_len) + 1] * 10 + reminder.buffer[k + (BUFLEN - dividend_len) + 2];
		int div2 = divisor.buffer[(BUFLEN - divisor_len)] * 10 + divisor.buffer[(BUFLEN - divisor_len) + 1];
		
		quotient_digit = ((rem3 / div2) < 9) ? (rem3 / div2) : 9;
		
		
		printf("___________\n");
		divisor.show();
		printf("%d\n", quotient_digit);
		printf("___________\n");
		part_product = divisor * quotient_digit;
		printf("reminder "); reminder.show();
		printf("part_product "); part_product.show();
		if(reminder < part_product) {
			printf("<<<<<<<<<<<<<<<<<<\n");
			quotient_digit = quotient_digit - 1;
			part_product = divisor * quotient_digit;
		}
		
		quotient.buffer[k] = quotient_digit;
		
		printf("part_product "); part_product.show();
		reminder -= part_product;
		printf("reminder "); reminder.show();
	}
	
	char temp;
	int i;
	for(i=0; i!=k; i++) {
		quotient.buffer[BUFLEN - k + i] = quotient.buffer[i];
		quotient.buffer[i] = 0;
	}
	printf("quotient = "); quotient.show();
	
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
	printf("doing mod...\n");
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
	printf("trace1\n");
	Anumber b = 10;
	Anumber k = op.len();
	printf("trace1.2\n");
	Anumber u = b.pow(k*2) / op;
	printf("trace2\n");
	Anumber q1 = *this / (b.pow(k-1));
	printf("trace3\n");
	Anumber q2 = q1 * u;
	Anumber q3 = q2 / (b.pow(++k)); //now k is k + 1
	printf("trace4\n");
	Anumber r1 = *this % b.pow(k);
	Anumber r2 = (q3 * op) % b.pow(k);
	Anumber r = r1 - r2;
	printf("trace5\n");
	if(r.sign) r += b.pow(k);
	printf("trace6\n");
	while((r > op) || (r == op)) r -= op;
	printf("...done\n");
	return r;
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
	
	char len1 = op1.len();
	if(len1 % 2 == 1) {
		len1++;
		div1 = true;
	}
	char len2 = op2.len();
	if(len2 % 2 == 1) {
		len2++;
		div2 = true;
	}
	char lung = (len1 > len2) ? len1 : len2;
	
	Anumber a;
	Anumber b;
	Anumber c;
	
	Anumber half11, half12;
	Anumber half21, half22;
	op1.split(&half11, &half12);
	op2.split(&half21, &half22);
	
	if(div1) half12.exp10(1);
	if(div2) half22.exp10(1);
	
	a = half11 * half21;
	b = half12 * half22;
	
	c = (((half11 + half12) * (half21 + half22)) - a) - b;
	
	Anumber returning;
	
	a.exp10(lung);
	c.exp10(lung / 2);
	returning = a + c + b;
	
	if(div1)
		returning /= 10;
	if(div2)
		returning /= 10;
	
	return returning;
}

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

Anumber newPrime() {
	Anumber p;
	
	bool found = false;
	int tentativi = 0;;
	
	srand(time(NULL));
	int i;
	int len = BUFLEN / 10;
	
	while(!found) {
		i=BUFLEN - 1;
		p.buffer[i] = ((rand()%5) * 2) + 1;
		i--;
		for(; i>BUFLEN - len; i--)
			p.buffer[i] = rand()%10;
		p.buffer[i] = (rand()%9) + 1;
		
		Anumber n(len);
		Anumber a(2);
		Anumber resto = a.pow(p - 1) % p;
		
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
