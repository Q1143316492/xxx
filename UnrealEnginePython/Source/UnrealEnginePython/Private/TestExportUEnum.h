#pragma once

#include "UEPyModule.h"

typedef struct
{
	PyObject_HEAD
} ue_PyUEnum;

static PyObject* ue_uenum_test_getattro(ue_PyUObject* self, PyObject* attr_name)
{

	PyObject* ret = PyObject_GenericGetAttr((PyObject*)self, attr_name);
	if (!ret)
	{
		if (PyUnicodeOrString_Check(attr_name))
		{
			const char* attr = UEPyUnicode_AsUTF8(attr_name);
			return Py_BuildValue("i", 1);
		}
	}
	return ret;
}

static PyTypeObject ue_UEnumType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"ue_enum.test",             /* tp_name */
	sizeof(ue_PyUEnum), /* tp_basicsize */
	0,                         /* tp_itemsize */
	0,                        /* tp_dealloc */
	0,                         /* tp_print */
	(getattrfunc)ue_uenum_test_getattro,  /* tp_getattr */
	0,                         /* tp_setattr */
	0,                         /* tp_reserved */
	0,                         /* tp_repr */
	0,                         /* tp_as_number */
	0,                         /* tp_as_sequence */
	0,                         /* tp_as_mapping */
	0,                         /* tp_hash  */
	0,                         /* tp_call */
	0,                         /* tp_str */
	(getattrofunc)ue_uenum_test_getattro, /* tp_getattro */
	0, /* tp_setattro */
	0,                         /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT,        /* tp_flags */
	"uenum",           /* tp_doc */
	0,                         /* tp_traverse */
	0,                         /* tp_clear */
	0,                         /* tp_richcompare */
	0,                         /* tp_weaklistoffset */
	0,                         /* tp_iter */
	0,                         /* tp_iternext */
	0,             /* tp_methods */
};

static void UePyUenum_dealloc(PyObject *self)
{
	Py_TYPE(self)->tp_free((PyObject *)self);
}

void test_export_uenum()
{
#if PY_MAJOR_VERSION >= 3
	PyObject * new_module = PyImport_AddModule("ue_enum");
#else
	PyObject* new_module = Py_InitModule3("ue_enum", NULL, NULL);
#endif
	//ue_UEnumType.tp_new = PyType_GenericNew;
	//ue_UEnumType.tp_name = "ue_enum.test";
	//ue_UEnumType.tp_basicsize = sizeof(ue_PyUEnum);
	//ue_UEnumType.tp_dealloc = (destructor)UePyUenum_dealloc;
	//ue_UEnumType.tp_getattro = (getattrofunc)ue_uenum_test_getattro;
	//ue_UEnumType.tp_flags = Py_TPFLAGS_DEFAULT;

	if (PyType_Ready(&ue_UEnumType) < 0)
		return;
	Py_INCREF(&ue_UEnumType);
	PyModule_AddObject(new_module, "test", (PyObject*)& ue_UEnumType);
}