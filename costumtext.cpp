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
    // int i = 0;
    // while( i++ < _Characters.size()) {
    //     delete _Characters.at(i);
    //}

    _Characters.clear();
}

bool COSTUMTEXT::TextBase::UpdateText(std::string text, int index){
    if (index >= _StringList.size() || index < 0  || _StringList.empty() ) return false;

    _StringList[index] = text;
    return true;
}

void COSTUMTEXT::TextBase::AddText(std::string text){

    // Todo -> insert
    _StringList.push_back(text);
}

void COSTUMTEXT::TextBase::AddCharToMap(char c, int tileposX, int tileposY){

    std::map<char,sPoint>::iterator it = _Characters.end();

    _Characters.insert(it,std::pair<char,sPoint>(c,sPoint(tileposX,tileposY)));

}

void COSTUMTEXT::TextBase::AddMapToMap(std::map<char, sPoint> m){
    _Characters.insert(m.begin(),m.end());
}

std::map<char,sPoint> COSTUMTEXT::TextBase::GetCharacters(){
    return _Characters;
}

void COSTUMTEXT::TextBase::RenderText(sPoint pos){

    if (_StringList.empty() ) return;

    setPos(pos.x,pos.y);
    int x = pos.x;
    int y = pos.y;

    for(const std::string &st: _StringList) {
        for(char c: st){

            sPoint p = _Characters[c];
            setPos(x,y);
            RenderFromAsset(p.x,p.y);
            x += SpriteSize().w;

        }
        x = pos.x;
        y+= SpriteSize().h;
    }
}




