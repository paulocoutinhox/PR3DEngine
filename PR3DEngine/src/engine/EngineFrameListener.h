/*
-----------------------------------------------------------------------------
Arquivo:     EngineFrameListener.h
Descricao:   Classe que recebe e controla os eventos
-----------------------------------------------------------------------------
*/

#ifndef ENGINE_FRAME_LISTENER_H_
#define ENGINE_FRAME_LISTENER_H_

/*
Inclui classes da Ogre
*/
#include "Ogre.h"
#include <OIS/OIS.h>
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
#include "EnginePlayer.h"

class EngineFrameListener : public FrameListener , public WindowEventListener {

	public:
		
		//construtor e destrutor
		EngineFrameListener(bool bufferedKeys, bool bufferedMouse, bool bufferedJoy);
		virtual ~EngineFrameListener();
		
		//métodos
		virtual void showDebugOverlay(bool show);
		bool frameStarted(const FrameEvent& evt);
		bool frameEnded(const FrameEvent& evt);

		//getters e setters
		void setPlayer(EnginePlayer *player) { _player = player; }
		
	private:

		//atributos
		bool _statsOn;		
		Overlay *_debugOverlay;
		std::string _debugText;

		OIS::InputManager *_inputManager;
		OIS::Mouse *_mouse;
		OIS::Keyboard *_keyboard;
		OIS::JoyStick *_joystick;

		EnginePlayer *_player;

		NxOgre::TimeController* _timeController;
		
		//métodos
		void EngineFrameListener::updateStats();
};

#endif
