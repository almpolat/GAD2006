// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeCoA_init() {}
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_CoA;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_CoA()
	{
		if (!Z_Registration_Info_UPackage__Script_CoA.OuterSingleton)
		{
			static const UECodeGen_Private::FPackageParams PackageParams = {
				"/Script/CoA",
				nullptr,
				0,
				PKG_CompiledIn | 0x00000000,
				0x009EDC3D,
				0xE89F6886,
				METADATA_PARAMS(0, nullptr)
			};
			UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_CoA.OuterSingleton, PackageParams);
		}
		return Z_Registration_Info_UPackage__Script_CoA.OuterSingleton;
	}
	static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_CoA(Z_Construct_UPackage__Script_CoA, TEXT("/Script/CoA"), Z_Registration_Info_UPackage__Script_CoA, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0x009EDC3D, 0xE89F6886));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
