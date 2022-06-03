// #define OLC_PGE_APPLICATION

// #include<iostream>
// #include "olcPixelGameEngine.h"

// using namespace std;
// int main(){

// cout<<"hello world.\n";

//     return 0;
// }

#include "olcPixelGameEngine.h"

class DesignTool : public olc::PixelGameEngine{
    public:
        DesignTool(){//constructor
            sAppName = "Polymorphism";
        }

        bool OnUserCreate() override{//overrides mean that the compiler will throw an error if the signature of the function changes from the base class function
            return true;
        }

        bool OnUserUpdate(float fElapsedTime) override{
            olc::vf2d vMouse = { (float)GetMouseX(), (float)GetMouseY()};
            return true;
        }
};



int main(){

    DesignTool demo;
    if(demo.Construct(1600, 960, 1, 1)){//pge function since it is the parent class of DesignTool
        demo.Start();
    }
    return 0;
}