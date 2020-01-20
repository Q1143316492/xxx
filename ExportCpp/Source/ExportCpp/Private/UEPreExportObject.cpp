#include "UEPreExportObject.h"

const FString UEPreExportObject::UClassNeedExport[] = { "Actor" };
const FString UEPreExportObject::UEnumNeedExport[] = { "ETeleportType" };
const FString UEPreExportObject::UScriptStructNeedExport[] = { "HitResult" };

const FString UEPreExportObject::ExportPath = "D:\\ue4\\ext";

UEPreExportObject::UEPreExportObject()
{
	for (size_t i = 0; i < ARRAY_COUNT(UClassNeedExport); i++)
	{
		ExportUClassSet.Add(UClassNeedExport[i]);
	}
	for (size_t i = 0; i < ARRAY_COUNT(UEnumNeedExport); i++)
	{
		ExportUEnumSet.Add(UEnumNeedExport[i]);
	}
	for (size_t i = 0; i < ARRAY_COUNT(UScriptStructNeedExport); i++)
	{
		ExportUScriptStructSet.Add(UScriptStructNeedExport[i]);
	}
}

void UEPreExportObject::ExportUClass(bool bExportAll)
{
	// todo 导出多个类的时候要改
	TArray<UObject*> ObjectsToProcess;
	FString GeneratedFileContent;
	// 第三个参数是否获取子类方法
	GetObjectsOfClass(UClass::StaticClass(), ObjectsToProcess, true);
	for (UObject *obj : ObjectsToProcess)
	{
		if (obj->IsA<UClass>() && (bExportAll || ExportUClassSet.Find(obj->GetName()) != nullptr))
		{
			UClass *Class = Cast<UClass>(obj);
			UEPreExportUClass exp(Class);
			exp.PreAnalyzeFunction();
			exp.AppendCppHeaderAndDefine(GeneratedFileContent);
			exp.AppendModuleObjectDefine(GeneratedFileContent);
			exp.AppendPyMethodDefAndPyMethodTable(GeneratedFileContent);
			exp.AppendPyTypeObject(GeneratedFileContent);
			exp.AppendPyMagicFunction(GeneratedFileContent);

			// todo
			exp.AppendFunctionContain(GeneratedFileContent);
			
			exp.AppendInitModuleHook(GeneratedFileContent);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *GeneratedFileContent);
		}
	}
}

void UEPreExportObject::ExportUEnum(bool bExportAll)
{
	TArray<UObject*> ObjectsToProcess;
	FString GeneratedFileContent;
	TArray<FString> FuncNameList;
	// 导出所有枚举 
	GetObjectsOfClass(UEnum::StaticClass(), ObjectsToProcess, true);

	GeneratedFileContent += FString::Printf(TEXT(
		"#pragma once\n"
		"#include \"UEPyModule.h\"\n"));

	// 每个Eunum生成一个注册函数
	for (UObject *obj : ObjectsToProcess)
	{
		if (obj->IsA<UEnum>() && (bExportAll || ExportUEnumSet.Find(obj->GetName()) != nullptr))
		{
			UEnum *Enum = Cast<UEnum>(obj);
			UEPreExportUEnum exp(Enum);
			exp.AppendUEnumRegisterFunction(GeneratedFileContent, FuncNameList);
		}
	}
	// 生成统一调用注册函数的入口
	FString RegisterFuncContain;
	for (size_t i = 0; i < FuncNameList.Num(); i++) {
		RegisterFuncContain += FString::Printf(TEXT("\t%s();\n"), *FuncNameList[i]);
	}
	GeneratedFileContent += FString::Printf(TEXT(
		"void unreal_export_uenum() \n {\n%s\n}\n"), *RegisterFuncContain);
	// 导出文件
	UEPreExportObject::ExportContainToFile(GeneratedFileContent);
}

void UEPreExportObject::ExportUScriptStruct(bool bExportAll)
{
	TArray<UObject*> ObjectsToProcess;
	FString GeneratedFileContent;
	GetObjectsOfClass(UScriptStruct::StaticClass(), ObjectsToProcess, true);

	for (UObject *obj : ObjectsToProcess)
	{

		if (obj->IsA<UScriptStruct>() && (bExportAll || ExportUScriptStructSet.Find(obj->GetName()) != nullptr))
		{
			UScriptStruct *Struct = Cast<UScriptStruct>(obj);
			UE_LOG(LogTemp, Warning, TEXT("find one %s \n"), *Struct->GetName());
			for (TFieldIterator<UProperty> i(Struct, EFieldIteratorFlags::IncludeSuper, EFieldIteratorFlags::IncludeDeprecated); i; ++i)
			{
				UE_LOG(LogTemp, Warning, TEXT("%s \n"), *i->GetName());
			}

		}
	}
}

bool UEPreExportObject::ExportContainToFile(const FString &contain)
{
	FILE* f;
	f = fopen(TCHAR_TO_ANSI(*(ExportPath + "\\ue_enum.h")), "w+");
	fprintf(f, TCHAR_TO_ANSI(*contain));
	fclose(f);
	return false;
}
