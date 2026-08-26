
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include "sprite.h"
#include "imageloader.h"


ENGINE::Sprite::Sprite(int resx, int resy, Shader *sh)
    :BaseObject2D(resx,resy,sh)
{
    _IsRunning = false;
    _AnimationDone = true;
}

ENGINE::Sprite::Sprite(int resx, int resy, std::string path, Shader * sh)
    :BaseObject2D(resx,resy,path,sh)
{
    _IsRunning = false;
    _AnimationDone = true;
}

ENGINE::Sprite::~Sprite(){
    // for (int i =0; i<_TileTextureColumns; i++)
    //     delete [] tiles[i];

    // delete tiles;

    tilecoordinats.clear();
}

void ENGINE::Sprite::InitTextureMap(int colums, int rows){

    _TileTextureColumns = colums;
    _TileTextureRows = rows;

    float ftw = (float)colums;
    float fth = (float) rows;

    std::cout << "tilewidth " << ftw << "image width " << Width() << std::endl;
    std::cout << "tileheight " << fth << "image height " << Width() << std::endl;

    _TileWidth = 1.0f / ftw;
    _TileHeight =  1.0f / fth;

    //     ftexX             ftexX
    float ftexX = 1.0f / ftw;//         |-----------------|-----------------|--
    float ftexY = 1.0f / fth;//         |                 |                 |
    //      |ftexY            |                 |
    //      |                 |                 |
    //      |-----------------|-----------------|--
    //      |                 |                 |
    /*
Ursprung: Liegt unten links bei (0.0, 0.0).
Ecken:Unten links: (0.0, 0.0)
Unten rechts: (1.0, 0.0)
Oben rechts: (1.0, 1.0)
Oben links: (0.0, 1.0)

Achsen: S steht für die X-Achse (waagerecht) und T für die Y-Achse (senkrecht).
*/


    std::cout << "textur - S " << ftexX << std::endl;
    std::cout << "Textur - T" << ftexY << std::endl;

    float fx = 0.0f;
    float fy = 0.0f;

    //    Tilemap.InitMatrix(rows,colums);

    for(int j = 0;j<rows;j++){
        for(int i=0;i<colums;i++){
            sTileTextureCoords tmp;
            tmp.sx = fx;
            tmp.sy = fy;

            tmp.sWidth = ftexX;
            tmp.sHeight = ftexY;
            tilecoordinats.push_back(tmp);
            fx += ftexX;
        }
        fx = 0.0f;
        fy += ftexY;
    }
}

void ENGINE::Sprite::SetTimeToAnimate(uint64_t t){
    _TimeToAnimate = t;
}

bool ENGINE::Sprite::IsLocked(){
    return _IsLocked;
}

bool ENGINE::Sprite::IsRunning(){
    return _IsRunning;
}

void ENGINE::Sprite::MoveSprite(int pixelsX, int pixelsY, uint64_t timetoanimate, int steps,uint64_t elapsed,
                                int tilex,int tiley){

    static uint64_t steptime = 0;
    static uint64_t nextstep =0;
    static uint64_t time = 0;

    static double stepx = 0;
    static double stepy = 0;

    static double pixX = 0;
    static double pixY = 0;

    steptime += elapsed;
    time += elapsed;


   // if (! _IsLocked) {

        pixX = (double)pixelsX;
        pixY = (double)pixelsY;

        stepx = (double)elapsed/(double)timetoanimate * pixX;   //pixelsX / steps;
        stepy = (double)elapsed/(double)timetoanimate * pixY;    //pixelsY / steps;

        if (pixelsX != 0)
        {
            if (stepx >=1.0 || stepx <= -1.0){
                _Pos.x += round(stepx);
                if (steptime >= nextstep) {

                    std::cout << "Steptime : " << steptime  << std::endl;
                    steptime = 0;
                }
            }
        }

        if (pixelsY != 0)
        {            
            if (stepy < 0) {    //negativ
                if (_Pos.y + stepy >= _Bounds.y){
                    _Pos.y += round(stepy);

                    if (steptime >= nextstep) {

                        std::cout << "Steptime : " << steptime  << std::endl;

                        steptime = 0;
                    }
                }
                else{   // ToPosY auf die aktuelle Pos einstellen, sonst läuft der Frog weiter.....
                    _ToPosY = _Pos.y;
                }
            }

            if (stepy > 0) {    // positiv
                if (_Pos.y + stepy <= _Bounds.y1){
                    _Pos.y += round(stepy);

                    std::cout << "_Pos.y : " << _Pos.y  << std::endl;
                    if (steptime >= nextstep) {

                        std::cout << "Steptime : " << steptime  << std::endl;

                        steptime = 0;
                    }
                }
                else{   // ToPosY auf die aktuelle Pos einstellen, sonst läuft der Frog weiter.....
                    _ToPosY = _Pos.y;
                }
            }


            // if (stepy >= 1.0 || stepy <= -1.0) {
            //     // check up

            //         // stepy kann auch negativ sein...
            //         if ( (_Pos.y + stepy <= _Bounds.y1) ||  // stepy positiv
            //              (_Pos.y + stepy >= _Bounds.y)){    // stepy negativ

            //             _Pos.y += (int)stepy;

            //             std::cout << "_Pos.y : " << _Pos.y  << std::endl;
            //             if (steptime >= nextstep) {

            //                 std::cout << "Steptime : " << steptime  << std::endl;

            //                 steptime = 0;
            //             }
            //         }
            // }
        }
    //}
    if (time >= timetoanimate){
        _AnimationDone = true;
        _IsLocked = true;
        _Pos.x = _ToPosX;
        _Pos.y = _ToPosY;
        steptime = 0;
        std::cout << "time : " << time << std::endl;
        time = 0;
        nextstep = 0;
    }
    RenderFromAsset(tilex,tiley);
}

void ENGINE::Sprite::Animate(uint64_t elapsed, int pixelXperSecond, int pixelYperSecond,
                             int fromTileX, int toTileX, int tileY){

    // zum aufaddieren mit elapsed bis 1000
    // static double stepx = 0;
    // static double stepy = 0;
    // static uint64_t second = 0;
    // static uint64_t steptime = 0;
    // static int countframes = 0;



    // //speedXpersecond und speedYpersecond sind die pixel pro sekunde, tileX und Y geben das tile im image an
    // second += elapsed;
    // steptime += elapsed;
    // countframes ++;

    // //  if (second >= _TimePerSequence) {

    // //      nextframe++;

    // //      if (nextframe > toTileX )
    // //          nextframe = toTileX;
    // //     // second = 0;
    // // }

    // // double toX = (double)(_ToPosX);
    // // double toY = (double)(_ToPosY);

    // double pixelxpersec = (double)(pixelXperSecond);
    // double pixelypersec = (double)(pixelYperSecond);

    // // double sx = static_cast<double>(_StartPos.x);
    // // double sy = static_cast<double>(_StartPos.y);
    // double timetoanaim = (double)(_TimeToAnimate);



    // // erstmal den faktor für die pixels per frame berechnen..
    // double pixelperframex;
    // double pixelperframey;

    // if (pixelXperSecond != 0 ){
    //     pixelperframex = pixelxpersec / timetoanaim;
    //     stepx += pixelperframex * elapsed;
    // }
    // //else
    // //    stepx =0;

    // if (pixelYperSecond != 0){
    //     pixelperframey = pixelypersec / timetoanaim;
    //     stepy += pixelperframey * elapsed;
    // }





    // // if (pixelXperSecond > 0 )
    // // //    stepx = (toX-sx) / timetoanaim * elapsed;  // wird bei 64 pixel breite 64 sein..... muss aber nicht
    // //     stepx += pixelxpersec  / timetoanaim * elapsed;  // wird bei 64 pixel breite 64 sein..... muss aber nicht
    // // else
    // //     stepx = 0;

    // // if (pixelYperSecond> 0)
    // //     //stepy = (toY-sy) / timetoanaim * elapsed;
    // //     stepy += pixelypersec / timetoanaim * elapsed;
    // // else
    // //     stepy = 0;

    // if (stepx >= 1.0 || stepx <= -1.0)
    //     _Pos.x += (int)stepx;

    // //if (_Pos.x == (_Pos.x + _ToPosX) )
    // //    _Pos.x = _ToPosX;

    // if (stepy >= 1.0 || stepy <= -1.0)
    //     _Pos.y += (int)stepy;

    // std::cout << "Elapsed : " << elapsed << std::endl;
    // std::cout << "Pos x : " << _Pos.x << std::endl;
    // std::cout << "Pos y : "  <<_Pos.y << std::endl << std::endl;




    // RenderFromAsset(toTileX, tileY);

    // //if (abs(stepx) >= pixelXperSecond && abs(stepy) >= pixelYperSecond){

    // std::cout << "second: " << second << std::endl;

    // if (second >= _TimeToAnimate && ! _AnimationDone) {
    //     //_Pos.x = _ToPosX; //fromstartX + toDestX;//stepx;
    //     std::cout << "second: " << second << std::endl;

    //     std::cout << "Count frames : " << countframes << std::endl;

    //     //_Pos.y = _ToPosY;//fromStartY + toDestY;// stepy;
    //     _AnimationDone = true;
    //     _IsLocked = true;

    //     second = 0;   // zum aufaddieren mit elapsed bis 1000
    //     stepx = 0;
    //     stepy = 0;
    //     countframes = 0;
    //     nextframe = toTileX;
    // }
}

void ENGINE::Sprite::SetCountSequences(int count){
    _CountSteps = count;
}

void ENGINE::Sprite::EndAnimation(int endtileX, int endtileY, uint64_t delay,uint64_t elapsed){

    static uint64_t time = 0;

    time += elapsed;

    if (time <= delay)
    {
        setPos(_ToPosX,_ToPosY);
        _IsLocked = false;
        _IsRunning = false;
        _EndAnimationDone = false;
        RenderFromAsset(endtileX,endtileY);
        time += elapsed;

        cout << "FrogPos X : "<< _Pos.x << "   FrogPos Y : "<< _Pos.y << std::endl;

    }
    else{
        time = 0;
        _EndAnimationDone = true;
    }

}

bool ENGINE::Sprite::AnimationDone(){ return _AnimationDone; }

bool ENGINE::Sprite::EndAnimationDone(){
    return _EndAnimationDone;
}

void ENGINE::Sprite::StartAnimation(int tileX, int tileY,uint64_t timetoanimation,int stepsPerMove,int pixelsX, int pixelsY){

    _IsLocked = false;
    _AnimationDone = false;
    _IsRunning = true;
    _TimePerSequence =  timetoanimation/stepsPerMove;
    // _CountSteps = stepsPerMove;


    if (pixelsX < 0){
        if (_Pos.x + pixelsX < _Bounds.x){
            _AnimationDone = true;
            _ToPosX = _Pos.x;
        }
        else
            _ToPosX = _Pos.x + pixelsX;
    }
    else
    if (pixelsX > 0){
        if (_Pos.x + pixelsX > _Bounds.x1){
            _AnimationDone = true;
            _ToPosX = _Pos.x;
        }
        else
            _ToPosX = _Pos.x + pixelsX;
    }


    if (pixelsY < 0){
        if (_Pos.y + pixelsY < _Bounds.y){
            _AnimationDone = true;
            _ToPosY = _Pos.y;
        }
        else{           
            _ToPosY = (_Pos.y + pixelsY);
        }
    }
    else
    if (pixelsY > 0){
        if (_Pos.y + pixelsY + 64 > _Bounds.y1){
            cout << "Posy +stepy + size.h " << _Pos.y + pixelsY + 64
                 <<  "  " << _Pos.y << "  " <<  pixelsY << "  " << "64" << std::endl;
            _AnimationDone = true;
            _ToPosY = _Pos.y;
        }
        else{
            _ToPosY = (_Pos.y + pixelsY);
        }
    }

    RenderFromAsset(tileX,tileY);
}

void ENGINE::Sprite::SetPosition(int x, int y){
    setPos(x,y);

    _ToPosX = x;
    _ToPosY = y;
}

void ENGINE::Sprite::SetMoveArea(int left, int top, int right, int bottom){
    _Bounds.x = left;
    _Bounds.y = top;
    _Bounds.x1 = right;
    _Bounds.y1 = bottom;
}



void ENGINE::Sprite::RenderFromAsset(int fromcol, int fromrow)
{
    GLfloat w = _Size.w /_TileTextureColumns;
    GLfloat h = _Size.h / _TileTextureRows;

    GLfloat px = static_cast<GLfloat>(Base::PosX());
    GLfloat py = static_cast<GLfloat>(Base::PosY());


    // Texturekoordinaten berechnen
    // s:
    int columns = _TileTextureColumns;
    int index = fromrow * columns + fromcol;

    sTileTextureCoords tmp;
    tmp = tilecoordinats.at(index);


    GLfloat vertsTileMap[6][4] = {
        { px,     py  + h,        tmp.sx, tmp.sy + tmp.sHeight}, //0.0, 1.0},//0.0 },
        { px,     py,             tmp.sx, tmp.sy},      //0.0, 0.0},//1.0 },
        { px + w, py,             tmp.sx + tmp.sWidth, tmp.sy}, //1.0, 0.0},//1.0 },

        { px, py + h,             tmp.sx,tmp.sy + tmp.sHeight},  //0.0, 1.0},//0.0 }, // w muss weg für 6  uv = 0,0 !!
        { px + w, py,             tmp.sx + tmp.sWidth, tmp.sy},//1.0, 0.0},//1.0 },
        { px + w, py + h,         tmp.sx + tmp.sWidth,tmp.sy + tmp.sHeight} //1.0, 1.0} //0.0 }

    };

    glUseProgram(_CurrentShader);

    glActiveTexture(GL_TEXTURE0);
    _Projection =  glm::ortho(0.0f,static_cast<float>(_ResX),static_cast<float>(_ResY), 0.0f,  -1.0f, 1.0f);

    _Uniform_mv_projectloc = glGetUniformLocation(_CurrentShader,"projection");

    _Uniform_colorloc   = glGetUniformLocation(_CurrentShader,"col2D");

    glUniform4f(_Uniform_colorloc, _Color.r, _Color.g, _Color.b, _Color.a);




    glm::mat4 Model(1.0f);
    glm::mat4 mvp = _Projection * Model ;
    glUniformMatrix4fv(_Uniform_mv_projectloc, 1, GL_FALSE, glm::value_ptr(mvp)); //projection));


    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER,_VBO);
    glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(vertsTileMap),vertsTileMap);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    // Texture
    glBindTexture(GL_TEXTURE_2D,_Texture);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_EBO);
    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);


    // Aufräumen
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D,0);
}

void ENGINE::Sprite::Render(){
    if (_CurrentShader) {
        glUseProgram(_CurrentShader);

        glActiveTexture(GL_TEXTURE0);
        _Projection =  glm::ortho(0.0f,static_cast<float>(_ResX),static_cast<float>(_ResY), 0.0f,  -1.0f, 1.0f);

        _Uniform_mv_projectloc = glGetUniformLocation(_CurrentShader,"projection");

        _Uniform_colorloc   = glGetUniformLocation(_CurrentShader,"col2D");

        glUniform4f(_Uniform_colorloc, _Color.r, _Color.g, _Color.b, _Color.a);

        GLfloat w = _Size.w;
        GLfloat h = _Size.h;

        GLfloat px = static_cast<GLfloat>(PosX());
        GLfloat py = static_cast<GLfloat>(PosY());


        GLfloat vertices[6][4] = {
            { px,     py  + h,        0.0, 1.0},//0.0 },
            { px,     py,             0.0, 0.0},//1.0 },
            { px + w, py,             1.0, 0.0},//1.0 },

            { px, py + h,             0.0, 1.0},//0.0 }, // w muss weg für 6  uv = 0,0 !!
            { px + w, py,             1.0, 0.0},//1.0 },
            { px + w, py + h,         1.0, 1.0} //0.0 }
        };

        glm::mat4 Model(1.0f);
        glm::mat4 mvp = _Projection * Model ;
        glUniformMatrix4fv(_Uniform_mv_projectloc, 1, GL_FALSE, glm::value_ptr(mvp)); //projection));


        glBindVertexArray(_VAO);
        glBindBuffer(GL_ARRAY_BUFFER,_VBO);
        glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(vertices),vertices);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        // Texture
        glBindTexture(GL_TEXTURE_2D,_Texture);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_EBO);
        glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);


        // Aufräumen
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D,0);
    }
}

