#include "EngineTPCamera.h"

EngineTPCamera::EngineTPCamera(SceneNode *target) {
	//inicializa os atributos
	_target = target;
	_cameraZoom = EngineGlobalObjects::getInstance().getConfiguration()->getCameraZoom();

	EngineGlobalObjects::getInstance().getRoot()->addFrameListener(this);
	
	//inicializa OIS
	OIS::ParamList paramList;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	EngineGlobalObjects::getInstance().getRenderWindow()->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	paramList.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	_inputManager = OIS::InputManager::createInputSystem( paramList );

	//cria os dispositivos
	_keyboard = static_cast<OIS::Keyboard*>(_inputManager->createInputObject( OIS::OISKeyboard, true ));
	_mouse = static_cast<OIS::Mouse*>(_inputManager->createInputObject( OIS::OISMouse, true ));

	try {
		_joystick = static_cast<OIS::JoyStick*>(_inputManager->createInputObject( OIS::OISJoyStick, true ));
	}
	catch(...) {
		_joystick = 0;
	}

	if (_keyboard)
        _keyboard->setEventCallback(this);

    if (_mouse)
        _mouse->setEventCallback(this);

    if (_joystick)
        _joystick->setEventCallback(this);

	
}

EngineTPCamera::~EngineTPCamera() {
	
}

bool EngineTPCamera::mouseMoved(const OIS::MouseEvent &e) {
	Real rotateSpeed = 90;

	_rotX += e.state.Y.rel * -rotateSpeed;
	_rotY += e.state.X.rel * -rotateSpeed;
	
	return true;
}

bool EngineTPCamera::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
	if (id == OIS::MB_Left) {
		_cameraZoom += 10;
	}

	return true;
}

bool EngineTPCamera::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id) {
	return true;
}

bool EngineTPCamera::frameStarted(const FrameEvent& evt) {
	//captura de eventos
	_keyboard->capture();
	_mouse->capture();

	if (_joystick) {
		_joystick->capture();
	}

	//define posição da câmera
	EngineGlobalObjects::getInstance().getCamera()->setPosition(_target->getPosition() + ((_target->getOrientation() * Vector3::NEGATIVE_UNIT_Z)) * _cameraZoom + EngineGlobalObjects::getInstance().getConfiguration()->getCameraModifier());
	EngineGlobalObjects::getInstance().getCamera()->lookAt(_target->getPosition());

	if(_keyboard->isKeyDown(OIS::KC_O)) {
		_cameraZoom -= 10;
	} else if (_keyboard->isKeyDown(OIS::KC_P)) {
		_cameraZoom += 10;
	}

	return true;
}

bool EngineTPCamera::keyPressed(const OIS::KeyEvent &arg) {
	//altera zoom da câmera
	if (arg.key == OIS::KC_O) {
		_cameraZoom -= 10;
	} else if (arg.key == OIS::KC_P) {
		_cameraZoom += 10;
	}

	return true;
}

bool EngineTPCamera::keyReleased(const OIS::KeyEvent &arg) {
	return true;
}

bool EngineTPCamera::buttonPressed(const OIS::JoyStickEvent &arg, int button) {
	return true;
}

bool EngineTPCamera::buttonReleased(const OIS::JoyStickEvent &arg, int button) {
	return true;
}

bool EngineTPCamera::axisMoved(const OIS::JoyStickEvent &arg, int axis) {
	return true;
}