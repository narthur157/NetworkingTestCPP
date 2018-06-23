// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/TargetTypes.h"


bool FTargetDataHandle::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	uint8 DataNum;
	if (Ar.IsSaving())
	{
		//UE_LOG(Data.Num() > MAX_uint8, LogAbilitySystem, Warning, TEXT("Too many TargetData sources (%d!) to net serialize. Clamping to %d"), Data.Num(), MAX_uint8);
		DataNum = FMath::Min<int32>(Data.Num(), MAX_uint8);
	}
	Ar << DataNum;
	if (Ar.IsLoading())
	{
		Data.SetNumZeroed(DataNum);
	}

	for (int32 i = 0; i < DataNum && !Ar.IsError(); ++i)
	{
		TCheckedObjPtr<UScriptStruct> ScriptStruct = Data[i].IsValid() ? Data[i]->GetScriptStruct() : NULL;

		Ar << ScriptStruct;

		if (ScriptStruct.IsValid())
		{
			if (Ar.IsLoading())
			{
				// For now, just always reset/reallocate the data when loading.
				// Longer term if we want to generalize this and use it for property replication, we should support
				// only reallocating when necessary
				check(!Data[i].IsValid());

				FTargetData * NewData = (FTargetData*)FMemory::Malloc(ScriptStruct->GetCppStructOps()->GetSize());
				ScriptStruct->InitializeStruct(NewData);

				Data[i] = TSharedPtr<FTargetData>(NewData);
			}

			void* ContainerPtr = Data[i].Get();

			if (ScriptStruct->StructFlags & STRUCT_NetSerializeNative)
			{
				ScriptStruct->GetCppStructOps()->NetSerialize(Ar, Map, bOutSuccess, Data[i].Get());
			}
			else
			{
				// This won't work since UStructProperty::NetSerializeItem is deprecrated.
				//	1) we have to manually crawl through the topmost struct's fields since we don't have a UStructProperty for it (just the UScriptProperty)
				//	2) if there are any UStructProperties in the topmost struct's fields, we will assert in UStructProperty::NetSerializeItem.

				//ABILITY_LOG(Fatal, TEXT("FGameplayAbilityTargetDataHandle::NetSerialize called on data struct %s without a native NetSerialize"), *ScriptStruct->GetName());

				for (TFieldIterator<UProperty> It(ScriptStruct.Get()); It; ++It)
				{
					if (It->PropertyFlags & CPF_RepSkip)
					{
						continue;
					}

					void* PropertyData = It->ContainerPtrToValuePtr<void*>(ContainerPtr);

					It->NetSerializeItem(Ar, Map, PropertyData);
				}
			}
		}
		else if (ScriptStruct.IsError())
		{
#if !UE_BUILD_SHIPPING
			//ABILITY_LOG(Error, TEXT("FGameplayAbilityTargetDataHandle::NetSerialize: Bad ScriptStruct serialized, can't recover."))
#endif

				Ar.SetError();
		}
	}

	//ABILITY_LOG(Warning, TEXT("FGameplayAbilityTargetDataHandle Serialized: %s"), ScriptStruct ? *ScriptStruct->GetName() : TEXT("NULL") );

	bOutSuccess = true;
	return true;
}


bool FTargetData_Projectile::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	Ar << SourceLocation;
	Ar << ChargeTime;

	bOutSuccess = true;
	return true;
}