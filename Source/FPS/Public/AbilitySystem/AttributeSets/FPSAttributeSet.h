// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "FPSAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UFPSAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UFPSAttributeSet();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Health Attributes
	UPROPERTY(BlueprintReadOnly, Category="Attribute|Health", ReplicatedUsing=OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS_BASIC(UFPSAttributeSet, Health);
	
	UPROPERTY(BlueprintReadOnly, Category="Attribute|Health", ReplicatedUsing=OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS_BASIC(UFPSAttributeSet, MaxHealth);
	
	// Move Speed Attributes
	UPROPERTY(BlueprintReadOnly, Category="Attribute|MoveSpeed", ReplicatedUsing=OnRep_MoveSpeed)
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS_BASIC(UFPSAttributeSet, MoveSpeed);
	
	UPROPERTY(BlueprintReadOnly, Category="Attribute|MoveSpeed", ReplicatedUsing=OnRep_MaxMoveSpeed)
	FGameplayAttributeData MaxMoveSpeed;
	ATTRIBUTE_ACCESSORS_BASIC(UFPSAttributeSet, MaxMoveSpeed);
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData &Prev) const;
	
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData &Prev) const;
	
	UFUNCTION()
	void OnRep_MoveSpeed(const FGameplayAttributeData &Prev) const;
	
	UFUNCTION()
	void OnRep_MaxMoveSpeed(const FGameplayAttributeData &Prev) const;
};
