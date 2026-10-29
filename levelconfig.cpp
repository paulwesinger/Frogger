#include "levelconfig.h"

CONFIG::AnyLevel::AnyLevel() {}

CONFIG::AnyLevel::AnyLevel(int level,int timeperlevel,std::vector<Row> rows){

}

CONFIG::AnyLevel::~AnyLevel(){

}

void CONFIG::AnyLevel::AddLevelConfiguration(Level level){
    _Levels.push_back(level);
}

void CONFIG::AnyLevel::AddLevelConfiguration(int level, int timeperlevel, std::vector<Row> row){
    Level l(level,timeperlevel,row);
    _Levels.push_back(l);
}

std::vector<CONFIG::Level> CONFIG::AnyLevel::GetAllLevels(){
    return _Levels;
}

CONFIG::Level CONFIG::AnyLevel::GetLevelData(int level){
    CONFIG::Level emptylevel;
    if (level >= _Levels.size()) return emptylevel;

    return _Levels[level];
}

std::vector<CONFIG::Row>CONFIG::AnyLevel::GetLevelRows(int level){
    std::vector<CONFIG::Row> emptyRow;

    if (level >= _Levels.size()) return emptyRow;
    if (level < 0) return emptyRow;

    return _Levels[level].Rows;
}

