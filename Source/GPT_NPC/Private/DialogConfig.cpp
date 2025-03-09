// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogConfig.h"

UDialogConfig::UDialogConfig()
{

}

FString UDialogConfig::ToSystemPrompt() const
{
	FString Prompt;
   
    // Description Cat
    Prompt += TEXT("You are an NPC with the following description:\n");
    Prompt += Description.MainDescription + TEXT("\n\n");

    // Trait Cat
    if (Trait.Num() > 0)
    {
        FString TraitEntry;
        Prompt += TEXT("Your key traits include:\n");

        for (int i = 0; i <= Trait.Num() - 1; ++i)
        {
            TraitEntry = FString::Printf(TEXT("%d) %s: %s, intensity (ranging from -1.0 - 1.0) : %.2f\n"),
                i + 1, *Trait[i].TraitName.ToString(), *Trait[i].TraitDescription, Trait[i].TraitIntensity);
        }
        Prompt += TraitEntry;
    }

    // Edge Case Cat
    if (!EdgeCase.ForbiddenTopics.IsEmpty())
    {
        Prompt += TEXT("Edge cases are discussion topics you are forbidden to approach. ");
                  TEXT("Upon approach, you should either direct the conversation elsewhere, or resort to a default if present.These are your edge cases : \n");
        
        for (int i = 0; i <= EdgeCase.ForbiddenTopics.Num() - 1; ++i)
        {
            Prompt += EdgeCase.ForbiddenTopics[i] + TEXT(", ");
        }
    }

    // DefaultsCat
    if (!Default.DefaultResponses.IsEmpty())
    {
        Prompt += TEXT("Default responses are conversational fallbacks for you to utilize when a conversation begins to reach the outter bounds of your NPCs knowledge scope. ");
        Prompt += TEXT("The following are your default responses: \n");
        
        for (int i = 0; i <= Default.DefaultResponses.Num() - 1; ++i)
        {
            Prompt += Default.DefaultResponses[i] + TEXT(".\n");
        }
    }

    // End with generic instructions
    Prompt += TEXT("Stay in character at all times, no matter what. Respond to the user’s input accordingly.\n");
    return Prompt;
}
