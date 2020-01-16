#include "UEPreExportUClass.h"

UEPreExportUClass::UEPreExportUClass(UClass * Class)
	: Class(Class)
{
	this->UePyObjectCppName = FString::Printf(TEXT("ue_Export%s"), *Class->GetName());
	this->UePyObjectPyName = Class->GetName().ToLower();
	this->PyModuleName = FString::Printf(TEXT("ue_%s"), *Class->GetName().ToLower());
	this->UePyObjectTypeName = FString::Printf(TEXT("ue_Export%sType"), *Class->GetName());
	this->PyMethodDefListName = FString::Printf(TEXT("unreal_export_%s_methods"), *Class->GetName().ToLower());
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
	GeneratedFileContent += FString::Printf(TEXT("\nstatic PyTypeObject %s = {\n"), *UePyObjectTypeName);
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

	
	GeneratedFileContent += FString::Printf(TEXT("static int ue_%s_magic_init(%s *self, PyObject *args, PyObject *kwds)\n{\n%s}\n"),
								*PyModuleName,
								*UePyObjectCppName,
								*TpInitCodeContain);

	// 预定义 tp_dealloc
	GeneratedFileContent += "static void Custom_dealloc(PyObject *self)\n{\n    Py_TYPE(self)->tp_free((PyObject *)self);\n}\n";
}

void UEPreExportUClass::AppendFunctionContain(FString & GeneratedFileContent)
{
	// todo
}

void UEPreExportUClass::AppendInitModuleHook(FString & GeneratedFileContent)
{
	// todo
	FString InitModuleCodeContain;
	InitModuleCodeContain += FString::Printf(TEXT(
		"#if PY_MAJOR_VERSION >= 3\n"
		"\tPyObject * new_module = PyImport_AddModule(\"%s\");\n"
		"#else\n"
		"\tPyObject* new_module = Py_InitModule3(\"%s\", NULL, NULL);\n"
		"#endif\n"
		), *PyModuleName, *PyModuleName);

	InitModuleCodeContain += FString::Printf(TEXT(
		"%s.tp_new = PyType_GenericNew;"
		"%s.tp_name = \"%s\";"
		"%s.tp_basicsize = sizeof(%s);"
		"%s.tp_dealloc = (destructor)Custom_dealloc;"
		"%s.tp_flags = Py_TPFLAGS_DEFAULT;"
		"%s.tp_init = (initproc)ue_actor_magic_init;"
		"%s.tp_methods = unreal_actor_methods;"
	));

	InitModuleCodeContain += FString::Printf(TEXT(
		"\tif (PyType_Ready(&%s) < 0)\n"
		"\t\treturn;\n"
		"\tPy_INCREF(&%s);\n"
		"\tPyModule_AddObject(new_module, \"%s\", (PyObject*)& ue_PyActorType);\n"
	), *UePyObjectTypeName, *UePyObjectTypeName, *PyModuleName, *UePyObjectTypeName);
	GeneratedFileContent += FString::Printf(TEXT("void ue_py_export_%s_init()\n{\n%s}\n"), *PyModuleName, *InitModuleCodeContain);
}
