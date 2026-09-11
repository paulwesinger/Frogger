#ifndef COSTUMTEXT_H
#define COSTUMTEXT_H

#include "sprite.h"
#include <map>
#include <string>
#include<vector>

namespace  COSTUMTEXT {

    class TextBase : public ENGINE::Sprite
    {
    public:
        TextBase(int resx, int resy, Shader *sh);
        TextBase(int resx, int resy, std::string path, Shader *sh);
        ~TextBase();
        void RenderText(std::string text,sPoint pos);
    protected:
        std::map<char,ENGINE::BaseObject2D*> _Characters;

        std::vector<BaseObject2D*> _StringList;

    private:
        float _Scale;
    };

}
#endif // COSTUMTEXT_H
