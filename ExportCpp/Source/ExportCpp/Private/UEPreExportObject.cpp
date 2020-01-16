#include "UEPreExportObject.h"

const FString UEPreExportObject::UClassNeedExport[] = { "Actor" };

const FString UEPreExportObject::ExportPath = "D:\\ue4\\ext";

UEPreExportObject::UEPreExportObject()
{
	for (size_t i = 0; i < ARRAY_COUNT(UClassNeedExport); i++)
	{
		ExportUClassSet.Add(UClassNeedExport[i]);
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
			// todo 导出c api函数体
			exp.AppendFunctionContain(GeneratedFileContent);

			// todo 导出初始化模块
			exp.AppendInitModuleHook(GeneratedFileContent);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *GeneratedFileContent);


		}
	}
}
