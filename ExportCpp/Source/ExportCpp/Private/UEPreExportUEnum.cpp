#include "UEPreExportUEnum.h"

UEPreExportUEnum::UEPreExportUEnum(UEnum * Enum)
	:m_enum(Enum)
{

}

void UEPreExportUEnum::AppendUEnumContain(FString & GeneratedFileContent)
{
	for (int i = 0; i < m_enum->NumEnums(); ++i)
	{
		FName name = m_enum->GetNameByIndex(i);
		int value = m_enum->GetValueByIndex(i);
		GeneratedFileContent += FString::Printf(TEXT("%s = %d\n"), *name.ToString(), value);
	}
}
