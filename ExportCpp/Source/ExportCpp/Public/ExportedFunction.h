#pragma once

#include "Containers/Array.h"
#include "UObject/UnrealType.h"

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
	ExportedFunction(UFunction *Func, UObject* obj = nullptr);
	~ExportedFunction() = default;
private:
	// todo
	//FString ExportDefine();

	TArray<UProperty*> params;
	UProperty* ReturnValue;
	UFunction* Func;
	EFunctionType FuncType;
	UObject* BelongUObject;
};