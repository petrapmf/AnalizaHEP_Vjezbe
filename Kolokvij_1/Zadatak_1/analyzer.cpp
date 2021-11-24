#include "Statistics.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <random>

using namespace std;

int main() {
	Statistics* stat = new Statistics();

	stat->CalculatePi(20000);
	stat->NormalDistribution(185.0, 178.2, 6.4);
	stat->NormalDistribution(205.0, 178.2, 6.4);
	stat->NormalDistribution(185.0, 160.2, 7.2);
	return 0;
}