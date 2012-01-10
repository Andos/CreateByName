
#include "Common.h"

void Extension::CreateAtPos(char* objName, eventParam* evtParam, int layer)
{
	//Iterate OI list
	objInfoList* list = rhPtr->rhOiList;
	int num = rhPtr->rhNumberOi-1;
	short creationOI = -1;
	for(int i=0; i<num; i++)
	{
		objInfoList info = *(objInfoList*)(((char*)list) + i*oiListItemSize);
		if (! strcmp((char*)&info.oilName, objName))
			creationOI = info.oilOi;
	}
	//No object was found
	if(creationOI == -1)
		return;

	//Create the event buffer (with plenty space):
	int bufferSize = sizeof(event)+sizeof(eventParam)+sizeof(CreateDuplicateParam);
	char* buffer = new char[bufferSize];
	memset(buffer, 0, bufferSize);

	//Put the layer number in to a proper range
	if(layer >= rhPtr->rhFrame->m_nLayers)
		layer = rhPtr->rhFrame->m_nLayers-1;
	if(layer < -1)
		layer = -1;

	//The event that should be passed to the CreateObject routine
	event* evt = (event*)&buffer[0];
	evt->evtCode = MAKELONG(0,0);

	//Resides at event+14
	eventParam* creationParams = (eventParam*)((char*)&buffer[0]+ACT_SIZE);

	//The object creation parameters
	CreateDuplicateParam* cdp = (CreateDuplicateParam*)&creationParams->evp.evpW.evpW0;
	cdp->cdpHFII = rhPtr->rhNumberOi;
	cdp->cdpOi = creationOI;
	cdp->cdpPos = *(PositionParam*)&evtParam->evp;
	cdp->cdpPos.posLayer = layer;
		
	//Call the routine
	CallTables* tables = Runtime.GetCallTables();
	CALLACTION_ROUTINE createObject = tables->pActions[2];
	createObject(evt);

	mvFree(SDK->mV, tables);
	free(buffer);
}

void Extension::CreateAtPosXY(char* objName, int x, int y, int layer)
{
	//Iterate OI list
	objInfoList* list = rhPtr->rhOiList;
	int num = rhPtr->rhNumberOi-1;
	short creationOI = -1;
	for(int i=0; i<num; i++)
	{
		objInfoList info = list[i];
		if (! strcmp((char*)&info.oilName, objName))
			creationOI = info.oilOi;
	}
	//No object was found
	if(creationOI == -1)
		return;

	//Create the event buffer (with plenty space):
	int bufferSize = sizeof(event)+sizeof(eventParam)+sizeof(CreateDuplicateParam);
	char* buffer = new char[bufferSize];
	memset(buffer, 0, bufferSize);

	//Put the layer number in to a proper range
	if(layer >= rhPtr->rhFrame->m_nLayers)
		layer = rhPtr->rhFrame->m_nLayers-1;
	if(layer < -1)
		layer = -1;

	//The event that should be passed to the CreateObject routine
	event* evt = (event*)&buffer[0];
	evt->evtCode = MAKELONG(0,0);

	//Resides at event+14
	eventParam* creationParams = (eventParam*)((char*)&buffer[0]+ACT_SIZE);

	//The object creation parameters
	CreateDuplicateParam* cdp = (CreateDuplicateParam*)&creationParams->evp.evpW.evpW0;
	cdp->cdpHFII = rhPtr->rhNumberOi;
	cdp->cdpOi = creationOI;
	cdp->cdpPos.posX = x;
	cdp->cdpPos.posY = y;
	cdp->cdpPos.posLayer = layer;
	cdp->cdpPos.posOINUMParent = -1;
	cdp->cdpPos.posFlags = 8;
	
	//Call the routine
	CallTables* tables = Runtime.GetCallTables();
	CALLACTION_ROUTINE createObject = tables->pActions[2];
	createObject(evt);
	
	mvFree(SDK->mV, tables);
	free(buffer);
}

void Extension::CreateBackdropAtPos(char* objName, long position, int type, int layer)
{
	CreateBackdropAtPosXY(objName, HIWORD(position), LOWORD(position), type, layer);
}

void Extension::CreateBackdropAtPosXY(char* objName, int x, int y, int type, int layer)
{
	LPOI* ois = rhPtr->rhApp->m_ois;
	LPOBL oblPtr = (LPOBL)rhPtr->rhObjectList;

	int additionalLayerSize = isHWA ? 8 : 0;

	CRunFrame* frame = (CRunFrame*)rhPtr->rhFrame;
	RunFrameLayer* layerPtr = frame->m_pLayers;

	// Find backdrop
	for(int i=0; i<frame->m_nLayers; ++i)
	{
		LPLO backdropPtr = (LPLO)(frame->m_los + layerPtr->nFirstLOIndex);
		for( int j = 0; j<(int)layerPtr->nBkdLOs; ++j )
		{
			LPOI objOI = rhPtr->rhApp->m_ois[rhPtr->rhApp->m_oi_handle_to_index[backdropPtr->loOiHandle]];
			if( strcmp(objOI->oiName,objName) == 0 )
			{
				LPBackdrop_OC test = (LPBackdrop_OC)objOI->oiOC;

				cSurface imageSurface;
				LockImageSurface (rhPtr->rhIdAppli, test->ocImage, imageSurface, LOCKIMAGE_READBLITONLY);

				rhPtr->rh4.rh4Mv->mvAddBackdrop(
					&imageSurface,
					x,
					y,
					objOI->oiHdr.oiInkEffect,
					objOI->oiHdr.oiInkEffectParam,
					type,
					layer
				);
				UnlockImageSurface(imageSurface);
				return;
			}
			backdropPtr++;
		}
		layerPtr = (RunFrameLayer*)(((char*)layerPtr++)+additionalLayerSize);
	}
}