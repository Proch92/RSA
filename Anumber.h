#define BUFLEN 2000

class Anumber {
	public:
		Anumber();
		Anumber(char*);
		Anumber(char*, int);
		Anumber(int);
		int len();
		void split(Anumber*, Anumber*, int);
		void random();
		void random(int);
		void show();
		void show_all();
		void invert();
		void exp10(int);
		int Alog10();
		bool is10pow();
		void shr(int);
		Anumber invers(Anumber);
		Anumber pow(Anumber);
		Anumber modExp(Anumber, Anumber);
		void operator = (Anumber);
		void operator = (char);
		Anumber operator ++ ();
		void operator += (Anumber);
		void operator -= (Anumber);
		Anumber operator + (Anumber);
		Anumber operator - (Anumber);
		Anumber operator - (char);
		void operator *= (Anumber);
		void operator *= (int);
		Anumber operator * (Anumber);
		Anumber operator * (int);
		void operator /= (Anumber);
		void operator /= (char);
		Anumber operator / (Anumber);
		Anumber operator / (char);
		Anumber operator % (Anumber);
		char operator % (char);
		Anumber operator ^ (Anumber);
		void operator ^= (Anumber);
		bool operator == (int);
		bool operator == (Anumber);
		bool operator != (int);
		bool operator > (Anumber);
		bool operator < (Anumber);
		
		char buffer[BUFLEN];
		bool sign; //true is negative;
};

Anumber Aabs(Anumber);
Anumber MCD(Anumber, Anumber);
Anumber newPrime(int);
bool coprime(Anumber, Anumber);
