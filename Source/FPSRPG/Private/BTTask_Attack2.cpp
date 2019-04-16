// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Attack2.h"
#include "ZombieAIController2.h"
#include "Zombie2.h"

UBTTask_Attack2::UBTTask_Attack2()
{
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UBTTask_Attack2::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Zombie = Cast<AZombie2>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Zombie)
		return EBTNodeResult::Failed;


	Zombie->Attack();
	IsAttacking = true;

	//UE_LOG(LogTemp, Log, TEXT("ZombieAttack"));


	Zombie->OnAttackEnd.AddLambda([this]() -> void {
		IsAttacking = false;
	});
	return EBTNodeResult::InProgress;


}

void UBTTask_Attack2::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		//UE_LOG(LogTemp, Log, TEXT("AttackEnd"));
	}
}




