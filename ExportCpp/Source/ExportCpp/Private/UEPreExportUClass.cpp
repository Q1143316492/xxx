#include "UEPreExportUClass.h"

UEPreExportUClass::UEPreExportUClass(UClass * Class)
	: Class(Class)
{
	this->UePyObjectCppName = FString::Printf(TEXT("ue_Export%s"), *Class->GetName());
	this->PyTypeObjectName = FString::Printf(TEXT("ue_Export%sType"), *Class->GetName());
	this->PyMethodDefListName = FString::Printf(TEXT("unreal_export_%s_methods"), *Class->GetName().ToLower());

	this->PyModuleName = FString::Printf(TEXT("ue_%s"), *Class->GetName().ToLower());
	this->UePyObjectPyName = Class->GetName().ToLower();

	this->UClassNameWithPrefix = Class->GetPrefixCPP() + Class->GetName();

	this->MagicFuncTpInitName = FString::Printf(TEXT("%s_magic_init"), *PyModuleName);
	this->MagicFuncTpDeallocName = FString::Printf(TEXT("%s_magic_dealloc"), *PyModuleName);
}

void UEPreExportUClass::PreAnalyzeFunction()
{
	for (TFieldIterator<UFunction> i(Class, EFieldIteratorFlags::IncludeSuper, EFieldIteratorFlags::IncludeDeprecated); i; ++i)
	{
		TSharedPtr<ExportedFunction> func(new ExportedFunction(*i, Class));
		FuncList.Add(func);
	}
}

void UEPreExportUClass::AppendCppHeaderAndDefine(FString & GeneratedFileContent)
{
	GeneratedFileContent += "\n";
	// todo
	// ...
}

void UEPreExportUClass::AppendModuleObjectDefine(FString & GeneratedFileContent)
{
	GeneratedFileContent += FString::Printf(TEXT("\ntypedef struct\n{\n    PyObject_HEAD\n    UObject *ue_object;\n} %s;\n"), *UePyObjectCppName);
}

void UEPreExportUClass::AppendPyMethodDefAndPyMethodTable(FString & GeneratedFileContent)
{
	// 导出扩展函数声明
	GeneratedFileContent += "\n";
	for (size_t i = 0; i < FuncList.Num(); i++)
	{
		GeneratedFileContent += FString::Printf(TEXT("PyObject *py_ue_export_%s(%s *self, PyObject * args);\n"),
									*FuncList[i].Get()->GetFunctionName(),
									*UePyObjectCppName);
	}
	GeneratedFileContent += "\n";
	// 导出 PyMethodDef
	GeneratedFileContent += FString::Printf(TEXT("static PyMethodDef %s[] = {\n"), *PyMethodDefListName);
	for (size_t i = 0; i < FuncList.Num(); i++)
	{
		GeneratedFileContent += FString::Printf(TEXT("    { \"%s\", (PyCFunction)py_ue_export_%s, METH_VARARGS, \"\"},\n"),
									*FuncList[i].Get()->GetFunctionName(),
									*FuncList[i].Get()->GetFunctionName());
	}
	GeneratedFileContent += "    { NULL, NULL, NULL }\n};\n";
}

void UEPreExportUClass::AppendPyTypeObject(FString & GeneratedFileContent)
{
	GeneratedFileContent += FString::Printf(TEXT("\nstatic PyTypeObject %s = {\n"), *PyTypeObjectName);
	GeneratedFileContent += "	PyVarObject_HEAD_INIT(NULL, 0)\n};\n";
}

void UEPreExportUClass::AppendPyMagicFunction(FString & GeneratedFileContent)
{
	// 预定义 tp_init 
	// todo 不给参数的构造函数待添加
	FString TpInitCodeContain = FString::Printf(TEXT(
		"\t%s* tmp = nullptr;\n"
		"\tif (!PyArg_ParseTuple(args, \"O\", &tmp))\n"
		"\t\treturn -1;\n"
		"\tself->ue_object = tmp->ue_object;\n"
		"\t%s *obj = Cast<%s>(self->ue_object);\n"
		"\treturn obj == nullptr ? -1 : 0;\n"),
		*UePyObjectCppName,
		*(Class->GetPrefixCPP() + Class->GetName()),
		*(Class->GetPrefixCPP() + Class->GetName()));

	GeneratedFileContent += FString::Printf(TEXT("static int %s(%s *self, PyObject *args, PyObject *kwds)\n{\n%s}\n"),
								*MagicFuncTpInitName,
								*UePyObjectCppName,
								*TpInitCodeContain);

	// 预定义 tp_dealloc
	GeneratedFileContent += FString::Printf(TEXT("static void %s(PyObject *self)\n{\n    Py_TYPE(self)->tp_free((PyObject *)self);\n}\n"), 
								*MagicFuncTpDeallocName);
}

void UEPreExportUClass::AppendFunctionContain(FString & GeneratedFileContent)
{
	// todo
	GeneratedFileContent += "\n";
	for (size_t i = 0; i < FuncList.Num(); i++)
	{
		FString CodeContain = ExportUFunctionContin(FuncList[i].Get());
		GeneratedFileContent += FString::Printf(TEXT("PyObject *py_ue_export_%s(%s *self, PyObject * args)\n{\n%s\n}\n"),
			*FuncList[i].Get()->GetFunctionName(),
			*UePyObjectCppName,
			*CodeContain);
	}
}

void UEPreExportUClass::AppendInitModuleHook(FString & GeneratedFileContent)
{
	FString InitModuleCodeContain;
	InitModuleCodeContain += FString::Printf(TEXT(
		"#if PY_MAJOR_VERSION >= 3\n"
		"\tPyObject * new_module = PyImport_AddModule(\"%s\");\n"
		"#else\n"
		"\tPyObject* new_module = Py_InitModule3(\"%s\", NULL, NULL);\n"
		"#endif\n"
		), *PyModuleName, *PyModuleName);

	FString TpName = PyModuleName;
	TpName += ".";
	TpName += UePyObjectPyName;
	InitModuleCodeContain += FString::Printf(TEXT(
		"\t%s.tp_new = PyType_GenericNew;\n"
		"\t%s.tp_name = \"%s\";\n"
		"\t%s.tp_basicsize = sizeof(%s);\n"
		"\t%s.tp_dealloc = (destructor)%s;\n"
		"\t%s.tp_flags = Py_TPFLAGS_DEFAULT;\n"
		"\t%s.tp_init = (initproc)%s;\n"
		"\t%s.tp_methods = %s;\n"
	),  *PyTypeObjectName,
	    *PyTypeObjectName, *TpName,
	    *PyTypeObjectName, *UePyObjectCppName,
		*PyTypeObjectName, *MagicFuncTpDeallocName,
		*PyTypeObjectName,
		*PyTypeObjectName, *MagicFuncTpInitName,
		*PyTypeObjectName, *PyMethodDefListName);
	
	InitModuleCodeContain += FString::Printf(TEXT(
		"\tif (PyType_Ready(&%s) < 0)\n"
		"\t\treturn;\n"
		"\tPy_INCREF(&%s);\n"
		"\tPyModule_AddObject(new_module, \"%s\", (PyObject*)& %s);\n"
	), *PyTypeObjectName, *PyTypeObjectName, *PyModuleName, *PyTypeObjectName);
	GeneratedFileContent += FString::Printf(TEXT("void ue_py_export_%s_init()\n{\n%s}\n"), *PyModuleName, *InitModuleCodeContain);
}

FString UEPreExportUClass::ExportUFunctionContin(ExportedFunction * Func)
{
	FString CodeContains;
	CodeContains += FString::Printf(TEXT("\
	%s *u_obj = Cast<%s>(self->ue_object)							        \n\
	if (u_obj == nullptr)												    \n\
	{																		\n\
		UE_LOG(LogTemp, Warning, TEXT(\"Cast fail\"));						\n\
		return Py_None;														\n\
	}																		\n\
	"), *UClassNameWithPrefix, *UClassNameWithPrefix);
	
	// todo 参数解析
	CodeContains += Func->ParseParams();

	return CodeContains;
}
