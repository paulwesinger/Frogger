#ifndef LEVELCONFIG_H
#define LEVELCONFIG_H

#include <vector>
#include <iostream>
#include <map>
#include <memory>


namespace CONFIG{

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
