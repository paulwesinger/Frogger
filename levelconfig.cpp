#include "levelconfig.h"

CONFIG::AnyLevel::AnyLevel() {}

CONFIG::AnyLevel::AnyLevel(int level,int timeperlevel,std::vector<Row> rows){

}

CONFIG::AnyLevel::~AnyLevel(){

    try {
        for(Level l:_Levels){
            for(Row row: l.Rows){
                row.Objects.clear();
                row.Enemies.clear();
            }
            l.Rows.clear();
        }
        _Levels.clear();
    }
    catch(const std::exception& ex){
        std::cout << ex.what() << std::endl;
    }
}

bool CONFIG::AnyLevel::ResetLevel(Level &l){
    for(Row row: l.Rows){
        for(ENGINE::Sprite* sp:row.Objects)
        {
            delete sp;
        }
        std::vector<ENGINE::Sprite*>().swap(row.Objects);

        for(ENGINE::Sprite* sp:row.Enemies)
        {
            delete sp;
        }

        std::vector<ENGINE::Sprite*>().swap(row.Enemies);
    }
    //std::vector<CONFIG::Row>().swap(l.Rows);
    l.Rows.clear();
    return true;
}

int CONFIG::AnyLevel::LevelCount(){
    return _Levels.size();
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

// !!!! Achtung Level 1 == Level 0
CONFIG::Level CONFIG::AnyLevel::GetLevelData(int level){
    CONFIG::Level emptylevel;
    if (level < _Levels.size()){
        _IsEmptyLevel = false;
        return _Levels[level];
    }

    _IsEmptyLevel = true;
    return emptylevel;
}

bool CONFIG::AnyLevel::IsEmptyLevel(){
    return _IsEmptyLevel;
}

std::vector<CONFIG::Row>CONFIG::AnyLevel::GetLevelRows(int level){
    std::vector<CONFIG::Row> emptyRow;

    if (level >= _Levels.size()) return emptyRow;
    if (level < 0) return emptyRow;

    return _Levels[level].Rows;
}

