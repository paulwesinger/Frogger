#include "costumtext.h"
#include <iostream>

COSTUMTEXT::TextBase::TextBase(int resx, int resy,Shader* sh)
    :Sprite(resx,resy,sh)
{
    _Scale = 1.0f;
}

COSTUMTEXT::TextBase::TextBase(int resx, int resy, std::string path, Shader *sh)
    :Sprite(resx, resy,path,sh)
{
    _Scale = 1.0f;
}


COSTUMTEXT::TextBase::~TextBase(){
    int i = 0;
     while( i++ < _Characters.size()) {
        delete _Characters.at(i);
    }
}


