/*
-----------------------------------------------------------------------------
Arquivo:     EngineMap.h
Descricao:   Classe que representa um mapa com seus objetos
-----------------------------------------------------------------------------
*/

#ifndef ENGINE_MAP_H_
#define ENGINE_MAP_H_

/*
Inclui classes da Engine
*/
#include "EngineGlobalObjects.h"
#include "EngineUtil.h"

/*
Inclui classes da Ogre
*/
#include "Ogre.h"
using namespace Ogre;

/*
Inclui classes da NxOgre
*/
#include <NxOgre.h>
#include <NxOgreOGRE3D.h>

/*
Inclui classes da TinyXml
*/
#include "tinyxml.h"

class EngineMap {

	public:
		
		//construtor/destrutor
		EngineMap(String name);
		~EngineMap();
		
		//métodos
		void create();
		void createSkyBox();
		void forceCallback(OgreNewt::Body *body, float timeStep, int threadIndex);
		
		//getters e setters
		void setName(String name) { _name = name; }
		String getName() { return _name; }

		Vector3 getPlayerStart() { return _playerStart; }
		void setPlayerStart(Vector3 playerStart) { _playerStart = playerStart; }

		Quaternion getPlayerStartOrientation() { return _playerStartOrientation; }
		void setPlayerStartOrientation(Quaternion playerStartOrientation) { _playerStartOrientation = playerStartOrientation; }
		
		void setSkyBox(String skybox) { _skybox = skybox; }
		String getSkyBox() { return _skybox; }

		Vector3 getMinWorldSize() { return _minWorldSize; }
		void setMinWorldSize(Vector3 minWorldSize) { _minWorldSize = minWorldSize; }

		Vector3 getMaxWorldSize() { return _maxWorldSize; }
		void setMaxWorldSize(Vector3 maxWorldSize) { _maxWorldSize = maxWorldSize; }

		Vector3 getAmbientLight() { return _ambientLight; }
		void setAmbientLight(Vector3 ambientLight) { _ambientLight = ambientLight; }

		ColourValue getFogColour() { return _fogColour; }
		void setFogColour(ColourValue fogColour) { _fogColour = fogColour; }

		void setFogMode(String fogMode) { _fogMode = fogMode; }
		String getFogMode() { return _fogMode; }

		Real getFogStart() { return _fogStart; }
		void setFogStart(Real fogStart) { _fogStart = fogStart; }

		Real getFogEnd() { return _fogEnd; }
		void setFogEnd(Real fogEnd) { _fogEnd = fogEnd; }

		Real getFogDensity() { return _fogDensity; }
		void setFogDensity(Real fogDensity) { _fogDensity = fogDensity; }
		
	private:
		
		//atributos
		String _name;
		Vector3 _playerStart;
		Quaternion _playerStartOrientation;
		String _skybox;
		Vector3 _minWorldSize;
		Vector3 _maxWorldSize;
		Vector3 _ambientLight;

		String _fogMode;
		ColourValue _fogColour;
		Real _fogStart;
		Real _fogEnd;
		Real _fogDensity;

};

#endif