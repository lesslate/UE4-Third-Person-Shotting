// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieAIController2.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "ConstructorHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "FPSGameMode.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

const FName AZombieAIController2::HomePosKey(TEXT("HomePos"));
const FName AZombieAIController2::PatrolPosKey(TEXT("PatrolPos"));
const FName AZombieAIController2::TargetKey(TEXT("Target"));

AZombieAIController2::AZombieAIController2()
{
	// BlackBoard 초기화
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("BlackboardData'/Game/AI/BB_Zombie2.BB_Zombie2'"));
	if (BBObject.Succeeded())
	{
		BBZombie = BBObject.Object;
	}
	// BehaviorTree 초기화
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/AI/BT_Zombie2.BT_Zombie2'"));
	if (BTObject.Succeeded())
	{
		BTZombie = BTObject.Object;
	}
	Radius = 1500;
}

void AZombieAIController2::Possess(APawn * InPawn)
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
		gameMode->OnAggro.AddUObject(this, &AZombieAIController2::SetRadius);
	}
}

void AZombieAIController2::SetRadius()
{
	FVector Center = GetPawn()->GetActorLocation();
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam;
	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_Visibility,
		FCollisionShape::MakeSphere(3500.0f),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto OverlapResult : OverlapResults)
		{
			if (OverlapResult.GetActor()->ActorHasTag("Player")&& Radius != 3500.0f)
			{
				DrawDebugSphere(GetWorld(), Center, 3500.0f, 16, FColor::Green, false, 0.2f);
				Radius = 3500.0f;
			}

		}
	}
}

void AZombieAIController2::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}




