// Fill out your copyright notice in the Description page of Project Settings.

#include "GuardCharacter.h"

#include "GuardAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"

AGuardCharacter::AGuardCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	m_SpreadFearCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	m_SpreadFearCapsule->InitCapsuleSize(m_Vicinity, m_Vicinity);
	m_SpreadFearCapsule->SetCollisionProfileName(TEXT("Pawn"));
	m_SpreadFearCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_SpreadFearCapsule->SetCollisionObjectType(ECC_WorldDynamic);
	m_SpreadFearCapsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	m_SpreadFearCapsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	m_SpreadFearCapsule->SetupAttachment(GetMesh());
	m_SpreadFearCapsule->OnComponentBeginOverlap.AddDynamic(this, &AGuardCharacter::OnCapsuleOverlap);
}

void AGuardCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AGuardCharacter::OnCapsuleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && bCanSpreadFear && m_FearLevel >= (m_MaxFearLevel / 2))
	{
		UE_LOG(LogTemp, Warning, TEXT("Guard capsule overlapped with: %s"), *OtherActor->GetName());

		if (APawn* OverlappingPawn = Cast<APawn>(OtherActor))
		{
			if (AGuardCharacter* NPC = Cast<AGuardCharacter>(OverlappingPawn))
			{
				if(!NPC->GetSmoothOperator() || NPC->m_FearLevel >= (NPC->m_MaxFearLevel / 2))
				{
					NPC->AddFearLevel(1 + m_HeardAmount);
				}
				else
				{
					this->AddFearLevel(-1 - m_HeardAmount);
				}
				++m_HeardAmount;
			}
		}

		bCanSpreadFear = false;
		GetWorld()->GetTimerManager().SetTimer(FearCooldownTimerHandle, this, &AGuardCharacter::ResetFearCooldown, 3.0f, false);
	}
}

void AGuardCharacter::ResetFearCooldown()
{
	bCanSpreadFear = true;
}

void AGuardCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(m_FearLevel >= m_MaxFearLevel)
	{
		FearHastStruck = true;
	}
	
	if(FearHastStruck && !DoItOnce)
	{
		AGuardAIController* AIController = Cast<AGuardAIController>(GetController());
		if (AIController && AIController->GetBlackboardComponent())
		{
			AIController->GetBlackboardComponent()->SetValueAsBool(FName("FearHasStruck"), true);
		}
		DoItOnce = true;
	}

	FVector TextLocation = GetActorLocation() + FVector(0.f, 0.f, 100.f); // Position above head
	FString FearText = FString::Printf(TEXT("Fear: %d"), m_FearLevel);

	// Draw text above the character
	DrawDebugString(GetWorld(), TextLocation, FearText, nullptr, FColor::Red, 0.f, true);
}

APathPoints* AGuardCharacter::GetDropOffLocation() const
{
	return m_pDropOffLocation;
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
}

bool AGuardCharacter::IsScared() const
{
	return m_FearLevel >= m_MaxFearLevel;
}

bool AGuardCharacter::GetSmoothOperator()
{
	return m_SmoothOperator;
}
