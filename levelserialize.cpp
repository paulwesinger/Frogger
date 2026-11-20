#include "levelserialize.h"
#include <bits/stdc++.h>

SERIALIZE::Serialize::Serialize() {
    _Path = "";
    _Shader = nullptr;
}
SERIALIZE::Serialize::Serialize(string path, Shader *shader, int resx, int resy){
    _Path = path;    
    _Shader = shader;
    _Resx = resx;
    _Resy = resy;
}

bool SERIALIZE::Serialize::SerializeData(const string path, Shader *shader, int resx, int resy){
    _Path = path;
    _Shader = shader;
    _Resx = resx;
    _Resy = resy;
    std::ofstream file(path); // Kein append !!
    if ( ! file.is_open()) return false;

    file << "test" <<";"<< "test 1" <<";" << std::endl;

    file.close();
    return true;
}

bool SERIALIZE::Serialize::DeSerializeData()
{
    if (SERIALIZE::Serialize::ParseData(_Path))
        return true;
    return false;
}

/*
 * Textfile -Structure
 * LEVELBGIN
 *  Levelnr 1..n
 *  TimeperLevel n-sekunden
 *  Rows[
 *          Row_1:
 *                  Objects[
 *                              Baum_kurz
 *                              GAP : integer_value
 *                              Baum_Kurz
 *                              GAP : integer_Value
 *                              Baum lang
 *                              GAP: integer_value
 *                              ......
 *                          ]
 *                   Enemies [
 *                              Biber
 *                              Snake
 *                          ]
 *                   Step -> negativ oder positiv --> geschwindig der objekte
 *
 *
 *          Row_2:
 *              ... usw
 *      ]
 * LEVELEND
 * #nächster
 * LevelBegin
 *              siehe oben
 * LevelEnd
 * #nächster
 *
 */

CONFIG::AnyLevel &SERIALIZE::Serialize::AnyLevels(){
    return _AnyLevel;
}

/// ******************************
/// Hilfsfunktionen
/// ******************************
/// Stringsplitter:
std::vector<std::string> SERIALIZE::Serialize::splitt(const string& str, char delimiter){
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delimiter, start);
    }

    tokens.push_back(str.substr(start));
    return tokens;
}
/// stringtoint converter
int SERIALIZE::Serialize::StringToInt(std::vector<string> strings, int index){
    if (index >= strings.size()) return -1;

    int tmp;
    try{
        tmp = stoi(strings[index]);
    }
    catch(const std::invalid_argument &){
        std::cout << "Invalid argument" << std::endl;
        tmp = -1;
    }
    catch(const std::out_of_range&){
        std::cout << "argument out of range" << std::endl;
        tmp = -1;
    }
    return tmp;
}

int SERIALIZE::Serialize::StringToInt(string str,int FailedVal){
    int tmp;
    try{
        tmp = stoi(str);
    }
    catch(const std::invalid_argument &){
        std::cout << "Invalid argument" << std::endl;
        tmp = FailedVal;
    }
    catch(const std::out_of_range&){
        std::cout << "argument out of range" << std::endl;
        tmp = FailedVal;
    }
    return tmp;
}

std::string SERIALIZE::Serialize::ToUpper(string t){
    transform(t.begin(),t.end(),t.begin(),::toupper);
    return t;
}


/// die  gelesene Zeile zerlegen und werte zuiweisen

void SERIALIZE::Serialize::InitDataLine(string line,std::ifstream & f){
    std::vector<std::string> splitted = splitt(line,' ');
    if (splitted.size() < 1) return;

    // **************************************************
    // Zeit für jeden Level
    // **************************************************
    //transform(splitted[0].begin(),splitted[0].end(),splitted[0].begin(),::toupper);

    splitted[0] = ToUpper(splitted[0]);

    if (splitted[0] == "TIMEPERLEVEL" ){

        _TimePerLevel = StringToInt(splitted,1);
        if (_TimePerLevel < 0)
            _TimePerLevel = DEFAULT_TIME_PER_LEVEL;

        _Level.TimePerLevel = _TimePerLevel;
    }

    // ***************************************************
    // Level Name
    // ***************************************************
    if (splitted[0] == "LEVELNAME"){
        // _LevelNr = StringToInt(splitted,1);
        // if (_LevelNr < 0)
        //     _LevelNr = DEFAULT_LEVELNR;

        if (splitted.size() >= 2)
            _Level.LevelName = splitted[1];
        else
            _Level.LevelName = "Unbekannter Level";
    }


    // ***************************************************
    // Level Nummer
    // ***************************************************
    if (splitted[0] == "LEVELNR"){
        _LevelNr = StringToInt(splitted,1);
        if (_LevelNr < 0)
            _LevelNr = DEFAULT_LEVELNR;

        _Level.Level = _LevelNr;
    }

    // ***************************************************
    // RowNumber's
    // ***************************************************
    if (splitted[0] == "ROWNUMBER"){
        _RowNumber = StringToInt(splitted,1);
        if (_RowNumber < 0)
            _RowNumber = DEFAULT_ROWNUMBER;
    }

    // ***************************************************
    // Step in row
    // ***************************************************

    if (splitted[0] == "STEP_OBJECT"){
        _StepPerRow = StringToInt(splitted,1);
        if (_StepPerRow == CONVERT_FAILED_VALUE)
            _StepPerRow = DEFAULT_STEPPERROW;

        _Row.ObjectStep = _StepPerRow;
    }

    // ***************************************************
    // Step in row enemy
    // ***************************************************

    if (splitted[0] == "STEP_ENEMY"){
        _StepEnemyPerRow = StringToInt(splitted,1);
        if (_StepEnemyPerRow == CONVERT_FAILED_VALUE)
            _StepEnemyPerRow = DEFAULT_STEP_ENEMY;
        _Row.EnemyStep = _StepEnemyPerRow;
    }

    if (splitted[0] == "OBJECTS_END")
    {
        return;
    }

    // ***************************************************
    // Objects
    // ***************************************************
    if (splitted[0] == "OBJECTS_BEGIN"){
        string l;
        int pos_x = 0;

        do
        {
            std::getline(f,l);
        }
        while(l == " " || l == "#" );

        l= ToUpper(l);

        //while (std::getline(f,l)) {

            std::vector<string> objects = splitt(l,' ');
            bool nextisgap=false;

            for(string s: objects) {
                if (s == "GAP")
                {
                    nextisgap = true;
                    continue;
                }
                else
                if (nextisgap){
                    _Gap = StringToInt(s);
                    if (_Gap == CONVERT_FAILED_VALUE)
                        _Gap = DEFAULT_GAP;

                    nextisgap = false;
                    pos_x += _Gap;
                }
                else
                {
                    ENGINE::Sprite* sp = nullptr;
                    if (s == "BAUM_LANG"){
                        sp = new ENGINE::Sprite(_Resx,_Resy,IMG_TreeLarge,_Shader);
                        sp->InitTextureMap(1,1);
                        sp->SetPosition(pos_x,0);
                    }
                    else
                    if ( s == "BAUM_KURZ"){
                        sp = new ENGINE::Sprite(_Resx,_Resy,IMG_Tree_Small,_Shader);
                        sp->InitTextureMap(1,1);
                        sp->SetPosition(pos_x,0);
                    }
                    else
                    if ( s== "TURTLE"){
                        sp = new ENGINE::Sprite(_Resx,_Resy,IMG_Turtles,_Shader);
                        sp->InitTextureMap(10,1);
                        sp->SetPosition(pos_x,0);
                    }
                    else
                    if ( s== "TURTLEDIVING"){
                        sp = new ENGINE::Sprite(_Resx,_Resy,IMG_Turtles,_Shader);
                        sp->InitTextureMap(10,1);
                        sp->SetPosition(pos_x,0);
                        sp->SetDiving(true);
                    }
                    else
                    if ( s == "VEHICLE"){ // Welches Teil angezeigt wird, wird in der engine
                        // ausgesucht...
                        sp = new ENGINE::Sprite(_Resx,_Resy,IMG_Vehicles,_Shader);
                        sp->InitTextureMap(6,1);
                        sp->SetPosition(pos_x,0);

                        //_Row.Objects.push_back(sp);
                    }
                    else
                    if (s == "TRUCK_FRONT"){ // sonderfall, der truck besteht aus 2 tiles
                        sp = new ENGINE::Sprite(_Resx,_Resy,IMG_Vehicles,_Shader);
                        sp->InitTextureMap(6,1);                     
                    }
                    else
                    if (s == "TRUCK_BACK"){
                        pos_x += 64;
                        sp = new ENGINE::Sprite(_Resx,_Resy,IMG_Vehicles,_Shader);
                        sp->InitTextureMap(6,1);
                                        // y position ist die selbe wie truck front !!                    
                    }


                    if (sp != nullptr){
                        //sp->SetPosition(pos_x,0);  //Y-pos Wird im eigentlichen programm dann gesetzt!
                        _Row.Objects.push_back(sp);
                    }
                    pos_x += 64;
                }
            }
            return;
        //}while
    }

    // ***************************************************
    // Enemies
    // ***************************************************
    if (splitted[0] == "ENEMIES_END")
    {
        return;
    }

    if (splitted[0] == "ENEMIES_BEGIN"){
        string l;
        int pos_x = 0;

        // while (std::getline(f,l)) {
        //     l= ToUpper(l);
        //     std::vector<string> enemies = splitt(l,' ');


            // hier die logik und enemies.push_back
        //}
    }

    // ***************************************************
    // Row Ende
    // ***************************************************

    if (splitted[0] == "END_ROW"){
        _Level.Rows.push_back(_Row);

        // Speicher nicht freigeben,
        // aber den Row Vector leeren
       _Row.Objects.clear();
       _Row.Enemies.clear();       
    }

    // ***************************************************
    // Level Ende
    // ***************************************************

    if (splitted[0] == "LEVEL_END"){
        _AnyLevel.AddLevelConfiguration(_Level);
    }
}

bool SERIALIZE::Serialize::ParseData(const string path){
    std::ifstream file(path);


    if (! file.is_open()) return false; //empty;
    string line;

    while (std::getline(file, line)) {
        if (line.starts_with("#")) { // Kommentar
            std::cout << "Kommentar" << std::endl;
            continue;
        }
        else
        if (line == "") {
            std::cout << "Leere zeile" << std::endl;
            continue;
        }
        else{
            std::cout << line << std::endl;
            InitDataLine(line,file);
        }
    }
    file.close();
    _DataParsingOk = true;
    return true;
}

bool SERIALIZE::Serialize::LoadingDone(){
    return _DataParsingOk;
}
