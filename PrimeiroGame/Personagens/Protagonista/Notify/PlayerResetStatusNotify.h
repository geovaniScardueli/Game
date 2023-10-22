// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "PlayerResetStatusNotify.generated.h"

/**
 * 
 */
UCLASS()
class PRIMEIROGAME_API UPlayerResetStatusNotify : public UAnimNotify
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = AnimNotify)
	FName IdSection;

	virtual bool ShouldFireInEditor() override { return false; }

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
