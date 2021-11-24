#include<iostream>
#include<cmath>
#include<cstdlib>
#include<ctime>
#include<random>

#include "Statistics.h"

using namespace std;

Statistics::Statistics(){

	cout << "Objekt je uspijesno kreiran." << endl;

	}
void Statistics::CalculatePi(int n) {
	float randx;
	float randy;
	float origin, pi;
	int circle = 0;
	int square = 0;
	int i;

	srand(time(NULL));

	for (i = 0; i < n * n; i++) {
		randx = float(rand() % (n + 1)) / n;
		randy = float(rand() % (n + 1)) / n;

		origin = randx * randy + randy * randy;

		if (origin <= 1)
			circle++;
		square++;

		pi = float(4 * circle) / square;

		//cout << randx << " " << randy << " " << circle << " " << square << " " << " - "<< pi << endl;
	}

	cout << "\nPi = " << pi << endl;
}

void Statistics::NormalDistribution(float x, float m, float s) {
	float P;
	float minus;
	minus = (x - m);
	P = (1.0 / s * sqrt(2.0 * 3.14)) * exp(-1.0 *(minus*minus)/(2 * s*s));
	cout << "Normal Distribution: " << P << endl;
}