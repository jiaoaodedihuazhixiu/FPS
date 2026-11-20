// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Base.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UGA_Base : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS|UI")
	bool bShouldShowOnHUD = false;
};
