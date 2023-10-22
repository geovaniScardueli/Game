// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "PlayerItemNotify.generated.h"

/**
 * 
 */

UENUM()
namespace EMontageItens
{
	enum Type
	{
		Heal
	};
}

UCLASS()
class PRIMEIROGAME_API UPlayerItemNotify : public UAnimNotify
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	TEnumAsByte<EMontageItens::Type> IdSection;

	virtual bool ShouldFireInEditor() override { return false; }

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
