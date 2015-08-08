#include "rider_manager.h"

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
	if (!SD.begin(this->_chipSelect)) 
	{
		this->_isReady = false;
		Serial.println("Card failed, or not present");
	}
	else
	{
		Serial.println("RiderManager construction successful");
	}
	delay(50);
	// On suppose qu'il y a autant de cavaliers déjà enregistrés
	// que de dossiers présents sur la carte
	//this->_numberOfRiders = this->countDirectories(root);
	for(int i=0; i<MAX_RIDER; i++)
	{
		// TODO :initialiser au vrai nombre d'enregistrements
		this->_numberOfRecords[i] = 0;//this->countRecords(i);
	}
	return this->_isReady;
}

bool RiderManager::init(String root)
{
	bool res = this->init();
	this->_numberOfRiders = this->countDirectories(SD.open(root.c_str()));
	return res;
}

bool RiderManager::init(int numberOfRiders)
{
	bool res = this->init();
	this->_numberOfRiders = numberOfRiders;
	return res;
}

int RiderManager::countDirectories(File root)
{
	int dirCount = 0;
	root.rewindDirectory();
	File f = root.openNextFile();
	while(f)
	{
		if(f.isDirectory() && (String(f.name()).length() == FOLDER_NAME_SIZE))
		{
			dirCount++;
		}
		f.close();
		f = root.openNextFile();
	}
	root.close();
	return dirCount;
}

File RiderManager::findRiderFolder(int rider)
{
	File f = SD.open(String(rider).c_str());
	return f;
}

String RiderManager::getRiderFolderName(int rider)
{
	return riderIdToName(rider).c_str();
}

int RiderManager::countRecords(int rider)
{
	int recordCount = 0;
	File root = SD.open(String(rider).c_str());
	if(root)
	{
		root.rewindDirectory();
		File f = root.openNextFile();
		while(f)
		{
			Serial.println("Found a record");
			recordCount++;
			f.close();
			f = root.openNextFile();
		}
		root.close();
		return recordCount;
	}
	else
	{
		Serial.println("Erreur à l'ouverture du dossier");
		return 0;
	}
}

int RiderManager::getNumberOfRiders()
{
	return this->_numberOfRiders;
}

void RiderManager::setNumberOfRiders(int n)
{
	this->_numberOfRiders = n;
}

File RiderManager::addRecord(int rider)
{
	Serial.println("Adding record to rider "+String(rider));
	this->_numberOfRecords[rider]++;
	String newFilePath = String(rider)+"/"+String(this->_numberOfRecords[rider])+".txt";
	Serial.print("New file path : ");
	Serial.println(newFilePath);
	return SD.open(newFilePath.c_str(), FILE_WRITE);
}

bool RiderManager::addRider()
{
	_numberOfRiders += 1;
	char fname[16];
	itoa(_numberOfRiders, fname, 10);
	//String fname = String(_numberOfRiders);
	if(SD.mkdir(fname))
	{
		Serial.println("New rider created");
		return true;
	}
	else
	{
		Serial.println("New rider creation failed");
		_numberOfRiders -= 1;
		return false;
	}
}
