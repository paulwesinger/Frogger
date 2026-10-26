#ifndef LEVELCONFIG_H
#define LEVELCONFIG_H

#include <vector>
#include <iostream>
#include <map>
#include <memory>


namespace CONFIG{

/*
 * Layout:
 *
 * enum ENEMYTYPE{
 *  Croco = 0,
 *  Snake ,
 *  Biber
 * }
 *
 *
 *
 * Level  (int), TimePerLevel)  |
 *                              | vector<Row [Count_Objects,[Enemiy[Frames]],Step]>
 *
 *
 * struct Row{
 *   vector<Sprite*> Objects
 *   vector<Sprite*> Enemies
 *   Enemietype tpye --> für die animation.Biber ist im Waser zwichen dem Holz, Snke eigentlich überall, Croco im Wasser..
 *   int Step
 * }
 *
 * struct Level{
 *  int _Level
 *  int _TimePerLevel
 *  vector<Row>
 * }
 *
 * Initialisieren:
 * Row row(vector,vector,enemietype,step) // Gap zwischen den objekten ?
 *
 * Level level(Level1,TIME_LEVEL1, row)
 *
 * Für Class LevelConfig:
 *
 * LevelConfig->AddLevel(Level level,Row row)
 *              AddLevel(LEVEL_1,TIME_LEVEL_n,vector,vector,enemietype,step)
 *
*/

typedef struct sLevelRow {
    int CountObjects;
    int Step;
    int TimePerLevel;
    sLevelRow(){}
    sLevelRow(int _countobjects,int _step, int _timeperlevel) : CountObjects(_countobjects),
            Step(_step), TimePerLevel(_timeperlevel){}
} LevelRow;



typedef struct sLevelConfigurations {
    std::vector<LevelRow> Rows;
    int Level;
}LevelConfigurations;



class LevelConfig
{
public:
    LevelConfig();
    ~LevelConfig();



    void AddLevelConfiguration(LevelRow config, int level);

    // Alle LEvel Config zurück
    std::vector<LevelConfigurations> LevelConfiguration();
    //LevelConfigurations GetLevelConfig(int level);


private:

    std::map<int,std::vector<LevelRow>> Levels;




    std::vector<LevelConfigurations> LevelsConfig;

};
} //CONFIG


#endif // LEVELCONFIG_H
