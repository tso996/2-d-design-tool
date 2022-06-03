// #define OLC_PGE_APPLICATION

// #include<iostream>
// #include "olcPixelGameEngine.h"

// using namespace std;
// int main(){

// cout<<"hello world.\n";

//     return 0;
// }

#include "olcPixelGameEngine.h"

class Polymorphism : public olc::PixelGameEngine{
    public:
        Polymorphism(){
            sAppName = "Polymorphism";
        }
};



int main(){
    return 0;
}