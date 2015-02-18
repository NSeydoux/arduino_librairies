#include "rider_manager.h"

using namespace std;


RiderManager::RiderManager(int chipSelect)
{
	_isReady = false;
	_numberOfRiders = 0;
	this->_chipSelect = chipSelect;
	for(int i=0; i<MAX_RIDER; i++)
	{
		this->_numberOfRecords[i] = 0;
	}
}

int RiderManager::folderNameToId(String folderName)
{
	return atoi(folderName.c_str());
}

String RiderManager::riderIdToName(int id)
{
	return String(id);
}

bool RiderManager::init()
{
	pinMode(this->_chipSelect, OUTPUT);
	this->_isReady = true;
	// On suppose qu'il y a autant de cavaliers déjà enregistrés
	// que de dossiers présents sur la carte
	this->_numberOfRiders = this->countDirectories();
	for(int i=0; i<MAX_RIDER; i++)
	{
		this->_numberOfRecords[i] = 0;
	}
	
	if (!SD.begin(this->_chipSelect)) 
	{
		this->_isReady = false;
		Serial.println("Card failed, or not present");
	}
	else
	{
		Serial.println("RiderManager construction successful");
	}
	return this->_isReady;
}

int RiderManager::countDirectories()
{
	int dirCount = 0;
	File root = SD.open("/");
	File f = root.openNextFile();
	while(f)
	{
		if(f.isDirectory() && ((String)f.name()).length() == FOLDER_NAME_SIZE)
		{
			Serial.println(f.name());
			dirCount++;
		}
		f = root.openNextFile();
	}
	return dirCount;
}

File RiderManager::findRiderFolder(int rider)
{
	return SD.open(riderIdToName(rider).c_str());
}

int RiderManager::countRecords(int rider)
{
	int recordCount = 0;
	File root = findRiderFolder(rider);
	File f = root.openNextFile();
	while(f)
	{
		if(((String)f.name()).length() == RECORD_NAME_SIZE)
		{
			Serial.println(f.name());
			recordCount++;
		}
		f = root.openNextFile();
	}
	return recordCount;
}

/*void RiderManager::addRider()
{
	
}*/
