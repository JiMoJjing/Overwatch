#include "ActorComponents/Ability/Genji/Genji_SwiftStrikeComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

#include "Components/CapsuleComponent.h"

#include "Kismet/GameplayStatics.h"

#include "Characters/Player/Genji/Genji.h"
#include "Colliders/Genji/SwiftStrikeCollider.h"

#include "Utilities.h"


UGenji_SwiftStrikeComponent::UGenji_SwiftStrikeComponent() :  SwiftStrikeDistance(1884.f), SwiftStrikeSpeed(5000.f),  CapsuleSize2D(42.f, 96.f)
, SwiftStrikeCapsuleSize2D(21.f, 48.f), SwiftStrikeDamage(50.f), bSwiftStrike(false)
{
	PrimaryComponentTick.bCanEverTick = true;

	SwiftStrikeCollider = nullptr;

	OnSwiftStrikeCapsuleSizeTimelineUpdate.BindUFunction(this, FName("SwiftStrikeCapsuleSizeTimelineUpdate"));
	OnSwiftStrikeCapsuleSizeTimelineFinished.BindUFunction(this, FName("SwiftStrikeCapsuleSizeTimelineFinished"));
	
	CooldownTime = 8.f;
}

void UGenji_SwiftStrikeComponent::BeginPlay()
{
	Super::BeginPlay();

	GenjiRef = Cast<AGenji>(GetOwner());
	
	if (GenjiRef)
	{
		GenjiRef->GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &UGenji_SwiftStrikeComponent::OnSwiftStrikeComponentHit);
		GenjiRef->GetCharacterMovement()->MaxFlySpeed = SwiftStrikeSpeed;

		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.Owner = GenjiRef;
		ActorSpawnParams.Instigator = GenjiRef;

		FVector SpawnLocation = FVector::ZeroVector;
		FRotator SpawnRotation = FRotator::ZeroRotator;
		if (SwiftStrikeColliderClass)
		{
			SwiftStrikeCollider = GetWorld()->SpawnActor<ASwiftStrikeCollider>(SwiftStrikeColliderClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			if (SwiftStrikeCollider)
			{
				FAttachmentTransformRules TransformRules(EAttachmentRule::KeepRelative, true);
				SwiftStrikeCollider->AttachToActor(GenjiRef, TransformRules);
			}
		}
		// ��� ���� �� ��ǳ�� ��Ÿ�� �ʱ�ȭ
		GenjiRef->OnDragonbladeActive.AddDynamic(this, &UGenji_SwiftStrikeComponent::DragonbladeActive);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s -> %s -> %s] GenjiRef is nullptr"), *GetOwner()->GetName(), *GetName(), TEXT("BeginPlay"));
	}

	SwiftStrikeCapsuleSizeTimelineSettings();
}

void UGenji_SwiftStrikeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SwiftStrikeCapsuleSizeTimeline.TickTimeline(DeltaTime);
}

void UGenji_SwiftStrikeComponent::StartAbility()
{
	Super::StartAbility();
	
	PlayAbilityMontage();
}

void UGenji_SwiftStrikeComponent::FinishAbility()
{
	Super::FinishAbility();
	SwiftStrikeFinishSetting();
	CooldownTimerStart();
}

void UGenji_SwiftStrikeComponent::PlayAbilityMontage()
{
	if (GenjiRef && AbilityMontage)
	{
		SwiftStrikeStartSetting();
		GenjiRef->GetMesh()->GetAnimInstance()->Montage_Play(AbilityMontage);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s -> %s -> %s] GenjiRef or AbilityMontage is nullptr"), *GetOwner()->GetName(), *GetName(), TEXT("StartAbility"));
	}
}

void UGenji_SwiftStrikeComponent::SetSwiftStrikeStartLocation()
{
	SwiftStrikeStartLocation = GenjiRef->GetActorLocation();
}

void UGenji_SwiftStrikeComponent::SetSwiftStrikeEndLocation()
{
	FHitResult HitResult;
	FVector TraceEndLocation;

	// ���� �������� ��Ÿ� ���� ��ŭ LineTrace ����
	bool bLineTraceSuccess = GenjiRef->TraceUnderCrosshair(SwiftStrikeDistance, HitResult, TraceEndLocation, ECollisionChannel::ECC_Camera);

	// ���� ������ SwiftStrikeEndLocation �� HitResult�� Location���� ����, ���� �� traceEndLocation���� ����
	if (bLineTraceSuccess)
	{
		// Hit�Ǿ��ٸ� �ش� ��ġ���� ĸ�������ŭ �����ؼ� ��ġ�� ���Ѵ�.
		const FVector HitNormal = HitResult.Normal;
		const FVector AdjustedVector(HitNormal.X * CapsuleSize2D.X, HitNormal.Y * CapsuleSize2D.X, HitNormal.Z * CapsuleSize2D.Y);
		SwiftStrikeEndLocation = HitResult.Location + AdjustedVector;
	}
	else
	{
		SwiftStrikeEndLocation = TraceEndLocation;
	}

#if WITH_EDITOR
	// Draw Debug
	DrawDebugSphere(GetWorld(), SwiftStrikeEndLocation, 10.f, 20, FColor::Green, false, 5.f, 0U, 1.f);
	DrawDebugLine(GetWorld(), GenjiRef->GetActorLocation(), SwiftStrikeEndLocation, FColor::Green, false, 5.f, 0U, 1.f);
#endif
}

void UGenji_SwiftStrikeComponent::SwiftStrikeStartSetting()
{
	// ������, ������ ����
	SetSwiftStrikeStartLocation();
	SetSwiftStrikeEndLocation();

	bSwiftStrike = true;

	// ���ʷ� ĸ�� ũ�� Ÿ�Ӷ��� ������ ���� ����Ǹ� Ÿ�Ӷ��� ����
	if (SwiftStrikeCapsuleSizeTimeline.IsPlaying())
	{
		SwiftStrikeCapsuleSizeTimeline.Stop();
	}

	// ĸ�� ������ ����
	GenjiRef->GetCapsuleComponent()->SetCapsuleSize(SwiftStrikeCapsuleSize2D.X, SwiftStrikeCapsuleSize2D.Y);

	// ĸ�� �ݸ����� �� ĸ�� �ݸ����� �����ϵ��� �ϱ�
	GenjiRef->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);

	// ��ǳ�� ���� Input ����
	if (AController* Controller = GenjiRef->GetController())
	{
		Controller->SetIgnoreMoveInput(true);
		Controller->SetIgnoreLookInput(true);
	}
		
	// MovementMode Flying ����
	GenjiRef->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	// �ִ� ���ӵ� ū �� -> Velocity 0���� 5000���� �ް���
	GenjiRef->GetCharacterMovement()->MaxAcceleration = 1000000.f;

	// ��ǳ�� �ݶ��̴� �ݸ��� On
	if (SwiftStrikeCollider)
	{
		SwiftStrikeCollider->Activate();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s -> %s -> %s] SwiftStrikeCollider is nullptr"), *GetOwner()->GetName(), *GetName(), TEXT("BegiSwiftStrikeStartSettingnPlay"));
	}
}

void UGenji_SwiftStrikeComponent::SwiftStrikeFinishSetting()
{
	if (GenjiRef->GetMesh()->GetAnimInstance()->Montage_IsPlaying(AbilityMontage))
	{
		GenjiRef->GetMesh()->GetAnimInstance()->Montage_Stop(0.f, AbilityMontage);
	}

	bSwiftStrike = false;

	// MovementMode Falling ���� ����
	GenjiRef->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);

	// �ִ� ���ӵ� ����ȭ
	GenjiRef->GetCharacterMovement()->MaxAcceleration = 2048.f;

	// ���߿� �ִµ��� ���� �߷°��ӵ� ����
	GenjiRef->StopMovement();

	// Input �ٽ� �ޱ�
	if (AController* Controller = GenjiRef->GetController())
	{
		Controller->SetIgnoreMoveInput(false);
		Controller->SetIgnoreLookInput(false);
	}
	
	// ĸ�� ������Ʈ ũ�� ����
	SwiftStrikeCapsuleSizeTimeline.PlayFromStart();

	// �� ĸ�� ���� �ٽ� Block
	GenjiRef->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block);

	FVector zeroVector = FVector::ZeroVector;
	SwiftStrikeStartLocation = zeroVector;
	SwiftStrikeEndLocation = zeroVector;
	SwiftStrikeHitNormalProjection = zeroVector;

	// ��ǳ�� �ݶ��̴� �ݸ��� ����
	if (SwiftStrikeCollider)
	{
		SwiftStrikeCollider->Deactivate();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s -> %s -> %s] SwiftStrikeCollider is nullptr"), *GetOwner()->GetName(), *GetName(), TEXT("SwiftStrikeFinishSetting"));
	}
}

void UGenji_SwiftStrikeComponent::SwiftStrikeUpdate(float DeltaTime)
{
	// ���� ������ ���� ��ġ���� ���⺤��
	FVector DirectionVector = SwiftStrikeEndLocation - GenjiRef->GetActorLocation();
	DirectionVector.Normalize();

	// ���� ���� ���ϱ�
	DirectionVector += SwiftStrikeHitNormalProjection;
	DirectionVector.Normalize();

	// �̵�
	GenjiRef->AddMovementInput(DirectionVector, 1.f, true);

	// ���� ���ʹ� ���� ����
	SwiftStrikeHitNormalProjection = FMath::VInterpTo(SwiftStrikeHitNormalProjection, FVector::ZeroVector, DeltaTime, HitNormalProjectionInterpSpeed);

	// ��ǥ ��ġ�� �����ϸ� ��Ÿ�� ����
	if (GenjiRef->GetActorLocation().Equals(SwiftStrikeEndLocation, 20.f))
	{
		GenjiRef->GetMesh()->GetAnimInstance()->Montage_Stop(0.f, AbilityMontage);
	}
}

void UGenji_SwiftStrikeComponent::OnSwiftStrikeComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	// ���� ��Ұ�, ��ǳ�� �������� ���
	if (Hit.bBlockingHit && bSwiftStrike)
	{
		// ���� �븻 ���Ϳ� ������ Right���͸� ����
		const FVector HitNormal = Hit.Normal;
		const FVector ActorRightVector = GenjiRef->GetActorRightVector();
		const float dot = FVector::DotProduct(ActorRightVector, HitNormal);

		// ���⿡ �°� ���� ���Ͱ� �Է�, ���� ���� ������ ��� �� ���� x
		if (dot < -0.1f)
		{
			SwiftStrikeHitNormalProjection = FVector(HitNormal.Y * -1, HitNormal.X, 0.f);
		}
		else if (dot > 0.1f)
		{
			SwiftStrikeHitNormalProjection = FVector(HitNormal.Y, HitNormal.X * -1, 0.f);
		}
	}
}

void UGenji_SwiftStrikeComponent::SwiftStrikeCapsuleSizeTimelineSettings()
{
	if (SwiftStrikeCapsuleSizeCurveFloat)
	{
		SwiftStrikeCapsuleSizeTimeline.AddInterpFloat(SwiftStrikeCapsuleSizeCurveFloat, OnSwiftStrikeCapsuleSizeTimelineUpdate);
		SwiftStrikeCapsuleSizeTimeline.SetTimelineFinishedFunc(OnSwiftStrikeCapsuleSizeTimelineFinished);
		SwiftStrikeCapsuleSizeTimeline.SetLooping(false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s -> %s -> %s] SwiftStrikeCapsuleSizeCurveFloat is nullptr"), *GetOwner()->GetName(), *GetName(), TEXT("SwiftStrikeCapsuleSizeTimelineSettings"));
	}
}

void UGenji_SwiftStrikeComponent::SwiftStrikeCapsuleSizeTimelineUpdate(float Alpha)
{
	FVector2D CapsuleSize = FMath::Lerp(SwiftStrikeCapsuleSize2D, CapsuleSize2D, Alpha);
	 
	GenjiRef->GetCapsuleComponent()->SetCapsuleSize(CapsuleSize.X, CapsuleSize.Y);
}

void UGenji_SwiftStrikeComponent::SwiftStrikeCapsuleSizeTimelineFinished()
{
	GenjiRef->GetCapsuleComponent()->SetCapsuleSize(CapsuleSize2D.X, CapsuleSize2D.Y);
}

void UGenji_SwiftStrikeComponent::DragonbladeActive()
{
	CooldownReset(); 
}

void UGenji_SwiftStrikeComponent::SwiftStrikeMontageFinished()
{
	FinishAbility();
}
