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

#include "SkyboxManager.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/DirectionalLight.h"

// Sets default values
ASkyboxManager::ASkyboxManager()
{
    // Set this actor to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    // Create the sky sphere component
    SkySphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkySphere"));
    SetRootComponent(SkySphere);
    
    // Load the sky sphere mesh
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SkySphereAsset(TEXT("StaticMesh'/Engine/EngineSky/SM_SkySphere.SM_SkySphere'"));
    if (SkySphereAsset.Succeeded())
    {
        SkySphere->SetStaticMesh(SkySphereAsset.Object);
    }
    
    // Create the sun light component
    SunLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("SunLight"));
    SunLight->SetupAttachment(RootComponent);
    SunLight->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));  // Default pointing down at 45 degrees
    
    // Create weather particle system component
    WeatherParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("WeatherParticles"));
    WeatherParticles->SetupAttachment(RootComponent);
    WeatherParticles->SetAutoActivate(false);
    
    // Initialize default values
    CurrentTimeOfDay = 12.0f;  // Start at noon
    CurrentWeather = EWeatherCondition::Clear;
    
    // Set default colors for different times of day
    DawnHorizonColor = FLinearColor(0.9f, 0.6f, 0.3f);  // Orange-ish
    DawnZenithColor = FLinearColor(0.5f, 0.7f, 1.0f);   // Light blue
    
    DayHorizonColor = FLinearColor(0.9f, 0.9f, 1.0f);   // Almost white
    DayZenithColor = FLinearColor(0.3f, 0.6f, 1.0f);    // Blue
    
    DuskHorizonColor = FLinearColor(0.9f, 0.4f, 0.2f);  // Orange-red
    DuskZenithColor = FLinearColor(0.2f, 0.4f, 0.8f);   // Deep blue
    
    NightHorizonColor = FLinearColor(0.05f, 0.05f, 0.1f);  // Very dark blue
    NightZenithColor = FLinearColor(0.02f, 0.02f, 0.05f);  // Almost black
    
    // Set default time ranges
    DawnTimeRange = FVector2D(5.0f, 8.0f);    // 5 AM to 8 AM
    DayTimeRange = FVector2D(8.0f, 18.0f);    // 8 AM to 6 PM
    DuskTimeRange = FVector2D(18.0f, 21.0f);  // 6 PM to 9 PM
    // Night is implied as everything else
}

// Called when the game starts or when spawned
void ASkyboxManager::BeginPlay()
{
    Super::BeginPlay();
    
    // Create dynamic material instance for the sky sphere
    if (BaseSkyMaterial && SkySphere)
    {
        SkyMaterial = UMaterialInstanceDynamic::Create(BaseSkyMaterial, this);
        SkySphere->SetMaterial(0, SkyMaterial);
    }
    else if (!BaseSkyMaterial)
    {
        UE_LOG(LogTemp, Warning, TEXT("SkyboxManager: No base sky material set!"));
    }
    
    // Update skybox for initial time and weather
    UpdateSkyColors();
    UpdateWeatherEffects();
}

// Called every frame
void ASkyboxManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ASkyboxManager::SetTimeOfDay(float TimeOfDay)
{
    // Clamp time to 0-24 hour range
    CurrentTimeOfDay = FMath::Clamp(TimeOfDay, 0.0f, 24.0f);
    
    // Update skybox appearance
    UpdateSkyColors();
    
    // Update sun rotation
    if (SunLight)
    {
        SunLight->SetWorldRotation(CalculateSunRotation(CurrentTimeOfDay));
        
        // Adjust light intensity based on time of day
        float Intensity = 1.0f;
        
        if (CurrentTimeOfDay < DawnTimeRange.X || CurrentTimeOfDay > DuskTimeRange.Y)
        {
            // Night time - dim light
            Intensity = 0.1f;
        }
        else if (CurrentTimeOfDay >= DawnTimeRange.X && CurrentTimeOfDay < DawnTimeRange.Y)
        {
            // Dawn - increasing intensity
            Intensity = FMath::Lerp(0.1f, 1.0f, CalculateInterpolationFactor(CurrentTimeOfDay, DawnTimeRange));
        }
        else if (CurrentTimeOfDay >= DuskTimeRange.X && CurrentTimeOfDay < DuskTimeRange.Y)
        {
            // Dusk - decreasing intensity
            Intensity = FMath::Lerp(1.0f, 0.1f, CalculateInterpolationFactor(CurrentTimeOfDay, DuskTimeRange));
        }
        
        SunLight->SetIntensity(Intensity * 3.0f);  // Scale for good visual range
    }
    
    UE_LOG(LogTemp, Warning, TEXT("SkyboxManager: Time of day set to %.2f"), CurrentTimeOfDay);
}

void ASkyboxManager::SetWeatherCondition(EWeatherCondition NewWeather)
{
    if (CurrentWeather == NewWeather)
    {
        return;  // No change needed
    }
    
    CurrentWeather = NewWeather;
    UpdateWeatherEffects();
    
    UE_LOG(LogTemp, Warning, TEXT("SkyboxManager: Weather condition set to %s"), 
        *UEnum::GetValueAsString(CurrentWeather));
}

void ASkyboxManager::UpdateSkyColors()
{
    if (!SkyMaterial)
    {
        UE_LOG(LogTemp, Warning, TEXT("SkyboxManager: No sky material available for color update"));
        return;
    }
    
    FLinearColor HorizonColor, ZenithColor;
    
    // Determine time of day phase and interpolation factor
    if (CurrentTimeOfDay >= DawnTimeRange.X && CurrentTimeOfDay < DawnTimeRange.Y)
    {
        // Dawn
        float Alpha = CalculateInterpolationFactor(CurrentTimeOfDay, DawnTimeRange);
        HorizonColor = FMath::Lerp(NightHorizonColor, DawnHorizonColor, Alpha);
        ZenithColor = FMath::Lerp(NightZenithColor, DawnZenithColor, Alpha);
    }
    else if (CurrentTimeOfDay >= DawnTimeRange.Y && CurrentTimeOfDay < DayTimeRange.Y)
    {
        // Day
        float Alpha = CalculateInterpolationFactor(CurrentTimeOfDay, DayTimeRange);
        HorizonColor = FMath::Lerp(DawnHorizonColor, DayHorizonColor, Alpha);
        ZenithColor = FMath::Lerp(DawnZenithColor, DayZenithColor, Alpha);
    }
    else if (CurrentTimeOfDay >= DayTimeRange.Y && CurrentTimeOfDay < DuskTimeRange.Y)
    {
        // Dusk
        float Alpha = CalculateInterpolationFactor(CurrentTimeOfDay, DuskTimeRange);
        HorizonColor = FMath::Lerp(DayHorizonColor, DuskHorizonColor, Alpha);
        ZenithColor = FMath::Lerp(DayZenithColor, DuskZenithColor, Alpha);
    }
    else
    {
        // Night
        HorizonColor = NightHorizonColor;
        ZenithColor = NightZenithColor;
    }
    
    // Adjust colors based on weather
    if (CurrentWeather == EWeatherCondition::Cloudy)
    {
        // Desaturate and slightly darken for cloudy weather
        HorizonColor.Desaturate(0.3f);
        ZenithColor.Desaturate(0.3f);
        HorizonColor *= 0.9f;
        ZenithColor *= 0.9f;
    }
    else if (CurrentWeather == EWeatherCondition::LightSnow || CurrentWeather == EWeatherCondition::HeavySnow)
    {
        // Desaturate more and whiten/brighten a bit for snowy weather
        HorizonColor.Desaturate(0.5f);
        ZenithColor.Desaturate(0.5f);
        HorizonColor = FMath::Lerp(HorizonColor, FLinearColor(1.0f, 1.0f, 1.0f, 1.0f), 0.2f);
        ZenithColor = FMath::Lerp(ZenithColor, FLinearColor(0.9f, 0.9f, 0.95f, 1.0f), 0.2f);
    }
    else if (CurrentWeather == EWeatherCondition::Raining)
    {
        // Darken and gray out for rainy weather
        HorizonColor.Desaturate(0.7f);
        ZenithColor.Desaturate(0.7f);
        HorizonColor *= 0.7f;
        ZenithColor *= 0.7f;
    }
    
    // Set material parameters
    SkyMaterial->SetVectorParameterValue("HorizonColor", HorizonColor);
    SkyMaterial->SetVectorParameterValue("ZenithColor", ZenithColor);
    
    // Optional: set cloud coverage parameter if your material supports it
    float CloudCoverage = 0.0f;
    switch (CurrentWeather)
    {
        case EWeatherCondition::Clear:
            CloudCoverage = 0.1f;
            break;
        case EWeatherCondition::Cloudy:
            CloudCoverage = 0.7f;
            break;
        case EWeatherCondition::LightSnow:
            CloudCoverage = 0.6f;
            break;
        case EWeatherCondition::HeavySnow:
            CloudCoverage = 0.9f;
            break;
        case EWeatherCondition::Raining:
            CloudCoverage = 0.8f;
            break;
    }
    
    if (SkyMaterial->HasParameter("CloudCoverage"))
    {
        SkyMaterial->SetScalarParameterValue("CloudCoverage", CloudCoverage);
    }
}

void ASkyboxManager::UpdateWeatherEffects()
{
    if (!WeatherParticles)
    {
        return;
    }
    
    // Deactivate current particle system
    WeatherParticles->DeactivateSystem();
    
    // Set appropriate particle system based on weather condition
    switch (CurrentWeather)
    {
        case EWeatherCondition::Clear:
        case EWeatherCondition::Cloudy:
            // No particles for clear or cloudy
            break;
            
        case EWeatherCondition::LightSnow:
            if (SnowParticleSystem)
            {
                WeatherParticles->SetTemplate(SnowParticleSystem);
                WeatherParticles->ActivateSystem();
            }
            break;
            
        case EWeatherCondition::HeavySnow:
            if (HeavySnowParticleSystem)
            {
                WeatherParticles->SetTemplate(HeavySnowParticleSystem);
                WeatherParticles->ActivateSystem();
            }
            break;
            
        case EWeatherCondition::Raining:
            if (RainParticleSystem)
            {
                WeatherParticles->SetTemplate(RainParticleSystem);
                WeatherParticles->ActivateSystem();
            }
            break;
    }
}

float ASkyboxManager::CalculateInterpolationFactor(float CurrentTime, const FVector2D& Range) const
{
    // Calculate how far we are through this time range (0-1)
    return FMath::Clamp((CurrentTime - Range.X) / (Range.Y - Range.X), 0.0f, 1.0f);
}

FRotator ASkyboxManager::CalculateSunRotation(float TimeOfDay) const
{
    // Map 24-hour time to 360 degrees
    // 0 hours = -90 degrees (sun at eastern horizon)
    // 6 hours = 0 degrees (sun at zenith)
    // 12 hours = 90 degrees (sun at western horizon)
    // 18 hours = 180 degrees (sun at nadir, i.e., midnight)
    
    float SunAngle = ((TimeOfDay / 24.0f) * 360.0f) - 90.0f;
    return FRotator(-SunAngle, 0.0f, 0.0f);
}
