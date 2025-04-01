// Fill out your copyright notice in the Description page of Project Settings.

#include "GuardAIController.h"
#include "GuardCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

DEFINE_LOG_CATEGORY(LogAI);

AGuardAIController::AGuardAIController(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer)
{
	m_pPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component"));
	
	SetupHearingSystem();
}

void AGuardAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	m_ControlledEnemy = Cast<AGuardCharacter>(InPawn);

	const AGuardCharacter* EnemyCharacter {Cast<AGuardCharacter>(InPawn)};
	
	if (EnemyCharacter)
	{
		UBehaviorTree* BehaviourTree {EnemyCharacter->GetBehaviorTree()};
		
		if (BehaviourTree)
		{
			UBlackboardComponent* BlackboardComponent;
			UseBlackboard(BehaviourTree->BlackboardAsset, BlackboardComponent);
			Blackboard = BlackboardComponent;
			RunBehaviorTree(BehaviourTree);
		}
	}
}

void AGuardAIController::SetupHearingSystem()
{
	m_pHearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	if (m_pHearingConfig)
	{
		SetPerceptionComponent(*m_pPerceptionComponent);
		
		m_pHearingConfig->HearingRange = 1500.0f;
		m_pHearingConfig->SetMaxAge(5.0f);
		m_pHearingConfig->DetectionByAffiliation.bDetectEnemies = true;
		m_pHearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
		m_pHearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->SetDominantSense(*m_pHearingConfig->GetSenseImplementation());
		m_pPerceptionComponent->ConfigureSense(*m_pHearingConfig);
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AGuardAIController::OnTargetDetected);
	}
}

void AGuardAIController::OnTargetDetected(AActor* Actor, const FAIStimulus stimulus)
{
	if (!stimulus.WasSuccessfullySensed())
	{
		return;
	}
	
	if (stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
	{
		float newSoundVolume = stimulus.Strength;
		
		if (newSoundVolume > m_CurrentSoundVolume)
		{
			m_SoundLocation = stimulus.StimulusLocation;
        
			m_CurrentSoundVolume = newSoundVolume;
        
			if (Blackboard)
			{
				Blackboard->SetValueAsBool("CanHearSomething", false);
				Blackboard->SetValueAsBool("IsInvestigating", false);
				
				Blackboard->SetValueAsVector("LastHeardLocation", m_SoundLocation);

				AGuardCharacter* NPC{Cast<AGuardCharacter>(GetPawn())};

				int32 RandomFearIncrease = FMath::RandRange(1, 3 + m_HeardTimes);
				NPC->AddFearLevel(RandomFearIncrease);
				
				Blackboard->SetValueAsBool("CanHearSomething", true);

				++m_HeardTimes;
			}
		}
	}
}
