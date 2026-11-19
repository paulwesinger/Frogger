#ifndef LEVELSERIALIZE_H
#define LEVELSERIALIZE_H

#include <iostream>
#include <fstream>

#include "levelconfig.h"
#include <base2dobject.h>

using namespace  std;

namespace SERIALIZE{

const int DEFAULT_TIME_PER_LEVEL= 50;
const int DEFAULT_LEVELNR       = 1;
const int DEFAULT_ROWNUMBER     = 1;
const int DEFAULT_GAP           = 64;
const int DEFAULT_STEPPERROW    = 2;
const int DEFAULT_STEP_ENEMY    = 3;

const int CONVERT_FAILED_VALUE  = 32767;

const std::string IMG_Turtles = "/home/paul/workspace/Frogger/images/Turtles10x1_64x64.png";
const std::string IMG_Vehicles ="/home/paul/workspace/Frogger/images/Vehicles6x1_64x64.png";
const std::string IMG_TreeLarge = "/home/paul/workspace/Frogger/images/Baum320x68.png";
const std::string IMG_Tree_Small = "/home/paul/workspace/Frogger/images/Baum192x68.png";

// Enemies
const std::string IMG_Croco = "/home/paul/workspace/Frogger/images/Crocs392x64_2_1.png";

    class Serialize
    {
    public:
        Serialize();
        Serialize(string path, Shader *shader, int resx, int resy);

        bool SerializeData(const string path,Shader* shader,int resx, int resy);
        bool DeSerializeData();

        CONFIG::AnyLevel& AnyLevels();
        bool LoadingDone();



    protected:
        bool ParseData(const string path);
        void InitDataLine(std::string line, ifstream &f);
    private:
        string _Path;
        CONFIG::Level _Level;
        CONFIG::AnyLevel _AnyLevel;

        // helpers
        int _TimePerLevel;
        int _LevelNr;
        int _RowNumber;
        int _Gap;
        int _StepPerRow;
        int _StepEnemyPerRow;

        CONFIG::Row _Row;


        std::vector<std::string> splitt(const std::string& str, char delimiter);
        int StringToInt(std::vector<string> strings,int index);
        int StringToInt(std::string str, int FailedVal = 32767);
        std::string ToUpper(std::string t);
        Shader * _Shader;
        int _Resx,_Resy;
        bool _DataParsingOk = false;
    };
}


#endif // LEVELSERIALIZE_H
