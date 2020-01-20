#pragma once

#include "UEPyModule.h"

void test_export_uenum()
{
	
#if PY_MAJOR_VERSION >= 3
	PyObject * new_module = PyImport_AddModule("ue_enum");
#else
	PyObject* new__module = Py_InitModule3("ue_enum", NULL, NULL);
#endif
	
	PyObject* new_dict = PyModule_GetDict(new_module);
	PyDict_SetItemString(new_dict, "test", Py_BuildValue("i", 1));
}