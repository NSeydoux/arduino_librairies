/**
 * Bibliothèque rider_manager, qui sert à créer des fichiers et des dossiers
 * de manière transparente et facile pour le projet Equi-Librium. Les noms de
 * dossiers seront nommés par un chiffre
 * 
 */

#ifndef RIDER_MNGR_H
#define RIDER_MNGR_H

#define MAX_RIDER 32
#define FOLDER_NAME_SIZE 1
#define RECORD_NAME_SIZE 1


#include "Arduino.h"
#include <SD.h>

class RiderManager
{
	public:
		// Connaître la pin de chipSelect permet de s'assurer qu'elle soit 
		// correctement configurée.
		RiderManager(int chipSelect);
		// Repère les dossiers déjà créés, vérifie la présence de la carte SD...
		bool init();
		//void addRider();
		//void addRecordToRider(int rider);
		bool isReady(){return this->_isReady;};
		int countDirectories();
		// Compte le nombre d'éléments dans le dossier spécifié
		int countRecords(int rider);
		File findRiderFolder(int rider);
	private:
		int folderNameToId(String folderName);
		String riderIdToName(int riderId);
		int _chipSelect;
		// Nombre de cavaliers connus par l'application
		int _numberOfRiders;
		// Nombre de fichiers dans chaque dossier
		int _numberOfRecords[MAX_RIDER] ;
		bool _isReady;
};

#endif
