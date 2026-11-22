// Copyright Druid Mechanics


#include "Component/Combat.h"

#include "Net/UnrealNetwork.h"
#include "Weapon/Weapon.h"


UCombat::UCombat()
{

	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCombat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

void UCombat::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UCombat, InventoryWeapons);
	DOREPLIFETIME(UCombat, CurrentWeapon);
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

void UCombat::EquipWeapon(AWeapon* WeaponToEquip)
{
	/* 导致属性更改以触发OnRep函数 */
	CurrentWeapon = WeaponToEquip;
	CurrentWeapon->AttachToOwingPawn();
}

void UCombat::SpawnInventoryWeapons()
{
	/* 因为此时在服务器端生成，
	 * 在运行时如果开全客户端会生成DefaultWeaponClass里的所有武器*/
	if (GetOwner()->GetLocalRole() < ROLE_Authority) return;
	
	for (TSubclassOf<AWeapon>& WeaponClass : DefaultWeaponClass)
	{
		AWeapon* NewWeapon = SpawnWeapon(WeaponClass);
		InventoryWeapons.AddUnique(NewWeapon);
	}
	
	if (InventoryWeapons.Num() > 0)
	{
		EquipWeapon(InventoryWeapons[0]);
	}
}

void UCombat::DestroyInventoryWeapons()
{
	// No Inventory Now
	for (AWeapon* Weapon : InventoryWeapons)
	{
		if (IsValid(Weapon))
		{
			Weapon->Destroy();
		}
	}
}

void UCombat::OnRep_CurrentWeapon(AWeapon* PrevWeapon) const
{
	if (!IsValid(CurrentWeapon)) return;
	
	CurrentWeapon->AttachToOwingPawn();
}

AWeapon* UCombat::SpawnWeapon(TSubclassOf<AWeapon> WeaponClass) const
{
	AActor* OwningActor = GetOwner();
	if (!OwningActor)
	{
		return nullptr;
	}
	
	if (OwningActor->GetLocalRole() < ROLE_Authority)
	{
		return nullptr;
	}
	
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = Cast<APawn>(OwningActor);
	SpawnInfo.Owner = OwningActor;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	return GetWorld()->SpawnActor<AWeapon>(WeaponClass, SpawnInfo);
}

