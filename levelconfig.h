#ifndef LEVELCONFIG_H
#define LEVELCONFIG_H

#include <vector>
#include <iostream>
#include <map>
#include <memory>
#include <sprite.h>


namespace CONFIG{

/*
 * Layout:
 */
 enum ENEMYTYPE{
    Croco = 0,
    Snake ,
    Biber
};
 /*
 *
 *
 * Level  (int), TimePerLevel)  |
 *                              | vector<Row [Count_Objects,[Enemiy[Frames]],Step]>  --> vector[0] == Row 1, vector[1] == Row 8, usw...
 *
 *-------------------------------------------------------------------------------------------------------------------------------------
 * typedef struct Row{
 *   vector<Sprite*> Objects
 *   vector<Sprite*> Enemies
 *   Enemietype tpye --> für die animation.Biber ist im Waser zwichen dem Holz, Snake eigentlich überall, Croco im Wasser..
 *   int Step  // Geschwindigkeit der objeckte.
 * }
 *
 * typedef struct Level{
 *  int _Level
 *  int _TimePerLevel
 *  vector<Row>
 * }
 *-------------------------------------------------------------------------------------------------------------------------------------
 *
 * Initialisieren:
 * Row row(vector,vector,enemietype,step) // Gap zwischen den objekten ?
 *          davon brauchen wir beim Frogger 9 pro Level!
 *
 * Level level(Level1,TIME_LEVEL1, row)
 *
 * Für Class LevelConfig:
 *
 * LevelConfig->AddLevel(Level level,Row row);
 *              AddLevel(LEVEL_1,TIME_LEVEL_n,vector,vector,enemietype,step);
 *
*/

typedef struct sRow {
    std::vector<ENGINE::Sprite*> Objects;
    std::vector<ENGINE::Sprite*> Enemies;

    ENEMYTYPE Enemytype;
    int Step;

    sRow(){}
    sRow(std::vector<ENGINE::Sprite*> objects,std::vector<ENGINE::Sprite*> enemies, ENEMYTYPE enemytype,int _step) : Objects(objects),
            Enemies(enemies),Step(_step){
        Enemytype = enemytype;
    }
} Row;

typedef struct sLevel {
    int Level;
    int TimePerLevel;
    std::vector<Row> Rows;

    sLevel(){}
    sLevel(int level, int timeperlevel,std::vector<Row> rows):
        Level(level),TimePerLevel(timeperlevel),Rows(rows){}
}Level;

class AnyLevel
{
public:
    AnyLevel();
    AnyLevel(int level,int timeperlevel,std::vector<Row> rows);
    ~AnyLevel();
    void AddLevelConfiguration(Level level);
    void AddLevelConfiguration(int level,int timeperlevel,std::vector<Row> row);
    std::vector<Row> GetLevelRows(int level);

    std::vector<Level> GetAllLevels();
    Level GetLevelData(int level);

private:
    std::vector<Level> _Levels;


};
} //CONFIG


#endif // LEVELCONFIG_H
