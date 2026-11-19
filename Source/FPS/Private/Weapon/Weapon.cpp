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
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->SetupAttachment(WeaponRoot);
	
	ThirdPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ThirdPersonMesh"));
	ThirdPersonMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	ThirdPersonMesh->bReceivesDecals = false;
	ThirdPersonMesh->CastShadow = true;
	ThirdPersonMesh->SetOwnerNoSee(true);	
	ThirdPersonMesh->SetupAttachment(WeaponRoot);
}

USkeletalMeshComponent* AWeapon::GetFirstPersonMesh() const
{
	return FirstPersonMesh;
}

USkeletalMeshComponent* AWeapon::GetThirdPersonMesh() const
{
	return ThirdPersonMesh;
}

FName AWeapon::GetGripSocketName() const
{
	if (!WeaponData)
	{
		return NAME_None;
	}
	
	return WeaponData->GripPoints;
}

void AWeapon::AttachToOwner()
{
	AGASCharacter* OwnerCharacter = Cast<AGASCharacter>(GetOwner());
	if (!OwnerCharacter)
	{
		ResetAttachment();
		return;
	}
	
	USkeletalMeshComponent* FirstPersonArms = OwnerCharacter->GetFirstPersonArms();
	USkeletalMeshComponent* ThirdPersonBody = OwnerCharacter->GetMesh();
	if (!FirstPersonArms || !ThirdPersonBody)
	{
		ResetAttachment();
		return;
	}
	
	const FName GripSocket = GetGripSocketName();
	
	// ① Actor 级：挂到身体骨骼的握把 socket。
	//    只在服务器做 —— 它会走 FRepAttachment 自动复制到客户端。
	if (HasAuthority())
	{
		AttachToComponent(ThirdPersonBody, FAttachmentTransformRules::SnapToTargetNotIncludingScale, GripSocket);
	}

	// ② 组件级：FP mesh 单独挂到第一人称手臂。
	//    两端都要做 —— 组件挂载（USceneComponent::AttachParent）默认不复制。
	if (FirstPersonMesh)
	{
		FirstPersonMesh->AttachToComponent(FirstPersonArms, FAttachmentTransformRules::KeepRelativeTransform, GripSocket);
	}

	// ③ ThirdPersonMesh 不用动：它是 WeaponRoot 的子级，跟着 ① 走。
}

void AWeapon::ResetAttachment()
{
	if (FirstPersonMesh && FirstPersonMesh->GetAttachParent() != WeaponRoot)
	{
		// 掉枪/没 owner：把 FP mesh 收回到武器自己的 root 下
		FirstPersonMesh->AttachToComponent(WeaponRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}

	if (GetRootComponent() && GetRootComponent()->GetAttachParent())
	{
		// 将 Actor 的根组件从其当前附加的任意场景组件上分离
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
}

void AWeapon::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);

	// SpawnActorDeferred 里设 Owner 时组件还没注册，交给 BeginPlay 统一挂
	if (HasActorBegunPlay())
	{
		AttachToOwner();
	}
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	AttachToOwner();
}

void AWeapon::OnRep_Owner()
{
	Super::OnRep_Owner();
	
	AttachToOwner();
}
