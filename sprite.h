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
    // Copy Constructor
    Sprite(Sprite &sprite);
    // Default constructor
    Sprite();


    ~Sprite();

    void Render() override;
    void RenderFromAsset(int fromcol, int fromrow,bool reverseX=false,bool reverseY=false);



    void MoveSprite(int pixelsX, int pixelsY, uint64_t timetoanimate, int steps, uint64_t elapsed, int tilex, int tiley);
    void MoveSprite(int starttile, int lasttile, int tilesizeX, int tilesizeY, uint64_t timeperTile, int stepx, int stepy,
                    uint64_t elapsed,bool mirrorX=false,bool mirrorY=false);
    void MoveSprite(int tilex, int tiley, uint64_t elapsed, int stepx, int stepy);

    void Animate(int toposx, int toposy, uint64_t timetoanimate,
                 int counttiles, int starttile, int endtile, int yrow, uint64_t elapsed);
    void InitTextureMap(int colums,int rows);
    bool AnimationDone();
    bool EndAnimationDone();
    bool AnimationStarted();
    void StartAnimation(int tileX, int tileY, uint64_t timetoanimatem, int stepsPerMove, int pixelsX, int pixelsY);
    void StartAnimation(int firsttile, int lasttile);
    void StartAnimation(int fromPosX,int fromPosY,int toPosX,int toPosY,int starttilecolumn,int starttilerow,
                        int starttile, int endtile,uint64_t timetoanimate,int counttiles);
    void EndAnimation(int endtileX, int endtileY, uint64_t delay, uint64_t elapsed);

    void SetPosition(int x, int y);
    void SetCountSequences(int count);
    void SetTimeToAnimate(uint64_t t);

    sSize SpriteSize();
    void setRenderSprite(bool render);

    // neu für copyconstructor
    void setImagePath(std::string path);

    // Collission detection
    bool IsColliding(sPoint p, sSize s);
    int GetCurrentTile();

    // Bewegungsgrenzen
    void SetMoveArea(int left, int top,int right, int bottom);
    void SetDiving(bool diving);
    void SetRenderFromTile(int tile);

    bool IsLocked();
    bool IsRunning();
    bool IsDiver();
    int RenderFromTile();



protected:

//    GLfloat[][4] Reverse(GLfloat[][4] origin, bool revx, bool revy);

    std::vector<sTileTextureCoords> tilecoordinats;
    float _TileWidth;
    float _TileHeight;
    sSize _SpriteSize;


    int _TileTextureRows;
    int _TileTextureColumns;

    sPoint _StartPos;
    bool _IsLocked;
    bool _IsRunning;
    bool _RenderSprite;
    bool _IsDiver = false;  // Nur für die Turtles derzeit eventuel neue abgeleitete klasse erstellen !

    Bounds _Bounds;
    int _NextTile = 0;

    int _CountSteps = 1;
    int _CountTiles = 1;
    int _StepX = 0;
    int _StepY = 0;


    uint64_t _TimePerSequence;
    int _ToPosX=0;
    int _ToPosY=0;
    int _RenderFromTile = 0;
    bool _AnimationDone;
    bool _EndAnimationDone;
    int _FromPosX = 0;
    int _FromPosY = 0;

    int _StartTileX = 0;
    int _StartTileY = 0;
    int _EndTileX = 0;
    int _EndTileY = 0;
    int _EndTile = 0;
    int _TileRow = 0;

    uint64_t _TimeToAnimate;

private:
    uint64_t steptime = 0;
    uint64_t time = 0;
    uint64_t deltatime = 0;

    bool _AnimateReverse = false;
    bool _AnimationStarted = false;

    // Die grösse des sprites in tilesize

};
}

#endif // SPRITE_H

