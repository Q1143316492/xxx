#pragma once

#include "Containers/UnrealString.h"
#include "ExportedFunction.h"

#include "Templates/Casts.h"

#include "UObject/UObjectHash.h"
#include "UObject/UnrealType.h"
#include "UObject/Class.h"

#include "UEPreExportUClass.h"
#include "UEPreExportUEnum.h"

class UEPreExportObject
{
public:
	UEPreExportObject();
	~UEPreExportObject() = default;

	void ExportUClass();

	void ExportUEnum();
private:

	static const FString UClassNeedExport[];
	static const FString UEnumNeedExport[];

	static const FString ExportPath;

	TSet<FString> ExportUClassSet;
	TSet<FString> ExportUEnumSet;
};
