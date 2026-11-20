// Copyright Druid Mechanics


#include "Component/Combat.h"



UCombat::UCombat()
{

	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCombat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

void UCombat::Initiate_CycleWeapon()
{
	GEngine->AddOnScreenDebugMessage(
		-1, 
		5.f, 
		FColor::Green, 
		TEXT("Initiate_CycleWeapon"),
		false);
}

void UCombat::Initiate_Aim()
{
	GEngine->AddOnScreenDebugMessage(
		-1, 
		5.f, 
		FColor::Green, 
		TEXT("Initiate_Aim"),
		false);
}

void UCombat::Initiate_Fire_Pressed()
{
	GEngine->AddOnScreenDebugMessage(
		-1, 
		5.f, 
		FColor::Green, 
		TEXT("Initiate_Fire_Pressed"), 
		false);
}

void UCombat::Initiate_Fire_Released()
{
	GEngine->AddOnScreenDebugMessage(
		-1, 
		5.f, 
		FColor::Green, 
		TEXT("Initiate_Fire_Released"), 
		false);
}

