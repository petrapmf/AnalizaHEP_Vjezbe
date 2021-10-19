#include<iostream>

#include "ElementaryParticle.h"

using namespace EP;
using namespace std;

ElementaryParticle::ElementaryParticle(string n, float m, bool b) {
    name = n;
    mass = m;
    boson = b;

    cout << "NAME: " << n << endl;
    cout << "MASS: " << m << " GeV/c^2" << endl;

    if (b)
        cout << "THE PARTICLE IS A BOSON!" << endl;
    else
        cout << "THE PARTICLE IS NOT A BOSON!" << endl;
}