#include "costumtext.h"
#include <iostream>

COSTUMTEXT::CostumText::CostumText(int resx, int resy,Shader* sh)
    :Sprite(resx,resy,sh)
{
    _Scale = 1.0f;
}

COSTUMTEXT::CostumText::CostumText(int resx, int resy, std::string path, Shader *sh)
    :Sprite(resx, resy,path,sh)
{
    _Scale = 1.0f;
}


COSTUMTEXT::CostumText::~CostumText(){
    int i = 0;
     while( i++ < _Characters.size()) {
        delete _Characters.at(i);
    }
}


