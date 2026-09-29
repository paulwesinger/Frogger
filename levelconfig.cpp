#include "levelconfig.h"

CONFIG::LevelConfig::LevelConfig() {}


CONFIG::LevelConfig::~LevelConfig(){


    for (auto & paar: Levels){
        paar.second.clear();
    }
    Levels.clear();
}

void CONFIG::LevelConfig::AddLevelConfiguration(LevelRow config,int level){

//    Levels.insert_or_assign(level,config);
    if (Levels.contains(level)) {
        Levels[level].push_back(config);
    }
    else{

        std::vector<LevelRow> tmp;
        tmp.push_back(config);

        Levels.insert(std::make_pair(level,tmp) );
    }


}

// ENGINE::CONFIG::LevelConfigurations ENGINE::CONFIG::GetLevelConfig(int level){

//     std::vector<LevelConfigurations>::iterator it = std::vector<LevelConfigurations>.begin();

//     for (std::vector<LevelConfigurations>::iterator it =  std::vector<LevelConfigurations>.begin();
//          it <  std::vector<LevelConfigurations>.begin(); it ++){

//         if (it->Level == level) {
//             return it->Rows;


//         }
//     }
// }

std::vector<CONFIG::LevelConfigurations> CONFIG::LevelConfig::LevelConfiguration(){

        return LevelsConfig;
        // std::vector<ENGINE::CONFIG::LevelConfigurations> ::iterator it =
        //     std::vector<ENGINE::CONFIG::LevelConfigurations>.at(level);
        // return LevelRowsConfig[level];
}
