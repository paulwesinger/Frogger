
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include "sprite.h"
#include "imageloader.h"


ENGINE::Sprite::Sprite()
    : BaseObject2D()
{
    _IsRunning = false;
    _AnimationDone = true;
    _RenderSprite = true;
    steptime = 0;
}

ENGINE::Sprite::Sprite(int resx, int resy, Shader *sh)
    :BaseObject2D(resx,resy,sh)
{
    _IsRunning = false;
    _AnimationDone = true;
    _RenderSprite = true;
    steptime = 0;
}

ENGINE::Sprite::Sprite(int resx, int resy, std::string path, Shader * sh)
    :BaseObject2D(resx,resy,path,sh)
{
    _IsRunning = false;
    _AnimationDone = true;
    _RenderSprite = true;
    steptime = 0;
}


ENGINE::Sprite::Sprite(ENGINE::Sprite& sprite)
    :BaseObject2D(sprite._ResX,sprite._ResY,sprite._ImagePath,sprite.getShaderPtr())
{


}

ENGINE::Sprite::~Sprite(){
    // for (int i =0; i<_TileTextureColumns; i++)
    //     delete [] tiles[i];

    // delete tiles;

    tilecoordinats.clear();
}

void ENGINE::Sprite::setImagePath(string path){
    _ImagePath = path;
}

void ENGINE::Sprite::InitTextureMap(int colums, int rows){

    _TileTextureColumns = colums;
    _TileTextureRows = rows;

    float ftw = (float)colums;
    float fth = (float) rows;

    std::cout << "tilewidth " << ftw << "image width " << Width() << std::endl;
    std::cout << "tileheight " << fth << "image height " << Width() << std::endl;


    _SpriteSize.w = Width() / colums;
    _SpriteSize.h = Height() / rows;



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

void ENGINE::Sprite::setRenderSprite(bool render){
    _RenderSprite = render;
}

bool ENGINE::Sprite::IsLocked(){
    return _IsLocked;
}

bool ENGINE::Sprite::IsRunning(){
    return _IsRunning;
}

sSize ENGINE::Sprite::SpriteSize(){
    return _SpriteSize;
}

bool ENGINE::Sprite::IsDiver(){
    return _IsDiver;
}

void ENGINE::Sprite::SetDiving(bool diving){
    _IsDiver = diving;
}

void ENGINE::Sprite::SetRenderFromTile(int tile){
    _RenderFromTile = tile;
}
int ENGINE::Sprite::RenderFromTile(){
    return _RenderFromTile;
}

bool ENGINE::Sprite::IsColliding(sPoint p, sSize s){

    int x1 = _Pos.x + SpriteSize().w;
    int y1 = _Pos.y + SpriteSize().h;

    int spritemiddleX = s.w/2 + p.x;
    int spritemiddleY = s.h/2 + p.y;

     bool ret = ((spritemiddleX >= _Pos.x && spritemiddleX  < x1 ) &&
                (spritemiddleY >= _Pos.y && spritemiddleY <= y1));

     return ret;
            //((p.x+s.w >= _Pos.x && p.x + s.w <= x1)  &&  (p.y >= _Pos.y && p.y <= y1));
}


int ENGINE::Sprite::GetCurrentTile(){
    return _NextTile;
}

void ENGINE::Sprite::MoveSprite(int starttile, int lasttile, int tilesizeX, int tilesizeY,
                                uint64_t timeperTile, int stepx, int stepy, uint64_t elapsed, bool mirrorX, bool mirrorY){


    // mirrory derzeit noch nicht fertig
    steptime += elapsed;

    if (_RenderSprite)
        RenderFromAsset(_NextTile,0,mirrorX,mirrorY);

    if (steptime >= timeperTile){

        if ( ! _AnimateReverse) {

            _NextTile ++;

            if (_NextTile > lasttile) { // Restart mit StartAnimation(starttile,lasttile)
                _NextTile = lasttile;
                _AnimationDone = true;
                mirrorX = true;
            }
        }
        else{
            _NextTile --;

            if (_NextTile <= starttile) {  // restart from first image
                _NextTile = lasttile;
                _AnimationDone = true;
                mirrorX = true;
            }
        }
        steptime = 0;
    }
    else
        _AnimationDone = false;


    _Pos.x += stepx;
    _Pos.y += stepy;

    if (_Pos.x > _ResX)
        _Pos.x = -SpriteSize().w;  //tilesizeX;

    if (_Pos.x < -SpriteSize().w) //tilesizeX)
        _Pos.x = _ResX;

    if (_Pos.y > _ResY)
        _Pos.y = -SpriteSize().h; //tilesizeY;

    if (_Pos.y < -SpriteSize().h) //tilesizeY)
        _Pos.y = - SpriteSize().h; //_ResY;
}


void ENGINE::Sprite::MoveSprite(int tilex,int tiley,uint64_t elapsed,int stepx, int stepy){

    static double sx = 0.0;
    static double sy = 0.0;

    steptime += elapsed;

    if (steptime >= _TimeToAnimate){
        _AnimationDone = true;
        setPos(_FromPosX+_ToPosX,_FromPosY+_ToPosY);

        //_Pos.x = _ToPosX;
        //_Pos.y = _ToPosY;
        steptime = 0;
    }
    else {
        _AnimationDone = false;
        sx = round((double)elapsed/(double)_TimeToAnimate * stepx);   //pixelsX / steps;
        sy = round((double)elapsed/(double)_TimeToAnimate * stepy);

        if (sx > 0){
            if (_Pos.x + sx < _Bounds.x1){
                setPos(_Pos.x+sx,_Pos.y);
            }

        }
        else
            if (sx < 0){
                if (_Pos.x + sx - SpriteSize().w > _Bounds.x){
                    _Pos.x += sx;
                }
            }


        if (sy > 0){
            if (_Pos.y + sy < _Bounds.y1){
                setPos(_Pos.x,_Pos.y+sy);
            }

        }
        else
            if (sy < 0){
                if (_Pos.y + sy - SpriteSize().h > _Bounds.y){
                    _Pos.y += sy;
                }
            }

        // if (sy >= 1.0 || sy <= -1.0)
        //     _Pos.y += sy;

        // if (_Pos.x > _ResX)
        //     _Pos.x = -SpriteSize().w;  //tilesizeX;

        // if (_Pos.x < -SpriteSize().w) //tilesizeX)
        //     _Pos.x = _ResX;

        // if (_Pos.y > _ResY)
        //     _Pos.y = -SpriteSize().h; //tilesizeY;

        // if (_Pos.y < -SpriteSize().h) //tilesizeY)
        //     _Pos.y = - SpriteSize().h; //_ResY;
    }
    if (_RenderSprite)
        RenderFromAsset(tilex,tiley);
}



void ENGINE::Sprite::MoveSprite(int pixelsX, int pixelsY, uint64_t timetoanimate, int steps,uint64_t elapsed,
                                int tilex,int tiley){

    static double stepx = 0;
    static double stepy = 0;

    static double pixX = 0;
    static double pixY = 0;

    steptime += elapsed;
    time += elapsed;

    pixX = (double)pixelsX;
    pixY = (double)pixelsY;

    stepx = (double)elapsed/(double)timetoanimate * pixX;   //pixelsX / steps;
    stepy = (double)elapsed/(double)timetoanimate * pixY;    //pixelsY / steps;

    if (pixelsX != 0)
    {
        if (stepx <0.0){
            if (_Pos.x + stepx >= _Bounds.x){
                _Pos.x += round(stepx);
            }
            else
                 _ToPosX = _Pos.x;
        }

        if (stepx > 0.0){
            if (_Pos.x + stepx <= _Bounds.x1){
                _Pos.x += round(stepx);
                // if (steptime >= nextstep) {
                //     _NextTile ++;
                //     steptime = 0;
                // }
            }
            else
            if (_Pos.x >= _ResX){
                _Pos.x = -SpriteSize().w;
                _ToPosX = _Pos.x;
            }
            else{

                _ToPosX = _Pos.x;
            }
        }
    }


    if (pixelsY != 0)
    {
        if (stepy < 0) {    //negativ
            if (_Pos.y + stepy >= _Bounds.y){
                _Pos.y += round(stepy);

                // if (steptime >= nextstep) {
                //     _NextTile++;
                //     steptime = 0;
                // }
            }
            else{   // ToPosY auf die aktuelle Pos einstellen, sonst läuft der Frog weiter.....
                _ToPosY = _Pos.y;
            }
        }

        if (stepy > 0) {    // positiv
            if (_Pos.y + stepy <= _Bounds.y1){
                _Pos.y += round(stepy);

                // std::cout << "_Pos.y : " << _Pos.y  << std::endl;
                // if (steptime >= nextstep) {
                //     _NextTile ++;
                //     steptime = 0;
                // }
            }
            else{   // ToPosY auf die aktuelle Pos einstellen, sonst läuft der Frog weiter.....
                _ToPosY = _Pos.y;
            }
        }
    }

    if (steptime >= _TimePerSequence) {

        std::cout << "Step Time " << steptime << std::endl;
        _NextTile ++;
        steptime = 0;
    }
    // -----------------------------------
    // Save range check
    // Todo: Tile indicator für x und y !
    // -----------------------------------
    if (_NextTile >= _CountSteps)
        _NextTile = _CountSteps -1;

    if (time >= timetoanimate){
        std::cout << "Time " << time << std::endl;
        _AnimationDone = true;
        _IsLocked = true;
        _Pos.x = _ToPosX;
        _Pos.y = _ToPosY;
        steptime = 0;
        time = 0;
        _NextTile = 0;
    }

    if (_RenderSprite)
        RenderFromAsset(_NextTile,tiley);
}
bool ENGINE::Sprite::AnimationStarted(){
    return _AnimationStarted;
}

void ENGINE::Sprite::Animate(int toposx,int toposy,uint64_t timetoanimate,int counttiles,
                             int starttile, int endtile,int yrow,uint64_t elapsed){

    if ( ! _AnimationStarted){
        _AnimationStarted = true;
        deltatime  = 0;
        if (counttiles <= 0)
            counttiles = 1;

        _TimeToAnimate = timetoanimate;
        _TimePerSequence = timetoanimate > 0 ? timetoanimate/ counttiles : timetoanimate;
        _ToPosX = toposx;
        _ToPosY = toposy;
        _StartTileX = starttile;
        _NextTile = starttile;
        _EndTileX = endtile;
        _TileRow = yrow;
        _AnimationDone = false;
        RenderFromAsset(starttile,_TileRow);
    }
    else{

        double stepx = 0;
        double stepy = 0;

        steptime += elapsed;

        // Nächste sequence oder fertig..
        if (steptime >= _TimePerSequence) {

            // wieviel ist ingesamt vergangen?
            deltatime += steptime;

            // Wir sind fertig
            if (deltatime >= _TimeToAnimate){
                RenderFromAsset(_EndTileX,_TileRow);
                _AnimationDone = true;
                _AnimationStarted =false;
                steptime = 0;
                deltatime = 0;
            }
            else{

                _NextTile++;
                if(_NextTile > endtile){
                    _NextTile = endtile;
                }

                double pixX = PosX() - _ToPosX;
                double pixY = PosY() - _ToPosY;

                stepx = round((double)(steptime)/(double)timetoanimate * pixX);   //pixelsX / steps;
                stepy = round((double)(steptime)/(double)timetoanimate * pixY);    //pixelsY / steps;

                sPoint pos = Pos();
                if (PosX() + stepx-SpriteSize().w < _Bounds.x1 && PosX() + stepx > _Bounds.x){
                    setPos(pos.x + stepx,pos.y);
                }

                if (PosY() + stepy-SpriteSize().h < _Bounds.y1 && PosY() + stepy > _Bounds.y){
                    setPos(pos.x ,pos.y+stepy);
                }

                // if (_Pos.x > _ResX)
                //     _Pos.x = -SpriteSize().w;  //tilesizeX;

                // if (_Pos.x < -SpriteSize().w) //tilesizeX)
                //     _Pos.x = _ResX;

                // if (_Pos.y > _ResY)
                //     _Pos.y = -SpriteSize().h; //tilesizeY;

                // if (_Pos.y < -SpriteSize().h) //tilesizeY)
                //     _Pos.y = - SpriteSize().h; //_ResY;



                RenderFromAsset(_NextTile,_TileRow);
            }
        }
    }
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

void ENGINE::Sprite::StartAnimation(int fromPosX, int fromPosY, int toPosX, int toPosY,
                                    int starttilecolumn, int starttilerow,
                                    int endtilex, int endtiley, uint64_t timetoanimate, int counttiles){
    _CountTiles = counttiles;
    _StepX = toPosX-fromPosX;
    _StepY = toPosY-fromPosY;

    _TimePerSequence = timetoanimate / _CountTiles;
    _TimeToAnimate = timetoanimate;
    _AnimationDone = false;
    _ToPosX = toPosX;
    _ToPosY = toPosY;
    _FromPosX = fromPosX;
    _FromPosY = fromPosY;
    deltatime = 0;
    _NextTile = starttilecolumn;

    _StartTileX = starttilecolumn;
    _StartTileY = starttilerow;
    _EndTileX = endtilex;
    _EndTileY = endtiley;


    RenderFromAsset(starttilecolumn,starttilerow);
}

void ENGINE::Sprite::StartAnimation(int firsttile, int lasttile){
    _AnimationDone = false;
    _EndAnimationDone = false;

    // Check for revert animation:
    if (lasttile >= firsttile){
        _NextTile = firsttile;
        _EndTile = lasttile;
        _AnimateReverse = false;
    }
    else{
        _NextTile = lasttile;
        _EndTile = firsttile;
        _AnimateReverse = true;
    }

}

void ENGINE::Sprite::StartAnimation(int tileX, int tileY,uint64_t timetoanimation,int stepsPerMove,int pixelsX, int pixelsY){

    _IsLocked = false;
    _AnimationDone = false;
    _EndAnimationDone = false;
    _IsRunning = true;
    _TimePerSequence =  timetoanimation/stepsPerMove;

    // NextTile bei mehreren animationsteps-> sind die texturen im image
    _NextTile = 0;

    // Wieviele Tiles sind für eine Animation?
    _CountSteps = stepsPerMove;

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

// GLfloat[][4] ENGINE::Sprite::Reverse(GLfloat origin[][4],bool revx, bool revy){

//     // -------------------------------------------------------
//     // spiegelt eine Textur(image) in horizontaler(s) und/oder
//     // vertikaler(T) richtung
//     // origin enthält die original textur und vektor daten
//     // -------------------------------------------------------

//     if (revx) {

//     }

//     return origin;


// }

void ENGINE::Sprite::RenderFromAsset(int fromcol, int fromrow,bool reverseX,bool reverseY)
{
    GLfloat w ;
    GLfloat h ;

    if (_TileTextureColumns > 0){

        w = _Size.w /_TileTextureColumns;
        h = _Size.h / _TileTextureRows;
    }
    else{
        w = _Size.w;
        h = _Size.h;
    }

    GLfloat px = static_cast<GLfloat>(Base::PosX());
    GLfloat py = static_cast<GLfloat>(Base::PosY());


    // Texturekoordinaten berechnen
    // s:
    int columns = _TileTextureColumns;
    int index = fromrow * columns + fromcol;

    sTileTextureCoords tmp;
    if (tilecoordinats.size() > 0 && index < tilecoordinats.size())
        tmp = tilecoordinats.at(index);
    else{

        std::cout << "Sprite::RenderfromAsset -> tilecoordinate map nicht enhält 0 einträge !!" << std::endl;
        return;
    }

    GLfloat vertsTileMap[6][4];

    if (reverseX){
        vertsTileMap[0][0] = px+w;      vertsTileMap[0][1] = py + h;  vertsTileMap[0][2] = tmp.sx;            vertsTileMap[0][3] = tmp.sy + tmp.sHeight;
        vertsTileMap[1][0] = px+w;      vertsTileMap[1][1] = py;      vertsTileMap[1][2] = tmp.sx;            vertsTileMap[1][3] = tmp.sy ;
        vertsTileMap[2][0] = px;        vertsTileMap[2][1] = py;      vertsTileMap[2][2] = tmp.sx+tmp.sWidth; vertsTileMap[2][3] = tmp.sy;

        vertsTileMap[3][0] = px+w;      vertsTileMap[3][1] = py + h;  vertsTileMap[3][2] = tmp.sx;             vertsTileMap[3][3] = tmp.sy + tmp.sHeight;
        vertsTileMap[4][0] = px;        vertsTileMap[4][1] = py;      vertsTileMap[4][2] = tmp.sx+tmp.sWidth;  vertsTileMap[4][3] = tmp.sy;
        vertsTileMap[5][0] = px;        vertsTileMap[5][1] = py + h;  vertsTileMap[5][2] = tmp.sx+tmp.sWidth;  vertsTileMap[5][3] = tmp.sy + tmp.sHeight;

    }
    else {
        vertsTileMap[0][0] = px;      vertsTileMap[0][1] = py + h;  vertsTileMap[0][2] = tmp.sx;            vertsTileMap[0][3] = tmp.sy + tmp.sHeight;
        vertsTileMap[1][0] = px;      vertsTileMap[1][1] = py;      vertsTileMap[1][2] = tmp.sx;            vertsTileMap[1][3] = tmp.sy ;
        vertsTileMap[2][0] = px + w;  vertsTileMap[2][1] = py;      vertsTileMap[2][2] = tmp.sx+tmp.sWidth; vertsTileMap[2][3] = tmp.sy;

        vertsTileMap[3][0] = px;      vertsTileMap[3][1] = py + h;  vertsTileMap[3][2] = tmp.sx;             vertsTileMap[3][3] = tmp.sy + tmp.sHeight;
        vertsTileMap[4][0] = px +w;   vertsTileMap[4][1] = py;      vertsTileMap[4][2] = tmp.sx+tmp.sWidth;  vertsTileMap[4][3] = tmp.sy;
        vertsTileMap[5][0] = px +w;   vertsTileMap[5][1] = py + h;  vertsTileMap[5][2] = tmp.sx+tmp.sWidth;  vertsTileMap[5][3] = tmp.sy + tmp.sHeight;



        //     {
        //     { px,     py  + h,        tmp.sx, tmp.sy + tmp.sHeight}, //0.0, 1.0},//0.0 },
        //     { px,     py,             tmp.sx, tmp.sy},      //0.0, 0.0},//1.0 },
        //     { px + w, py,             tmp.sx + tmp.sWidth, tmp.sy}, //1.0, 0.0},//1.0 },

        //     { px, py + h,             tmp.sx,tmp.sy + tmp.sHeight},  //0.0, 1.0},//0.0 }, // w muss weg für 6  uv = 0,0 !!
        //     { px + w, py,             tmp.sx + tmp.sWidth, tmp.sy},//1.0, 0.0},//1.0 },
        //     { px + w, py + h,         tmp.sx + tmp.sWidth,tmp.sy + tmp.sHeight} //1.0, 1.0} //0.0 }

        // };
    }


   // GLfloat** test = Reverse(vertsTileMap, true,false);

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

