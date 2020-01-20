#pragma once

#include "UObject/Class.h"

class UEPreExportUScriptStruct
{
public:
	UEPreExportUScriptStruct(UScriptStruct *Struct);

private:
	UScriptStruct* m_struct;
};