#pragma once

#include "UEPyModule.h"

typedef struct
{
	PyObject_HEAD
	UObject *ue_object;
} ue_ExPyHitResult;

static PyTypeObject ue_PyHitResultType = {
	PyVarObject_HEAD_INIT(NULL, 0)
};

void test_export_uscriptstruct()
{
#if PY_MAJOR_VERSION >= 3
	PyObject * new_module = PyImport_AddModule("ue_HitResult");
#else
	PyObject* new_module = Py_InitModule3("ue_HitResult", NULL, NULL);
#endif
	ue_PyHitResultType.tp_new = PyType_GenericNew;
	ue_PyHitResultType.tp_name = "ue_HitResult.HitResult";
	ue_PyHitResultType.tp_basicsize = sizeof(ue_ExPyHitResult);
	ue_PyHitResultType.tp_flags = Py_TPFLAGS_DEFAULT;

	//ue_PyHitResultType.tp_dealloc = (destructor)Custom_dealloc;
	//ue_PyHitResultType.tp_init = (initproc)ue_actor_magic_init;
	//ue_PyHitResultType.tp_methods = unreal_actor_methods;

	if (PyType_Ready(&ue_PyHitResultType) < 0)
		return;
	Py_INCREF(&ue_PyHitResultType);
	PyModule_AddObject(new_module, "HitResult", (PyObject*)& ue_PyHitResultType);
}