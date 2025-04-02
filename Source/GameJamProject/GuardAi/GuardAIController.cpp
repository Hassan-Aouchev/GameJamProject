// Fill out your copyright notice in the Description page of Project Settings.

#include "GuardAIController.h"
#include "GuardCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"

AGuardAIController::AGuardAIController(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer)
{
	m_pPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component"));
	
	SetupHearingSystem();
}

void AGuardAIController::ResetHeardStimuli()
{
	m_PreviouslyHeardStimuli.Empty();
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
		TArray<FName> fearNames = m_ControlledEnemy->GetFearNames();

		// Check if we've already processed this stimulus tag
		if (!m_PreviouslyHeardStimuli.Contains(stimulus.Tag))
		{
			// Add to our set of heard stimuli
			m_PreviouslyHeardStimuli.Add(stimulus.Tag);
    
			if (fearNames.Contains(stimulus.Tag))
			{
				// Major fear reaction - this is something the guard specifically fears
				float newSoundVolume = stimulus.Strength;
				m_SoundLocation = stimulus.StimulusLocation;
				m_CurrentSoundVolume = newSoundVolume;
     
				if (Blackboard)
				{
					Blackboard->SetValueAsBool("CanHearSomething", false);
					Blackboard->SetValueAsBool("IsInvestigating", false);
    
					Blackboard->SetValueAsVector("LastHeardLocation", m_SoundLocation);
					AGuardCharacter* NPC{Cast<AGuardCharacter>(GetPawn())};
					int32 RandomFearIncrease = FMath::RandRange(2, 4);
					NPC->AddFearLevel(RandomFearIncrease);
    
					Blackboard->SetValueAsBool("CanHearSomething", true);
				}
			}
			else
			{
				// Minor fear reaction - not a specific fear but still causes some concern
				m_SoundLocation = stimulus.StimulusLocation;
      
				if (Blackboard)	
				{
					Blackboard->SetValueAsBool("CanHearSomething", false);
					Blackboard->SetValueAsBool("IsInvestigating", false);
           
					Blackboard->SetValueAsVector("LastHeardLocation", m_SoundLocation);
					AGuardCharacter* NPC{Cast<AGuardCharacter>(GetPawn())};
          
					// Smaller fear increase for non-specific sounds
					int32 MinorFearIncrease = FMath::RandRange(0, 1);
					NPC->AddFearLevel(MinorFearIncrease);
          
					Blackboard->SetValueAsBool("CanHearSomething", true);
				}
			}
		}
	}
}
