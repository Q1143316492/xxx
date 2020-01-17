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

	FString UePyObjectCppName;			// ģ���еĶ�����, typedef struct { ... } UePyObjectCppName;
	FString PyTypeObjectName;			// ������ PyTypeObject ʵ����
	FString PyMethodDefListName;		// ������ PyMethod     ʵ����

	FString PyModuleName;				// py��ģ���µĶ����� import {PyModuleName}.{UePyObjectPyName}
	FString UePyObjectPyName;			

	FString UClassNameWithPrefix;

	FString MagicFuncTpInitName;		// tp_init
	FString MagicFuncTpDeallocName;		// tp_dealloc

	TArray<TSharedPtr<ExportedFunction> > FuncList;

};