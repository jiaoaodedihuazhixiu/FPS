// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbility.h"
#include "Engine/DataAsset.h"
#include "WeaponData.generated.h"


/**
 * 
 */
USTRUCT(BlueprintType)
struct FPlayerAnims
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> IdleAnim = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> AimIdleAnim = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> CrouchIdleAnim = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> SpringAnim = nullptr;
	
	// Only for First Person Arms
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UBlendSpace> AimOffset_Hip;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)	
	TObjectPtr<UBlendSpace> AimOffset_Aim;
	
	// Only for Third Person Arms
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UBlendSpace> Strafe_Standing;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UBlendSpace> Strafe_Crouching;
};

USTRUCT(BlueprintType)
struct FMontages
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> EquipMontage = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> FireMontage = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> ReloadMontage = nullptr;
};

UCLASS()
class FPS_API UWeaponData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim|FirstPerson")
	FPlayerAnims FirstPersonAnims;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim|ThirdPerson")	
	FPlayerAnims ThirdPersonAnims;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim|Montage")
	FMontages FirstPersonMontages;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim|Montage")
	FMontages ThirdPersonMontages;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim|Montage")
	FMontages WeaponMontages;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	TArray<TSubclassOf<UGameplayAbility>> AbilitiesToGrant;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FName GripPoints;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Range")
	float Range = 5000.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TraceRadius")
	float TraceRadius = 5.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aim")
	float AimFOV = 90.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FABRIK")
	FName FABRIK_SocketName = "FABRIK_Socket";
};
