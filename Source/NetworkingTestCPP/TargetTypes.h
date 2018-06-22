// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TargetTypes.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FTargetData
{

	GENERATED_BODY()

	virtual ~FTargetData() {};

	UPROPERTY(BlueprintReadWrite)
	float ChargeTime;

	virtual bool HasTargetedActors()
	{
		return false;
	};

	virtual bool HasSourceLocation()
	{
		return false;

	};

	virtual bool HasTargetLocation()
	{
		return false;

	};

	virtual AActor* getTargetedActor()
	{
		return nullptr;
	};

	virtual TArray<AActor*> getTargetedActors()
	{ 
		TArray<AActor*> ZeroArray;
		return ZeroArray;
	};

	float getChargeTime()
	{
		return ChargeTime;
	}

	virtual FString ToString() const
	{
		return TEXT("FTargetData");
	}

	virtual FTransform getSourceLocation() { return FTransform();  };
	virtual FTransform getTargetLocation() { return FTransform(); };

	virtual UScriptStruct* GetScriptStruct() const
	{
		return FTargetData::StaticStruct();
	}

	virtual FTargetData* Clone() { return this; };
	
};

USTRUCT(BlueprintType)
struct FTargetDataHandle
{

	GENERATED_BODY()

	// convert to a shared_ptr maybe?
	TArray<TSharedPtr<FTargetData>, TInlineAllocator<1> >	Data;

	FTargetDataHandle() {};
	FTargetDataHandle(FTargetData *NewData)
	{
		makeData(NewData);
	}

	void makeData(FTargetData *NewData)
	{
		Data.Add(TSharedPtr<FTargetData>(NewData->Clone()));
		
	}

	FTargetData* getData()
	{
		if (Data.Num() != 0)
		{
			return Data[0].Get();
		}
		else
		{
			return nullptr;
		}

	}

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);


	bool operator==(const FTargetDataHandle& Other) const
	{
		// Both invalid structs or both valid and Pointer compare (???) // deep comparison equality
		if (Data.Num() != Other.Data.Num())
		{
			return false;
		}
		for (int32 i = 0; i < Data.Num(); ++i)
		{
			if (Data[i].IsValid() != Other.Data[i].IsValid())
			{
				return false;
			}
			if (Data[i].Get() != Other.Data[i].Get())
			{
				return false;
			}
		}
		return true;
	}

	/** Comparison operator */
	bool operator!=(const FTargetDataHandle& Other) const
	{
		return !(FTargetDataHandle::operator==(Other));
	}

};

template<>
struct TStructOpsTypeTraits < FTargetDataHandle > : public TStructOpsTypeTraitsBase2<FTargetDataHandle>
{
	enum
	{
		WithCopy = true,		// Necessary so that TSharedPtr<FGameplayAbilityTargetData> Data is copied around
		WithNetSerializer = true,
		WithIdenticalViaEquality = true,
	};
};

USTRUCT(BlueprintType)
struct FTargetData_Location : public FTargetData
{
	GENERATED_BODY()

	~FTargetData_Location() {};

	UPROPERTY(BlueprintReadWrite)
	FTransform SourceLocation;
	UPROPERTY(BlueprintReadWrite)
	FTransform TargetLocation;

	bool HasSourceLocation() override
	{
		return true;
	}
	
	bool HasTargetLocation() override
	{
		return true;
	}

	FTransform getSourceLocation() override
	{

		return SourceLocation;
	}

	FTransform getTargetLocation() override
	{

		return TargetLocation;
	}

	UScriptStruct* GetScriptStruct() const override
	{
		return FTargetData_Location::StaticStruct();
	}
};

USTRUCT(BlueprintType)
struct FTargetData_Actor : public FTargetData
{

	GENERATED_BODY()
	~FTargetData_Actor() {};

	UPROPERTY(BlueprintReadWrite)
	TArray<AActor*> TargetedActors;

	bool HasTargetedActors() override
	{
		if (TargetedActors.Num() > 0)
			return true;
		else
			return false;
	};

	TArray<AActor*> getTargetedActors() override
	{
		return TargetedActors;
	};

	 AActor* getTargetedActor() override
	 {
		if (HasTargetedActors())
		{
			return TargetedActors[0];

		}
		else
		{
			return nullptr;
		}
	};

	 UScriptStruct* GetScriptStruct() const override
	 {
		 return FTargetData_Actor::StaticStruct();
	 }

};

USTRUCT(BlueprintType)
struct FTargetData_Projectile : public FTargetData
{
	GENERATED_BODY()

	~FTargetData_Projectile() {};

	UPROPERTY(BlueprintReadWrite)
	FTransform SourceLocation = FTransform();


	bool HasSourceLocation() override
	{
		return true;
	}

	FTransform getSourceLocation() override
	{
		return SourceLocation;
	}

	FTargetData* Clone() override
	{
		FTargetData_Projectile* newProjectileData = new FTargetData_Projectile();
		newProjectileData->SourceLocation = SourceLocation;
		return newProjectileData;
	}

	// -------------------------------- 


	UScriptStruct* GetScriptStruct() const override
	{
		return FTargetData_Projectile::StaticStruct();
	}

	virtual FString ToString() const override
	{
		return TEXT("FTargetData_Projectile");
	}

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);
};

template<>
struct TStructOpsTypeTraits<FTargetData_Projectile> : public TStructOpsTypeTraitsBase2<FTargetData_Projectile>
{
	enum
	{
		WithNetSerializer = true	// For now this is REQUIRED for FGameplayAbilityTargetDataHandle net serialization to work
	};
};