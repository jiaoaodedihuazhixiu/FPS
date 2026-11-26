// Copyright Druid Mechanics


#include "Character/GASCharacter.h"

#include "EnhancedInputComponent.h"
#include "AbilitySystem/GameplayAbilities/GA_Aim_Base.h"
#include "AbilitySystem/GameplayAbilities/GA_Fire_Base.h"
#include "Camera/CameraComponent.h"
#include "Components/SlateWrapperTypes.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AGASCharacter::AGASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// FP SpringArm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 0.f;
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 15.f;
	SpringArm->bUsePawnControlRotation = true;
	
	// FP Camera
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(FName("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(SpringArm);
	FirstPersonCamera->bUsePawnControlRotation = false;
	
	// FP Arms
	FirstPersonArms = CreateDefaultSubobject<USkeletalMeshComponent>(FName("FirstPersonArms"));
	FirstPersonArms->SetupAttachment(FirstPersonCamera);
	FirstPersonArms->bOnlyOwnerSee = true;
	FirstPersonArms->bOwnerNoSee = false;
	FirstPersonArms->bCastDynamicShadow = false;
	FirstPersonArms->bReceivesDecals = false;
	FirstPersonArms->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	FirstPersonArms->PrimaryComponentTick.TickGroup = TG_PrePhysics; // Tick before the camera so that the animation is up to date when the camera renders
	
	// Get normal Character Mesh that others can see and set it to not be visible to the owner
	GetMesh()->bOnlyOwnerSee = false;
	GetMesh()->bOwnerNoSee = true;
	GetMesh()->bReceivesDecals = false;
	
	// Add the Ability System Component
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(ASCReplicationMode);
	
	// Component for Combat
	CombatComponent = CreateDefaultSubobject<UCombat>(TEXT("CombatComponent"));
	CombatComponent->SetIsReplicated(true);
}

// Called every frame
void AGASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* CombatInputComponent = CastChecked<UEnhancedInputComponent>(GetOwner()->InputComponent);
	
	/*  战斗相关输入绑定  */
	CombatInputComponent->BindAction(IA_CycleWeapon, ETriggerEvent::Started, this, &AGASCharacter::Input_CycleWeapon);
	CombatInputComponent->BindAction(IA_Aim, ETriggerEvent::Started, this, &AGASCharacter::Input_Aim_Pressed);
	CombatInputComponent->BindAction(IA_Aim, ETriggerEvent::Completed, this, &AGASCharacter::Input_Aim_Released);
	CombatInputComponent->BindAction(IA_Fire, ETriggerEvent::Started, this, &AGASCharacter::Input_Fire_Pressed);
	CombatInputComponent->BindAction(IA_Fire, ETriggerEvent::Completed,this, &AGASCharacter::Input_Fire_Released);
}

/*  只作为转发函数调用CombatComponent里的Initiate函数  */
void AGASCharacter::Input_CycleWeapon()
{
	CombatComponent->Initiate_CycleWeapon();
}

void AGASCharacter::Input_Aim_Pressed()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f,
			FColor::Red,
			TEXT("Input_Aim_Pressed"));
	CombatComponent->Initiate_Aim_Pressed();
}

void AGASCharacter::Input_Aim_Released()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f,
			FColor::Red,
			TEXT("Input_Aim_Released"));
	CombatComponent->Initiate_Aim_Released();
}

void AGASCharacter::Input_Fire_Pressed()
{
	CombatComponent->Initiate_Fire_Pressed();
}

void AGASCharacter::Input_Fire_Released()
{
	CombatComponent->Initiate_Fire_Released();
}


// return AbilitySystemComponent
UAbilitySystemComponent* AGASCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGASCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	
		if (HasAuthority())
		{
			// demo 临时：现在没有 GA 蓝图子类，直接授 C++ 类
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(UGA_Fire_Base::StaticClass(), 1,
					(int32)EFPSInputID::Fire));
		}
	}
	
	if (IsValid(CombatComponent))
	{
		CombatComponent->SpawnInventoryWeapons();
	}
	
	if (AbilitySystemComponent)
	{
		if (HasAuthority())
		{
			// demo 临时：现在没有 GA 蓝图子类，直接授 C++ 类
			
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(UGA_Aim_Base::StaticClass(), 1,
					(int32)EFPSInputID::Aim));
		}
	}
}

void AGASCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

USkeletalMeshComponent* AGASCharacter::GetFirstPersonArms_Implementation() const
{
	return FirstPersonArms;
}

USkeletalMeshComponent* AGASCharacter::GetThirdPersonMesh_Implementation() const
{
	return GetMesh();
}

void AGASCharacter::BeginDestroy()
{
	Super::BeginDestroy();
	
	if (CombatComponent)
	{
		CombatComponent->DestroyInventoryWeapons();
	}
}

