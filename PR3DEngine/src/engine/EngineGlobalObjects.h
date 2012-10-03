/*
-----------------------------------------------------------------------------
Arquivo:     EngineGlobalObjects.h
Descricao:   Classe que guarda objetos de acesso global
-----------------------------------------------------------------------------
*/

#ifndef ENGINE_GLOBAL_OBJECTS_H_
#define ENGINE_GLOBAL_OBJECTS_H_

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
using namespace NxOgre;

/*
Inclui classes da Engine
*/
#include "EngineConfiguration.h"
#include "EngineWeapon.h"

class EngineGlobalObjects {

	private:
		
		//atributos
		static EngineGlobalObjects* _instance;
		
		Root *_root;
		Camera *_camera;
		SceneManager *_sceneManager;
		RenderWindow *_window;
		World *_world;
		EngineConfiguration *_configuration;

		std::vector<EngineWeapon*> _weapons;

	public:
		
		//construtor e destrutor
		EngineGlobalObjects();
		~EngineGlobalObjects();

		//métodos
		static EngineGlobalObjects& getInstance();
		
		//getters e setters
		Root* getRoot() { return _root; }
		void setRoot(Root *root) { _root = root; }

		Camera* getCamera() { return _camera; }
		void setCamera(Camera *camera) { _camera = camera; }

		SceneManager* getSceneManager() { return _sceneManager; }
		void setSceneManager(SceneManager *sceneManager) { _sceneManager = sceneManager; }

		RenderWindow* getRenderWindow() { return _window; }
		void setRenderWindow(RenderWindow *window) { _window = window; }

		World* getWorld() { return _world; }
		void setWorld(World *world) { _world = world; }

		EngineConfiguration* getConfiguration() { return _configuration; }
		void setConfiguration(EngineConfiguration *configuration) { _configuration = configuration; }
		
		void setWeapons(std::vector<EngineWeapon*> weapons) { _weapons = weapons; }
		std::vector<EngineWeapon*> getWeapons() { return _weapons; }
};

#endif