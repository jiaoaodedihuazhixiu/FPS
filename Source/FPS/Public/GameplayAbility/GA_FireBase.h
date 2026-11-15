// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GASGameplayAbilityBase.h"
#include "GA_FireBase.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UGA_FireBase : public UGASGameplayAbilityBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Weapon|Fire")
	void StartFire();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Weapon|Fire")
	void StopFire();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Weapon|Fire")
	void FireOnce();
};
