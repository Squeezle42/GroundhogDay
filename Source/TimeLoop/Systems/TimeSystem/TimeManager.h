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
#include "TimeManager.generated.h"

// Delegate for time change notifications
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHourChangedDelegate, int32, NewHour);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDayResetDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeOfDayChangedDelegate, FName, NewTimeOfDay);

/**
 * TimeOfDay enum - Represents different periods of the day
 */
UENUM(BlueprintType)
enum class ETimeOfDay : uint8
{
	Morning UMETA(DisplayName = "Morning"),
	Day UMETA(DisplayName = "Day"),
	Evening UMETA(DisplayName = "Evening"),
	Night UMETA(DisplayName = "Night")
};

/**
 * UTimeManager - Manages game time progression and time loop mechanics
 */
UCLASS(Blueprintable)
class TIMELOOP_API UTimeManager : public UObject
{
	GENERATED_BODY()
	
public:
	UTimeManager();
	
	// Initialize the time manager
	void Initialize();
	
	// Update game time based on real time
	void UpdateTime(float DeltaTime);
	
	// Reset time to morning of the current day
	UFUNCTION(BlueprintCallable, Category = "Time Loop")
	void ResetToMorning();
	
	// Reset time to morning and increment loop count
	UFUNCTION(BlueprintCallable, Category = "Time Loop")
	void AdvanceToNextLoop();
	
	// Get current hour (0-23)
	UFUNCTION(BlueprintPure, Category = "Time Loop")
	int32 GetCurrentHour() const { return CurrentHour; }
	
	// Get current minute (0-59)
	UFUNCTION(BlueprintPure, Category = "Time Loop")
	int32 GetCurrentMinute() const { return CurrentMinute; }
	
	// Get current formatted time string (HH:MM AM/PM)
	UFUNCTION(BlueprintPure, Category = "Time Loop")
	FString GetTimeString() const;
	
	// Get current time of day period
	UFUNCTION(BlueprintPure, Category = "Time Loop")
	ETimeOfDay GetTimeOfDay() const { return CurrentTimeOfDay; }
	
	// Get time of day as string
	UFUNCTION(BlueprintPure, Category = "Time Loop")
	FString GetTimeOfDayString() const;
	
	// Get loop count
	UFUNCTION(BlueprintPure, Category = "Time Loop")
	int32 GetLoopCount() const { return LoopCount; }
	
	// Set loop count (used when loading saved game)
	void SetLoopCount(int32 NewLoopCount) { LoopCount = NewLoopCount; }
	
	// Check if it's currently a specific time of day
	UFUNCTION(BlueprintPure, Category = "Time Loop")
	bool IsTimeOfDay(ETimeOfDay TimeOfDay) const { return CurrentTimeOfDay == TimeOfDay; }
	
	// Set game time scale (how fast time passes)
	UFUNCTION(BlueprintCallable, Category = "Time Loop")
	void SetTimeScale(float NewTimeScale) { TimeScale = FMath::Max(0.0f, NewTimeScale); }
	
public:
	// Delegate fired when the hour changes
	UPROPERTY(BlueprintAssignable, Category = "Time Loop|Events")
	FOnHourChangedDelegate OnHourChanged;
	
	// Delegate fired when the day is reset
	UPROPERTY(BlueprintAssignable, Category = "Time Loop|Events")
	FOnDayResetDelegate OnDayReset;
	
	// Delegate fired when the time of day period changes
	UPROPERTY(BlueprintAssignable, Category = "Time Loop|Events")
	FOnTimeOfDayChangedDelegate OnTimeOfDayChanged;
	
private:
	// Update time of day period based on current hour
	void UpdateTimeOfDay();
	
	// Set current time (internal function)
	void SetTime(int32 Hour, int32 Minute);
	
protected:
	// Current hour (0-23)
	UPROPERTY(BlueprintReadOnly, Category = "Time Loop")
	int32 CurrentHour;
	
	// Current minute (0-59)
	UPROPERTY(BlueprintReadOnly, Category = "Time Loop")
	int32 CurrentMinute;
	
	// Current time of day period
	UPROPERTY(BlueprintReadOnly, Category = "Time Loop")
	ETimeOfDay CurrentTimeOfDay;
	
	// Number of loops completed
	UPROPERTY(BlueprintReadOnly, Category = "Time Loop")
	int32 LoopCount;
	
	// Game time scale (1.0 = real-time, 2.0 = 2x speed)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Loop", meta = (ClampMin = "0.0"))
	float TimeScale;
	
	// Game minutes per real-time second at 1.0 time scale
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Time Loop", meta = (ClampMin = "0.0"))
	float GameMinutesPerRealSecond;
	
	// Maximum hour before day resets (typically 24)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Time Loop")
	int32 MaxHour;
	
	// Starting hour when day begins (typically 6 for 6 AM)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Time Loop")
	int32 StartingHour;
	
	// Time accumulated since last minute update
	float TimeAccumulator;
};
