// Copyright (C) 2025 Time Loop Game Development Team
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DialogueManager.generated.h"

class UQuestManager;

// Delegate for dialogue choice events
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDialogueChoiceMadeDelegate, int32, ChoiceIndex, FText, ChoiceText);

/**
 * FDialogueChoice - Represents a choice in a dialogue
 */
USTRUCT(BlueprintType)
struct FDialogueChoice
{
    GENERATED_BODY()

    // The text of the choice
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FText ChoiceText;

    // ID of the dialogue node this choice leads to
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FName NextNodeId;

    // Optional knowledge flag required for this choice to be visible
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FName RequiredKnowledgeFlag;

    // Optional flag to set when this choice is selected
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FName KnowledgeFlagToSet;

    // Optional relationship impact when selecting this choice
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    float RelationshipImpact;

    // Constructor
    FDialogueChoice()
        : NextNodeId(NAME_None)
        , RequiredKnowledgeFlag(NAME_None)
        , KnowledgeFlagToSet(NAME_None)
        , RelationshipImpact(0.0f)
    {
    }
};

/**
 * FDialogueNode - Represents a node in the dialogue tree
 */
USTRUCT(BlueprintType)
struct FDialogueNode
{
    GENERATED_BODY()

    // Unique ID for this node
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FName NodeId;

    // The character speaking this line
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FName SpeakerId;

    // The dialogue text
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FText DialogueText;

    // Available player responses
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    TArray<FDialogueChoice> Choices;

    // Optional knowledge flag required for this node
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FName RequiredKnowledgeFlag;

    // Knowledge flag to set when reaching this node
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FName KnowledgeFlagToSet;

    // Whether this is an ending node in a conversation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    bool bIsEndNode;

    // Whether this node triggers a quest
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    bool bTriggersQuest;

    // The ID of the quest to trigger
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue", meta = (EditCondition = "bTriggersQuest"))
    FName QuestToTrigger;

    // Constructor
    FDialogueNode()
        : NodeId(NAME_None)
        , SpeakerId(NAME_None)
        , RequiredKnowledgeFlag(NAME_None)
        , KnowledgeFlagToSet(NAME_None)
        , bIsEndNode(false)
        , bTriggersQuest(false)
        , QuestToTrigger(NAME_None)
    {
    }
};

/**
 * FDialogueTree - Represents a complete dialogue tree
 */
USTRUCT(BlueprintType)
struct FDialogueTree
{
    GENERATED_BODY()

    // Unique ID for this dialogue tree
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FName DialogueId;

    // NPC associated with this dialogue tree
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FName NPCId;

    // All nodes in the dialogue tree
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    TMap<FName, FDialogueNode> Nodes;

    // ID of the entry node
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FName EntryNodeId;

    // Constructor
    FDialogueTree()
        : DialogueId(NAME_None)
        , NPCId(NAME_None)
        , EntryNodeId(NAME_None)
    {
    }
};

/**
 * UDialogueManager - Manages NPC dialogue interactions
 */
UCLASS(Blueprintable)
class TIMELOOP_API UDialogueManager : public UObject
{
    GENERATED_BODY()

public:
    UDialogueManager();

    // Initialize the dialogue manager
    void Initialize();

    // Reset dialogue state for a new day
    UFUNCTION(BlueprintCallable, Category = "Dialogue System")
    void ResetForNewDay();

    // Start a dialogue with an NPC
    UFUNCTION(BlueprintCallable, Category = "Dialogue System")
    bool StartDialogue(FName DialogueId);

    // Get the current dialogue node
    UFUNCTION(BlueprintPure, Category = "Dialogue System")
    FDialogueNode GetCurrentNode() const;

    // Make a dialogue choice
    UFUNCTION(BlueprintCallable, Category = "Dialogue System")
    bool MakeChoice(int32 ChoiceIndex);

    // End the current dialogue
    UFUNCTION(BlueprintCallable, Category = "Dialogue System")
    void EndDialogue();

    // Check if in a dialogue
    UFUNCTION(BlueprintPure, Category = "Dialogue System")
    bool IsInDialogue() const { return bInDialogue; }

    // Register a dialogue tree
    UFUNCTION(BlueprintCallable, Category = "Dialogue System")
    void RegisterDialogueTree(const FDialogueTree& DialogueTree);

    // Get all available choices for the current node
    UFUNCTION(BlueprintPure, Category = "Dialogue System")
    TArray<FText> GetAvailableChoices() const;

    // Check if a choice is available based on knowledge
    UFUNCTION(BlueprintPure, Category = "Dialogue System")
    bool IsChoiceAvailable(const FDialogueChoice& Choice) const;

    // Set quest manager reference
    void SetQuestManager(UQuestManager* InQuestManager) { QuestManager = InQuestManager; }

public:
    // Delegate fired when a dialogue choice is made
    UPROPERTY(BlueprintAssignable, Category = "Dialogue System|Events")
    FDialogueChoiceMadeDelegate OnDialogueChoiceMade;

protected:
    // Reference to the quest manager
    UPROPERTY()
    UQuestManager* QuestManager;

    // All dialogue trees, mapped by ID
    UPROPERTY()
    TMap<FName, FDialogueTree> DialogueTrees;

    // The current dialogue tree ID
    UPROPERTY()
    FName CurrentDialogueId;

    // The current dialogue node ID
    UPROPERTY()
    FName CurrentNodeId;

    // Whether the player is currently in a dialogue
    UPROPERTY()
    bool bInDialogue;

    // Previously visited nodes in the current conversation
    UPROPERTY()
    TArray<FName> ConversationHistory;
};
