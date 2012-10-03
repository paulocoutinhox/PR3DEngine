/*
-----------------------------------------------------------------------------
Arquivo:     Engine.h
Descricao:   Classe base da Engine
-----------------------------------------------------------------------------
*/

#ifndef ENGINE_H_
#define ENGINE_H_

/*
Inclui classes da Ogre
*/
#define OIS_DYNAMIC_LIB
#include "Ogre.h"
#include "OgreConfigFile.h"
using namespace Ogre;

/*
Inclui classes da NxOgre
*/
#include <NxOgre.h>
#include <NxOgreOGRE3D.h>

/*
Inclui classes da Engine
*/
#include "EngineGlobalObjects.h"
#include "EngineConfiguration.h"
#include "EngineFrameListener.h"
#include "EnginePlayer.h"
#include "EngineMap.h"
#include "EngineTPCamera.h"
#include "EngineBarraProgresso.h"
#include "EngineWeapon.h"

/*
Inclui classes da TinyXml
*/
#include "tinyxml.h"

class Engine {

	public:
    
		//construtor e destrutor
		Engine();
		virtual ~Engine();
	    
		//métodos
		virtual void initialize();

	protected:
		
		//atributos
		Root *_root;
		Camera *_camera;
		SceneManager *_sceneManager;
		NxOgre::Scene *_scene;
		EngineFrameListener *_frameListener;
		RenderWindow *_window;
		EngineConfiguration *_configuration;

		EngineMap *_map;

		vector<EnginePlayer*> _players;
		vector<EngineWeapon*> _weapons;

		NxOgre::World *_world;
		Ogre::FrameListener *_newtonListener;
		
		EngineTPCamera *_engineTPCamera;

		EngineBarraProgresso _barraProgresso;
		
		//métodos		
		virtual bool setup();
		virtual bool configure();
		virtual void chooseSceneManager();
		virtual void createCamera();
		virtual void createFrameListener();
		virtual void createScene();
		virtual void destroyScene();
		virtual void createViewports();
		virtual void setupResources();
		virtual void createResourceListener();
		virtual void loadResources();
		virtual void createPlayers();
		virtual void createMap();
		virtual void createLighting();
		virtual void createWeapons();

		//getters e setters
		void setWorld(NxOgre::World *world) { _world = world;	}
		NxOgre::World *getWorld() { return _world; }

};


#endif
