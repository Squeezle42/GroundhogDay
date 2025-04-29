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

#include "InteractionComponent.h"

UInteractionComponent::UInteractionComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.
    PrimaryComponentTick.bCanEverTick = true;

    // Default values
    MaxInteractionDistance = 200.0f;
    bDrawDebug = false;
    CurrentInteractionTarget = nullptr;
}

void UInteractionComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Update the interaction target each tick
    UpdateInteractionTarget();
}

bool UInteractionComponent::TriggerInteraction()
{
    if (CurrentInteractionTarget)
    {
        // Here you would normally check if the target implements an interactable interface
        // For now, just return true to indicate an interaction was attempted
        UE_LOG(LogTemp, Warning, TEXT("Interacted with %s"), *CurrentInteractionTarget->GetName());
        return true;
    }
    return false;
}

void UInteractionComponent::UpdateInteractionTarget()
{
    // Reset current target
    CurrentInteractionTarget = nullptr;
    
    // Get the owner actor (should be the player)
    AActor* OwnerActor = GetOwner();
    if (!OwnerActor)
    {
        return;
    }
    
    // Get player camera location and forward vector
    FVector CameraLocation;
    FRotator CameraRotation;
    OwnerActor->GetActorEyesViewPoint(CameraLocation, CameraRotation);
    
    FVector TraceStart = CameraLocation;
    FVector TraceEnd = TraceStart + (CameraRotation.Vector() * MaxInteractionDistance);
    
    // Perform the raycast
    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(OwnerActor);
    
    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        TraceStart,
        TraceEnd,
        ECC_Visibility,
        QueryParams
    );
    
    if (bHit)
    {
        // In a full implementation, you would check if the hit actor implements IInteractable
        // For now, just set it as the current target
        CurrentInteractionTarget = HitResult.GetActor();
    }
    
    // Debug drawing
    if (bDrawDebug)
    {
        FColor LineColor = CurrentInteractionTarget ? FColor::Green : FColor::Red;
        DrawDebugLine(GetWorld(), TraceStart, TraceEnd, LineColor, false, -1.0f, 0, 1.0f);
        
        if (CurrentInteractionTarget)
        {
            DrawDebugSphere(GetWorld(), HitResult.Location, 10.0f, 8, FColor::Green, false, -1.0f, 0, 1.0f);
        }
    }
}
