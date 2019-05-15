// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_BossAttack3.h"
#include "BossZombieAIController.h"
#include "BossZombie.h"

UBTTask_BossAttack3::UBTTask_BossAttack3()
{
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UBTTask_BossAttack3::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Zombie = Cast<ABossZombie>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Zombie)
		return EBTNodeResult::Failed;


	Zombie->Attack3();
	IsAttacking = true;

	//UE_LOG(LogTemp, Log, TEXT("ZombieAttack"));


	Zombie->OnAttackEnd.AddLambda([this]() -> void {
		IsAttacking = false;
	});
	return EBTNodeResult::InProgress;


}

void UBTTask_BossAttack3::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		//UE_LOG(LogTemp, Log, TEXT("AttackEnd"));
	}
}




