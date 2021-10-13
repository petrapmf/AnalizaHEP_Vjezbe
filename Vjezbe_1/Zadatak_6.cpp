#include<iostream>
#include<string>

using namespace std;

class ElementaryParticle {
public:
    string name;
    float mass;
    bool boson;

    /*void printInfo() {
        cout << "NAME: " << name << endl;
        cout << "MASS: " << mass << " GeV/c^2" << endl;

        if (boson)
            cout << "THE PARTICLE IS A BOSON\n";
        else
            cout << "THE PARTICLE IS NOT A BOSON\n";
    }*/

    ElementaryParticle(string n, float m, bool b) {
        name = n;
        mass = m;
        boson = b;

        cout << "NAME: " << n << endl;
        cout << "MASS: " << m << " GeV/c^2" << endl;

        if (b)
            cout << "THE PARTICLE IS A BOSON\n";
        else
            cout << "THE PARTICLE IS NOT A BOSON\n";
    }
};

int main() {
  
    ElementaryParticle higgs("Higgs boson", 124.97, true);
    ElementaryParticle top("Top quark", 173.1, false);

    ElementaryParticle Z("Z boson", 91.19, true);
    ElementaryParticle* ptrZ;
    ptrZ = &Z;

    //ElementaryParticle* ptrZ("Z boson", 91.19, true);

  /*ElementaryParticle higgs, top;

    higgs.name = "Higgs boson";
    higgs.mass = 124.97; //u GeV/c2
    higgs.boson = true;

    top.name = "Top quark";
    top.mass = 173.1; // GeV/c^2
    top.boson = false;

    higgs.printInfo();
    top.printInfo();

    ElementaryParticle* ptrZ;

    ptrZ->name = "Z boson";
    ptrZ->mass = 91.19; // GeV/c^2
    ptrZ->boson = true;

    ptrZ->printInfo();
  */

    return 0;
}
