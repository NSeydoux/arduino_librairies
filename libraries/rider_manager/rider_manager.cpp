#include "rider_manager.h"

RiderManager::RiderManager(int chipSelect)
{
	this->_chipSelect = chipSelect;
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
    if(f.isDirectory() && f.name().length() == 2)
    {
       dirCount++;
    }
    f = root.openNextFile();
  }
  // On retire 1 au compteur à cause du répertoire Trash par défaut
  return dirCount-1;
}

File RiderManager::findRiderFolder(int rider)
{
	File root = SD.open("/");
	File f;
	// +1 à cause du répertoire Trash en début de répertoire
	int decount = rider+1;
	while(decount > 0)
	{
		File tmp = root.openNextFile();
		Serial.println(tmp.name());
		if(tmp)
			f = tmp;
		if(f.isDirectory())
		{
				decount--;
		}
	}
	return f;
}

/*void RiderManager::addRider()
{
	
}*/
