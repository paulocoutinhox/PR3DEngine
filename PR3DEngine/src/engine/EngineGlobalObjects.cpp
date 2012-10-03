#include "EngineGlobalObjects.h"

EngineGlobalObjects* EngineGlobalObjects::_instance = NULL;

EngineGlobalObjects& EngineGlobalObjects::getInstance() {
	if (_instance == NULL) {
		_instance = new EngineGlobalObjects();
	}

	return *_instance;
}

EngineGlobalObjects::EngineGlobalObjects() {
	_weapons = std::vector<EngineWeapon*>();
}

EngineGlobalObjects::~EngineGlobalObjects() {

}
