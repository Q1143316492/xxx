#pragma once

#include "UObject/Class.h"

class UEPreExportUEnum
{
public:
	UEPreExportUEnum(UEnum *Enum);

	void AppendUEnumRegisterFunction(FString &GeneratedFileContent, TArray<FString> & FuncNameList);

private:
	UEnum *m_enum;
};