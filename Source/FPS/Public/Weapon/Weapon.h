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
	
	/** 当前武器对应的握把 socket：WeaponData.GripPoints[WeaponType] */
	UFUNCTION(BlueprintPure, Category = "FPS|Weapon")
	FName GetGripSocketName() const;

	/** 手动挂一次（BP 里 SetOwner 之后想立刻生效就直接调它） */
	UFUNCTION(BlueprintCallable, Category = "FPS|Weapon")
	void AttachToOwner();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FPS|WeaponData")
	UWeaponData* WeaponData;
	
	virtual void SetOwner(AActor* NewOwner) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnRep_Owner() override;
	
	UPROPERTY(EditAnywhere, Category = "FPS|WeaponType")
	FGameplayTag WeaponType;
	

private:
	/* 没有有效 owner 时把 mesh 收回来 */
	void ResetAttachment();
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> WeaponRoot;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> FirstPersonMesh;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> ThirdPersonMesh;
	
};
