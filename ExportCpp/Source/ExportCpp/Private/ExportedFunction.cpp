#include "ExportedFunction.h"

ExportedFunction::ExportedFunction(UFunction *func, UObject* BelongObj)
	: 
	Func(func),
	ReturnValue(nullptr),
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

FString ExportedFunction::ParseParams()
{
	FString Result;
	
	// 函数原定义注释
	Result += "// ";
	Result += ExportUeCppFunctionHeader();
	Result += ";\n";

	TArray<FString> ParamFormatList;
	TArray<FString> ParamNameList;

	//Result += FString::Printf(TEXT(
	//"if (!PyArg_ParseTuple(args, \"%s\", %s))		\n\
	//{												\n\
	//	return %s;   								\n\
	//}"));


	for (size_t i = 0; i < params.Num(); i++)
	{
		FString PyTypeFormat = "";
		// 先加上参数定义
		Result += ParseUProperty(params[i], ParamFormatList, ParamNameList);
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

FString ExportedFunction::ParseUProperty(UProperty* Property, TArray<FString> &ParamFormatList, TArray<FString> &ParamNameList)
{
	if (Property == nullptr) throw 1;
	
	//int8	    UInt8Property
	//int16	    UInt16Property
	//int32	    UIntProperty
	//int64     UInt64Property
	//uint8     UByteProperty
	//uint16    UUInt16Property
	//uint32    UUInt32Property
	//uint64    UUInt64Property

	FString Result;
	if (const UInt8Property *TempI8Property = Cast<UInt8Property>(Property))
	{
		// int8
		ParamFormatList.Add("i");
		ParamNameList.Add(Property->GetNameCPP());
		return FString::Printf(TEXT("    int8 %s;\n"), *Property->GetNameCPP());
	}
	else if (const UInt16Property *TempI16Property = Cast<UInt16Property>(Property))
	{
		// int16
		ParamFormatList.Add("i");
		ParamNameList.Add(Property->GetNameCPP());
		return FString::Printf(TEXT("    int16 %s;\n"), *Property->GetNameCPP());
	}
	else if (const UIntProperty *TempI32Property = Cast<UIntProperty>(Property))
	{
		// int32
		ParamFormatList.Add("i");
		ParamNameList.Add(Property->GetNameCPP());
		return FString::Printf(TEXT("    int32 %s;\n"), *Property->GetNameCPP());
	}
	else if (const UInt64Property *TempI64Property = Cast<UInt64Property>(Property))
	{
		// int64
		ParamFormatList.Add("L");
		ParamNameList.Add(Property->GetNameCPP());
		return FString::Printf(TEXT("    int64 %s;\n"), *Property->GetNameCPP());
	}
	else if (const UByteProperty *TempByteProperty = Cast<UByteProperty>(Property))
	{
		// uint8
		ParamFormatList.Add("I");
		ParamNameList.Add(Property->GetNameCPP());
		return FString::Printf(TEXT("    uint8 %s;\n"), *Property->GetNameCPP());
	}
	else if (const UUInt16Property *TempU16Property = Cast<UUInt16Property>(Property))
	{
		// uint16
		ParamFormatList.Add("I");
		ParamNameList.Add(Property->GetNameCPP());
		return FString::Printf(TEXT("    uint16 %s;\n"), *Property->GetNameCPP());
	}
	else if (const UUInt32Property *TempU32Property = Cast<UUInt32Property>(Property))
	{
		// uint32
		ParamFormatList.Add("I");
		ParamNameList.Add(Property->GetNameCPP());
		return FString::Printf(TEXT("    uint32 %s;\n"), *Property->GetNameCPP());
	}
	else if (const UUInt64Property *TempU64Property = Cast<UUInt64Property>(Property))
	{
		// uint64
		ParamFormatList.Add("K");
		ParamNameList.Add(Property->GetNameCPP());
		return FString::Printf(TEXT("    uint64 %s;\n"), *Property->GetNameCPP());
	}
	else if (const UFloatProperty *TempFloatProperty = Cast<UFloatProperty>(Property))
	{
		// float
		ParamFormatList.Add("f");
		ParamNameList.Add(Property->GetNameCPP());
		return FString::Printf(TEXT("    float %s;\n"), *Property->GetNameCPP());
	}
	else if (const UDoubleProperty *TempDoubleProperty = Cast<UDoubleProperty>(Property))
	{
		// double
		ParamFormatList.Add("d");
		ParamNameList.Add(Property->GetNameCPP());
		return FString::Printf(TEXT("    double %s;\n"), *Property->GetNameCPP());
	}
	else if (const UBoolProperty *TempBoolProperty = Cast<UBoolProperty>(Property))
	{
		// bool
		ParamFormatList.Add("p");
		ParamNameList.Add(Property->GetNameCPP());
		return FString::Printf(TEXT("    bool %s;\n"), *Property->GetNameCPP());
	}



	// TODO 待解析
	//ue_PyFVector		*param1;
	//ue_PyFVector2D		*param2;
	//ue_PyFRotator		*param3;
	//ue_PyFTransform		*param4;
	//ue_PyFColor			*param5;
	//ue_PyFLinearColor	*param6;
	//ue_PyFHitResult		*param7;

	return FString::Printf(TEXT("    unknow %s %s;\n"), *Property->GetName(), *Property->GetNameCPP());
}

