// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieAnimInstance.h"
#include "ConstructorHelpers.h"
#include "Zombie.h"

UZombieAnimInstance::UZombieAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("AnimMontage'/Game/zombie/Zombie_Attack_mixamo_com_Montage.Zombie_Attack_mixamo_com_Montage'"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DEATH_MONTAGE(TEXT("AnimMontage'/Game/zombie/Zombie_Dying_mixamo_com_Montage.Zombie_Dying_mixamo_com_Montage'"));
	if (DEATH_MONTAGE.Succeeded())
	{
		DeathMontage = DEATH_MONTAGE.Object;
	}
}

void UZombieAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.0f);
}

void UZombieAnimInstance::PlayDeathMontage()
{
	Montage_Play(DeathMontage, 1.0f);
}

void UZombieAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}



