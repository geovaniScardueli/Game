// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "DefenseAtackNotify.generated.h"

/**
 * 
 */
UCLASS()
class PRIMEIROGAME_API UDefenseAtackNotify : public UAnimNotify
{
	GENERATED_BODY()

virtual bool ShouldFireInEditor() override { return false; }

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
