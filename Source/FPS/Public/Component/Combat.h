// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/Data/WeaponData.h"
#include "Combat.generated.h"


class AWeapon;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FPS_API UCombat : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombat();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	/*  战斗输入的入口，在这里面实现真正逻辑  */
	void Initiate_CycleWeapon();
	void Initiate_Aim();
	void Initiate_Fire_Pressed();
	void Initiate_Fire_Released();
	
	/* 暂时用不到 */
	// UPROPERTY(EditDefaultsOnly, Category = "FPS|Weapon")
	// TObjectPtr<UWeaponData> WeaponData;
	
	void EquipWeapon(AWeapon* WeaponToEquip);
	void SpawnInventoryWeapons();
	void DestroyInventoryWeapons();
	
protected:
	UPROPERTY(Transient, BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentWeapon)
	TObjectPtr<AWeapon> CurrentWeapon;
	
private:
	UFUNCTION()
	void OnRep_CurrentWeapon(AWeapon* PrevWeapon) const;
	
	UPROPERTY(Transient, Replicated)
	TArray<AWeapon*> InventoryWeapons;
	
	UPROPERTY(EditDefaultsOnly, Category = "FPS|Weapon")
	TArray<TSubclassOf<AWeapon>> DefaultWeaponClass;
	
	AWeapon* SpawnWeapon(TSubclassOf<AWeapon> WeaponClass) const;
};

