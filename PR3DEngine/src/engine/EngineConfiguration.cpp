#include "EngineConfiguration.h"

EngineConfiguration::EngineConfiguration() {
}

EngineConfiguration::~EngineConfiguration() {

}

void EngineConfiguration::load() {
	//carrega atributos do xml
	String fileName = "engine.xml";
	
	TiXmlDocument doc(fileName.c_str());
	if (!doc.LoadFile()) return;

	TiXmlHandle handleDoc(&doc);
			
	//obtém elemento com as configurações da engine
	TiXmlElement* elEngine = handleDoc.FirstChild("engine").Element();
	if (!elEngine) return;

	//obtém atributos
	String stTitle            = elEngine->Attribute("title");
	String stConfigScreen     = elEngine->Attribute("config_screen");
	String stResourcePath     = elEngine->Attribute("resource_path");
	String stGravity          = elEngine->Attribute("gravity");
	String stShowBoudingBox   = elEngine->Attribute("show_bounding_box");
	
	_title               = stTitle;
	_resourcePath        = stResourcePath;
	_configurationScreen = StringConverter::parseBool(stConfigScreen);
	_gravity             = StringConverter::parseVector3(stGravity);
	_showBoundingBox     = StringConverter::parseBool(stShowBoudingBox);
	
	//obtém atributos do mapa
	TiXmlElement* elMapa = elEngine->FirstChildElement("map");
	
	if (elMapa) {
		String stMapName = elMapa->Attribute("name");
		_mapName = stMapName;
	}

	//obtém atributos do player
	TiXmlElement* elPlayer = elEngine->FirstChildElement("player");
	
	if (elPlayer) {
		String stPlayerName = elPlayer->Attribute("name");
		_playerName = stPlayerName;
	}

	//obtém atributos da câmera
	TiXmlElement* elCamera = elEngine->FirstChildElement("camera");
	
	if (elCamera) {
		String stNearClipDistance = elCamera->Attribute("near_clip_distance");
		String stFarClipDistance  = elCamera->Attribute("far_clip_distance");
		String stCameraZoom       = elCamera->Attribute("zoom");
		String stCameraModifier   = elCamera->Attribute("modifier");
		
		_nearClipDistance    = StringConverter::parseReal(stNearClipDistance);
		_farClipDistance     = StringConverter::parseReal(stFarClipDistance);
		_cameraZoom          = StringConverter::parseReal(stCameraZoom);
		_cameraModifier      = StringConverter::parseVector3(stCameraModifier);
	}

	//obtém atributos da viewport
	TiXmlElement* elViewPort = elEngine->FirstChildElement("viewport");
	
	if (elViewPort) {
		String stViewportShadown  = elViewPort->Attribute("shadown");
		String stViewportSkies    = elViewPort->Attribute("skies");
		String stViewportOverlays = elViewPort->Attribute("overlays");
		
		_viewportShadown  = StringConverter::parseBool(stViewportShadown);
		_viewportSkies    = StringConverter::parseBool(stViewportSkies);
		_viewportOverlays = StringConverter::parseBool(stViewportOverlays);
	}

	doc.Clear();
}
