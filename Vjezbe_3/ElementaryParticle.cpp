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

void ElementaryParticle::momentum(float p_x, float p_y, float p_z, float _m) {
 
    px = p_x;
    py = p_y;
    pz = p_z;

    float p = sqrt(pow(px, 2.) + pow(py, 2.) + pow(pz, 2.));
    mass = _m;
    float En = sqrt(pow(p, 2.) + pow(_m, 2.));

    energy = En;
    //cout << "Energy: " << energy << endl;
}

void ElementaryParticle::transMomentum(float p_x, float p_y) {
    
    px = p_x;
    py = p_y;
    float br = pow(px, 2.) + pow(py, 2.);

    pt = sqrt(br);

    cout << "Transversal Momentum: " << pt << endl;
}

void ElementaryParticle::bosonDecay(ElementaryParticle* p1, ElementaryParticle* p2) {

    if (this->boson == false){
        cout << "THE PARTICLE " << this->name << " IS NOT A BOSON, DECAY IS NOT POSSIBLE!" << endl;
        return;
    }
     
    srand(time(NULL)); //za rand() fju
    random_device rd; // random seed
    mt19937 gen(rd()); // initialize pseudo-random number generator
    uniform_real_distribution<float> dist(0.0, 1.0); // range 
    float higgsDecay = dist(gen);
    
    if (higgsDecay <= 0.214) { // 21.4% slucaja
        //cout << "Higgs boson decayed into W bosons." << endl;
        p1->name = "W";
        p2->name = "W";
        p1->mass = 80.39;
        p2->mass = 80.39;

        p1->px = dist(gen) * px;
        p2->px = px - p1->px;

        p1->py = dist(gen) * py;
        p2->py = py - p1->py;

        p1->pz = dist(gen) * pz;
        p2->pz = pz - p1->pz;

        //cout << "1st W boson ";
        p1->momentum(p1->px, p1->py, p1->pz, p1->mass);
        p1->transMomentum(p1->px, p1->py);
        //cout << "px = " << p1->px << "\tpy = " << p1->py << "\tpz = " << p1->pz << endl;
        //cout << "2nd W boson "; 
        p2->momentum(p2->px, p2->py, p2->pz, p2->mass);
        p2->transMomentum(p2->px, p2->py);
        //cout << "px = " << p2->px << "\tpy = " << p2->py << "\tpz = " << p2->pz << endl;
    }
    else if (0.214 < higgsDecay <= 0.278) { // 6.4% slucaja
        //cout << "Higgs boson decayed into tau leptons." << endl;
        p1->name = "Tau";
        p2->name = "Tau";
        p1->mass = 1.7768;
        p2->mass = 1.7768;

        p1->px = dist(gen) * px;
        p2->px = px - p1->px;

        p1->py = dist(gen) * py;
        p2->py = py - p1->py;

        p1->pz = dist(gen) * pz;
        p2->pz = pz - p1->pz;

        //cout << "1st tau lepton ";
        p1->momentum(p1->px, p1->py, p1->pz, p1->mass);
        p1->transMomentum(p1->px, p1->py);
        //cout << "px = " << p1->px << "\tpy = " << p1->py << "\tpz = " << p1->pz << endl;
        //cout << "2nd tau lepton ";
        p2->momentum(p2->px, p2->py, p2->pz, p2->mass);
        p2->transMomentum(p2->px, p2->py);
        //cout << "px = " << p2->px << "\tpy = " << p2->py << "\tpz = " << p2->pz << endl;
    }
    else if (0.278 < higgsDecay <= 0.304) { // 2.6% slucaja
        //cout << "Higgs boson decayed into Z bosons." << endl;
        p1->name = "Z";
        p2->name = "Z";
        p1->mass = 91.19;
        p2->mass = 91.19;

        p1->px = dist(gen) * px;
        p2->px = px - p1->px;

        p1->py = dist(gen) * py;
        p2->py = py - p1->py;

        p1->pz = dist(gen) * pz;
        p2->pz = pz - p1->pz;

        //cout << "1st Z boson ";
        p1->momentum(p1->px, p1->py, p1->pz, p1->mass);
        p1->transMomentum(p1->px, p1->py);
        //cout << "px = " << p1->px << "\tpy = " << p1->py << "\tpz = " << p1->pz << endl;
        //cout << "2nd Z boson ";
        p2->momentum(p2->px, p2->py, p2->pz, p2->mass);
        p2->transMomentum(p2->px, p2->py);
        //cout << "px = " << p2->px << "\tpy = " << p2->py << "\tpz = " << p2->pz << endl;
    }
    else {
        //cout << "Higgs boson decayed into b quarks." << endl;
        p1->name = "b";
        p2->name = "b";
        p1->mass = 4.19;
        p2->mass = 4.19;

        p1->px = dist(gen) * px;
        p2->px = px - p1->px;

        p1->py = dist(gen) * py;
        p2->py = py - p1->py;

        p1->pz = dist(gen) * pz;
        p2->pz = pz - p1->pz;

        //cout << "1st b quark ";
        p1->momentum(p1->px, p1->py, p1->pz, p1->mass);
        p1->transMomentum(p1->px, p1->py);
        //cout << "px = " << p1->px << "\tpy = " << p1->py << "\tpz = " << p1->pz << endl;
        //cout << "2nd b quark ";
        p2->momentum(p2->px, p2->py, p2->pz, p2->mass);
        p2->transMomentum(p2->px, p2->py);
        //cout << "px = " << p2->px << "\tpy = " << p2->py << "\tpz = " << p2->pz << endl;
    }

}