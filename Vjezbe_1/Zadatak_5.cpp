#include<iostream>

using namespace std;

int _my_cube(int br){
    int kub = br*br*br;
    return kub;
}

int main(){
    int br, cube;
    
    cout << "Upisi broj \n";
    cin >> br;
    
    cube = _my_cube(br);

    cout << "Kub broja je" << cube;

    return 0;
}
