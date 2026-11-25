// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Base.generated.h"

/**
 * 能力输入 ID：ASC 靠这个 int32 把"哪个按键"和"哪份 spec"对起来。
 * 需要强制转换成int32
 */
UENUM(BlueprintType)
enum class EFPSInputID : uint8
{
	None        = 0,
	Fire        = 1,
	Aim         = 2,
	Reload      = 3,
	CycleWeapon = 4,
};

UCLASS()
class FPS_API UGA_Base : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS|UI")
	bool bShouldShowOnHUD = false;
};
