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

	FString ExportUFunctionContin(ExportedFunction *Func);

	UClass *Class;

	FString UePyObjectCppName;			// 模块中的对象名, typedef struct { ... } UePyObjectCppName;
	FString PyTypeObjectName;			// 导出的 PyTypeObject 实例名
	FString PyMethodDefListName;		// 导出的 PyMethod     实例名

	FString PyModuleName;				// py中模块下的对象名 import {PyModuleName}.{UePyObjectPyName}
	FString UePyObjectPyName;			

	FString UClassNameWithPrefix;

	FString MagicFuncTpInitName;		// tp_init
	FString MagicFuncTpDeallocName;		// tp_dealloc

	TArray<TSharedPtr<ExportedFunction> > FuncList;

};