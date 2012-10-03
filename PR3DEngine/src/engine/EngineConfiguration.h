/*
-----------------------------------------------------------------------------
Arquivo:     EngineConfiguration.h
Descricao:   Classe base da Engine
-----------------------------------------------------------------------------
*/

#ifndef ENGINE_CONFIGURATION_H_
#define ENGINE_CONFIGURATION_H_

/*
Inclui classes da Ogre
*/
#include "Ogre.h"
using namespace Ogre;

/*
Inclui classes da TinyXml
*/
#include "tinyxml.h"

class EngineConfiguration {
	public:

		//construtor e destrutor
		EngineConfiguration();
		~EngineConfiguration();
		
		//métodos
		void load();

		//getters e setters
		Ogre::String getTitle() { return _title; }

		Ogre::String getResourcePath() { return _resourcePath; }

		Ogre::String getMapName() { return _mapName; }
		
		Ogre::String getPlayerName() { return _playerName; }

		bool getConfigurationScreen() { return _configurationScreen; }

		Ogre::Real getNearClipDistance() { return _nearClipDistance; }

		Ogre::Real getFarClipDistance() { return _farClipDistance; }

		Ogre::Vector3 getGravity() { return _gravity; }

		Ogre::Real getCameraZoom() { return _cameraZoom; }

		Ogre::Vector3 getCameraModifier() { return _cameraModifier; }

		bool getShowBoundingBox() { return _showBoundingBox; }

		bool getViewportShadown() { return _viewportShadown; }

		bool getViewportSkies() { return _viewportSkies; }

		bool getViewportOverlays() { return _viewportOverlays; }

	private:

		//atributos
		Ogre::String _title;
		Ogre::String _resourcePath;
		Ogre::String _mapName;
		Ogre::String _playerName;		
		
		bool _configurationScreen;
		
		Ogre::Real _nearClipDistance;
		Ogre::Real _farClipDistance;

		Ogre::Real _cameraZoom;
		Ogre::Vector3 _cameraModifier;

		Ogre::Vector3 _gravity;

		bool _showBoundingBox;

		bool _viewportShadown;
		bool _viewportSkies;
		bool _viewportOverlays;
};

#endif
