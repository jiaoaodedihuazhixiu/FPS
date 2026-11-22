// Copyright Druid Mechanics


#include "Weapon/Weapon.h"

#include "Character/GASCharacter.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	bReplicates = true;
	bNetUseOwnerRelevancy = true;
	
	WeaponRoot = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponRoot"));
	SetRootComponent(WeaponRoot);
	
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FirstPersonMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	FirstPersonMesh->bReceivesDecals = false;
	FirstPersonMesh->CastShadow = false;
	FirstPersonMesh->SetHiddenInGame(true);
	FirstPersonMesh->SetupAttachment(WeaponRoot);
	
	ThirdPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ThirdPersonMesh"));
	ThirdPersonMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	ThirdPersonMesh->bReceivesDecals = false;
	ThirdPersonMesh->CastShadow = true;
	ThirdPersonMesh->SetHiddenInGame(true);	
	ThirdPersonMesh->SetupAttachment(WeaponRoot);
}

void AWeapon::OnRep_Instigator()
{
	Super::OnRep_Instigator();
	AttachToOwingPawn();
}

void AWeapon::AttachToOwingPawn()
{
	APawn* OwningCharacter = GetInstigator();
	if (!IsValid(OwningCharacter) || !OwningCharacter->Implements<UPlayerInterface>())
	{
		return;
	}
	
	SetMeshVisibility(OwningCharacter);
	
	if (!IsValid(WeaponData))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: WeaponData 未配置，跳过挂点"), *GetName());
		return;
	}
	
	const FName AttachSocketName = WeaponData->GripPoints;
	USkeletalMeshComponent* CharacterFirstPersonMesh = IPlayerInterface::Execute_GetFirstPersonArms(OwningCharacter);
	USkeletalMeshComponent* CharacterThirdPersonMesh = IPlayerInterface::Execute_GetThirdPersonMesh(OwningCharacter);
	
	if (CharacterFirstPersonMesh)
	{
		FirstPersonMesh->AttachToComponent(CharacterFirstPersonMesh, FAttachmentTransformRules::KeepRelativeTransform, AttachSocketName);
	}
	if (CharacterThirdPersonMesh)
	{
		ThirdPersonMesh->AttachToComponent(CharacterThirdPersonMesh, FAttachmentTransformRules::KeepRelativeTransform, AttachSocketName);
	}
}

void AWeapon::SetMeshVisibility(APawn* OwningPawn)
{
	if (OwningPawn->IsLocallyControlled())
	{
		FirstPersonMesh->SetHiddenInGame(false);
		ThirdPersonMesh->SetHiddenInGame(true);
	}
	else
	{
		FirstPersonMesh->SetHiddenInGame(true);
		ThirdPersonMesh->SetHiddenInGame(false);
	}
}


// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	
}

