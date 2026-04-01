// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "CoA/Public/COAGameMode.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeCOAGameMode() {}

// Begin Cross Module References
COA_API UClass* Z_Construct_UClass_ACOAGameMode();
COA_API UClass* Z_Construct_UClass_ACOAGameMode_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
UPackage* Z_Construct_UPackage__Script_CoA();
// End Cross Module References

// Begin Class ACOAGameMode
void ACOAGameMode::StaticRegisterNativesACOAGameMode()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ACOAGameMode);
UClass* Z_Construct_UClass_ACOAGameMode_NoRegister()
{
	return ACOAGameMode::StaticClass();
}
struct Z_Construct_UClass_ACOAGameMode_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "COAGameMode.h" },
		{ "ModuleRelativePath", "Public/COAGameMode.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ACOAGameMode>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_ACOAGameMode_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AGameModeBase,
	(UObject* (*)())Z_Construct_UPackage__Script_CoA,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ACOAGameMode_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_ACOAGameMode_Statics::ClassParams = {
	&ACOAGameMode::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x009002ACu,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ACOAGameMode_Statics::Class_MetaDataParams), Z_Construct_UClass_ACOAGameMode_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_ACOAGameMode()
{
	if (!Z_Registration_Info_UClass_ACOAGameMode.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ACOAGameMode.OuterSingleton, Z_Construct_UClass_ACOAGameMode_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_ACOAGameMode.OuterSingleton;
}
template<> COA_API UClass* StaticClass<ACOAGameMode>()
{
	return ACOAGameMode::StaticClass();
}
ACOAGameMode::ACOAGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(ACOAGameMode);
ACOAGameMode::~ACOAGameMode() {}
// End Class ACOAGameMode

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_Alimert_Documents_Unreal_Projects_CoA_Source_CoA_Public_COAGameMode_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_ACOAGameMode, ACOAGameMode::StaticClass, TEXT("ACOAGameMode"), &Z_Registration_Info_UClass_ACOAGameMode, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ACOAGameMode), 3355808828U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_Alimert_Documents_Unreal_Projects_CoA_Source_CoA_Public_COAGameMode_h_1021489589(TEXT("/Script/CoA"),
	Z_CompiledInDeferFile_FID_Users_Alimert_Documents_Unreal_Projects_CoA_Source_CoA_Public_COAGameMode_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_Alimert_Documents_Unreal_Projects_CoA_Source_CoA_Public_COAGameMode_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
