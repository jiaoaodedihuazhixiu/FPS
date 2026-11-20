// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Combat.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FPS_API UCombat : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombat();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	/*  战斗输入的入口，在这里面实现真正逻辑  */
	void Initiate_CycleWeapon();
	void Initiate_Aim();
	void Initiate_Fire_Pressed();
	void Initiate_Fire_Released();
};
