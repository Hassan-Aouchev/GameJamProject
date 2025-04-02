// Fill out your copyright notice in the Description page of Project Settings.


#include "Throwable.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SceneComponent.h"
#include "Perception/AISense_Hearing.h"

// Sets default values
AThrowable::AThrowable()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> placeholder(TEXT("'/Engine/BasicShapes/Cube.Cube'"));

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("Root");

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(SceneRoot);
	Mesh->SetStaticMesh(placeholder.Object);
	Mesh->SetSimulatePhysics(true);
	Mesh->SetLinearDamping(0.99f);
	Mesh->SetNotifyRigidBodyCollision(true);
	Mesh->OnComponentHit.AddDynamic(this, &AThrowable::OnThrowableHit);
}

// Called when the game starts or when spawned
void AThrowable::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AThrowable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AThrowable::OnThrowableHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	const FHitResult& Hit)
{
	if (GetWorld()->GetTimeSeconds() - m_LastNoiseTime < m_NoiseCooldown)
	{
		return;
	}
    
	float impactForce{static_cast<float>(NormalImpulse.Size())};
    
	if (impactForce > m_MinImpactForceThreshold)
	{
		UAISense_Hearing::ReportNoiseEvent(
			GetWorld(),
			Hit.ImpactPoint,
			impactForce,
			this,
			1000.0f
		);
		m_LastNoiseTime = GetWorld()->GetTimeSeconds();
	}
}