/*
-----------------------------------------------------------------------------
Arquivo:     EngineTPCamera.h
Descricao:   Classe que controla a câmera em modo terceira pessoa
-----------------------------------------------------------------------------
*/

#ifndef ENGINE_TP_CAMERA_H_
#define ENGINE_TP_CAMERA_H_

/*
Inclui classes da Ogre
*/
#include "Ogre.h"
#include <OIS/OIS.h>
using namespace Ogre;

/*
Inclui classes da Engine
*/
#include "EngineGlobalObjects.h"

class EngineTPCamera : public Ogre::FrameListener, public OIS::MouseListener, public OIS::KeyListener, public OIS::JoyStickListener {

	public:
		
		//métodos construtor e destrutor
		EngineTPCamera(SceneNode *target);
		virtual ~EngineTPCamera();
		
		//métodos
		bool frameStarted(const FrameEvent& evt);

		bool mouseMoved(const OIS::MouseEvent &e);
		bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
		bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);

		bool keyPressed(const OIS::KeyEvent &arg);
		bool keyReleased(const OIS::KeyEvent &arg);

		bool buttonPressed(const OIS::JoyStickEvent &arg, int button);
        bool buttonReleased(const OIS::JoyStickEvent &arg, int button);
        bool axisMoved(const OIS::JoyStickEvent &arg, int axis);


		//métodos getters e setters
		void setCamera(Camera *camera) { _camera = camera; }
		void setTarget(SceneNode *target) { _target = target; }
		
	protected:

		//atributos
		Camera *_camera;
		SceneNode *_target;
		Root *_root;
		
		Real _rotX;
		Real _rotY;

		Real _cameraZoom;

		OIS::InputManager* _inputManager;
		OIS::Mouse* _mouse;
		OIS::Keyboard* _keyboard;
		OIS::JoyStick* _joystick;

		
};

#endif
