#pragma once

#include "UEPyModule.h"

#include "UEPyEngine.h"
#include "UEPyTimer.h"
#include "UEPyTicker.h"
#include "UEPyVisualLogger.h"

#include "UObject/UEPyObject.h"
#include "UObject/UEPyActor.h"
#include "UObject/UEPyTransform.h"
#include "UObject/UEPyPlayer.h"
#include "UObject/UEPyInput.h"
#include "UObject/UEPyWorld.h"
#include "UObject/UEPyNavigation.h"
#include "UObject/UEPySpline.h"
#include "UObject/UEPyMovements.h"
#include "UObject/UEPyAttaching.h"
#include "UObject/UEPySkeletal.h"
#include "UObject/UEPyStaticMesh.h"
#include "UObject/UEPyTraceAndSweep.h"
#include "UObject/UEPyPhysics.h"
#include "UObject/UEPyAudio.h"
#include "UObject/UEPySequencer.h"
#include "UObject/UEPyViewport.h"
#include "UObject/UEPyWidget.h"
#include "UObject/UEPyWidgetComponent.h"
#include "UObject/UEPyPackage.h"
#include "UObject/UEPyTexture.h"
#include "UObject/UEPyMaterial.h"
#include "UObject/UEPyPawn.h"
#include "UObject/UEPyController.h"
#include "UObject/UEPyHUD.h"
#include "UObject/UEPyAnimSequence.h"
#include "UObject/UEPyCapture.h"
#include "UObject/UEPyLandscape.h"
#include "UObject/UEPyUserDefinedStruct.h"
#include "UObject/UEPyDataTable.h"
#include "UObject/UEPyExporter.h"
#include "UObject/UEPyFoliage.h"


#include "UEPyAssetUserData.h"
#if WITH_EDITOR
#include "UEPyEditor.h"
#include "Blueprint/UEPyEdGraph.h"
#include "Fbx/UEPyFbx.h"
#include "Editor/BlueprintGraph/Classes/EdGraphSchema_K2.h"
#include "Editor/BlueprintGraph/Public/BlueprintActionDatabase.h"
#endif

#include "Wrappers/UEPyESlateEnums.h"

#include "Wrappers/UEPyFVector.h"
#include "Wrappers/UEPyFVector2D.h"
#include "Wrappers/UEPyFHitResult.h"
#include "Wrappers/UEPyFRotator.h"
#include "Wrappers/UEPyFTransform.h"
#include "Wrappers/UEPyFColor.h"
#include "Wrappers/UEPyFLinearColor.h"
#include "Wrappers/UEPyFSocket.h"
#include "Wrappers/UEPyFQuat.h"

#include "Wrappers/UEPyFRawAnimSequenceTrack.h"

#include "Wrappers/UEPyFRandomStream.h"

#include "Wrappers/UEPyFPythonOutputDevice.h"
#if WITH_EDITOR
#include "Wrappers/UEPyFSoftSkinVertex.h"
#endif
#include "Wrappers/UEPyFMorphTargetDelta.h"
#include "Wrappers/UEPyFObjectThumbnail.h"

#include "Wrappers/UEPyFViewportClient.h"
#if WITH_EDITOR
#include "Wrappers/UEPyFEditorViewportClient.h"
#include "Wrappers/UEPyIAssetEditorInstance.h"
#endif

#include "Wrappers/UEPyFFoliageInstance.h"

#include "UEPyCallable.h"
#include "UEPyUClassesImporter.h"
#include "UEPyEnumsImporter.h"
#include "UEPyUStructsImporter.h"

#include "UEPyUScriptStruct.h"

#if WITH_EDITOR
#include "Wrappers/UEPyFSlowTask.h"
#include "Wrappers/UEPyFAssetData.h"
#include "Wrappers/UEPyFARFilter.h"
#include "Wrappers/UEPyFRawMesh.h"
#include "Wrappers/UEPyFStringAssetReference.h"

#include "UObject/UEPyAnimSequence.h"
#include "Blueprint/UEPyEdGraphPin.h"
#include "UEPyIPlugin.h"
#include "CollectionManager/UEPyICollectionManager.h"
#include "MaterialEditorUtilities/UEPyFMaterialEditorUtilities.h"
#endif

#include "Wrappers/UEPyFFrameNumber.h"

#include "Slate/UEPySlate.h"
#include "Http/UEPyIHttp.h"
#include "ConsoleManager/UEPyIConsoleManager.h"
#include "SlateApplication/UEPyFSlateApplication.h"
#include "Voice/UEPyIVoiceCapture.h"

#include "PythonFunction.h"
#include "PythonClass.h"

#if ENGINE_MINOR_VERSION >= 15
#include "Engine/UserDefinedEnum.h"
#endif

#include "Runtime/Core/Public/UObject/PropertyPortFlags.h"

#if ENGINE_MINOR_VERSION < 18
#define USoftObjectProperty UAssetObjectProperty
#define USoftClassProperty UAssetClassProperty
typedef FAssetPtr FSoftObjectPtr;
#endif

typedef struct
{
	PyObject_HEAD
	UObject *ue_object;
} ue_ExPyActor;

PyObject *py_ue_export_get_actor_location(ue_ExPyActor *self, PyObject * args);
PyObject *py_ue_export_set_actor_location(ue_ExPyActor *self, PyObject * args);

static PyMethodDef unreal_actor_methods[] = {
	{ "get_actor_location", (PyCFunction)py_ue_export_get_actor_location, METH_VARARGS, ""},
	{ "set_actor_location", (PyCFunction)py_ue_export_set_actor_location, METH_VARARGS, ""},
	{ NULL, NULL, NULL }
};

static void Custom_dealloc(PyObject *self)
{
	Py_TYPE(self)->tp_free((PyObject *)self);
}

static PyTypeObject ue_PyActorType = {
	PyVarObject_HEAD_INIT(NULL, 0)
};


//static PyTypeObject ue_PyActorType = {
//	PyVarObject_HEAD_INIT(NULL, 0)
//	"ue_actor.actor",          /* tp_name */
//	sizeof(ue_ExPyActor),      /* tp_basicsize */
//	0,                         /* tp_itemsize */
//	Custom_dealloc,		       /* tp_dealloc */
//	0,                         /* tp_print */
//	0,                         /* tp_getattr */
//	0,                         /* tp_setattr */
//	0,                         /* tp_reserved */
//	0,                         /* tp_repr */
//	0,                         /* tp_as_number */
//	0,                         /* tp_as_sequence */
//	0,                         /* tp_as_mapping */
//	0,                         /* tp_hash  */
//	0,                         /* tp_call */
//	0,                         /* tp_str */
//	0,						   /* tp_getattro */
//	0,						   /* tp_setattro */
//	0,                         /* tp_as_buffer */
//	Py_TPFLAGS_DEFAULT,		   /* tp_flags */
//	"ue_actor actor",          /* tp_doc */
//	0,                         /* tp_traverse */
//	0,                         /* tp_clear */
//	0,                         /* tp_richcompare */
//	0,                         /* tp_weaklistoffset */
//	0,                         /* tp_iter */
//	0,                         /* tp_iternext */
//	unreal_actor_methods,      /* tp_methods */
//};


// todo test get_actor_location
PyObject *py_ue_export_get_actor_location(ue_ExPyActor *self, PyObject * args)
{
	AActor *actor = Cast<AActor>(self->ue_object);
	if (actor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cast fail"));
		return Py_None;
	}

	// todo 测试输出参数 X
	//if (!PyArg_ParseTuple(args, "O", &obj))
	//{
	//	//return nullptr;
	//}
	//&obj = Py_BuildValue("i", 2);

	FVector vec = actor->K2_GetActorLocation();
	ue_PyFVector *ret = (ue_PyFVector *)PyObject_New(ue_PyFVector, &ue_PyFVectorType);
	ret->vec = vec;
	return (PyObject *)ret;
}

// todo test set_actor_location
PyObject *py_ue_export_set_actor_location(ue_ExPyActor *self, PyObject * args)
{
	// param 1
	PyObject *py_vec = nullptr;
	FVector vec;

	// param 2
	bool sweep = false;

	// param 3
	FHitResult hit;
	// param 4
	// ETeleportType

	bool success = false;

	// FVector bool UScriptStruct UEnum
	if (!PyArg_ParseTuple(args, "O", &py_vec))
	{
		return nullptr;
	}

	// FVector
	if (!PyObject_IsInstance(py_vec, (PyObject *)&ue_PyFVectorType))
		return nullptr;
	ue_PyFVector *ue_py_vec = ((ue_PyFVector *)py_vec);
	if (!ue_py_vec)
	{
		return nullptr;
	}
	else
	{
		vec = ue_py_vec->vec;
	}

	// 转换 + 调用
	AActor *actor = Cast<AActor>(self->ue_object);
	if (actor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cast fail"));
		return Py_None;
	}
	success = actor->SetActorLocation(vec, false, &hit, ETeleportType::None);
	if (success)
	{
		Py_RETURN_TRUE;
	}
	Py_RETURN_FALSE;
	// return Py_BuildValue("(OO)", success ? Py_True : Py_False, py_ue_new_fhitresult(hit));
}

// tp_init
static int ue_actor_magic_init(ue_ExPyActor *self, PyObject *args, PyObject *kwds)
{
	UE_LOG(LogTemp, Warning, TEXT("ue_actor_magic_init"));
	ue_ExPyActor* tmp = nullptr;
	if (!PyArg_ParseTuple(args, "O", &tmp))
		return -1;
	self->ue_object = tmp->ue_object;
	AActor *actor = Cast<AActor>(self->ue_object);
	if (actor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cast fail"));
		return 0;
	}
	FHitResult hit;
	FVector vec = actor->K2_GetActorLocation();
	vec.Z += 100;
	actor->SetActorLocation(vec, false, &hit, ETeleportType::TeleportPhysics);
	return 0;
}

// tp_new
static PyObject* ue_actor_magic_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	UE_LOG(LogTemp, Warning, TEXT("ue_actor_magic_new"));
	ue_ExPyActor *self = (ue_ExPyActor*)type->tp_alloc(type, 0);
	return (PyObject *)self;
}

void test_export_actor()
{
#if PY_MAJOR_VERSION >= 3
	PyObject * new_unreal_actor_module = PyImport_AddModule("ue_actor");
#else
	PyObject* new_unreal_actor_module = Py_InitModule3("ue_actor", NULL, NULL);
#endif

	ue_PyActorType.tp_new = PyType_GenericNew;
	ue_PyActorType.tp_name = "ue_actor.actor";
	ue_PyActorType.tp_basicsize = sizeof(ue_ExPyActor);
	ue_PyActorType.tp_dealloc = (destructor)Custom_dealloc;
	ue_PyActorType.tp_flags = Py_TPFLAGS_DEFAULT;
	ue_PyActorType.tp_init = (initproc)ue_actor_magic_init;
	ue_PyActorType.tp_methods = unreal_actor_methods;

	if (PyType_Ready(&ue_PyActorType) < 0)
		return;
	Py_INCREF(&ue_PyActorType);
	PyModule_AddObject(new_unreal_actor_module, "actor", (PyObject*)& ue_PyActorType);
}