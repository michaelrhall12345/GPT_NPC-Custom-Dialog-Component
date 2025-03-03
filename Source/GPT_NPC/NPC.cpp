// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"

// Sets default values
ANPC::ANPC()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionVolume"));
	InteractionVolume->SetupAttachment(RootComponent);

	InteractionVolume->SetBoxExtent(FVector(100.f, 100.f, 100.f));
    InteractionVolume->SetRelativeLocation(FVector(0.f, 0.f, 100.f));

	InteractionVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// Bind overlap functions
	InteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &ANPC::OnOverlapBegin);
	InteractionVolume->OnComponentEndOverlap.AddDynamic(this, &ANPC::OnOverlapEnd);

}


void ANPC::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        UE_LOG(LogTemp, Warning, TEXT("Player entered interaction range"));
        bIsOverlapping = true;
    }
}

void ANPC::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor != this)
    {
        UE_LOG(LogTemp, Warning, TEXT("Player left interaction range"));
        bIsOverlapping = false;
    }
}

void ANPC::Interact()
{
    // To be called by player
}
