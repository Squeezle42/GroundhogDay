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

#include "TimeManager.h"

UTimeManager::UTimeManager()
{
	// Default values
	TimeScale = 1.0f;
	GameMinutesPerRealSecond = 1.0f;  // 1 real second = 1 game minute at 1.0 time scale
	MaxHour = 24;
	StartingHour = 6;  // Game starts at 6 AM
	
	// Initialize time values
	CurrentHour = StartingHour;
	CurrentMinute = 0;
	LoopCount = 1;
	TimeAccumulator = 0.0f;
	
	// Set initial time of day
	UpdateTimeOfDay();
}

void UTimeManager::Initialize()
{
	// Reset to morning of first day
	ResetToMorning();
	LoopCount = 1;
	
	UE_LOG(LogTemp, Warning, TEXT("Time Manager: Initialized (Day %d, %s)"), 
		LoopCount, *GetTimeString());
}

void UTimeManager::UpdateTime(float DeltaTime)
{
	// Accumulate time
	TimeAccumulator += DeltaTime * TimeScale;
	
	// Calculate minutes to add
	const float SecondsPerGameMinute = 1.0f / GameMinutesPerRealSecond;
	
	// If enough time has passed to add at least one minute
	if (TimeAccumulator >= SecondsPerGameMinute)
	{
		// Calculate how many minutes to add
		int32 MinutesToAdd = FMath::FloorToInt(TimeAccumulator / SecondsPerGameMinute);
		TimeAccumulator -= MinutesToAdd * SecondsPerGameMinute;
		
		// Add minutes
		CurrentMinute += MinutesToAdd;
		
		// If minutes overflow, add hours
		while (CurrentMinute >= 60)
		{
			CurrentMinute -= 60;
			CurrentHour++;
			
			// Broadcast hour changed event
			OnHourChanged.Broadcast(CurrentHour);
			
			// Update time of day period if needed
			UpdateTimeOfDay();
			
			// Check for day end
			if (CurrentHour >= MaxHour)
			{
				// Auto reset to next day loop
				AdvanceToNextLoop();
				break;
			}
		}
	}
}

void UTimeManager::ResetToMorning()
{
	// Reset time to morning
	SetTime(StartingHour, 0);
	
	UE_LOG(LogTemp, Warning, TEXT("Time Manager: Reset to morning (Day %d, %s)"), 
		LoopCount, *GetTimeString());
}

void UTimeManager::AdvanceToNextLoop()
{
	// Increment loop count and reset to morning
	LoopCount++;
	ResetToMorning();
	
	// Broadcast day reset event
	OnDayReset.Broadcast();
	
	UE_LOG(LogTemp, Warning, TEXT("Time Manager: Advanced to day %d"), LoopCount);
}

FString UTimeManager::GetTimeString() const
{
	// Convert to 12-hour format
	int32 Hour12 = (CurrentHour % 12);
	if (Hour12 == 0)
	{
		Hour12 = 12; // 0 hour in 12-hour format is 12
	}
	
	// AM/PM
	const FString AmPm = (CurrentHour < 12) ? TEXT("AM") : TEXT("PM");
	
	// Format time string
	return FString::Printf(TEXT("%d:%02d %s"), Hour12, CurrentMinute, *AmPm);
}

FString UTimeManager::GetTimeOfDayString() const
{
	switch (CurrentTimeOfDay)
	{
		case ETimeOfDay::Morning:
			return TEXT("Morning");
		case ETimeOfDay::Day:
			return TEXT("Day");
		case ETimeOfDay::Evening:
			return TEXT("Evening");
		case ETimeOfDay::Night:
			return TEXT("Night");
		default:
			return TEXT("Unknown");
	}
}

void UTimeManager::UpdateTimeOfDay()
{
	ETimeOfDay PreviousTimeOfDay = CurrentTimeOfDay;
	
	// Determine time of day based on hour
	if (CurrentHour >= 5 && CurrentHour < 10)
	{
		CurrentTimeOfDay = ETimeOfDay::Morning;
	}
	else if (CurrentHour >= 10 && CurrentHour < 17)
	{
		CurrentTimeOfDay = ETimeOfDay::Day;
	}
	else if (CurrentHour >= 17 && CurrentHour < 21)
	{
		CurrentTimeOfDay = ETimeOfDay::Evening;
	}
	else
	{
		CurrentTimeOfDay = ETimeOfDay::Night;
	}
	
	// If time of day changed, broadcast event
	if (PreviousTimeOfDay != CurrentTimeOfDay)
	{
		FName TimeOfDayName = FName(*GetTimeOfDayString());
		OnTimeOfDayChanged.Broadcast(TimeOfDayName);
		
		UE_LOG(LogTemp, Warning, TEXT("Time Manager: Time of day changed to %s"), 
			*GetTimeOfDayString());
	}
}

void UTimeManager::SetTime(int32 Hour, int32 Minute)
{
	// Set time values
	CurrentHour = FMath::Clamp(Hour, 0, MaxHour - 1);
	CurrentMinute = FMath::Clamp(Minute, 0, 59);
	
	// Update time of day
	UpdateTimeOfDay();
}
