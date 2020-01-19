#include "UEPreExportObject.h"

const FString UEPreExportObject::UClassNeedExport[] = { "Actor" };
const FString UEPreExportObject::UEnumNeedExport[] = { "ETeleportType" };

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
}

void UEPreExportObject::ExportUClass()
{
	// todo 导出多个类的时候要改
	TArray<UObject*> ObjectsToProcess;
	FString GeneratedFileContent;
	// 第三个参数是否获取子类方法
	GetObjectsOfClass(UClass::StaticClass(), ObjectsToProcess, true);
	for (UObject *obj : ObjectsToProcess)
	{
		if (obj->IsA<UClass>() && ExportUClassSet.Find(obj->GetName()) != nullptr)
		{
			UClass *Class = Cast<UClass>(obj);
			UEPreExportUClass exp(Class);
			exp.PreAnalyzeFunction();
			exp.AppendCppHeaderAndDefine(GeneratedFileContent);
			exp.AppendModuleObjectDefine(GeneratedFileContent);
			exp.AppendPyMethodDefAndPyMethodTable(GeneratedFileContent);
			exp.AppendPyTypeObject(GeneratedFileContent);
			exp.AppendPyMagicFunction(GeneratedFileContent);
			exp.AppendFunctionContain(GeneratedFileContent);
			exp.AppendInitModuleHook(GeneratedFileContent);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *GeneratedFileContent);
		}
	}
}

void UEPreExportObject::ExportUEnum()
{
	TArray<UObject*> ObjectsToProcess;
	FString GeneratedFileContent;
	GetObjectsOfClass(UEnum::StaticClass(), ObjectsToProcess, true);
	for (UObject *obj : ObjectsToProcess)
	{
		if (obj->IsA<UEnum>() && ExportUEnumSet.Find(obj->GetName()) != nullptr)
		{
			UEnum *Enum = Cast<UEnum>(obj);
			UEPreExportUEnum exp(Enum);
			exp.AppendUEnumContain(GeneratedFileContent);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *GeneratedFileContent);
		}
	}
}
