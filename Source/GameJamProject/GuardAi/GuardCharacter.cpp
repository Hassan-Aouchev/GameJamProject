// Fill out your copyright notice in the Description page of Project Settings.

#include "GuardCharacter.h"
#include "GuardAIController.h"
#include "PathPoints.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameJamProject/GhostCharacter.h"
#include "Kismet/GameplayStatics.h"

AGuardCharacter::AGuardCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	m_SpreadFearCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SpreadFearCapsule"));
	m_SpreadFearCapsule->InitCapsuleSize(m_Vicinity, m_Vicinity);
	m_SpreadFearCapsule->SetCollisionProfileName(TEXT("Pawn"));
	m_SpreadFearCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_SpreadFearCapsule->SetCollisionObjectType(ECC_WorldDynamic);
	m_SpreadFearCapsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	m_SpreadFearCapsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	m_SpreadFearCapsule->SetupAttachment(GetMesh());
	
	m_PlayerDetectedCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("PlayerDetectedCapsule"));
	m_PlayerDetectedCapsule->InitCapsuleSize(DetectionRadius, DetectionRadius);
	m_PlayerDetectedCapsule->SetCollisionProfileName(TEXT("Pawn"));
	m_PlayerDetectedCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_PlayerDetectedCapsule->SetCollisionObjectType(ECC_WorldDynamic);
	m_PlayerDetectedCapsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	m_PlayerDetectedCapsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	m_PlayerDetectedCapsule->SetupAttachment(GetMesh());
}

void AGuardCharacter::BeginPlay()
{
	Super::BeginPlay();
    
	m_SpreadFearCapsule->OnComponentBeginOverlap.AddDynamic(this, &AGuardCharacter::OnCapsuleOverlapFear);
	m_PlayerDetectedCapsule->OnComponentBeginOverlap.AddDynamic(this, &AGuardCharacter::OnCapsuleOverlapPlayer);
	m_PlayerDetectedCapsule->OnComponentEndOverlap.AddDynamic(this, &AGuardCharacter::OnOverlapEndPlayer);
    
	m_Player = Cast<AGhostCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	AIController = Cast<AGuardAIController>(GetController());

	AIController->GetBlackboardComponent()->SetValueAsObject(FName("Player"), m_Player);

	MaxSatisfaction = m_pPatrolPath->Num();
}

void AGuardCharacter::IncrementSatisfaction()
{
	if (!m_IsHunter)
	{
		++CurrentSatisfaction;
	}
}

void AGuardCharacter::OnCapsuleOverlapFear(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlappedComponent != m_SpreadFearCapsule)
		return;
	
	if (OtherActor && OtherActor != this && bCanSpreadFear && m_FearLevel >= (m_MaxFearLevel / 2))
	{
		UE_LOG(LogTemp, Warning, TEXT("Guard capsule overlapped with: %s"), *OtherActor->GetName());

		if (APawn* OverlappingPawn = Cast<APawn>(OtherActor))
		{
			if (AGuardCharacter* NPC = Cast<AGuardCharacter>(OverlappingPawn))
			{
				if(!NPC->GetSmoothOperator() || NPC->m_FearLevel >= (NPC->m_MaxFearLevel / 2))
				{
					NPC->AddFearLevel(1);
				}
				else
				{
					this->AddFearLevel(-1);
				}
			}
		}

		bCanSpreadFear = false;
		GetWorld()->GetTimerManager().SetTimer(FearCooldownTimerHandle, this, &AGuardCharacter::ResetFearCooldown, 3.0f, false);
	}
}

void AGuardCharacter::OnCapsuleOverlapPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this || !m_Player)
		return;
        
	if (OverlappedComponent != m_PlayerDetectedCapsule)
		return;
        
	if (m_Player == Cast<AGhostCharacter>(OtherActor))
	{
		if (AIController && AIController->GetBlackboardComponent() && m_IsHunter)
		{
			AIController->GetBlackboardComponent()->SetValueAsBool(FName("HasSeenGhost"), true);
			AIController->GetBlackboardComponent()->SetValueAsBool(FName("IsInvestigating"), false);
			bHasSeenPlayer = true;
		}
	}
}

void AGuardCharacter::OnOverlapEndPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor || OtherActor == this || !m_Player)
		return;
        
	if (OverlappedComponent != m_PlayerDetectedCapsule)
		return;
        
	if (m_Player == Cast<AGhostCharacter>(OtherActor))
	{
		if (AIController && AIController->GetBlackboardComponent() && m_IsHunter)
		{
			AIController->GetBlackboardComponent()->SetValueAsBool(FName("HasSeenGhost"), false);
			AIController->GetBlackboardComponent()->SetValueAsBool(FName("IsInvestigating"), true);
			bHasSeenPlayer = false;
		}
	}
}

void AGuardCharacter::ResetFearCooldown()
{
	bCanSpreadFear = true;
}

void AGuardCharacter::Stun()
{
	AIController->GetBlackboardComponent()->SetValueAsBool(FName("Stunned"), true);

	OnStunned.Broadcast();
	
}

void AGuardCharacter::SetPossessed(bool setValue)
{
	if (!m_IsHunter)
	{
		bPossessed = setValue;
	}
}

TArray<FName> AGuardCharacter::GetFearNames() const
{
	return Fears;
}

void AGuardCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(m_FearLevel >= m_MaxFearLevel)
	{
		FearHastStruck = true;
		OnFeared.Broadcast();
	}
	
	if(FearHastStruck && !DoItOnce)
	{
		if (AIController && AIController->GetBlackboardComponent())
		{
			AIController->GetBlackboardComponent()->SetValueAsBool(FName("FearHasStruck"), true);
		}
		DoItOnce = true;
	}

	if(bHasSeenPlayer)
	{
		AIController->GetBlackboardComponent()->SetValueAsVector(FName("CurrentLocation"), m_Player->GetActorLocation());
	}

	if (CurrentSatisfaction >= MaxSatisfaction)
	{
		AIController->GetBlackboardComponent()->SetValueAsBool(FName("Satisfied"), true);
	}

	FVector TextLocation = GetActorLocation() + FVector(0.f, 0.f, 100.f); // Position above head
	FString FearText = FString::Printf(TEXT("Fear: %d"), m_FearLevel);

	DrawDebugString(GetWorld(), TextLocation, FearText, nullptr, FColor::Red, 0.f, true);
}

APathPoints* AGuardCharacter::GetDropOffLocation() const
{
	return m_pDropOffLocation;
}

APathPoints* AGuardCharacter::GetDropOffLocationHunter() const
{
	return m_pDropOffLocationHunter;
}

void AGuardCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UBehaviorTree* AGuardCharacter::GetBehaviorTree() const
{
	return m_pBehaviorTree;
}

APathPoints* AGuardCharacter::GetPatrolPath() const
{
	return m_pPatrolPath;
}

UAnimMontage* AGuardCharacter::GetAnimMontage() const
{
	return m_pAnimMontage;
}

int32 AGuardCharacter::GetFearLevel() const
{
	return m_FearLevel;
}

void AGuardCharacter::AddFearLevel(int amountToAdd)
{
	m_FearLevel += amountToAdd;
	OnFearInc.Broadcast();
}

bool AGuardCharacter::IsScared() const
{
	return m_FearLevel >= m_MaxFearLevel;
}

bool AGuardCharacter::GetSmoothOperator()
{
	return m_SmoothOperator;
}

#if WITH_EDITOR
void AGuardCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = PropertyChangedEvent.GetPropertyName();
    
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AGuardCharacter, m_Vicinity))
	{
		if (m_SpreadFearCapsule)
		{
			m_SpreadFearCapsule->SetCapsuleSize(m_Vicinity, m_Vicinity);
		}
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AGuardCharacter, DetectionRadius))
	{
		if (m_PlayerDetectedCapsule)
		{
			m_PlayerDetectedCapsule->SetCapsuleSize(DetectionRadius, DetectionRadius);
		}
	}
}
#endif