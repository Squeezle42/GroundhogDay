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
#include "GameFramework/SaveGame.h"
#include "TimeLoopSaveGame.generated.h"

/**
 * UTimeLoopSaveGame - Stores persistent game data across time loops
 * This save game handles information that should persist when a time loop occurs
 */
UCLASS()
class TIMELOOP_API UTimeLoopSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UTimeLoopSaveGame();
	
	// The number of time loops completed
	UPROPERTY(VisibleAnywhere, Category = "Time Loop")
	int32 LoopCount;
	
	// Knowledge flags that the player has discovered
	UPROPERTY(VisibleAnywhere, Category = "Time Loop")
	TMap<FName, bool> KnowledgeFlags;
	
	// Quest progress that persists between loops
	UPROPERTY(VisibleAnywhere, Category = "Time Loop")
	TMap<FName, int32> PersistentQuestProgress;
	
	// NPC relationship values
	UPROPERTY(VisibleAnywhere, Category = "Time Loop")
	TMap<FName, float> NPCRelationships;
	
	// Player growth areas (personality development)
	UPROPERTY(VisibleAnywhere, Category = "Time Loop")
	TMap<FName, float> PlayerGrowthValues;
	
	// Discovered locations that should remain discovered in future loops
	UPROPERTY(VisibleAnywhere, Category = "Time Loop")
	TArray<FName> DiscoveredLocations;
	
	// Items that persist between loops (e.g., special keys)
	UPROPERTY(VisibleAnywhere, Category = "Time Loop")
	TArray<FName> PersistentItems;
};
