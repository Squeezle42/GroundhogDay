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

#include "TimeLoopPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

// Define the InteractionComponent and InventoryComponent forward references
// These will need to be implemented separately
#include "Components/InteractionComponent.h"
#include "Components/InventoryComponent.h"

ATimeLoopPlayerCharacter::ATimeLoopPlayerCharacter()
{
    // Set this character to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    // Create a camera boom
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character
    CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

    // Create a follow camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false; // Camera doesn't rotate relative to arm

    // Create interaction component
    InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
    
    // Create inventory component
    InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

    // Set default values
    MaxInteractionDistance = 200.0f;
    EnergyLevel = 100.0f;
    SprintEnergyDrainRate = 10.0f;
    EnergyRegenerationRate = 5.0f;
    BaseWalkSpeed = 600.0f;
    SprintSpeed = 900.0f;
    bIsSprinting = false;
    bIsInteracting = false;

    // Set default movement values
    UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
    if (MovementComponent)
    {
        MovementComponent->MaxWalkSpeed = BaseWalkSpeed;
        MovementComponent->JumpZVelocity = 600.f;
        MovementComponent->AirControl = 0.2f;
    }
}

void ATimeLoopPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    // Initialize energy to max at start
    EnergyLevel = 100.0f;
}

void ATimeLoopPlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Handle energy management
    if (bIsSprinting && EnergyLevel > 0.0f)
    {
        // Drain energy while sprinting
        ModifyEnergyLevel(-SprintEnergyDrainRate * DeltaTime);
        
        // Stop sprinting if energy depleted
        if (EnergyLevel <= 0.0f)
        {
            StopSprinting();
        }
    }
    else if (!bIsSprinting && EnergyLevel < 100.0f)
    {
        // Regenerate energy when not sprinting
        ModifyEnergyLevel(EnergyRegenerationRate * DeltaTime);
    }
    
    // Update interaction detection
    UpdateInteractionDetection();
}

void ATimeLoopPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Set up movement bindings
    PlayerInputComponent->BindAxis("MoveForward", this, &ATimeLoopPlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ATimeLoopPlayerCharacter::MoveRight);
    
    // Set up look bindings
    PlayerInputComponent->BindAxis("LookUp", this, &ATimeLoopPlayerCharacter::LookUp);
    PlayerInputComponent->BindAxis("Turn", this, &ATimeLoopPlayerCharacter::Turn);
    
    // Set up action bindings
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ATimeLoopPlayerCharacter::StartSprinting);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ATimeLoopPlayerCharacter::StopSprinting);
    PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ATimeLoopPlayerCharacter::Interact);
    PlayerInputComponent->BindAction("Journal", IE_Pressed, this, &ATimeLoopPlayerCharacter::ToggleJournal);
    PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &ATimeLoopPlayerCharacter::ToggleInventory);
    
    // Add jump binding
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
}

void ATimeLoopPlayerCharacter::MoveForward(float Value)
{
    if (Controller && Value != 0.0f)
    {
        // Find out which way is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        
        // Get forward vector
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        
        // Add movement in that direction
        AddMovementInput(Direction, Value);
    }
}

void ATimeLoopPlayerCharacter::MoveRight(float Value)
{
    if (Controller && Value != 0.0f)
    {
        // Find out which way is right
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        
        // Get right vector
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        
        // Add movement in that direction
        AddMovementInput(Direction, Value);
    }
}

void ATimeLoopPlayerCharacter::LookUp(float Value)
{
    if (Value != 0.0f)
    {
        AddControllerPitchInput(Value);
    }
}

void ATimeLoopPlayerCharacter::Turn(float Value)
{
    if (Value != 0.0f)
    {
        AddControllerYawInput(Value);
    }
}

void ATimeLoopPlayerCharacter::StartSprinting()
{
    if (!bIsSprinting && EnergyLevel > 0.0f)
    {
        bIsSprinting = true;
        
        // Increase movement speed
        UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
        if (MovementComponent)
        {
            MovementComponent->MaxWalkSpeed = SprintSpeed;
        }
    }
}

void ATimeLoopPlayerCharacter::StopSprinting()
{
    if (bIsSprinting)
    {
        bIsSprinting = false;
        
        // Reset movement speed
        UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
        if (MovementComponent)
        {
            MovementComponent->MaxWalkSpeed = BaseWalkSpeed;
        }
    }
}

void ATimeLoopPlayerCharacter::Interact()
{
    // This will be expanded when the interaction component is implemented
    // For now, just log that we're trying to interact
    UE_LOG(LogTemp, Warning, TEXT("Player attempting to interact"));
    
    // Check if we have an interaction component
    if (InteractionComponent)
    {
        // InteractionComponent->TriggerInteraction();
        // Will be implemented when the component is available
    }
    
    // Simple interaction ray trace for testing
    FHitResult HitResult;
    FVector StartLocation = FollowCamera->GetComponentLocation();
    FVector EndLocation = StartLocation + (FollowCamera->GetForwardVector() * MaxInteractionDistance);
    
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);
    
    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        StartLocation,
        EndLocation,
        ECC_Visibility,
        QueryParams
    );
    
    if (bHit)
    {
        AActor* HitActor = HitResult.GetActor();
        if (HitActor)
        {
            UE_LOG(LogTemp, Warning, TEXT("Player interacted with: %s"), *HitActor->GetName());
            
            // Call the OnInteract function if the actor has the interface
            // This will be implemented when the interface is created
            // IInteractable* Interactable = Cast<IInteractable>(HitActor);
            // if (Interactable)
            // {
            //     Interactable->OnInteract(this);
            // }
        }
    }
}

void ATimeLoopPlayerCharacter::ToggleJournal()
{
    UE_LOG(LogTemp, Warning, TEXT("Player toggled journal"));
    
    // This will be implemented when the UI system is available
}

void ATimeLoopPlayerCharacter::ToggleInventory()
{
    UE_LOG(LogTemp, Warning, TEXT("Player toggled inventory"));
    
    // This will be implemented when the UI system is available
}

void ATimeLoopPlayerCharacter::ModifyEnergyLevel(float Delta)
{
    // Update energy level and clamp to valid range
    EnergyLevel = FMath::Clamp(EnergyLevel + Delta, 0.0f, 100.0f);
}

void ATimeLoopPlayerCharacter::UpdateInteractionDetection()
{
    // This will be expanded when the interaction component is implemented
    // For now, just perform a basic raycast to detect potential interactive objects
    
    FHitResult HitResult;
    FVector StartLocation = FollowCamera->GetComponentLocation();
    FVector EndLocation = StartLocation + (FollowCamera->GetForwardVector() * MaxInteractionDistance);
    
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);
    
    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        StartLocation,
        EndLocation,
        ECC_Visibility,
        QueryParams
    );
    
    if (bHit)
    {
        // We have something in range - this would be expanded to check if it's interactable
        // For now, just log that we have a potential target
        // UE_LOG(LogTemp, Verbose, TEXT("Potential interaction target in range: %s"), *HitResult.GetActor()->GetName());
    }
}
