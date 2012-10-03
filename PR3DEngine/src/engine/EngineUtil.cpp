#include "EngineUtil.h"

EngineUtil::EngineUtil() {

}

EngineUtil::~EngineUtil() {

}

Vector3 EngineUtil::stringToVector3(String data, bool removeFirstOnOverload) {
	int posX = 0;
	int posY = 0;
	int posZ = 0;

	float partX = 0;
	float partY = 0;
	float partZ = 0;

	String sPartX = "";
	String sPartY = "";
	String sPartZ = "";
	
	//verify if it have 4 values and remove first
	if (removeFirstOnOverload == true) {
		if ( EngineUtil::stringCount(data, " ") == 3 ) {
			posX   = data.find(" ");
			sPartX = data.substr(0, posX); 
			data   = data.erase(0, posX+1);

			posX   = 0;
			partX  = 0;
			sPartX = "";
		}
	}
	
	//separate the values and mount the vector
	posX   = data.find(" ");
	sPartX = data.substr(0, posX); 
	data   = data.erase(0, posX+1);

	posY   = data.find(" ");
	sPartY = data.substr(0, posY); 
	data   = data.erase(0, posY+1);

	sPartZ = data;
	
	partX  = atof(sPartX.c_str());
	partY  = atof(sPartY.c_str());
	partZ  = atof(sPartZ.c_str());

	return Vector3(partX, partY, partZ);
}

int EngineUtil::stringCount(const String data, const String find) {
	int x = 0;

	if (data.length() > 0) {
		int index = data.find(find.c_str());

		while(index != -1) {
			if(index = data.find(find.c_str(), index+1)) {
				x += 1;
			}

		}
	}

	return x;
}