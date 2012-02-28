
#include "Common.h"

TCHAR* Extension::NameFromFixed(int fixed)
{
	LPRO object = Runtime.LPROFromFixed(fixed);
	if(object == NULL)
		return Runtime.CopyString(TEXT(""));
	return Runtime.CopyString(object->roHo.hoOiList->oilName);
}