#pragma once

#include "Containers/Array.h"
#include "UObject/UnrealType.h"

//#include "Wrappers/UEPyFVector.h"
//#include "Wrappers/UEPyFVector2D.h"
//#include "Wrappers/UEPyFHitResult.h"
//#include "Wrappers/UEPyFRotator.h"
//#include "Wrappers/UEPyFTransform.h"
//#include "Wrappers/UEPyFColor.h"
//#include "Wrappers/UEPyFLinearColor.h"

static enum EFunctionType
{
	UNKNOW_TYPE,
	GLOBAL_FUNCTION,
	CLASS_STATIC_FUNCTION,
	CLASS_MEMBER_FUNCTION
};

class ExportedFunction
{
public:
	ExportedFunction(UFunction *Func, UObject* BelongObj = nullptr);
	~ExportedFunction() = default;

	FString ParseParams();
	FString ExportUeCppFunctionHeader();
	FString GetFunctionName();

private:
	FString ParseUProperty(UProperty* Property, TArray<FString> &ParamFormatList, TArray<FString> &ParamNameList);

	TArray<UProperty*> params;
	UProperty* ReturnValue;
	UFunction* Func;
	EFunctionType FuncType;
	UObject* BelongUObject;
};