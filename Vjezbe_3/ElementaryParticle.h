#include<string>
using namespace std;

namespace EP
{
    class ElementaryParticle
    {
    public:
        string name;
        float mass;
        bool boson;
        float energy, px, py, pz;

        void momentum(float px, float py, float pz);
        void transMomentum(float px, float py);
        void bosonDecay(ElementaryParticle* p1, ElementaryParticle* p2);

        ElementaryParticle(string n, float m, bool b);
        ElementaryParticle();
    };

}