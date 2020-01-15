#include "ExportedFunction.h"

ExportedFunction::ExportedFunction(UFunction *func, UObject* obj)
	: Func(func),
	FuncType(EFunctionType::UNKNOW_TYPE)
{
	//  analysis input param and return value
	for (TFieldIterator<UProperty> i(this->Func); i; ++i)
	{
		UProperty* param = *i;
		if (param->PropertyFlags & CPF_Parm)
		{
			this->params.Add(param);
		}
		if (param->PropertyFlags & CPF_ReturnParm)
		{
			this->ReturnValue = param;
		}
	}
	// analysis function type. 
	if (obj != nullptr)
	{
		if (this->Func->FunctionFlags & FUNC_Static)
		{
			this->FuncType = EFunctionType::CLASS_STATIC_FUNCTION;
		}
		else
		{
			this->FuncType = EFunctionType::CLASS_MEMBER_FUNCTION;
		}
	}
	else
	{
		this->FuncType = EFunctionType::GLOBAL_FUNCTION;
	}
}
