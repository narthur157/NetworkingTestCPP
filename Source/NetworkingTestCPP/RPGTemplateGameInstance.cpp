// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGTemplateGameInstance.h"
#include "NetworkingTestCPP.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"



URPGTemplateGameInstance::URPGTemplateGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UMaterial> materialOb_DefaultSpellTargetingDecal(TEXT("Material'/Game/ThirdPersonCPP/Spell/SpellMaterials/Targeting/Spell_MainTargetingDecalAOE.Spell_MainTargetingDecalAOE'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> materialOb_DefaultSpellDecal(TEXT("Material'/Game/ThirdPersonCPP/Spell/SpellMaterials/Targeting/Spell_MainDecalAOE.Spell_MainDecalAOE'"));
	
	if (materialOb_DefaultSpellTargetingDecal.Object != nullptr)
	{

		DefaultSpellTargetingDecalAOE = materialOb_DefaultSpellTargetingDecal.Object;


	}

	if (materialOb_DefaultSpellDecal.Object != nullptr)
	{

		DefaultSpellDecalAOE = materialOb_DefaultSpellDecal.Object;


	}



}


UMaterial* URPGTemplateGameInstance::getDefaultTargetingDecalAOE()
{
	

	return DefaultSpellTargetingDecalAOE;



}


UMaterial* URPGTemplateGameInstance::getDefaultDecalAOE()
{


	return DefaultSpellDecalAOE;



}