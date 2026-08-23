#ifndef SPRITE_H
#define SPRITE_H

#include <base2dobject.h>


namespace ENGINE{

typedef struct {                                    // sx,sy
    float sx,sy;            // starting point            |-------------- sWidth
    float sWidth,sHeight;   // ende der koordinaten      |
}sTileTextureCoords;                                //   |

typedef struct {
    int x;
    int y;
    int x1;
    int y1;
}Bounds;


class Sprite :public BaseObject2D
{
public:
    Sprite(int resx, int resy, Shader *sh);
    Sprite(int resx, int resy, std::string path, Shader * sh);
    ~Sprite();

    void Render() override;
    void RenderFromAsset(int fromcol, int fromrow);

    void MoveSprite(int pixelsX, int pixelsY, uint64_t timetoanimate, int steps, uint64_t elapsed, int tilex, int tiley);
    void Animate(uint64_t elapsed, int pixelXperSecond, int pixelYperSecond, int fromTileX, int toTileX, int tileY);
    void InitTextureMap(int colums,int rows);
    bool AnimationDone();
    bool EndAnimationDone();
    void StartAnimation(int tileX, int tileY, uint64_t timetoanimatem, int stepsPerMove, int pixelsX, int pixelsY);
    void EndAnimation(int endtileX, int endtileY, uint64_t delay, uint64_t elapsed);

    void SetPosition(int x, int y);
    void SetCountSequences(int count);
    void SetTimeToAnimate(uint64_t t);

    // Bewegungsgrenzen
    void SetMoveArea(int left, int top,int right, int bottom);

    bool IsLocked();
    bool IsRunning();


protected:
    std::vector<sTileTextureCoords> tilecoordinats;
    float _TileWidth;
    float _TileHeight;


private:
    int _TileTextureRows;
    int _TileTextureColumns;

    sPoint _StartPos;
    bool _IsLocked;
    bool _IsRunning;

    Bounds _Bounds;



    uint64_t nextframe;

    int _CountSteps;




    uint64_t _TimePerSequence;
    int _ToPosX;
    int _ToPosY;
    bool _AnimationDone;
    bool _EndAnimationDone;
    uint64_t _TimeToAnimate;
};
}

#endif // SPRITE_H

