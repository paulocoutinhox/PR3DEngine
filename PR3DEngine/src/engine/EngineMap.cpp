#include "EngineMap.h"

EngineMap::EngineMap(String name) {
	_name = name;	
}

EngineMap::~EngineMap() {
	
}

void EngineMap::create() {
	//carrega atributos do xml
	String fileName = "media/maps/";
	fileName += _name.c_str();
	fileName += ".xml";
	
	TiXmlDocument doc(fileName.c_str());
	if (!doc.LoadFile()) return;

	TiXmlHandle handleDoc(&doc);
	
	//obtém elemento com as configurações do mapa
	TiXmlElement* elMap = handleDoc.FirstChild("map").Element();
	if (!elMap) return;

	String stPlayerStart            = elMap->Attribute("player_start");
	String stPlayerStartOrientation = elMap->Attribute("player_start_orientation");
	String stSkyBox                 = elMap->Attribute("skybox");
	String stMinWorldSize           = elMap->Attribute("min_world_size");
	String stMaxWorldSize           = elMap->Attribute("max_world_size");
	String stAmbientLight           = elMap->Attribute("ambient_light");
	
	if (stPlayerStart != "") {
		_playerStart = StringConverter::parseVector3(stPlayerStart);
	} else {
		_playerStart = Vector3::ZERO;
	}

	if (stPlayerStartOrientation != "") {
		_playerStartOrientation = StringConverter::parseQuaternion(stPlayerStartOrientation);
	} else {
		_playerStartOrientation = Quaternion(1,0,0,0);
	}

	_skybox       = stSkyBox;
	_minWorldSize = StringConverter::parseVector3(stMinWorldSize);
	_maxWorldSize = StringConverter::parseVector3(stMaxWorldSize);

	//cria a iluminação geral do mapa
	Vector3 ambientLight = StringConverter::parseVector3(stAmbientLight);
	EngineGlobalObjects::getInstance().getSceneManager()->setAmbientLight(ColourValue(ambientLight.x, ambientLight.y, ambientLight.z));	
	
	//obtém elementos e cria
	TiXmlElement* elObject = elMap->FirstChildElement("object");

	while(elObject) {  
		
		//obtém o tipo do objeto
		String stType = elObject->Attribute("type");
		
		//verifica o tipo
		if (stType == "static_mesh") {
			String stMass         = elObject->Attribute("mass");
			String stInertia      = elObject->Attribute("inertia");
			String stCollision    = elObject->Attribute("collision");
			String stCastShadows  = elObject->Attribute("castshadows");
			String stMeshFile     = elObject->Attribute("meshfile");
			String stMaterial     = elObject->Attribute("material");
			String stName         = elObject->Attribute("name");
			String stPosition     = elObject->Attribute("position");
			String stOrientation  = elObject->Attribute("orientation");
			String stScale        = elObject->Attribute("scale");
			String stCenterOfMass = elObject->Attribute("center_of_mass");
			
			//prepara os atributos
			bool castShadow		   = StringConverter::parseBool(stCastShadows);
			String nodeName        = "map_object_" + stName;
			String entityName      = "map_object_entity_" + stName;
			Real mass			   = StringConverter::parseReal(stMass);
			Vector3 inertia	       = StringConverter::parseVector3(stInertia);
			Vector3 position	   = StringConverter::parseVector3(stPosition);
			Quaternion orientation = StringConverter::parseQuaternion(stOrientation);
			Vector3 scale          = StringConverter::parseVector3(stScale);
			Vector3 centerOfMass   = StringConverter::parseVector3(stCenterOfMass);

			//cria o node e entity
			Entity *entity;
			entity = EngineGlobalObjects::getInstance().getSceneManager()->createEntity(entityName, stMeshFile);
			
			if (stMaterial != "") {
				entity->setMaterialName(stMaterial);
			}

			entity->setCastShadows(castShadow);

			SceneNode *node = EngineGlobalObjects::getInstance().getSceneManager()->getRootSceneNode()->createChildSceneNode(nodeName);
			node->attachObject(entity);

			//define tamanho
			node->setScale(scale);

			//cria colisão
			if (stCollision == "fixed") {
				
				OgreNewt::CollisionPtr col = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::TreeCollision( EngineGlobalObjects::getInstance().getWorld(), entity, true, 0 ));
				OgreNewt::Body* bod = new OgreNewt::Body( EngineGlobalObjects::getInstance().getWorld(), col );			
				
				bod->attachNode( node );
				bod->setPositionOrientation( position, orientation );
				
				//define callback customizado
				bod->setCustomForceAndTorqueCallback<EngineMap>(&EngineMap::forceCallback, this);

			} else if (stCollision == "normal") {				
				
				OgreNewt::ConvexCollisionPtr col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(EngineGlobalObjects::getInstance().getWorld(), entity, 0));
				OgreNewt::Body* bod = new OgreNewt::Body( EngineGlobalObjects::getInstance().getWorld(), col );

				bod->attachNode( node );
				bod->setMassMatrix( mass, inertia );
				bod->setCenterOfMass(centerOfMass);
				bod->setStandardForceCallback();

				bod->setPositionOrientation( position, orientation );
				
				//define callback customizado
				bod->setCustomForceAndTorqueCallback<EngineMap>(&EngineMap::forceCallback, this);
			}

		} else if (stType == "light") {
			
			String stName         = elObject->Attribute("name");
			String stPosition     = elObject->Attribute("position");
			String stDirection    = elObject->Attribute("direction");
			String stAttenuation  = elObject->Attribute("attenuation");
			String stCastShadows  = elObject->Attribute("castshadows");
			String stDiffuse      = elObject->Attribute("diffuse");
			String stLightrange   = elObject->Attribute("lightrange");
			String stLighttype    = elObject->Attribute("lighttype");
			String stPower        = elObject->Attribute("power");
			String stSpecular     = elObject->Attribute("specular");

			Light *light;
			light = EngineGlobalObjects::getInstance().getSceneManager()->createLight(stName);
			
			if(stLighttype=="LT_POINT") light->setType(Light::LT_POINT);
			if(stLighttype=="LT_SPOTLIGHT") light->setType(Light::LT_SPOTLIGHT);
			if(stLighttype=="LT_DIRECTIONAL") light->setType(Light::LT_DIRECTIONAL);

			light->setPosition(StringConverter::parseVector3(stPosition));
			light->setDiffuseColour(StringConverter::parseColourValue(stDiffuse));
			light->setSpecularColour(StringConverter::parseColourValue(stSpecular));
			
			Vector4 att = StringConverter::parseVector4(stAttenuation);
			light->setAttenuation(att.x, att.y, att.z, att.w);
			light->setDirection(StringConverter::parseVector3(stDirection));
			light->setPowerScale(StringConverter::parseReal(stPower));
			light->setSpotlightFalloff(1);

			light->setCastShadows(StringConverter::parseBool(stCastShadows));

		}

		elObject = elObject->NextSiblingElement("object");
	}

	//obtém elementos da fog e cria
	TiXmlElement* elFog = elMap->FirstChildElement("fog");

	while(elFog) {  
		String stFogMode    = elFog->Attribute("mode");
		String stFogColour  = elFog->Attribute("colour");
		String stFogStart   = elFog->Attribute("start");
		String stFogEnd     = elFog->Attribute("end");
		String stFogDensity = elFog->Attribute("density");
		
		_fogMode    = stFogMode;
		_fogColour  = StringConverter::parseColourValue(stFogColour);
		_fogStart   = StringConverter::parseReal(stFogStart);
		_fogEnd     = StringConverter::parseReal(stFogEnd);
		_fogDensity = StringConverter::parseReal(stFogDensity);
		
		//configura a fog no cenário se o modo da fog foi informado
		if (stFogMode != "") {
			FogMode fogMode;

			if (_fogMode == "none") fogMode = FogMode::FOG_NONE;
			if (_fogMode == "exp") fogMode = FogMode::FOG_EXP;
			if (_fogMode == "exp2") fogMode = FogMode::FOG_EXP2;
			if (_fogMode == "linear") fogMode = FogMode::FOG_LINEAR;

			EngineGlobalObjects::getInstance().getSceneManager()->setFog(fogMode, _fogColour, _fogDensity, _fogStart, _fogEnd); 
		}


		elFog = elFog->NextSiblingElement("fog");
	}



	//cria o céu
	createSkyBox();	

	doc.Clear();
}


void EngineMap::createSkyBox() {
	if (_skybox != "") {
		EngineGlobalObjects::getInstance().getSceneManager()->setSkyBox(true, _skybox);
	}
}

void EngineMap::forceCallback(OgreNewt::Body *body, float timeStep, int threadIndex) {
	//obtém massa e inércia
	Ogre::Real mass;
    Ogre::Vector3 inertia;
	body->getMassMatrix(mass, inertia);
	
	//adiciona gravidade
	Ogre::Vector3 gravity = EngineGlobalObjects::getInstance().getConfiguration()->getGravity() * mass;
    body->addForce(gravity);
}