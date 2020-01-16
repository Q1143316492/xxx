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
	// todo ����������ʱ��Ҫ��
	TArray<UObject*> ObjectsToProcess;
	FString GeneratedFileContent;
	// �����������Ƿ��ȡ���෽��
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
			// todo ����c api������
			exp.AppendFunctionContain(GeneratedFileContent);

			// todo ������ʼ��ģ��
			exp.AppendInitModuleHook(GeneratedFileContent);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *GeneratedFileContent);


		}
	}
}
