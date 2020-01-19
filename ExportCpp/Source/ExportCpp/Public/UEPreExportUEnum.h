#pragma once

#include "UObject/Class.h"

class UEPreExportUEnum
{
public:
	UEPreExportUEnum(UEnum *Enum);

	void AppendUEnumContain(FString &GeneratedFileContent);

private:
	UEnum *m_enum;
};