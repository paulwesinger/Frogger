#include <QCoreApplication>
#include </usr/include/SDL2/SDL.h>
#include <iostream>
#include "engine.h"


//using namespace  ENGINE;
using namespace std;



int main(int argc, char *argv[])
{
    //ENGINE::GLFrameWork engine(1440,1080);
    TestEngine engine(1280,1024);

    if (engine.InitSDL()) {
        cout << "init sdl ok" << endl;

        engine.PrintDisplayModes();
        engine.PrintOpenGLData();
        engine.PrintDisplayModes();

        if (engine.initViewElements()){
            // +++++++++++++++++++++++++++++++++++++++
            // Init Userobjects
            // +++++++++++++++++++++++++++++++++++++++
            if (engine.InitUserObjects()){
                engine.StartUp();
                engine.StartBackgroundSound();
                engine.Run();
            }
        }
    }
    else{
        cout << "init sdl failed" << endl;
    }

    engine.DestroySDL();
    // Set up code that uses the Qt event loop here.
    // Call a.quit() or a.exit() to quit the application.
    // A not very useful example would be including
    // #include <QTimer>
    // near the top of the file and calling
    // QTimer::singleShot(5000, &a, &QCoreApplication::quit);
    // which quits the application after 5 seconds.

    // If you do not need a running Qt event loop, remove the call
    // to a.exec() or use the Non-Qt Plain C++ Application template.

    return 0;
}
