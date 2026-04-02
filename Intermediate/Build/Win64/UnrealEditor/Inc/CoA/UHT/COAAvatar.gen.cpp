// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "CoA/Public/COAAvatar.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeCOAAvatar() {}

// Begin Cross Module References
COA_API UClass* Z_Construct_UClass_ACOAAvatar();
COA_API UClass* Z_Construct_UClass_ACOAAvatar_NoRegister();
COA_API UClass* Z_Construct_UClass_ACOABaseCharacter();
ENGINE_API UClass* Z_Construct_UClass_UCameraComponent_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_USpringArmComponent_NoRegister();
UPackage* Z_Construct_UPackage__Script_CoA();
// End Cross Module References

// Begin Class ACOAAvatar
void ACOAAvatar::StaticRegisterNativesACOAAvatar()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ACOAAvatar);
UClass* Z_Construct_UClass_ACOAAvatar_NoRegister()
{
	return ACOAAvatar::StaticClass();
}
struct Z_Construct_UClass_ACOAAvatar_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "HideCategories", "Navigation" },
		{ "IncludePath", "COAAvatar.h" },
		{ "ModuleRelativePath", "Public/COAAvatar.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_mSpringArm_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "COAAvatar" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/COAAvatar.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_mCamera_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "COAAvatar" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/COAAvatar.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RunSpeed_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "COAAvatar" },
		{ "ModuleRelativePath", "Public/COAAvatar.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_mSpringArm;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_mCamera;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_RunSpeed;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ACOAAvatar>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ACOAAvatar_Statics::NewProp_mSpringArm = { "mSpringArm", nullptr, (EPropertyFlags)0x00400000000a001d, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ACOAAvatar, mSpringArm), Z_Construct_UClass_USpringArmComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_mSpringArm_MetaData), NewProp_mSpringArm_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ACOAAvatar_Statics::NewProp_mCamera = { "mCamera", nullptr, (EPropertyFlags)0x00400000000a001d, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ACOAAvatar, mCamera), Z_Construct_UClass_UCameraComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_mCamera_MetaData), NewProp_mCamera_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ACOAAvatar_Statics::NewProp_RunSpeed = { "RunSpeed", nullptr, (EPropertyFlags)0x0040000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ACOAAvatar, RunSpeed), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RunSpeed_MetaData), NewProp_RunSpeed_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_ACOAAvatar_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACOAAvatar_Statics::NewProp_mSpringArm,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACOAAvatar_Statics::NewProp_mCamera,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACOAAvatar_Statics::NewProp_RunSpeed,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ACOAAvatar_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_ACOAAvatar_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_ACOABaseCharacter,
	(UObject* (*)())Z_Construct_UPackage__Script_CoA,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ACOAAvatar_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_ACOAAvatar_Statics::ClassParams = {
	&ACOAAvatar::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_ACOAAvatar_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_ACOAAvatar_Statics::PropPointers),
	0,
	0x009000A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ACOAAvatar_Statics::Class_MetaDataParams), Z_Construct_UClass_ACOAAvatar_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_ACOAAvatar()
{
	if (!Z_Registration_Info_UClass_ACOAAvatar.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ACOAAvatar.OuterSingleton, Z_Construct_UClass_ACOAAvatar_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_ACOAAvatar.OuterSingleton;
}
template<> COA_API UClass* StaticClass<ACOAAvatar>()
{
	return ACOAAvatar::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(ACOAAvatar);
ACOAAvatar::~ACOAAvatar() {}
// End Class ACOAAvatar

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_Alimert_Documents_Unreal_Projects_CoA_Source_CoA_Public_COAAvatar_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_ACOAAvatar, ACOAAvatar::StaticClass, TEXT("ACOAAvatar"), &Z_Registration_Info_UClass_ACOAAvatar, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ACOAAvatar), 3331880051U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_Alimert_Documents_Unreal_Projects_CoA_Source_CoA_Public_COAAvatar_h_3313967964(TEXT("/Script/CoA"),
	Z_CompiledInDeferFile_FID_Users_Alimert_Documents_Unreal_Projects_CoA_Source_CoA_Public_COAAvatar_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_Alimert_Documents_Unreal_Projects_CoA_Source_CoA_Public_COAAvatar_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
