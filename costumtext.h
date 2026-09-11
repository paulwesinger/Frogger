#ifndef COSTUMTEXT_H
#define COSTUMTEXT_H

#include "sprite.h"
#include <map>
#include <string>

namespace  COSTUMTEXT {

    class CostumText : public ENGINE::Sprite
    {
    public:
        CostumText(int resx, int resy, Shader *sh);
        CostumText(int resx, int resy, std::string path, Shader *sh);
        ~CostumText();
        void RenderText(std::string text,sPoint pos);
    protected:
        std::map<char,ENGINE::BaseObject2D*> _Characters;

    private:
        float _Scale;
    };

}
#endif // COSTUMTEXT_H
