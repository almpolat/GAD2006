// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "COAAvatar.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef COA_COAAvatar_generated_h
#error "COAAvatar.generated.h already included, missing '#pragma once' in COAAvatar.h"
#endif
#define COA_COAAvatar_generated_h

#define FID_Users_Alimert_Documents_Unreal_Projects_CoA_Source_CoA_Public_COAAvatar_h_15_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesACOAAvatar(); \
	friend struct Z_Construct_UClass_ACOAAvatar_Statics; \
public: \
	DECLARE_CLASS(ACOAAvatar, ACOABaseCharacter, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/CoA"), NO_API) \
	DECLARE_SERIALIZER(ACOAAvatar)


#define FID_Users_Alimert_Documents_Unreal_Projects_CoA_Source_CoA_Public_COAAvatar_h_15_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	ACOAAvatar(ACOAAvatar&&); \
	ACOAAvatar(const ACOAAvatar&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ACOAAvatar); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ACOAAvatar); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(ACOAAvatar) \
	NO_API virtual ~ACOAAvatar();


#define FID_Users_Alimert_Documents_Unreal_Projects_CoA_Source_CoA_Public_COAAvatar_h_12_PROLOG
#define FID_Users_Alimert_Documents_Unreal_Projects_CoA_Source_CoA_Public_COAAvatar_h_15_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_Alimert_Documents_Unreal_Projects_CoA_Source_CoA_Public_COAAvatar_h_15_INCLASS_NO_PURE_DECLS \
	FID_Users_Alimert_Documents_Unreal_Projects_CoA_Source_CoA_Public_COAAvatar_h_15_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> COA_API UClass* StaticClass<class ACOAAvatar>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_Alimert_Documents_Unreal_Projects_CoA_Source_CoA_Public_COAAvatar_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
