#include "ExportedFunction.h"

ExportedFunction::ExportedFunction(UFunction *Func, UObject* BelongObj)
	: Func(Func),
	FuncType(EFunctionType::UNKNOW_TYPE)
{
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
	if (BelongObj != nullptr)
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


FString ExportedFunction::ExportFunctionHeader()
{
	FString FuncDefine;
	if (FuncType == EFunctionType::CLASS_STATIC_FUNCTION)
	{
		FuncDefine += "static ";
	}
	FuncDefine += GetPropertyCPPType(ReturnValue);
	FuncDefine += " ";
	FuncDefine += Func->GetName() + "(";

	for (size_t i = 0; i < params.Num(); i++)
	{
		if (i != 0) 
		{
			FuncDefine += ", ";
		}
		FuncDefine += GetPropertyCPPType(params[i]);
		FuncDefine += " ";
		FuncDefine += FString::Printf(TEXT("param%d"), i);
	}
	FuncDefine += ")";
	return FuncDefine;
}

FString ExportedFunction::GetFunctionName()
{
	return Func->GetName();
}

FString ExportedFunction::GetPropertyCPPType(UProperty * Property)
{
	return Property->GetCPPType();
}

FString ExportedFunction::GetPropertyCPPParamName(UProperty* Property)
{
	return "";
}
