#include "ExportedFunction.h"

ExportedFunction::ExportedFunction(UFunction *Func, UObject* BelongObj)
	: 
	Func(Func),
	FuncType(EFunctionType::UNKNOW_TYPE),
	ReturnValue(nullptr)
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

FString ExportedFunction::ParseParams()
{
	FString Result;
	Result += "// ";
	Result += ExportUeCppFunctionHeader();
	Result += ";\n";
	for (size_t i = 0; i < params.Num(); i++)
	{
		Result += ParseUProperty(params[i]);
	}

	return Result;
}

FString ExportedFunction::ExportUeCppFunctionHeader()
{
	FString FuncDefine;
	if (FuncType == EFunctionType::CLASS_STATIC_FUNCTION)
	{
		FuncDefine += "static ";
	}
	if (ReturnValue == nullptr)
	{
		FuncDefine += "void";
	}
	else
	{
		FuncDefine += ReturnValue->GetCPPType();
	}
	FuncDefine += " ";
	FuncDefine += Func->GetName() + "(";

	// todo 可以添加 const & 等字段的信息
	for (size_t i = 0; i < params.Num(); i++)
	{
		if (i != 0) 
		{
			FuncDefine += ", ";
		}
		FuncDefine += params[i]->GetCPPType();
		FuncDefine += " ";
		FuncDefine += FString::Printf(TEXT("%s"), *params[i]->GetNameCPP());
	}
	FuncDefine += ")";
	return FuncDefine;
}

FString ExportedFunction::GetFunctionName()
{
	return Func->GetName();
}

FString ExportedFunction::ParseUProperty(UProperty * Property)
{
	if (Property == nullptr) throw 1;


	return FString();
}

