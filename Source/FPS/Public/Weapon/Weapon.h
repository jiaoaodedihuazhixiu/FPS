// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Data/WeaponData.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class FPS_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeapon();
	
	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetFirstPersonMesh() const;	
	
	UFUNCTION(BlueprintCallable)	
	USkeletalMeshComponent* GetThirdPersonMesh() const;	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FPS|WeaponData")
	UWeaponData* WeaponData;
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "FPS|WeaponType")
	FGameplayTag WeaponType;
	

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> FirstPersonMesh;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> ThirdPersonMesh;
	
};
