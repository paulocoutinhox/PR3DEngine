/*
-----------------------------------------------------------------------------
Arquivo:     main.cpp
Descricao:   Arquivo inicial da aplicação
-----------------------------------------------------------------------------
*/
#include "engine/Engine.h"

#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
int main(int argc, char **argv)
#endif
{

	//inicializa a engine
	Engine engine;

    try {
    	
		engine.initialize();

    } catch(Exception& e) {

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBox(NULL, e.getFullDescription().c_str(), "Ocorreu um erro!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        fprintf(stderr, "Ocorreu um erro: %s\n",e.getFullDescription().c_str());
#endif

    }

    return 0;
}

