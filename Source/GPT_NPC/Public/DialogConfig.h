// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "DialogTraitLibrary.h"
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DialogConfig.generated.h"


/** Struct for defining character traits */
USTRUCT(BlueprintType)
struct FCharacterTrait
{
    GENERATED_BODY()

public:
    /** Name of the trait (e.g., "Optimism", "Sarcasm") */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traits")
    FName TraitName;

    /** Optional description for the trait */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traits")
    FString TraitDescription;

    /** Slider to define intensity (-1 = low, 1 = high) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traits", meta = (UIMin = "-1.0", UIMax = "1.0", ClampMin = "-1.0", ClampMax = "1.0"))
    float TraitIntensity = 0.0f;
};

/** Struct for defining an NPC’s backstory */
USTRUCT(BlueprintType)
struct FCharacterDescription
{
    GENERATED_BODY()

public:
    /** The main character backstory */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Description", meta = (MultiLine = "true", LongText = "true"))
    FString MainDescription;

    /** Supporting key details */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Description", meta = (MultiLine = "true", LongText = "true"))
    TArray<FText> SupportingDetails;
};

/** Struct for defining NPC edge cases (topics to avoid) */
USTRUCT(BlueprintType)
struct FCharacterEdgeCase
{
    GENERATED_BODY()

public:
    /** List of topics that should be avoided */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Edge Cases", meta = (MultiLine = "true", LongText = "true"))
    TArray<FString> ForbiddenTopics;

    /** Conditional edge cases (e.g., only avoided when a certain trait is high) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Edge Cases")
    TMap<FName, float> ConditionalEdgeCases;  // Trait-based conditions
};

/** Struct for defining default responses when an NPC reaches an edge case */
USTRUCT(BlueprintType)
struct FCharacterDefaults
{
    GENERATED_BODY()

public:
    /** Default fallback responses */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults", meta = (MultiLine = "true", LongText = "true"))
    TArray<FString> DefaultResponses;
};

/*
* UDialogConfig Class
* 
* @brief - A class which is to be a template for creating multiple, distint personalities. 
* This will be the main object fed to the NPC's communication component. 
*
*/

UCLASS(BlueprintType)
class GPT_NPC_API UDialogConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UDialogConfig();
 
    UFUNCTION()
    FString ToSystemPrompt() const;
protected:

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category= "Personality|Description")
   FCharacterDescription Description;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category= "Personality|Traits")
    TArray<FCharacterTrait> Trait;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category= "Personality|Edge Cases")
    FCharacterEdgeCase EdgeCase;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category= "Personality|Defaults")
    FCharacterDefaults Default;




};
