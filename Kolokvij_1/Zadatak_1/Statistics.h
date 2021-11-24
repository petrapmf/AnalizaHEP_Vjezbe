#include <string>
#include <iostream>

using namespace std;

class Statistics {
public:
	int n;

	void CalculatePi(int n);
	void NormalDistribution(float x, float mi, float sigma);
	Statistics();
};