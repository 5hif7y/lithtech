
#ifndef __CREATE_WORLD_TREE_H__
#define __CREATE_WORLD_TREE_H__


	#include "FindWorldModel.h"
	#include "PreWorld.h"

	// Creates the WorldTree based on the spatial layout of the polies.
	bool CreateWorldTree(WorldTree *pWorldTree, 
		CMoArray<CWorldModelDef*> &worldModels, char *pInfoString);


#endif



