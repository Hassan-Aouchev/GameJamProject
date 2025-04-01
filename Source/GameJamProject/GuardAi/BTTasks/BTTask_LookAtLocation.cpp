#include "BTTask_LookAtLocation.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_LookAtLocation::UBTTask_LookAtLocation(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Look At Location");
	
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_LookAtLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	APawn* Pawn = AIController->GetPawn();
	if (!Pawn)
	{
		return EBTNodeResult::Failed;
	}

	// Get the target location from blackboard
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	// Perform initial rotation
	FVector TargetLocation = BlackboardComp->GetValueAsVector(GetSelectedBlackboardKey());
	FVector PawnLocation = Pawn->GetActorLocation();
	FRotator CurrentRotation = Pawn->GetActorRotation();
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(PawnLocation, TargetLocation);
    
	// Only use the Yaw (horizontal rotation) to avoid tilting the character
	TargetRotation.Pitch = CurrentRotation.Pitch;
	TargetRotation.Roll = CurrentRotation.Roll;
    
	// Use RInterpTo for smooth rotation over time
	FRotator NewRotation = FMath::RInterpTo(
		CurrentRotation, 
		TargetRotation, 
		OwnerComp.GetWorld()->GetDeltaSeconds(), 
		RotationSpeed);
    
	// Apply the rotation
	Pawn->SetActorRotation(NewRotation);
    
	// Return InProgress to continue ticking
	return EBTNodeResult::InProgress;
}

void UBTTask_LookAtLocation::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	APawn* Pawn = AIController->GetPawn();
	if (!Pawn)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	FVector TargetLocation = BlackboardComp->GetValueAsVector(GetSelectedBlackboardKey());
	FVector PawnLocation = Pawn->GetActorLocation();
	FRotator CurrentRotation = Pawn->GetActorRotation();
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(PawnLocation, TargetLocation);
    
	// Only compare Yaw for completion
	if (FMath::IsNearlyEqual(CurrentRotation.Yaw, TargetRotation.Yaw, 1.0f))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		// Continue rotating
		TargetRotation.Pitch = CurrentRotation.Pitch;
		TargetRotation.Roll = CurrentRotation.Roll;
        
		FRotator NewRotation = FMath::RInterpTo(
			CurrentRotation, 
			TargetRotation, 
			DeltaSeconds, 
			RotationSpeed);
        
		Pawn->SetActorRotation(NewRotation);
	}
}
