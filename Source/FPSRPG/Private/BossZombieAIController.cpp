// Fill out your copyright notice in the Description page of Project Settings.

#include "BossZombieAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "ConstructorHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "FPSGameMode.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

const FName ABossZombieAIController::HomePosKey(TEXT("HomePos"));
const FName ABossZombieAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName ABossZombieAIController::TargetKey(TEXT("Target"));
const FName ABossZombieAIController::TaskNumberKey(TEXT("TaskNumber"));

ABossZombieAIController::ABossZombieAIController()
{
	// BlackBoard 초기화
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("BlackboardData'/Game/AI/BB_BossZombie.BB_BossZombie'"));
	if (BBObject.Succeeded())
	{
		BBZombie = BBObject.Object;
	}
	// BehaviorTree 초기화
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/AI/BT_BossZombie.BT_BossZombie'"));
	if (BTObject.Succeeded())
	{
		BTZombie = BTObject.Object;
	}
	Radius = 1500; 
}

void ABossZombieAIController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);
	if (UseBlackboard(BBZombie, Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		if (!RunBehaviorTree(BTZombie))
		{
			UE_LOG(LogTemp, Log, TEXT("BT NOT FOUND"));
		}
	}

}



void ABossZombieAIController::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}








