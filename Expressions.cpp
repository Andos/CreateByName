
#include "Common.h"

char* Extension::NameFromFixed(int fixed)
{
	LPRO object = Runtime.LPROFromFixed(fixed);
	if(object == NULL)
		return Runtime.CopyString("");
	return Runtime.CopyString((char*)&object->roHo.hoOiList->oilName);
}