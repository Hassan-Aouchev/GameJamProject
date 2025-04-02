#include "BTService_CheckForPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UBTService_CheckForPlayer::UBTService_CheckForPlayer()
{
	NodeName = TEXT("Check For Player");
}

void UBTService_CheckForPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return;

	APawn* AIPawn = AIController->GetPawn();
	if (!AIPawn) return;

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(AIPawn->GetWorld(), 0);
	if (!PlayerCharacter) return;
	
	

}	