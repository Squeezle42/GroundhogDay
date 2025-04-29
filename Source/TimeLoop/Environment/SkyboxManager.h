// Copyright (C) 2025 Squeezle Canada
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
#include "GameFramework/Actor.h"
#include "Engine/TextureCube.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "SkyboxManager.generated.h"

/**
 * EWeatherCondition - Represents different weather conditions
 */
UENUM(BlueprintType)
enum class EWeatherCondition : uint8
{
    Clear UMETA(DisplayName = "Clear"),
    Cloudy UMETA(DisplayName = "Cloudy"),
    LightSnow UMETA(DisplayName = "Light Snow"),
    HeavySnow UMETA(DisplayName = "Heavy Snow"),
    Raining UMETA(DisplayName = "Raining")
};

/**
 * ASkyboxManager - Manages the skybox and weather effects for the game
 */
UCLASS()
class TIMELOOP_API ASkyboxManager : public AActor
{
    GENERATED_BODY()
    
public:    
    // Sets default values for this actor's properties
    ASkyboxManager();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:    
    // Called every frame
    virtual void Tick(float DeltaTime) override;
    
    // Set the time of day (0-24)
    UFUNCTION(BlueprintCallable, Category = "Environment")
    void SetTimeOfDay(float TimeOfDay);
    
    // Get the current time of day
    UFUNCTION(BlueprintPure, Category = "Environment")
    float GetTimeOfDay() const { return CurrentTimeOfDay; }
    
    // Set the weather condition
    UFUNCTION(BlueprintCallable, Category = "Environment")
    void SetWeatherCondition(EWeatherCondition NewWeather);
    
    // Get the current weather condition
    UFUNCTION(BlueprintPure, Category = "Environment")
    EWeatherCondition GetWeatherCondition() const { return CurrentWeather; }
    
    // Update skybox colors based on time of day
    UFUNCTION(BlueprintCallable, Category = "Environment")
    void UpdateSkyColors();
    
    // Update weather visuals
    UFUNCTION(BlueprintCallable, Category = "Environment")
    void UpdateWeatherEffects();

protected:
    // The sky sphere mesh component
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UStaticMeshComponent* SkySphere;
    
    // The sky material instance
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Environment")
    UMaterialInstanceDynamic* SkyMaterial;
    
    // The base sky material
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Environment")
    class UMaterialInterface* BaseSkyMaterial;
    
    // The sun directional light
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UDirectionalLightComponent* SunLight;
    
    // Weather particle system component
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UParticleSystemComponent* WeatherParticles;
    
    // Snow particle system
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Environment")
    class UParticleSystem* SnowParticleSystem;
    
    // Heavy snow particle system
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Environment")
    class UParticleSystem* HeavySnowParticleSystem;
    
    // Rain particle system
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Environment")
    class UParticleSystem* RainParticleSystem;
    
    // Current time of day (0-24)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Environment", meta = (ClampMin = "0.0", ClampMax = "24.0"))
    float CurrentTimeOfDay;
    
    // Current weather condition
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Environment")
    EWeatherCondition CurrentWeather;
    
    // Horizon color at dawn
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment|Colors")
    FLinearColor DawnHorizonColor;
    
    // Zenith color at dawn
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment|Colors")
    FLinearColor DawnZenithColor;
    
    // Horizon color at day
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment|Colors")
    FLinearColor DayHorizonColor;
    
    // Zenith color at day
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment|Colors")
    FLinearColor DayZenithColor;
    
    // Horizon color at dusk
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment|Colors")
    FLinearColor DuskHorizonColor;
    
    // Zenith color at dusk
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment|Colors")
    FLinearColor DuskZenithColor;
    
    // Horizon color at night
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment|Colors")
    FLinearColor NightHorizonColor;
    
    // Zenith color at night
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment|Colors")
    FLinearColor NightZenithColor;
    
    // Dawn time range
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment|TimeRanges")
    FVector2D DawnTimeRange;
    
    // Day time range
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment|TimeRanges")
    FVector2D DayTimeRange;
    
    // Dusk time range
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment|TimeRanges")
    FVector2D DuskTimeRange;

private:
    // Calculate interpolation factor between two time ranges
    float CalculateInterpolationFactor(float CurrentTime, const FVector2D& Range) const;
    
    // Calculate the sun's directional light rotation based on time
    FRotator CalculateSunRotation(float TimeOfDay) const;
};
