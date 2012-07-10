#define BUFLEN 200

class Anumber {
	public:
		Anumber();
		Anumber(char*);
		Anumber(int);
		int len();
		void split(Anumber*, Anumber*);
		void random();
		void show();
		void show_all();
		void invert();
		void exp10(int);
		int Alog10();
		bool is10pow();
		Anumber invers(Anumber);
		Anumber pow(Anumber);
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
Anumber newPrime();
bool coprime(Anumber, Anumber);
