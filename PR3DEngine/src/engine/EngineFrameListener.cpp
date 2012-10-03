#include "EngineFrameListener.h"

EngineFrameListener::EngineFrameListener(bool bufferedKeys = false, bool bufferedMouse = false, bool bufferedJoy = false) {
	//inicializa os atributos
	_statsOn      = false;
	_debugOverlay = 0;

	_inputManager = 0;
	_mouse        = 0;
	_keyboard     = 0;
	_joystick     = 0;
	
	//camada de debug
	_debugOverlay = OverlayManager::getSingleton().getByName("Core/DebugOverlay");
	
	//inicializa OIS
	OIS::ParamList paramList;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	EngineGlobalObjects::getInstance().getRenderWindow()->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	paramList.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	_inputManager = OIS::InputManager::createInputSystem( paramList );
	
	//cria os dispositivos
	_keyboard = static_cast<OIS::Keyboard*>(_inputManager->createInputObject( OIS::OISKeyboard, bufferedKeys ));
	_mouse = static_cast<OIS::Mouse*>(_inputManager->createInputObject( OIS::OISMouse, bufferedMouse ));

	try {
		_joystick = static_cast<OIS::JoyStick*>(_inputManager->createInputObject( OIS::OISJoyStick, bufferedJoy ));
	}
	catch(...) {
		_joystick = 0;
	}

	//define o tamanho do mouse clipping
	windowResized(EngineGlobalObjects::getInstance().getRenderWindow());
	
	//exibe debug
	//showDebugOverlay(false);

	//registra como window listener
	WindowEventUtilities::addWindowEventListener(EngineGlobalObjects::getInstance().getRenderWindow(), this);
	
	//inicializa player
	_player = NULL;

	//inicializa o listenner do tempo da física
	_timeController = NxOgre::TimeController::getSingleton();
}

EngineFrameListener::~EngineFrameListener() {
	//Remove ourself as a Window listener
	WindowEventUtilities::removeWindowEventListener(EngineGlobalObjects::getInstance().getRenderWindow(), this);
	windowClosed(EngineGlobalObjects::getInstance().getRenderWindow());
}

void EngineFrameListener::updateStats() {
	/*
	static String currFps  = "Current FPS: ";
	static String avgFps   = "Average FPS: ";
	static String bestFps  = "Best FPS: ";
	static String worstFps = "Worst FPS: ";
	static String tris     = "Triangle Count: ";
	static String batches  = "Batch Count: ";

	// update stats when necessary
	try {
		OverlayElement* guiAvg = OverlayManager::getSingleton().getOverlayElement("Core/AverageFps");
		OverlayElement* guiCurr = OverlayManager::getSingleton().getOverlayElement("Core/CurrFps");
		OverlayElement* guiBest = OverlayManager::getSingleton().getOverlayElement("Core/BestFps");
		OverlayElement* guiWorst = OverlayManager::getSingleton().getOverlayElement("Core/WorstFps");

		const RenderTarget::FrameStats& stats = EngineGlobalObjects::getInstance().getRenderWindow()->getStatistics();
		guiAvg->setCaption(avgFps + StringConverter::toString(stats.avgFPS));
		guiCurr->setCaption(currFps + StringConverter::toString(stats.lastFPS));
		guiBest->setCaption(bestFps + StringConverter::toString(stats.bestFPS)
			+" "+StringConverter::toString(stats.bestFrameTime)+" ms");
		guiWorst->setCaption(worstFps + StringConverter::toString(stats.worstFPS)
			+" "+StringConverter::toString(stats.worstFrameTime)+" ms");

		OverlayElement* guiTris = OverlayManager::getSingleton().getOverlayElement("Core/NumTris");
		guiTris->setCaption(tris + StringConverter::toString(stats.triangleCount));

		OverlayElement* guiBatches = OverlayManager::getSingleton().getOverlayElement("Core/NumBatches");
		guiBatches->setCaption(batches + StringConverter::toString(stats.batchCount));

		OverlayElement* guiDbg = OverlayManager::getSingleton().getOverlayElement("Core/DebugText");
		guiDbg->setCaption(_debugText);
	}
	catch(...) { }
	*/
}

void EngineFrameListener::showDebugOverlay(bool show)
{
	/*
	if (_debugOverlay) {
		if (show)
			_debugOverlay->show();
		else
			_debugOverlay->hide();
	}
	*/
}

bool EngineFrameListener::frameEnded(const FrameEvent& evt)
{
	//updateStats();
	return true;
}

bool EngineFrameListener::frameStarted(const FrameEvent& evt) {
	//adiciona tempo do timer da física
	_timeController->advance(evt.timeSinceLastFrame);

	//captura de eventos
	_keyboard->capture();
	
	//realiza update no jogador
	if (_player) {
		_player->update(evt.timeSinceLastFrame, _keyboard);
	}

	//sai da aplicação
	if(_keyboard->isKeyDown (OIS::KC_ESCAPE)) {
		return false;
	}

	return true;
}
