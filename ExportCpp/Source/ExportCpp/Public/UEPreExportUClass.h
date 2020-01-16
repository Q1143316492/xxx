#pragma once

#include "UObject/UnrealType.h"
#include "ExportedFunction.h"
#include "UObject/UObjectHash.h"
#include "UObject/Class.h"

class UEPreExportUClass
{
public:
	UEPreExportUClass(UClass *Class);

	void PreAnalyzeFunction();

	void AppendCppHeaderAndDefine(FString &GeneratedFileContent);

	void AppendModuleObjectDefine(FString &GeneratedFileContent);

	void AppendPyMethodDefAndPyMethodTable(FString &GeneratedFileContent);

	void AppendPyTypeObject(FString &GeneratedFileContent);
	
	void AppendPyMagicFunction(FString &GeneratedFileContent);

	void AppendFunctionContain(FString &GeneratedFileContent);

	void AppendInitModuleHook(FString &GeneratedFileContent);
private:




	UClass *Class;

	// 模块在c-api中的名字, typedef struct { ... } UePyObjectCppName;
	FString UePyObjectCppName;
	
	// 模块下的对象名 import {PyModuleName}.{UePyObjectPyName}
	FString PyModuleName;		
	FString UePyObjectPyName;	

	// PyTypeObject instance name
	FString UePyObjectTypeName;

	// PyMethodDef instance name
	FString PyMethodDefListName;

	// magic func name



	TArray<TSharedPtr<ExportedFunction> > FuncList;

};