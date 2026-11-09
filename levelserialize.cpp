#include "levelserialize.h"

SERIALIZE::Serialize::Serialize() {
    _Path = "";
}
SERIALIZE::Serialize::Serialize(string path){
    _Path = path;
}

bool SERIALIZE::Serialize::SerializeData(const string path){
    _Path = path;

    std::ofstream file(path); // Kein append !!
    if ( ! file.is_open()) return false;

    file << "test" <<";"<< "test 1" <<";" << std::endl;

    file.close();
    return true;
}

bool SERIALIZE::Serialize::DeSerializeData(const string path)
{
    ParseData(path);
    return true;
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

void SERIALIZE::Serialize::ParseData(const string path){
    std::ifstream file(path);

    //CONFIG::AnyLevel empty;

    if (! file.is_open()) return ; //empty;
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
                }
        //
    }
    file.close();
}
