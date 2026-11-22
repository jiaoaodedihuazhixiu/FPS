// Copyright Druid Mechanics


#include "AbilitySystem/AttributeSets/FPSAttributeSet.h"

#include "Net/UnrealNetwork.h"

UFPSAttributeSet::UFPSAttributeSet()
{
	InitHealth(100.f);
	InitMaxHealth(100.f);
	InitMoveSpeed(600.f);
	InitMaxMoveSpeed(600.f);
}

void UFPSAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	/*DOREPLIFETIME_CONDITION_NOTIFY宏是 UE 属性复制系统的"注册入口"——告诉引擎：
	 *这个 UPROPERTY 要参与网络复制，
	 *按什么条件复制，
	 *复制到客户端后要不要触发 OnRep 回调。*/
	DOREPLIFETIME_CONDITION_NOTIFY(UFPSAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFPSAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFPSAttributeSet, MoveSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFPSAttributeSet, MaxMoveSpeed, COND_None, REPNOTIFY_Always);
}

/*  GAMEPLAYATTRIBUTE_REPNOTIFY 会通知 ASC 属性发生了变化，这样客户端上监听属性变化的委托（比如血条 UI）才会被触发  */
void UFPSAttributeSet::OnRep_Health(const FGameplayAttributeData& Prev) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFPSAttributeSet, Health, Prev);
}

void UFPSAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& Prev) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFPSAttributeSet, MaxHealth, Prev);
}

void UFPSAttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData& Prev) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFPSAttributeSet, MoveSpeed, Prev);
}

void UFPSAttributeSet::OnRep_MaxMoveSpeed(const FGameplayAttributeData& Prev) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFPSAttributeSet, MaxMoveSpeed, Prev);
}
