#include "EnginePlayer.h"

EnginePlayer::EnginePlayer(String name) {
	//inicializa atributos
	_name     = name;
	_position = Ogre::Vector3::ZERO;

	_walking   = false;
	_running   = false;
	_attacking = false;
	_jumping   = false;
	_canJump   = false;

	_shootCount = 0;
	_shootTime  = 0;
}

EnginePlayer::~EnginePlayer() {
	_node->detachAllObjects();
	delete _entity;
	_node->removeAndDestroyAllChildren();

	delete _body;
	delete _node;
}

void EnginePlayer::create() {
	//carrega atributos do xml
	String fileName = "media/players/";
	fileName += _name.c_str();
	fileName += ".xml";

	TiXmlDocument doc(fileName.c_str());
	if (!doc.LoadFile()) return;

	TiXmlHandle handleDoc(&doc);
			
	//obtém elemento com as configurações do player
	TiXmlElement* elPlayer = handleDoc.FirstChild("player").Element();
	if (!elPlayer) return;

	String stVersion         = elPlayer->Attribute("version");
	String stModel           = elPlayer->Attribute("model");
	String stMaterial        = elPlayer->Attribute("material");
	String stScale           = elPlayer->Attribute("scale");
	String stOrientation     = elPlayer->Attribute("orientation");
	String stMass            = elPlayer->Attribute("mass");
	String stVelocity        = elPlayer->Attribute("velocity");
	String stRunVelocity     = elPlayer->Attribute("run_velocity");
	String stJumpVelocity    = elPlayer->Attribute("jump_velocity");
	String stInertia         = elPlayer->Attribute("inertia");
	String stCenterOfMass    = elPlayer->Attribute("center_of_mass");
	String stWeapon          = elPlayer->Attribute("weapon");
	
	//atribui os valores do xml ao objeto
	_model    = stModel;
	_material = stMaterial;
	_weapon   = stWeapon;

	if (stScale != "") { 
		_scale = StringConverter::parseVector3(stScale);
	} else {
		_scale = Vector3(0,0,0);
	}

	if (stOrientation != "") { 
		_orientation = StringConverter::parseQuaternion(stOrientation);
	} else {
		_orientation = Quaternion(1, 0, 0, 0);
	}

	if (stMass != "") { 
		_mass = StringConverter::parseReal(stMass);
	} else {
		_mass = 0;
	}

	if (stVelocity != "") { 
		_velocity = StringConverter::parseReal(stVelocity);
	} else {
		_velocity = 0;
	}

	if (stRunVelocity != "") { 
		_runVelocity = StringConverter::parseReal(stRunVelocity);
	} else {
		_runVelocity = 0;
	}

	if (stJumpVelocity != "") { 
		_jumpVelocity = StringConverter::parseReal(stJumpVelocity);
	} else {
		_jumpVelocity = 0;
	}

	if (stInertia != "") { 
		_inertia = StringConverter::parseVector3(stInertia);
	} else {
		_inertia = Vector3(0,0,0);
	}

	if (stCenterOfMass != "") { 
		_centerOfMass = StringConverter::parseVector3(stCenterOfMass);
	} else {
		_centerOfMass = Vector3(0,0,0);
	}

	//carrega player e seus objetos
	_entity = EngineGlobalObjects::getInstance().getSceneManager()->createEntity("player_entity_" + _name, _model);
	_node = EngineGlobalObjects::getInstance().getSceneManager()->getRootSceneNode()->createChildSceneNode("player_node_" + _name, _scale);

	if (_material != "") {
		_entity->setMaterialName(_material);
	}
	
    _node->attachObject(_entity);
    _node->setScale(_scale);	
	_node->setPosition(_position);
	
	OgreNewt::ConvexCollisionPtr col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::Ellipsoid( EngineGlobalObjects::getInstance().getWorld(), _scale, 0 ));
	
	//cria corpo físico
    _body = new OgreNewt::Body( EngineGlobalObjects::getInstance().getWorld(), col );
    
	_body->attachNode(_node);
    _body->setMassMatrix(_mass, _inertia);
    _body->setCenterOfMass(_centerOfMass);
    _body->setStandardForceCallback();

	_body->setPositionOrientation(_position, _orientation);

	//**_playerController = new OgreNewt::PlayerController(_body, 0.4);

	doc.Clear();

	//define callback customizado
	_body->setCustomForceAndTorqueCallback<EnginePlayer>(&EnginePlayer::forceCallback, this);
}


void EnginePlayer::update(Real elapsedTime, OIS::Keyboard *input) {
	//adiciona tempo de tiro
	_shootTime += elapsedTime;

	//movimentação
	Vector3 playerPosition       = _node->getPosition();
	Quaternion playerOrientation = _node->getOrientation();
	
	_walking   = false;
	_running   = false;
	_attacking = false;
	_jumping   = false;
	_canJump   = false;

	//verifica se pode pular
	if (_playerController->getPlayerState() == _playerController->PS_ONLAND) {
		_canJump = true;
	}
	
	//movimentação pela física
	Ogre::Real forwardSpeed, sideSpeed;
    Ogre::Radian heading;
    
	_playerController->getVelocity(forwardSpeed, sideSpeed, heading);

    forwardSpeed = 0;
    sideSpeed = 0;	
	
	Degree angulo = Degree(heading.valueDegrees()-90);

	//calcula velocidade
	Real tempVelocity = _velocity;
	
	//verifica se está atacando ou não
	if (input->isKeyDown(OIS::KC_LCONTROL) || input->isKeyDown(OIS::KC_RCONTROL)) {
		
		_walking   = false;
		_running   = false;
		_attacking = true;
		
	} else {
		
		//realiza pulo
		if (input->isKeyDown(OIS::KC_SPACE) || input->isKeyDown(OIS::KC_SPACE)) {
			if (_canJump == true) {
				_jumping = true;
			}
		}

		//realiza os movimentos ou não
		if (input->isKeyDown(OIS::KC_LSHIFT) || input->isKeyDown(OIS::KC_RSHIFT)) {
			tempVelocity += _runVelocity;
			_running = true;
		}

		if (input->isKeyDown(OIS::KC_W)) {
			
			forwardSpeed += -cos(angulo.valueRadians()+3.14/2)*tempVelocity;
			sideSpeed += sin(angulo.valueRadians()+3.14/2)*tempVelocity;
			
			_walking = true;

		} else if (input->isKeyDown(OIS::KC_S)) {
			
			forwardSpeed -= -cos(angulo.valueRadians()+3.14/2)*tempVelocity;
			sideSpeed -= sin(angulo.valueRadians()+3.14/2)*tempVelocity;
			
			_walking = true;
		}

		if (input->isKeyDown(OIS::KC_A)) {
			
			heading += Degree(45)/60.0;

			_walking = true;

		} else if (input->isKeyDown(OIS::KC_D)) {
			
			heading -= Degree(45)/60.0;

			_walking = true;

		}

	}

	
	//controla a animação atual
	if (_attacking == true) {
		_animationState = _entity->getAnimationState("Walk");
		_animationState->setEnabled( false );

		_animationState = _entity->getAnimationState("Idle");
		_animationState->setEnabled( false );

		_animationState = _entity->getAnimationState("Run");
		_animationState->setEnabled( false );

		_animationState = _entity->getAnimationState("Jump");
		_animationState->setEnabled( false );

		//atira
		shootWeapon();

		_animationState = _entity->getAnimationState("Attack");
		_animationState->setLoop( true );
		_animationState->setEnabled( true );
		_animationState->addTime(elapsedTime * 2);	
	} else if (_running == true) {
		_animationState = _entity->getAnimationState("Walk");
		_animationState->setEnabled( false );

		_animationState = _entity->getAnimationState("Idle");
		_animationState->setEnabled( false );

		_animationState = _entity->getAnimationState("Attack");
		_animationState->setEnabled( false );

		_animationState = _entity->getAnimationState("Jump");
		_animationState->setEnabled( false );

		_animationState = _entity->getAnimationState("Run");
		_animationState->setLoop( true );
		_animationState->setEnabled( true );
		_animationState->addTime(elapsedTime * 2);	
	} else if (_walking == true) {
		_animationState = _entity->getAnimationState("Run");
		_animationState->setEnabled( false );

		_animationState = _entity->getAnimationState("Idle");
		_animationState->setEnabled( false );

		_animationState = _entity->getAnimationState("Attack");
		_animationState->setEnabled( false );

		_animationState = _entity->getAnimationState("Jump");
		_animationState->setEnabled( false );

		_animationState = _entity->getAnimationState("Walk");
		_animationState->setLoop( true );
		_animationState->setEnabled( true );
		_animationState->addTime(elapsedTime * 2);				
	} else {
		_animationState = _entity->getAnimationState("Walk");
		_animationState->setEnabled( false );

		_animationState = _entity->getAnimationState("Run");
		_animationState->setEnabled( false );

		_animationState = _entity->getAnimationState("Attack");
		_animationState->setEnabled( false );

		_animationState = _entity->getAnimationState("Jump");
		_animationState->setEnabled( false );

		_animationState = _entity->getAnimationState("Idle");
		_animationState->setLoop( true );
		_animationState->setEnabled( true );
		_animationState->addTime(elapsedTime * 2);
	}

	if (_jumping == true) {
		_animationState = _entity->getAnimationState("Walk");
		_animationState->setEnabled( false );

		_animationState = _entity->getAnimationState("Run");
		_animationState->setEnabled( false );

		_animationState = _entity->getAnimationState("Attack");
		_animationState->setEnabled( false );

		_animationState = _entity->getAnimationState("Idle");
		_animationState->setEnabled( false );

		_animationState = _entity->getAnimationState("Jump");
		_animationState->setLoop( true );
		_animationState->setEnabled( true );
		_animationState->addTime(elapsedTime * 2);

		
	}

	//define propriedades da física
	_playerController->setVelocity(forwardSpeed, sideSpeed, heading);

}

void EnginePlayer::shootWeapon() {
	
	for(unsigned int x = 0; x < EngineGlobalObjects::getInstance().getWeapons().size(); x++) {
		
		//verifica se a arma do loop é a arma atual do player
		if ( EngineGlobalObjects::getInstance().getWeapons()[x]->getName() == _weapon ) {
			
			//valida tempo do tiro
			if ( _shootTime >= EngineGlobalObjects::getInstance().getWeapons()[x]->getShootInterval() ) {
				_shootTime = 0;
				
				//cria objeto baseado nas propriedades da arma
				Entity *entity;
				entity = EngineGlobalObjects::getInstance().getSceneManager()->createEntity("shoot_player_" + StringConverter::toString(_shootCount++), EngineGlobalObjects::getInstance().getWeapons()[x]->getModel());
				
				entity->setMaterialName(EngineGlobalObjects::getInstance().getWeapons()[x]->getMaterial());
				entity->setCastShadows(EngineGlobalObjects::getInstance().getWeapons()[x]->getCastShadow());

				SceneNode *node = EngineGlobalObjects::getInstance().getSceneManager()->getRootSceneNode()->createChildSceneNode();
				node->attachObject(entity);

				//define tamanho
				node->setScale(EngineGlobalObjects::getInstance().getWeapons()[x]->getScale());

				OgreNewt::ConvexCollisionPtr col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(EngineGlobalObjects::getInstance().getWorld(), entity, 0));
				OgreNewt::Body* bod = new OgreNewt::Body( EngineGlobalObjects::getInstance().getWorld(), col );

				bod->attachNode( node );
				bod->setMassMatrix( EngineGlobalObjects::getInstance().getWeapons()[x]->getMass(), EngineGlobalObjects::getInstance().getWeapons()[x]->getInertia() );
				bod->setCenterOfMass(EngineGlobalObjects::getInstance().getWeapons()[x]->getCenterOfMass());
				bod->setStandardForceCallback();

				bod->setPositionOrientation( _node->getPosition() + EngineGlobalObjects::getInstance().getWeapons()[x]->getDistanceFromPlayer(), _node->getOrientation() );
				
				//define velocidade
				Vector3 direction = _node->getOrientation() * EngineGlobalObjects::getInstance().getWeapons()[x]->getMoveDirection();
				
				bod->setVelocity(direction * EngineGlobalObjects::getInstance().getWeapons()[x]->getVelocity());

				//define callback customizado
				bod->setCustomForceAndTorqueCallback<EnginePlayer>(&EnginePlayer::forceCallbackWeapon, this);
				
			}

		}

	}

}

void EnginePlayer::forceCallback(OgreNewt::Body *body, float timeStep, int threadIndex) {
	//obtém massa e inércia
	Ogre::Real mass;
    Ogre::Vector3 inertia;
	body->getMassMatrix(mass, inertia);
	
	//define variável de controle do pulo
	Vector3 jumpVelocity(0, _jumpVelocity, 0);

	//define física do pulo
	if (_jumping == true) {
		_playerController->setPlayerState(_playerController->PS_ONFREEFALL);
		body->addLocalForce(jumpVelocity, Vector3(0, 0, 0));
	}

	//adiciona gravidade
	Ogre::Vector3 gravity = EngineGlobalObjects::getInstance().getConfiguration()->getGravity() * mass;
    body->addForce(gravity);
}

void EnginePlayer::forceCallbackWeapon(OgreNewt::Body *body, float timeStep, int threadIndex) {
	//obtém massa e inércia
	Ogre::Real mass;
    Ogre::Vector3 inertia;
	body->getMassMatrix(mass, inertia);
	
	//adiciona gravidade
	Ogre::Vector3 gravity = EngineGlobalObjects::getInstance().getConfiguration()->getGravity() * mass;
    body->addForce(gravity);
}