#include "UEPreExportUEnum.h"

UEPreExportUEnum::UEPreExportUEnum(UEnum * Enum)
	:m_enum(Enum)
{

}

void UEPreExportUEnum::AppendUEnumRegisterFunction(FString & GeneratedFileContent, TArray<FString> & FuncNameList)
{
	FString FuncName = FString::Printf(TEXT("unreal_export_uenum_%s"), *m_enum->GetName());
	FuncNameList.Add(FuncName);

	// 组装函数体
	FString ExportFuncContain = FString::Printf(TEXT("                                        \n\
	#if PY_MAJOR_VERSION >= 3                                                                 \n\
		PyObject * new_module_%s = PyImport_AddModule(\"%s\");                                \n\
	#else                                                                                     \n\
		PyObject* new__module_%s = Py_InitModule3(\"%s\", NULL, NULL);                        \n\
	#endif\n\n"), *m_enum->GetName(), *m_enum->GetName(), *m_enum->GetName(), *m_enum->GetName());

	ExportFuncContain += FString::Printf(TEXT(
							"\tPyObject* new_dict_%s = PyModule_GetDict(new_module_%s);\n"),
							*m_enum->GetName(), *m_enum->GetName());

	for (int i = 0; i < m_enum->NumEnums(); ++i)
	{
		FString name = m_enum->GetNameStringByIndex(i);
		int value = m_enum->GetValueByIndex(i);
		FString EunumName = m_enum->GetName();
		
		// 枚举字段名不能为python关键字，目前就发现一个None
		if (EunumName.Equals("None"))
		{
			EunumName = "NONE";
		}

		ExportFuncContain += FString::Printf(TEXT(
			"\tPyDict_SetItemString(new_dict_%s, \"%s\", Py_BuildValue(\"i\", %d));\n"),
			*EunumName, *name, value);
	}

	// 组装函数
	GeneratedFileContent += FString::Printf(TEXT(
		"void %s() \n {\n%s\n}\n"), *FuncName, *ExportFuncContain);
}
