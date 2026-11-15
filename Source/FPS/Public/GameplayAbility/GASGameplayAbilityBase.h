// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GASGameplayAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UGASGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	bool bShouldShowOnHUD = false;
};
