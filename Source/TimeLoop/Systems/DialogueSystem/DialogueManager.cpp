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

#include "DialogueManager.h"
#include "Systems/QuestSystem/QuestManager.h"

UDialogueManager::UDialogueManager()
{
    // Default initialization
    bInDialogue = false;
    CurrentDialogueId = NAME_None;
    CurrentNodeId = NAME_None;
    QuestManager = nullptr;
}

void UDialogueManager::Initialize()
{
    UE_LOG(LogTemp, Warning, TEXT("Dialogue Manager: Initialized"));
}

void UDialogueManager::ResetForNewDay()
{
    // End any active dialogue
    if (bInDialogue)
    {
        EndDialogue();
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Dialogue Manager: Reset for new day"));
}

bool UDialogueManager::StartDialogue(FName DialogueId)
{
    // Make sure we're not already in a dialogue
    if (bInDialogue)
    {
        UE_LOG(LogTemp, Warning, TEXT("Dialogue Manager: Cannot start dialogue '%s', already in dialogue '%s'"), 
            *DialogueId.ToString(), *CurrentDialogueId.ToString());
        return false;
    }
    
    // Check if the dialogue exists
    if (!DialogueTrees.Contains(DialogueId))
    {
        UE_LOG(LogTemp, Warning, TEXT("Dialogue Manager: Dialogue '%s' not found"), *DialogueId.ToString());
        return false;
    }
    
    // Get the dialogue tree
    const FDialogueTree& Tree = DialogueTrees[DialogueId];
    
    // Make sure the tree has an entry node
    if (Tree.EntryNodeId == NAME_None || !Tree.Nodes.Contains(Tree.EntryNodeId))
    {
        UE_LOG(LogTemp, Warning, TEXT("Dialogue Manager: Dialogue '%s' has no valid entry node"), 
            *DialogueId.ToString());
        return false;
    }
    
    // Set up current dialogue
    CurrentDialogueId = DialogueId;
    CurrentNodeId = Tree.EntryNodeId;
    bInDialogue = true;
    ConversationHistory.Empty();
    ConversationHistory.Add(CurrentNodeId);
    
    // Set knowledge flag if specified
    const FDialogueNode& CurrentNode = Tree.Nodes[CurrentNodeId];
    if (QuestManager && CurrentNode.KnowledgeFlagToSet != NAME_None)
    {
        QuestManager->SetKnowledgeFlag(CurrentNode.KnowledgeFlagToSet);
    }
    
    // Handle quest triggers
    if (CurrentNode.bTriggersQuest && CurrentNode.QuestToTrigger != NAME_None && QuestManager)
    {
        QuestManager->UpdateQuestState(CurrentNode.QuestToTrigger, EQuestState::Available);
        UE_LOG(LogTemp, Warning, TEXT("Dialogue Manager: Triggered quest '%s'"), 
            *CurrentNode.QuestToTrigger.ToString());
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Dialogue Manager: Started dialogue '%s' at node '%s'"), 
        *DialogueId.ToString(), *CurrentNodeId.ToString());
    
    return true;
}

FDialogueNode UDialogueManager::GetCurrentNode() const
{
    // Make sure we're in a dialogue
    if (!bInDialogue || !DialogueTrees.Contains(CurrentDialogueId))
    {
        return FDialogueNode();
    }
    
    // Get the current node
    const FDialogueTree& Tree = DialogueTrees[CurrentDialogueId];
    if (Tree.Nodes.Contains(CurrentNodeId))
    {
        return Tree.Nodes[CurrentNodeId];
    }
    
    return FDialogueNode();
}

bool UDialogueManager::MakeChoice(int32 ChoiceIndex)
{
    // Make sure we're in a dialogue
    if (!bInDialogue)
    {
        UE_LOG(LogTemp, Warning, TEXT("Dialogue Manager: Cannot make choice, not in dialogue"));
        return false;
    }
    
    // Get the current node
    FDialogueNode CurrentNode = GetCurrentNode();
    if (CurrentNode.NodeId == NAME_None)
    {
        UE_LOG(LogTemp, Warning, TEXT("Dialogue Manager: Cannot make choice, invalid current node"));
        return false;
    }
    
    // Check if the choice index is valid
    if (ChoiceIndex < 0 || ChoiceIndex >= CurrentNode.Choices.Num())
    {
        UE_LOG(LogTemp, Warning, TEXT("Dialogue Manager: Choice index %d out of range (0-%d)"), 
            ChoiceIndex, CurrentNode.Choices.Num() - 1);
        return false;
    }
    
    // Get the selected choice
    const FDialogueChoice& Choice = CurrentNode.Choices[ChoiceIndex];
    
    // Check if the choice is available
    if (!IsChoiceAvailable(Choice))
    {
        UE_LOG(LogTemp, Warning, TEXT("Dialogue Manager: Choice %d is not available due to required knowledge"),
            ChoiceIndex);
        return false;
    }
    
    // Broadcast that a choice was made
    OnDialogueChoiceMade.Broadcast(ChoiceIndex, Choice.ChoiceText);
    
    // Set knowledge flag if specified
    if (QuestManager && Choice.KnowledgeFlagToSet != NAME_None)
    {
        QuestManager->SetKnowledgeFlag(Choice.KnowledgeFlagToSet);
        UE_LOG(LogTemp, Warning, TEXT("Dialogue Manager: Set knowledge flag '%s'"), 
            *Choice.KnowledgeFlagToSet.ToString());
    }
    
    // Handle relationship impact
    // TODO: Implement relationship impact through NPCScheduler
    
    // If the choice leads to another node, navigate to it
    if (Choice.NextNodeId != NAME_None)
    {
        const FDialogueTree& Tree = DialogueTrees[CurrentDialogueId];
        if (Tree.Nodes.Contains(Choice.NextNodeId))
        {
            // Update current node
            CurrentNodeId = Choice.NextNodeId;
            ConversationHistory.Add(CurrentNodeId);
            
            // Get the new current node
            const FDialogueNode& NewNode = Tree.Nodes[CurrentNodeId];
            
            // Set knowledge flag if specified
            if (QuestManager && NewNode.KnowledgeFlagToSet != NAME_None)
            {
                QuestManager->SetKnowledgeFlag(NewNode.KnowledgeFlagToSet);
            }
            
            // Handle quest triggers
            if (NewNode.bTriggersQuest && NewNode.QuestToTrigger != NAME_None && QuestManager)
            {
                QuestManager->UpdateQuestState(NewNode.QuestToTrigger, EQuestState::Available);
                UE_LOG(LogTemp, Warning, TEXT("Dialogue Manager: Triggered quest '%s'"), 
                    *NewNode.QuestToTrigger.ToString());
            }
            
            // Check if we've reached an end node
            if (NewNode.bIsEndNode)
            {
                UE_LOG(LogTemp, Warning, TEXT("Dialogue Manager: Reached end node, ending dialogue"));
                EndDialogue();
            }
            
            UE_LOG(LogTemp, Warning, TEXT("Dialogue Manager: Advanced to node '%s'"), *CurrentNodeId.ToString());
            return true;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Dialogue Manager: Next node '%s' not found in tree"), 
                *Choice.NextNodeId.ToString());
            return false;
        }
    }
    else
    {
        // End the dialogue if no next node is specified
        UE_LOG(LogTemp, Warning, TEXT("Dialogue Manager: No next node specified, ending dialogue"));
        EndDialogue();
        return true;
    }
}

void UDialogueManager::EndDialogue()
{
    // Reset current dialogue state
    bInDialogue = false;
    CurrentDialogueId = NAME_None;
    CurrentNodeId = NAME_None;
    ConversationHistory.Empty();
    
    UE_LOG(LogTemp, Warning, TEXT("Dialogue Manager: Dialogue ended"));
}

void UDialogueManager::RegisterDialogueTree(const FDialogueTree& DialogueTree)
{
    // Register the dialogue tree
    DialogueTrees.Add(DialogueTree.DialogueId, DialogueTree);
    
    UE_LOG(LogTemp, Warning, TEXT("Dialogue Manager: Registered dialogue tree '%s' with %d nodes"), 
        *DialogueTree.DialogueId.ToString(), DialogueTree.Nodes.Num());
}

TArray<FText> UDialogueManager::GetAvailableChoices() const
{
    TArray<FText> AvailableChoices;
    
    // Make sure we're in a dialogue
    if (!bInDialogue)
    {
        return AvailableChoices;
    }
    
    // Get the current node
    FDialogueNode CurrentNode = GetCurrentNode();
    if (CurrentNode.NodeId == NAME_None)
    {
        return AvailableChoices;
    }
    
    // Add all available choices
    for (const FDialogueChoice& Choice : CurrentNode.Choices)
    {
        if (IsChoiceAvailable(Choice))
        {
            AvailableChoices.Add(Choice.ChoiceText);
        }
    }
    
    return AvailableChoices;
}

bool UDialogueManager::IsChoiceAvailable(const FDialogueChoice& Choice) const
{
    // If no required knowledge flag, the choice is available
    if (Choice.RequiredKnowledgeFlag == NAME_None || !QuestManager)
    {
        return true;
    }
    
    // Check if the player has the required knowledge
    return QuestManager->HasKnowledgeFlag(Choice.RequiredKnowledgeFlag);
}
