#include "Engine.h"

Engine::Engine() {
	//inicializa objeto de configuração da engine
	_configuration = new EngineConfiguration();
	_configuration->load();
	EngineGlobalObjects::getInstance().setConfiguration(_configuration);

	//inicializa atributos
	_frameListener = NULL;
    _root = NULL;
	
	//inicializa jogadores
	_players = vector<EnginePlayer*>();

	//inicializa as armas
	_weapons = vector<EngineWeapon*>();
	
	//inicializa mundo físico
	_world = new NxOgre::World::createWorld();
	EngineGlobalObjects::getInstance().setWorld(_world);
}

Engine::~Engine() {	
	if (_world)
		delete _world;
	
	if (_frameListener)
		delete _frameListener;
    
	if (_root)
		OGRE_DELETE _root;
}

void Engine::initialize() {
    //configura
	if (!setup()) {
        return;
	}
	
	//renderiza tudo
    _root->startRendering();

    //faz a limpeza
    destroyScene();
}

bool Engine::setup() {
	//carrega plugins do arquivo de plugins(plugins.cfg)
	String pluginsPath;

	pluginsPath = _configuration->getResourcePath() + "plugins.cfg";
	
    _root = OGRE_NEW Root(pluginsPath, _configuration->getResourcePath() + "ogre.cfg", _configuration->getResourcePath() + "ogre.log");
	EngineGlobalObjects::getInstance().setRoot(_root);

    setupResources();

    bool configured = configure();
	if (!configured) return false;

	chooseSceneManager();
	createCamera();
	createViewports();

	// Set default mip_map level (NB some APIs ignore this)
	TextureManager::getSingleton().setDefaultNumMipmaps(5);

	//criamos um resource listeners (para tela de login)
	createResourceListener();

	//carregamos os recursos
	loadResources();

	//criamos o cenário
	createScene();
	
	//criamos os jogadores
	createPlayers();

	//criamos o mapa
	createMap();

	//criamos as armas
	createWeapons();

	//criamos o frame listener
	createFrameListener();
	
	return true;
}

bool Engine::configure() {
	if (_configuration->getConfigurationScreen() == true) {
				
		if(_root->showConfigDialog() == true) {
			//se retornou TRUE é porque o usuário selecionou OK, então inicializamos
			_window = _root->initialise(true, _configuration->getTitle());
			EngineGlobalObjects::getInstance().setRenderWindow(_window);
			return true;
		} else {
			return false;
		}

	} else {

		_root->restoreConfig();
		_window = _root->initialise(true, _configuration->getTitle());
		EngineGlobalObjects::getInstance().setRenderWindow(_window);
		return true;

	}
}

void Engine::chooseSceneManager() {
	_sceneManager = _root->createSceneManager(ST_EXTERIOR_CLOSE);
	EngineGlobalObjects::getInstance().setSceneManager(_sceneManager);

	//verifica se é para mostrar o bounding box
	EngineGlobalObjects::getInstance().getSceneManager()->showBoundingBoxes( EngineGlobalObjects::getInstance().getConfiguration()->getShowBoundingBox() );
}

void Engine::createCamera() {
	//cria a camera
	_camera = _sceneManager->createCamera("MainCamera");

	//posiciona a camera
	_camera->setPosition(Vector3(0, 0, 0));

	//muda o alvo da câmera	
	_camera->setNearClipDistance(_configuration->getNearClipDistance());
	_camera->setFarClipDistance(_configuration->getFarClipDistance());

	EngineGlobalObjects::getInstance().setCamera(_camera);	
}

void Engine::createFrameListener() {
	//cria o frame listener
	_frameListener= new EngineFrameListener(false, false, false);
	_frameListener->showDebugOverlay(true);
	_root->addFrameListener(_frameListener);	

	_frameListener->setPlayer(_players[0]);
	
	//cria listener da física
	_newtonListener = new OgreNewt::BasicFrameListener( _window, _world, 60 );
	_root->addFrameListener(_newtonListener);
	
	//cria câmera de interação
	_engineTPCamera = new EngineTPCamera(_players[0]->getNode());	
}

void Engine::createViewports() {
	//criamos uma viewport, ocupando toda a janela
	Viewport* vp = _window->addViewport(_camera);
	vp->setBackgroundColour(ColourValue(0,0,0));
	
	//define alguns atributos da viewport
	vp->setShadowsEnabled( EngineGlobalObjects::getInstance().getConfiguration()->getViewportShadown() );
	vp->setSkiesEnabled( EngineGlobalObjects::getInstance().getConfiguration()->getViewportSkies() );
	vp->setOverlaysEnabled( EngineGlobalObjects::getInstance().getConfiguration()->getViewportOverlays() );

	//alteramos o aspect ratio da camera para encaixar no viewport
	_camera->setAspectRatio( Real(vp->getActualWidth()) / Real(vp->getActualHeight()) );
}

void Engine::setupResources() {
	//carrega os recursos do arquivo de recursos(resources.cfg)
    ConfigFile cf;
    cf.load(_configuration->getResourcePath() + "resources.cfg");

    //agora jogamos pro ResourceGroupManager todos os recursos do arquivo
    ConfigFile::SectionIterator seci = cf.getSectionIterator();

    String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        ConfigFile::SettingsMultiMap *settings = seci.getNext();
        ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }
}

void Engine::createResourceListener() {
	//exibir tela de loading
}

void Engine::loadResources() {
	//inicializa, faz parse de scripts, etc
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	//inicia barra de progresso
	_barraProgresso.start(_window, 1, 1, 0.75);

	//para de renderizar tudo, menos a camada superior
	_sceneManager->clearSpecialCaseRenderQueues();
	_sceneManager->addSpecialCaseRenderQueue(RENDER_QUEUE_OVERLAY);
	_sceneManager->setSpecialCaseRenderQueueMode(SceneManager::SCRQM_INCLUDE);

	//inicializa o restante dos grupos de recursos, parse scripts, etc
	ResourceGroupManager::getSingleton().loadResourceGroup( ResourceGroupManager::getSingleton().getWorldResourceGroupName(), false, true);

	//volta a renderizar tudo
	_sceneManager->clearSpecialCaseRenderQueues();
	_sceneManager->setSpecialCaseRenderQueueMode(SceneManager::SCRQM_EXCLUDE);

	//finaliza barra de progresso
	_barraProgresso.finish();

}

void Engine::createScene() {
	//cria cenario da física
	NxOgre::SceneDescription sceneDesc;
	sceneDesc.mGravity = NxOgre::Vec3(0, -9.8f, 0);
	sceneDesc.mName = "ScenePhysics";

	_scene = _world->createScene(sceneDesc);

	_scene->getMaterial(0)->setStaticFriction(0.5);
	_scene->getMaterial(0)->setDynamicFriction(0.5);
	_scene->getMaterial(0)->setRestitution(0.1);
}

void Engine::destroyScene() {
	//destroi cenario
}

void Engine::createPlayers() {
	//cria os jogadores	
	_players.push_back( new EnginePlayer(_configuration->getPlayerName()) );
	_players[0]->create();
}

void Engine::createMap() {
	//inicializa _mapa
	_map = new EngineMap(_configuration->getMapName());	
	
	//cria o _mapa
	_map->create();

	//define tamanho do mundo físico
	_world->setWorldSize(_map->getMinWorldSize(), _map->getMaxWorldSize());

	//define posição do jogador
	_players[0]->getNode()->setPosition(_map->getPlayerStart());

	//define orientação do jogador
	_players[0]->getNode()->setOrientation(_map->getPlayerStartOrientation());
}

void Engine::createLighting() {
	
}

void Engine::createWeapons() {
	//carrega atributos do xml
	String fileName = "media/weapons/weapons.xml";
	
	TiXmlDocument doc(fileName.c_str());
	if (!doc.LoadFile()) return;

	TiXmlHandle handleDoc(&doc);
	
	//obtém elemento com as configurações do mapa
	TiXmlElement* elWeapons = handleDoc.FirstChild("weapons").Element();
	if (!elWeapons) return;

	//obtém elementos e cria
	TiXmlElement* elWeapon = elWeapons->FirstChildElement("weapon");

	while(elWeapon) {  
		
		String stMass			    = elWeapon->Attribute("mass");
		String stInertia		    = elWeapon->Attribute("inertia");
		String stCollision		    = elWeapon->Attribute("collision");
		String stCastShadows	    = elWeapon->Attribute("castshadows");
		String stMeshFile		    = elWeapon->Attribute("meshfile");
		String stName			    = elWeapon->Attribute("name");
		String stMaterial		    = elWeapon->Attribute("material");
		String stScale			    = elWeapon->Attribute("scale");
		String stCenterOfMass	    = elWeapon->Attribute("center_of_mass");
		String stVelocity		    = elWeapon->Attribute("velocity");
		String stDistanceFromPlayer = elWeapon->Attribute("distance_from_player");
		String stMoveDirection      = elWeapon->Attribute("move_direction");
		String stShootInterval      = elWeapon->Attribute("shoot_interval");
		
		//prepara os atributos
		bool castShadow		       = StringConverter::parseBool(stCastShadows);
		String entityName          = "weapon_object_entity_" + stName;
		Real mass				   = StringConverter::parseReal(stMass);
		Vector3 inertia	           = StringConverter::parseVector3(stInertia);
		Vector3 scale              = StringConverter::parseVector3(stScale);
		Vector3 centerOfMass	   = StringConverter::parseVector3(stCenterOfMass);
		Vector3 distanceFromPlayer = StringConverter::parseVector3(stDistanceFromPlayer);
		Vector3 moveDirection      = StringConverter::parseVector3(stMoveDirection);
		Real velocity		       = StringConverter::parseReal(stVelocity);
		Real shootInterval         = StringConverter::parseReal(stShootInterval);

		//cria o node e entity
		Entity *entity;
		entity = EngineGlobalObjects::getInstance().getSceneManager()->createEntity(entityName, stMeshFile);
		
		if (stMaterial != "") {
			entity->setMaterialName(stMaterial);
		}

		entity->setCastShadows(castShadow);
		
		//cria objeto e define atributos
		_weapons.push_back( new EngineWeapon() );
		
		int lastWeapon = _weapons.size()-1;
		
		_weapons[lastWeapon]->setCastShadow(castShadow);
		_weapons[lastWeapon]->setEntity(entity);
		_weapons[lastWeapon]->setMaterial(stMaterial);
		_weapons[lastWeapon]->setModel(stMeshFile);
		_weapons[lastWeapon]->setName(stName);
		_weapons[lastWeapon]->setScale(scale);
		_weapons[lastWeapon]->setVelocity(velocity);
		_weapons[lastWeapon]->setMass(mass);
		_weapons[lastWeapon]->setInertia(inertia);
		_weapons[lastWeapon]->setCenterOfMass(centerOfMass);
		_weapons[lastWeapon]->setDistanceFromPlayer(distanceFromPlayer);
		_weapons[lastWeapon]->setMoveDirection(moveDirection);
		_weapons[lastWeapon]->setShootInterval(shootInterval);

		elWeapon = elWeapon->NextSiblingElement("weapon");
	}

	doc.Clear();

	EngineGlobalObjects::getInstance().setWeapons(_weapons);
}