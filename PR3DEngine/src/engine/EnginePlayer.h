/*
-----------------------------------------------------------------------------
Arquivo:     Player.h
Descricao:   Classe que representa um jogador
-----------------------------------------------------------------------------
*/

#ifndef PLAYER_H_
#define PLAYER_H_

/*
Inclui classes padrões
*/
#include <string>
using namespace std;

/*
Inclui classes da Ogre
*/
#include "Ogre.h"
#include "OIS/OIS.h"
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

/*
Inclui classes da TinyXml
*/
#include "tinyxml.h"

class EnginePlayer {

	public:
		
		//construtor/destrutor
		EnginePlayer(Ogre::String name);
		virtual ~EnginePlayer();

		//métodos
		void create();
		void update(Ogre::Real elapsedTime, OIS::Keyboard *input);
		void shootWeapon();
		//void forceCallback(OgreNewt::Body *body, float timeStep, int threadIndex);
		//void forceCallbackWeapon(OgreNewt::Body *body, float timeStep, int threadIndex);
						
		//getters/setters
		Ogre::String getModel() { return _model; }
		void setModel(Ogre::String model) { _model = model; }

		Ogre::Vector3 getScale() { return _scale; }
		void setScale(Ogre::Vector3 scale) { _scale = scale; }

		Ogre::Real getVelocity() { return _velocity; }
		void setVelocity(Ogre::Real velocity) { _velocity = velocity; }

		Ogre::Vector3 getPosition() { return _position; }
		void setPosition(Ogre::Vector3 position) { _position = position; }

		Entity *getEntity() { return _entity; }
		void setEntity(Entity *entity) { _entity = entity; }

	    SceneNode *getNode() { return _node; }
	    void setNode(SceneNode *node) { _node = node; }

		Ogre::String getName() { return _name; }
		void setName(Ogre::String name) { _name = name; }

		Ogre::String getMaterial() { return _material; }
		void setMaterial(Ogre::String material) { _material = material; }

		Ogre::String getWeapon() { return _weapon; }
		void setWeapon(Ogre::String weapon) { _weapon = weapon; }
		
private:

		//atributos
	    Entity* _entity;
		SceneNode* _node;
		Ogre::Vector3 _position;
		Ogre::Quaternion _orientation;
		Ogre::Real _velocity;
		Ogre::Real _runVelocity;
		Ogre::Real _jumpVelocity;
		Ogre::Vector3 _inertia;
		Ogre::Vector3 _centerOfMass;
		Ogre::String _name;
		Ogre::String _model;
		Ogre::Vector3 _scale;
		Ogre::String _material;
		Ogre::Real _mass;

		Ogre::String _weapon;

		OgreNewt::Body* _body;

		AnimationState *_animationState;

		long int _shootCount;

		bool _walking;
		bool _running;
		bool _attacking;
		bool _jumping;
		bool _canJump;

		//OgreNewt::PlayerController *_playerController;

		Ogre::Real _shootTime;

};

#endif
