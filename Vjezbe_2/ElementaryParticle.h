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
    
        ElementaryParticle(string n, float m, bool b);
    };

}