#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "time.h"
#include "math.h"
#include "Anumber.h"

Anumber karatsuba(Anumber, Anumber);
Anumber schoolbook(Anumber, Anumber);

Anumber::Anumber() {
	sign = false;
	
	memset(buffer, 0, BUFLEN);
}

Anumber::Anumber(char* init) {
	sign = (init < 0);
	
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

int Anumber::len() {
	int i=0;
	bool found = false;
	
	while(!found && i != BUFLEN) {
		if(buffer[i] != 0) found = true;
		else i++;
	}
	
	return BUFLEN - i;
}

void Anumber::split(Anumber *half1, Anumber *half2) {
	int i;
	int lung = len();
	int med = lung / 2;
	
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
	for(i=BUFLEN - 1; i!=BUFLEN - len; i--)
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

void Anumber::pow(Anumber op) {
	Anumber base = *this;
	Anumber exp = op;
	
	int oplen = op.len();
	char* exp2 = (char*) malloc(oplen * 5);
	
	int i=0;
	while(!(exp == 0)) {
		exp2[i] = exp % 2;
		printf("%d",exp2[i]);
		exp /= 2;
		i++;
	}
	printf("\n");
	int len2 = i;
	
	Anumber pow;
	pow = 1;
	
	for(i=len2 - 1; i>=0; i--) {
		printf("%d ", exp2[i]);
		pow *= pow;
		if(exp2[i])
			pow *= base;
		pow.show();
	}
	
	*this = pow;
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

bool Anumber::operator == (int num) {
	if(num == 0)
		return !(len());
	
	return false; //it counts only 0
}

bool Anumber::operator == (Anumber op) {
	//to do
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
	Anumber result;
	
	if(len() < op.len()) *this = result;
	
	int dividend_len = len();
	int divisor_len = op.len();
	
	char factor = 10 / (op.buffer[BUFLEN - divisor_len] + 1);
	Anumber reminder = *this * factor;
	Anumber divisor = op * factor;
	
	Anumber part_product;
	
	Anumber quotient;
	
	int k;
	char quotient_digit;
	for(k = dividend_len - divisor_len; k >= 0; k--) {
		{
			printf("££££ %d, %d, %d, %d\n", k, k + (BUFLEN - divisor_len), BUFLEN - divisor_len, reminder.buffer[(BUFLEN - divisor_len)]);
			printf("%%%% %d, %d, %d\n", reminder.buffer[k + (BUFLEN - divisor_len)], reminder.buffer[k + (BUFLEN - divisor_len) + 1], reminder.buffer[k + (BUFLEN - divisor_len) + 2]);
			printf("@@@@ %d, %d, %d\n", reminder.buffer[k + (BUFLEN - divisor_len)] * 100, reminder.buffer[k + (BUFLEN - divisor_len) + 1] * 10, reminder.buffer[k + (BUFLEN - divisor_len) + 2]);
			int rem3 = reminder.buffer[k + (BUFLEN - divisor_len)] * 100 + reminder.buffer[k + (BUFLEN - divisor_len) + 1] * 10 + reminder.buffer[k + (BUFLEN - divisor_len) + 2];
			int div2 = divisor.buffer[(BUFLEN - divisor_len)] * 10 + divisor.buffer[(BUFLEN - divisor_len) + 1];
			printf("%d, %d, %d\n", rem3, div2, rem3/div2);
			quotient_digit = ((rem3 / div2) < 9) ? (rem3 / div2) : 9;
		}
		part_product = divisor * quotient_digit; //dq = Product(d, qt, m);
		
		if(reminder < part_product) {
			quotient_digit = quotient_digit - 1;
			part_product = divisor * quotient_digit;
		}
		quotient.buffer[k] = quotient_digit;
		reminder -= part_product; //r = Difference(r, dq, k, m);
	}
	
	char temp;
	for(k=0; k!=(dividend_len - divisor_len) + 1; k++) {
		quotient.buffer[k + BUFLEN - ((dividend_len - divisor_len) + 1)] = quotient.buffer[k];
		quotient.buffer[k] = 0;
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
	
	if(len() < op.len()) {
		result = *this;
		return result;
	}
	
	
	
	return result;
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
	if(op1 == 0 || op2 == 0) {
		Anumber zero;
		return zero;
	}
	if(op1.len() < 3 || op2.len() < 3)
		return schoolbook(op1, op2);
	
	char len1 = op1.len();
	char len2 = op2.len();
	char lung = (len1 > len2) ? len1 : len2;
	
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

Anumber Aabs(Anumber op) {
	op.sign = false;
	return op;
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
