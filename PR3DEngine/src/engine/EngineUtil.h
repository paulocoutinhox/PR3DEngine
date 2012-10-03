/*
-----------------------------------------------------------------------------
Arquivo:     EngineUtil.h
Descricao:   Classe contento métodos de utilidade comum
-----------------------------------------------------------------------------
*/

#ifndef ENGINE_UTIL_H_
#define ENGINE_UTIL_H_

/*
Inclui classes da Ogre
*/
#include "Ogre.h"
using namespace Ogre;

class EngineUtil {
	
	public:

		//construtor e destrutor
		EngineUtil();
		~EngineUtil();

		static Vector3 stringToVector3(String data, bool removeFirstOnOverload = true);
		static int stringCount(const String data, const String find);

};

#endif
