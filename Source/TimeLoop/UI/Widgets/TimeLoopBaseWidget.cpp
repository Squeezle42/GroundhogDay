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

#include "TimeLoopBaseWidget.h"

UTimeLoopBaseWidget::UTimeLoopBaseWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UTimeLoopBaseWidget::InitializeWidget()
{
	// Base initialization - override in child classes
}

void UTimeLoopBaseWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// Call initialization when the widget is constructed
	InitializeWidget();
}

void UTimeLoopBaseWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UTimeLoopBaseWidget::NativeOnAddedToViewport()
{
	Super::NativeOnAddedToViewport();
	
	// Play the show animation by default when added to viewport
	PlayShowAnimation();
}

void UTimeLoopBaseWidget::NativeOnRemoved()
{
	Super::NativeOnRemoved();
}

void UTimeLoopBaseWidget::PlayShowAnimation()
{
	if (ShowAnimation)
	{
		PlayAnimation(ShowAnimation);
	}
}

void UTimeLoopBaseWidget::PlayHideAnimation()
{
	if (HideAnimation)
	{
		PlayAnimation(HideAnimation);
	}
}

APlayerController* UTimeLoopBaseWidget::GetOwningPlayerController() const
{
	return GetOwningPlayer();
}
