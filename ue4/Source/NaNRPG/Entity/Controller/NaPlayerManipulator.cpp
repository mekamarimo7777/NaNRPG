// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"

#include "NaPlayerManipulator.h"

#include "Actor/Entity/Character/NaCharacter.h"

#include "UI/Widgets/Common/NaPopupDamage.h"

#include "Entity/Character/NaEntityPlayer.h"
#include "Entity/INaEntityFactory.h"

#include "Utility/Components/NaStateMachine.h"

#include "World/Event/NaEventManager.h"

#include "Singleton/NaGameInstance.h"
#include "Singleton/NaGameLogger.h"

#include "HUD/NaGameHUD.h"

#include "UI/Actor/Menu/NaMenuAgent.h"

#include "GameMode/NaMainGameMode.h"


//////////////////////////////////////////////////
// public methods
//////////////////////////////////////////////////

//////////////////////////////////////////////////
// protected methods
//////////////////////////////////////////////////
// 更新処理
void UNaPlayerManipulator::OnControl( float DeltaTime )
{
	if ( m_Target ){
		UNaStateMachine*	sm = m_Target->GetStateMachine();

		switch ( sm->GetState() ){
		case UNaEntityCharacter::EState::Waiting:
			ProcWaiting( sm, DeltaTime );
			break;
		case UNaEntityCharacter::EState::Action:
			ProcAction( sm, DeltaTime );
			break;
		case UNaEntityCharacter::EState::Menu:
			ProcMenu( sm, DeltaTime );
			break;
		case UNaEntityCharacter::EState::Inventory:
			ProcInventory( sm, DeltaTime );
			break;
		case UNaEntityCharacter::EState::Equipment:
			ProcEquipment( sm, DeltaTime );
			break;
		case UNaEntityCharacter::EState::Event:
			ProcEvent( sm, DeltaTime );
			break;
		}
	}

	Super::OnControl( DeltaTime );
}

//
void UNaPlayerManipulator::ProcWaiting( UNaStateMachine* sm, float DeltaTime )
{
}

//
void UNaPlayerManipulator::ProcAction( UNaStateMachine* sm, float DeltaTime )
{
	enum StateStep
	{
		Start,
		Main,
		Contact,
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
		if ( !actor->IsMoving() ){
			ENaDirection	dir = ENaDirection::Invalid;

			//! combat
			if ( pc->WasInputKeyJustPressed( EKeys::SpaceBar ) ){
				sm->SetPhase( Attack );
				break;
			}

			//! interact
			if ( pc->WasInputKeyJustPressed( EKeys::Enter ) ){
				sm->ChangeState( UNaEntityCharacter::EState::Event );
				break;
			}

			// test //
/*			if ( pc->WasInputKeyJustPressed( EKeys::Z ) ){
				UNaEntity*	entity;

				entity	= INaEntityFactory::NewEntity( ENaEntity::PORTAL );
				entity->SetStage( ENaEntityStage::Local );
				entity->SetWorldPosition( m_Target->GetWorldPosition() );
				naw->SpawnEntity( entity );

				sm->SetPhase( End );
				break;
			}*/

			// generate item(test)
/*			if ( pc->WasInputKeyJustPressed( EKeys::G ) ){
				UNaEntityItem*	item;

				item	= UNaEntityItem::GenerateItem( naw->DataAsset, 0, 0 );
				if ( item ){
					item->SetStage( ENaEntityStage::Local );
					item->SetWorldPosition( m_Target->GetWorldPosition() );
					naw->SpawnEntity( item );
				}
			}*/
			// take
/*			if ( pc->WasInputKeyJustPressed( EKeys::T ) ){
				TArray<UNaEntity*>	entities;

				naw->FindEntity( m_Target->GetWorldPosition(), entities );
				for ( auto& it : entities ){
					if ( it == m_Target ){
						continue;
					}
					if ( it->GetType() == ENaEntity::ITEM ){
						m_Target->GetInventory()->m_pItems.Add( Cast<UNaEntityItem>( it ) );
						it->Kill();
					}
				}
			}*/
			// open menu
			if ( pc->WasInputKeyJustPressed( EKeys::M ) ){
				sm->ChangeState( UNaEntityCharacter::EState::Menu, ANaMenuAgent::EState::Main );
				break;
			}
			// open inventory
			if ( pc->WasInputKeyJustPressed( EKeys::I ) ){
				sm->ChangeState( UNaEntityCharacter::EState::Menu, ANaMenuAgent::EState::Inventory );
				break;
			}

			if ( pc->WasInputKeyJustPressed( EKeys::X ) ){
//				UNaPopupDamage*	uiPopup;
//				FVector			pos;
//				FVector2D		spos;

//				pc->ProjectWorldLocationToScreen( actor->GetActorLocation(), spos );

//				uiPopup	= CreateWidget<UNaPopupDamage>( world, naw->DataAsset->TestDialogClass );
//				uiPopup->SetPositionInViewport( spos - FVector2D( 128.0f, 128.0f ) );
//				uiPopup->SetDamage( FMath::RandRange(0, 9999) );
//				uiPopup->AddToViewport();

//				m_pDialog	= CreateWidget<UUserWidget>( m_pWorld->GetUWorld(), m_pWorld->DataAsset->TestDialogClass );
//				m_pDialog->AddToViewport();
/*				m_pDialog	= CreateWidget<UNaUIPopupDamage>( m_pWorld->GetUWorld(), m_pWorld->DataAsset->TestDialogClass );
				m_pDialog->AddToViewport();
				m_pDialog->SetDamage( FMath::Rand() % 10000 );
				break;*/
			}

			if ( pc->WasInputKeyJustPressed( EKeys::NumPadSeven ) ){
				int32	tmp = int32( naw->GetWorldDirection() );

				tmp	= (tmp - 1) & 0x7;
				naw->SetWorldDirection( (ENaDirection)tmp );
			}
			else if ( pc->WasInputKeyJustPressed( EKeys::NumPadNine ) ){
				int32	tmp = int32( naw->GetWorldDirection() );

				tmp	= (tmp + 1) & 0x7;
				naw->SetWorldDirection( (ENaDirection)tmp );
			}

			if ( pc->WasInputKeyJustPressed( EKeys::NumPadZero ) ){
				ANaMainGameMode*	gm = Cast<ANaMainGameMode>( UGameplayStatics::GetGameMode( naw->GetWorldContext() ) );
				gm->TravelToWorld(0);
			}
			if ( pc->WasInputKeyJustPressed( EKeys::NumPadOne ) ){
				ANaMainGameMode*	gm = Cast<ANaMainGameMode>( UGameplayStatics::GetGameMode( naw->GetWorldContext() ) );
				gm->TravelToWorld(1);
			}
			if ( pc->WasInputKeyJustPressed( EKeys::NumPadTwo ) ){
				ANaMainGameMode*	gm = Cast<ANaMainGameMode>( UGameplayStatics::GetGameMode( naw->GetWorldContext() ) );
				gm->TravelToWorld(2);
			}
			if ( pc->WasInputKeyJustPressed( EKeys::NumPadThree ) ){
				ANaMainGameMode*	gm = Cast<ANaMainGameMode>( UGameplayStatics::GetGameMode( naw->GetWorldContext() ) );
				gm->TravelToWorld(3);
			}

			// 移動 //
			if ( pc->IsInputKeyDown( EKeys::W ) ){
				if ( pc->IsInputKeyDown( EKeys::A ) ){
					dir	= ENaDirection::NW;
				}
				else if ( pc->IsInputKeyDown( EKeys::D ) ){
					dir	= ENaDirection::NE;
				}
				else if ( !pc->IsInputKeyDown( EKeys::LeftShift ) && !pc->IsInputKeyDown( EKeys::RightShift ) ){
					dir	= ENaDirection::North;
				}
			}
			else if ( pc->IsInputKeyDown( EKeys::S ) ){
				if ( pc->IsInputKeyDown( EKeys::A ) ){
					dir	= ENaDirection::SW;
				}
				else if ( pc->IsInputKeyDown( EKeys::D ) ){
					dir	= ENaDirection::SE;
				}
				else if ( !pc->IsInputKeyDown( EKeys::LeftShift ) && !pc->IsInputKeyDown( EKeys::RightShift ) ){
					dir	= ENaDirection::South;
				}
			}
			else if ( !pc->IsInputKeyDown( EKeys::LeftShift ) && !pc->IsInputKeyDown( EKeys::RightShift ) ){
				if ( pc->IsInputKeyDown( EKeys::A ) ){
					dir	= ENaDirection::West;
				}
				else if ( pc->IsInputKeyDown( EKeys::D ) ){
					dir	= ENaDirection::East;
				}
				else if ( pc->WasInputKeyJustPressed( EKeys::Q ) ){
					m_Target->SetDirection( ENaDirection( (int32)m_Target->GetDirection() - 1 & 0x7 ) );
				}
				else if ( pc->WasInputKeyJustPressed( EKeys::E ) ){
					m_Target->SetDirection( ENaDirection( (int32)m_Target->GetDirection() + 1 & 0x7 ) );
				}
			}

			if ( dir != ENaDirection::Invalid ){
				FIntVector	pos;
				bool		bRes;

				dir	= ENaDirection( int32(dir) + int32( naw->GetWorldDirection() ) & 0x7 );
				m_Target->SetDirection( dir );

				if ( !pc->IsInputKeyDown( EKeys::LeftControl ) && !pc->IsInputKeyDown( EKeys::RightControl ) ){
					bRes	= m_Target->FindMovePosition( dir, pos );
					if ( bRes ){
						FIntVector	tpos;

						m_Target->MoveToWorldPosition( pos );

						naw->FindGroundPos( pos, tpos );
						if ( pos.Z > tpos.Z ){
							m_Target->MoveToWorldPosition( tpos );
						}

						sm->SetPhase( End );
					}
				}
			}
		}
		break;
	case Contact:
		break;
	case Attack:
		actor->ChangeState( ANaActorBase::EState::Attack );
		sm->AdvancePhase();
		break;
	case WaitAttack:
		if ( actor->IsWaiting() ){
			m_Target->Attack();
			sm->SetPhase( End );
		}
		break;

	case End:
		// @test プレイヤー移動位置を更新（暫定）
		naw->SetCurrentPosition( m_Target->GetWorldPosition() );

		sm->ChangeState( UNaEntityCharacter::EState::EndTurn );
		break;
	}
}

//
void UNaPlayerManipulator::ProcMenu( UNaStateMachine* sm, float DeltaTime )
{
	enum StateStep
	{
		//! 
		Init,
		Start,
		//! 
		Main,
		//! 
		End
	};

	switch ( sm->GetPhase() ){
	//! 初期化
	case Init:
		CreateMenuAgent();
		sm->AdvancePhase();
		break;
	case Start:
		m_UIAMenu->Start( sm->GetParam() );
		sm->AdvancePhase();
		break;

	//! メイン
	case Main:
		if ( m_UIAMenu->WaitForAction() ){
			ANaMenuAgent::EResult	result = m_UIAMenu->GetResult();

			switch ( result ){
			case ANaMenuAgent::EResult::Exit:
				break;

			case ANaMenuAgent::EResult::SelectItem:
				break;

			case ANaMenuAgent::EResult::SelectSkill:
				break;
			}
		}
		break;

	//! 
	case End:
		break;
	}


/*	enum StateStep
	{
		Start,
		Main,
		End
	};
	ANaActorBase*	actor = m_Target->GetActor();
	ANaGameHUD*		hud = m_Target->GetHUD();
	UNaUIMainMenu*	view = hud->GetMainMenu();

	switch ( sm->GetPhase() ){
	case Start:
		view->Open();
		sm->AdvancePhase();
		break;
	case Main:
		{
			UWorld*				world = actor->GetWorld();
			APlayerController*	pc = world->GetFirstPlayerController();
			EUIResult::Type		result;

			result	= view->GetResult();

			switch ( result ){
			case EUIResult::Decided:
				{
					FName	res;

					res = view->GetCategory();
					if ( res == "inventory" ){
						view->Close();
						sm->ChangeState( UNaEntityCharacter::EState::Inventory );
					}
					else if ( res == "equip" ){
						view->Close();
						sm->ChangeState( UNaEntityCharacter::EState::Equipment );
					}
					else if ( res == "status" ){
					
					}
					else {
						sm->AdvancePhase();
					}
				}
				break;
			case EUIResult::Canceled:
				sm->AdvancePhase();
				break;
			}
		}
		break;
	case End:
		view->Close();
		sm->ChangeState( UNaEntityCharacter::EState::Action );
		break;
	}*/
}

//! 
void UNaPlayerManipulator::ProcInventory( UNaStateMachine* sm, float DeltaTime )
{
/*	enum StateStep
	{
		Start,
		Main,
		End,
		EndTurn
	};
	ANaActorBase*	actor = m_Target->GetActor();
	ANaGameHUD*		hud = m_Target->GetHUD();
	UNaUIInventory*	view = hud->GetInventoryView();
	UNaWorld*		naw = m_Target->GetNaWorld();

	switch ( sm->GetPhase() ){
	case Start:
		{// 干渉対象列挙
			const TArray<UNaEntity*>&	entities = naw->GetEntities();

			for ( auto& it : entities ){
				if ( !it->GetItemProperty() ){
					continue;
				}
				if ( it->GetWorldPosition() == m_Target->GetWorldPosition() ){
					view->AddEntity( it );
				}
			}
		}
		view->SetInventory( m_Target->GetInventory() );
		view->Open();

		view->SetAction( ENaActionTrigger::None );
		view->SetMode( ENaInventoryMode::Inventory );

		sm->AdvancePhase();
		break;
	case Main:
		{
			UWorld*				world = actor->GetWorld();
			APlayerController*	pc = world->GetFirstPlayerController();
			EUIResult::Type		result;

			result	= view->GetResult();

			switch ( result ){
			case EUIResult::Decided:
				{
					FNaItemActionResult	ares;
					UNaInventory*		inv = m_Target->GetInventory();

					ares	= view->GetActionResult();

					switch ( ares.Action ){
					case ENaActionTrigger::Take:
						if ( ares.Item->GetOwner() ){
							inv->AddItem( ares.Item );

							view->SetInventory( m_Target->GetInventory() );
							view->RemoveEntity( ares.Item->GetOwner() );

							naw->DespawnEntity( ares.Item->GetOwner() );
							ares.Item->SetOwner( nullptr );

							UNaGameLogger::Get()->LogTakeItem( m_Target, ares.Item );
						}
						break;
					case ENaActionTrigger::Drop:
						{
							UNaEntityCharacter*	entity;

							entity	= ares.Item->GenerateEntity();
							naw->SpawnEntity( entity, m_Target->GetWorldPosition() );

							inv->RemoveItem( ares.Item );

							view->AddEntity( entity );
							view->RemoveItem( ares.Item );
						}
						break;
					case ENaActionTrigger::Quaff:
						m_Target->UseItem( ares.Item, ares.Action );

						ares.Item->AddStack( -1 );
						if ( ares.Item->GetStackCount() <= 0 ){
							if ( ares.Item->GetOwner() ){
								view->RemoveEntity( ares.Item->GetOwner() );
								naw->DespawnEntity( ares.Item->GetOwner() );
							}
							else {
								view->RemoveItem( ares.Item );
								inv->RemoveItem( ares.Item );
							}
						}

						UNaGameLogger::Get()->LogAction( m_Target, ares.Item, ares.Action );

						sm->SetPhase( EndTurn );
						break;
					}

					view->ClearResult();
				}
				break;
			case EUIResult::Canceled:
				sm->AdvancePhase();
				break;
			}
		}
		break;
	case End:
		view->Close();
		sm->ChangeState( UNaEntityCharacter::EState::Action );
		break;
	case EndTurn:
		view->Close();
		sm->ChangeState( UNaEntityCharacter::EState::EndTurn );
		break;
	}*/
}

//! 
void UNaPlayerManipulator::ProcEquipment( UNaStateMachine* sm, float DeltaTime )
{
/*	enum StateStep
	{
		Start,
		Main,
		End
	};
	ANaActorBase*		actor = m_Target->GetActor();
	ANaGameHUD*			hud = m_Target->GetHUD();
	UNaEquipmentWidget*	view = hud->GetEquipmentWidget();
	UNaWorld*			naw = m_Target->GetNaWorld();

	switch ( sm->GetPhase() ){
	case Start:
		view->SetCharacter( m_Target );
		view->Open();
		sm->AdvancePhase();
		break;
	case Main:
		{
			UWorld*				world = actor->GetWorld();
			APlayerController*	pc = world->GetFirstPlayerController();
			EUIResult::Type		result;

			result	= view->GetResult();

			switch ( result ){
			case EUIResult::Decided:
				{
				}
				break;
			case EUIResult::Canceled:
				sm->AdvancePhase();
				break;
			}
		}
		break;
	case End:
		view->Close();
		sm->ChangeState( UNaEntityCharacter::EState::Action );
		break;
	}*/
}

//! イベント実行中
void UNaPlayerManipulator::ProcEvent( UNaStateMachine* sm, float DeltaTime )
{
	enum StateStep
	{
		//! 開始
		Start,
		//! メイン
		Main,
		//! 終了
		End
	};
	ANaActorBase*	actor = m_Target->GetActor();
	UNaWorld*		naw = m_Target->GetNaWorld();

	switch ( sm->GetPhase() ){
	//! 
	case Start:
		{
			TArray<UNaEntity*>	entities;

			m_Target->GatherInteractTarget( entities );

			if ( entities.Num() ){
				const UNaEventAsset*	evt;
				int32	sheet;

				if ( entities[0]->FindEvent( ENaEventTrigger::Interacted, evt, sheet ) ){
					UNaEventManager*	em = naw->GetEventManager();

					em->PlayEvent( evt, sheet );
					sm->AdvancePhase();
					break;
				}
			}

			//! nothing
			sm->SetPhase( End );
		}
		break;

	case Main:
		{
			UNaEventManager*	em = naw->GetEventManager();
			
			if ( !em->IsPlaying() ){
				sm->AdvancePhase();
			}
		}
		break;

	case End:
		//! end
		sm->ChangeState( UNaEntityCharacter::EState::Action );
		break;
	}
}

//! メニューUI生成
void UNaPlayerManipulator::CreateMenuAgent()
{
	UWorld*				world	= GetWorldContext();
	UNaGameInstance*	gi = UNaGameInstance::Get( world );

	if ( !m_UIAMenu ){
		UClass*	cls = gi->FindUI( "Menu" );

		if ( cls ){
			m_UIAMenu	= world->SpawnActor<ANaMenuAgent>( cls );
		}
	}
}

//! メニューUI削除
void UNaPlayerManipulator::DisposeMenuAgent()
{
	if ( m_UIAMenu ){
		m_UIAMenu	= nullptr;
	}
}
