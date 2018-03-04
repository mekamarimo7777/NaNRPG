// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"

#include "NaAIManipulator.h"

#include "Actor/Entity/Character/NaCharacter.h"

#include "Entity/Character/NaEntityCharacter.h"

// XVˆ—
void UNaAIManipulator::OnControl( float DeltaTime )
{
	if ( m_Target ){
		UNaStateMachine*	sm = m_Target->GetStateMachine();
		ANaActorBase*		actor = m_Target->GetActor();

		switch ( sm->GetState() ){
		case UNaEntityCharacter::EState::Waiting:
			break;
		case UNaEntityCharacter::EState::Action:
			ProcAction( sm, DeltaTime );
			break;
		}
	}

	Super::OnControl( DeltaTime );
}

//
void UNaAIManipulator::ProcAction( UNaStateMachine* sm, float DeltaTime )
{
	enum StateStep
	{
		Start,
		Main,
		Attack,
		WaitAttack,
		End
	};
	ANaActorBase*		actor = m_Target->GetActor();
	UWorld*				world = actor->GetWorld();
	UNaWorld*			naw = m_Target->GetNaWorld();
	APlayerController*	pc = world->GetFirstPlayerController();

	switch ( sm->GetPhase() ){
	case Start:
		sm->AdvancePhase();
		// fall through
	case Main:
		//! 
		LockTarget();

		if ( m_LockTarget ){
			FIntVector	vec;
			int32		tmp;

			vec	= m_LockTarget->GetWorldPosition() - m_Target->GetWorldPosition();
			tmp	= FMath::RadiansToDegrees( FMath::Atan2( -vec.X, vec.Y ) ) + 180.0f;

//			GEngine->AddOnScreenDebugMessage( -1, 3.0f, FColor::White, FString::Printf(TEXT("DIR: %d"), tmp) );

			tmp	%= 360;
			tmp	-= 22.5f;
			tmp	= FMath::CeilToInt( tmp / 45.0f );
			
			if ( vec.Size() <= 1 ){
				m_Target->SetDirection( ENaDirection(tmp) );
				sm->SetPhase( Attack );
			}
			else {
				MoveTo( ENaDirection(tmp) );
				sm->ChangeState( UNaEntityCharacter::EState::EndTurn );
			}
		}
		else {
			ENaDirection	dir = ENaDirection( FMath::RandRange( 0, 7 ) );

			MoveTo( dir );
			sm->ChangeState( UNaEntityCharacter::EState::EndTurn );
		}
/*
		{
//		if ( !actor->IsMoving() ){
			
			if ( FMath::RandRange( 0, 100 ) < 20 ){
				sm->SetPhase( Attack );
				break;
			}
			else {
				ENaDirection	dir = ENaDirection( FMath::RandRange( 0, 7 ) );

				MoveTo( dir );

				sm->ChangeState( UNaEntityCharacter::EState::EndTurn );
			}
		}*/
		break;
	case Attack:
		if ( !actor->IsMoving() ){
			actor->ChangeState( ANaActorBase::EState::Attack );
			sm->AdvancePhase();
		}
		break;
	case WaitAttack:
		if ( actor->IsWaiting() ){
			m_Target->Attack();
			sm->SetPhase( End );
		}
		break;
	case End:
		sm->ChangeState( UNaEntityCharacter::EState::EndTurn );
		break;
	}
}

//
void UNaAIManipulator::LockTarget()
{
	const TArray<UNaEntity*>&	entities = m_Target->GetNaWorld()->GetEntities();

	m_LockTarget	= nullptr;

	for ( auto& it : entities ){
		if ( it->IsAbstract() ){
			continue;
		}
		if ( it->GetType() == ENaEntity::Player ){
			FIntVector	vec;

			vec	= m_Target->GetWorldPosition() - it->GetWorldPosition();
//			if ( vec.Size() <= 8 ){
				m_LockTarget	= it;
				return;
//			}
		}
	}
}

//
void UNaAIManipulator::MoveTo( ENaDirection dir )
{
	if ( dir != ENaDirection::Invalid ){
		UNaWorld*	naw = m_Target->GetNaWorld();
		FIntVector	pos;
		bool		bRes;

//		dir	= ENaDirection( dir + naw->GetWorldDirection() & 0x7 );
		m_Target->SetDirection( dir );

		bRes	= m_Target->FindMovePosition( dir, pos );
		if ( bRes ){
			FIntVector	tpos;

			m_Target->MoveToWorldPosition( pos );

			naw->FindGroundPos( pos, tpos );
			if ( pos.Z > tpos.Z ){
				m_Target->MoveToWorldPosition( tpos );
			}
		}
	}
}
