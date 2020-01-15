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
	TArray<UObject*> ObjectsToProcess;
	GetObjectsOfClass(UClass::StaticClass(), ObjectsToProcess, true);
	FString GeneratedFileContent;
	for (UObject *obj : ObjectsToProcess)
	{
		if (obj->IsA<UClass>() && ExportUClassSet.Find(obj->GetName()) != nullptr)
		{
			this->ExportFunctionFromUClass(obj, GeneratedFileContent);
		}
	}
}

void UEPreExportObject::ExportFunctionFromUClass(UObject * obj, FString &GeneratedFileContent)
{
	UClass *Class = Cast<UClass>(obj);
	if (Class == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("export uclass %s. is not a uclass"), *obj->GetName());
		return;
	}
	for (TFieldIterator<UFunction> i(Class, EFieldIteratorFlags::IncludeSuper, EFieldIteratorFlags::IncludeDeprecated); i; ++i)
	{
		TSharedPtr<ExportedFunction> func(new ExportedFunction(*i, Class));

	}
}
