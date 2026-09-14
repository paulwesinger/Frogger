#ifndef COSTUMTEXT_H
#define COSTUMTEXT_H

#include "sprite.h"
#include <map>
#include <string>
#include<vector>

namespace  COSTUMTEXT {

// ---------------------------------------------------------------
// TextBase: Stellt die Basis für die Texturen(Buchstaben)
// bereit.
// _Characters hält die Buchstaben von A - Z, 0 - 9, sonderzeichen
// siehe Beispiel Image vom Frogger. Texturmap mit 19x2 images
// 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I
// J K L M N O P Q R S T U V W X Y Z - copyright

    class TextBase : public ENGINE::Sprite
    {
    public:
        TextBase(int resx, int resy, Shader *sh);
        TextBase(int resx, int resy, std::string path, Shader *sh);
        ~TextBase();
        virtual void RenderText(sPoint pos);
        virtual void RenderText(std::string text,sPoint pos);

        //vorhanden string ändern
        bool UpdateText(std::string text,int index);

        // neuen TextZeile
        void AddText(std::string text);

        void AddCharToMap(char c, int tileposX, int tileposY);
        void AddMapToMap(std::map<char,sPoint> m);
        // Zum kopieren in andere Texts
        std::map<char,sPoint> GetCharacters();


    protected:
        // zb.: char 'A' sPoint(10,0)
        //      char 'J' sPoint(0,1)
        std::map<char,sPoint> _Characters;
        //
        std::vector<std::string> _StringList;




    private:
        float _Scale;
    };

}
#endif // COSTUMTEXT_H
