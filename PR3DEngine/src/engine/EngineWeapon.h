/*
-----------------------------------------------------------------------------
Arquivo:     EngineWeapon.h
Descricao:   Classe que representa uma arma ou magia
-----------------------------------------------------------------------------
*/

#ifndef ENGINE_WEAPON_H_
#define ENGINE_WEAPON_H_

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

class EngineWeapon {
	
	public:

		//construtor e destrutor
		EngineWeapon(void);
		~EngineWeapon(void);
		
		//getters/setters
		Ogre::String getModel() { return _model; }
		void setModel(Ogre::String model) { _model = model; }

		Ogre::Vector3 getScale() { return _scale; }
		void setScale(Ogre::Vector3 scale) { _scale = scale; }

		Ogre::Real getVelocity() { return _velocity; }
		void setVelocity(Ogre::Real velocity) { _velocity = velocity; }

		Ogre::Vector3 getPosition() { return _position; }
		void setPosition(Ogre::Vector3 position) { _position = position; }

		Ogre::Entity *getEntity() { return _entity; }
		void setEntity(Ogre::Entity *entity) { _entity = entity; }

	    SceneNode *getNode() { return _node; }
	    void setNode(SceneNode *node) { _node = node; }

		Ogre::String getName() { return _name; }
		void setName(Ogre::String name) { _name = name; }

		Ogre::String getMaterial() { return _material; }
		void setMaterial(Ogre::String material) { _material = material; }

		bool getCastShadow() { return _castShadow; }
		void setCastShadow(bool castShadow) { _castShadow = castShadow; }

		Ogre::Real getMass() { return _mass; }
		void setMass(Ogre::Real mass) { _mass = mass; }

		Ogre::Vector3 getCenterOfMass() { return _centerOfMass; }
		void setCenterOfMass(Ogre::Vector3 centerOfMass) { _centerOfMass = centerOfMass; }

		Ogre::Vector3 getInertia() { return _inertia; }
		void setInertia(Ogre::Vector3 inertia) { _inertia = inertia; }

		Ogre::Vector3 getDistanceFromPlayer() { return _distanceFromPlayer; }
		void setDistanceFromPlayer(Ogre::Vector3 distanceFromPlayer) { _distanceFromPlayer = distanceFromPlayer; }

		Ogre::Vector3 getMoveDirection() { return _moveDirection; }
		void setMoveDirection(Ogre::Vector3 moveDirection) { _moveDirection = moveDirection; }

		Ogre::Real getShootInterval() { return _shootInterval; }
		void setShootInterval(Ogre::Real shootInterval) { _shootInterval = shootInterval; }


	private:
		
		//atributos
		Entity* _entity;
		SceneNode* _node;
		Ogre::Vector3 _position;
		Ogre::Quaternion _orientation;
		Ogre::Real _velocity;
		Ogre::Vector3 _inertia;
		Ogre::Vector3 _centerOfMass;
		Ogre::String _name;
		Ogre::String _model;
		Ogre::Vector3 _scale;
		Ogre::Vector3 _distanceFromPlayer;
		Ogre::Vector3 _moveDirection;
		Ogre::String _material;
		Ogre::Real _mass;
		bool _castShadow;

		Ogre::Real _shootInterval;
		
		OGRE3DBody *_body;

};

#endif