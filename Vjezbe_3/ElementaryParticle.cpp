#include<iostream>
#include<cmath>
#include<cstdlib>
#include<ctime>
#include<random>

#include "ElementaryParticle.h"

using namespace EP;
using namespace std;

ElementaryParticle::ElementaryParticle() { //override

    name = "";
    mass = 0;
    boson = false;

    /*cout << "NAME: " << name << endl;
    cout << "MASS: " << mass << " GeV/c^2" << endl;

    if (!boson)
        cout << "THE PARTICLE IS A BOSON!" << endl;
    else
        cout << "THE PARTICLE IS NOT A BOSON!" << endl;*/
}

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

void ElementaryParticle::momentum(float px, float py, float pz) {

    float p = sqrt(pow(px, 2.) + pow(py, 2.) + pow(pz, 2.));

    energy = sqrt(pow(p, 2.) + pow(mass, 2.));

    cout << "Energy: " << energy << endl;
}

void ElementaryParticle::transMomentum(float px, float py) {
    float pt = sqrt(pow(px, 2.) + pow(py, 2.));

    cout << "Transversal Momentum: " << pt << endl;
}

void ElementaryParticle::bosonDecay(ElementaryParticle* p1, ElementaryParticle* p2) {

    if (this->boson == false){
        cout << "THE PARTICLE " << this->name << " IS NOT A BOSON, DECAY IS NOT POSSIBLE!" << endl;
        return;
    }
     
    srand(time(NULL));
    float higgsDecay = (float)rand() / RAND_MAX;
    
    if (higgsDecay <= 0.214) { // 21.4% slucaja
        cout << "Higgs boson decayed into W bosons." << endl;
        // randNum = (min + 1) + (((float) rand()) / (float) RAND_MAX) * (max - (min + 1))
        p1->px = (0.0 + 1.0) + (((float) rand()) / (float) RAND_MAX) * (2.0 - (0.0 + 1.0));
        p2->px = 2.0 - p1->px;

        p1->py = (0.0 + 1.0) + (((float)rand()) / (float)RAND_MAX) * (1.0 - (0.0 + 1.0));
        p2->py = 1.0 - p1->py;

        p1->pz = (0.0 + 1.0) + (((float)rand()) / (float)RAND_MAX) * (2.0 - (0.0 + 1.0));
        p2->pz = 2.0 - p1->pz;

        cout << "1st W boson ";
        p1->momentum(p1->px, p1->py, p2->pz);
        cout << "px = " << p1->px << "\tpy = " << p1->py << "\tpz = " << p1->pz << endl;
        cout << "2nd W boson "; 
        p2->momentum(p2->px, p2->py, p2->pz);
        cout << "px = " << p2->px << "\tpy = " << p2->py << "\tpz = " << p2->pz << endl;
    }
    else if (0.214 < higgsDecay <= 0.278) { // 6.4% slucaja
        cout << "Higgs boson decayed into tau leptons." << endl;
        p1->px = (0.0 + 1.0) + (((float)rand()) / (float)RAND_MAX) * (2.0 - (0.0 + 1));
        p2->px = 2.0 - p1->px;

        p1->py = (0.0 + 1.0) + (((float)rand()) / (float)RAND_MAX) * (1.0 - (0.0 + 1.0));
        p2->py = 1.0 - p1->py;

        p1->pz = (0.0 + 1.0) + (((float)rand()) / (float)RAND_MAX) * (2.0 - (0.0 + 1.0));
        p2->pz = 2.0 - p1->pz;

        cout << "1st tau lepton ";
        p1->momentum(p1->px, p1->py, p2->pz);
        cout << "px = " << p1->px << "\tpy = " << p1->py << "\tpz = " << p1->pz << endl;
        cout << "2nd tau lepton ";
        p2->momentum(p2->px, p2->py, p2->pz);
        cout << "px = " << p2->px << "\tpy = " << p2->py << "\tpz = " << p2->pz << endl;
    }
    else if (0.278 < higgsDecay <= 0.304) { // 2.6% slucaja
        cout << "Higgs boson decayed into Z bosons." << endl;
        p1->px = (0.0 + 1.0) + (((float)rand()) / (float)RAND_MAX) * (2.0 - (0.0 + 1.0));
        p2->px = 2.0 - p1->px;

        p1->py = (0.0 + 1.0) + (((float)rand()) / (float)RAND_MAX) * (1.0 - (0.0 + 1.0));
        p2->py = 1.0 - p1->py;

        p1->pz = (0.0 + 1.0) + (((float)rand()) / (float)RAND_MAX) * (2.0 - (0.0 + 1.0));
        p2->pz = 2.0 - p1->pz;

        cout << "1st Z boson ";
        p1->momentum(p1->px, p1->py, p2->pz);
        cout << "px = " << p1->px << "\tpy = " << p1->py << "\tpz = " << p1->pz << endl;
        cout << "2nd Z boson ";
        p2->momentum(p2->px, p2->py, p2->pz);
        cout << "px = " << p2->px << "\tpy = " << p2->py << "\tpz = " << p2->pz << endl;
    }
    else {
        cout << "Higgs boson decayed into b quarks." << endl;
        p1->px = (0.0 + 1.0) + (((float)rand()) / (float)RAND_MAX) * (2.0 - (0.0 + 1.0));
        p2->px = 2.0 - p1->px;

        p1->py = (0.0 + 1.0) + (((float)rand()) / (float)RAND_MAX) * (1.0 - (0.0 + 1.0));
        p2->py = 1.0 - p1->py;

        p1->pz = (0.0 + 1.0) + (((float)rand()) / (float)RAND_MAX) * (2.0 - (0.0 + 1.0));
        p2->pz = 2.0 - p1->pz;

        cout << "1st b quark ";
        p1->momentum(p1->px, p1->py, p2->pz);
        cout << "px = " << p1->px << "\tpy = " << p1->py << "\tpz = " << p1->pz << endl;
        cout << "2nd b quark ";
        p2->momentum(p2->px, p2->py, p2->pz);
        cout << "px = " << p2->px << "\tpy = " << p2->py << "\tpz = " << p2->pz << endl;
    }

}