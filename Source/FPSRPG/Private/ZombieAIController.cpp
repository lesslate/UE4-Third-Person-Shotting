// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "ConstructorHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "FPSGameMode.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "FPSPlayer.h"
#include "DrawDebugHelpers.h"

const FName AZombieAIController::HomePosKey(TEXT("HomePos"));
const FName AZombieAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AZombieAIController::TargetKey(TEXT("Target"));

AZombieAIController::AZombieAIController()
{
	// BlackBoard 초기화
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("BlackboardData'/Game/AI/BB_Zombie.BB_Zombie'")); 
	if (BBObject.Succeeded())
	{
		BBZombie = BBObject.Object;
	}
	// BehaviorTree 초기화
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/AI/BT_Zombie.BT_Zombie'"));
	if (BTObject.Succeeded())
	{
		BTZombie = BTObject.Object;
	}
	Radius = 1000;
	
}

void AZombieAIController::Possess(APawn * InPawn)
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

	AFPSGameMode* gameMode = Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (gameMode != nullptr)
	{
		gameMode->OnAggro.AddUObject(this, &AZombieAIController::SetRadius);
	}
}


void AZombieAIController::SetRadius()
{
	APawn* Pawn = GetPawn();
	if (!IsValid(Pawn))
	{
		return;
	}
	FVector Center = Pawn->GetActorLocation();
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam;
	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(3500.0f),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto OverlapResult : OverlapResults)
		{
			if (OverlapResult.GetActor()->ActorHasTag("Player")&&Radius!=3500.0f)
			{
				DrawDebugSphere(GetWorld(), Center, 3500.0f, 16, FColor::Green, false, 0.2f);
				Radius = 3500.0f;
			}

		}
	}

	
}

void AZombieAIController::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}
