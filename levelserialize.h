#ifndef LEVELSERIALIZE_H
#define LEVELSERIALIZE_H

#include <iostream>
#include <fstream>

#include "levelconfig.h"

using namespace  std;

namespace SERIALIZE{

    class Serialize
    {
    public:
        Serialize();
        Serialize(string path);

        bool SerializeData(const string path);
        bool DeSerializeData(const string path);


    protected:
        void ParseData(const string path);
    private:
        string _Path;
    };
}


#endif // LEVELSERIALIZE_H
