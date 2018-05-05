// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaMenuWidget.h"

#include "Entity/Character/NaEntityCharacter.h"

//////////////////////////////////////////////////
// public methods
//////////////////////////////////////////////////
//! 
bool UNaMenuWidget::Initialize()
{
	if ( Super::Initialize() ){
		//! 
		m_LvMenu	= WidgetTree->FindWidget<UNaListView>( "MenuList" );
		if ( m_LvMenu ){
		}

		//! 
		m_LvParty	= WidgetTree->FindWidget<UNaListView>( "PartyList" );
		if ( m_LvParty ){
		}

		m_SM->RegisterState( EState::Main, this, &UNaMenuWidget::ProcMain );

		return true;
	}
	else {
		return false;
	}
}

//!
void UNaMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

//! Tick
void UNaMenuWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick( MyGeometry, InDeltaTime );
}

//! �L�[����
FReply UNaMenuWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	switch ( m_SM->GetState() ){
	case EState::Main:
		ProcMainKeyDown( m_SM, &InKeyEvent );
		break;
	}

	return FReply::Unhandled();
}

//! �X�e�[�g�ݒ�
void UNaMenuWidget::SetState( EState state )
{
	m_SM->ChangeState( state );
}

//! ���j���[���ڒǉ�
void UNaMenuWidget::AddMenuItem( ENaMenuCategory category )
{
	m_MenuItems.Add( category );
}

//! ���j���[���ڃN���A
void UNaMenuWidget::ClearMenuItems()
{
	m_MenuItems.Reset();
}

//! �p�[�e�B�ǉ�
void UNaMenuWidget::AddPartyMember( UNaEntityCharacter* chara )
{
	m_PartyMembers.Add( chara );
}

//! �p�[�e�B�N���A
void UNaMenuWidget::ClearParty()
{
	m_PartyMembers.Reset();
}

//////////////////////////////////////////////////
// protected methods
//////////////////////////////////////////////////
//! 
void UNaMenuWidget::ProcMain( UNaStateMachine* sm, float DeltaTime )
{
	//! 
	enum EPhase
	{
		//! 
		Init,
		//! 
		Start,
		StartWait,
		//! 
		Main,
		//! 
		Decided,
		//! 
		End,
	};

	//! ���C��
	switch ( sm->GetPhase() ){
	case Init:
		UpdateMenuItems();
		UpdatePartyItems();
		sm->SetPhase( Start );
		break;

	//! 
	case Start:
		Transition( "Show" );
		sm->Advance();
		break;
	case StartWait:
		if ( !IsTransition() ){
			m_LvParty->SetKeyboardFocus();
			sm->SetPhase( Main );
		}
		break;

	case Main:
		break;

	case Decided:
		break;

	case End:
		break;
	}
}
FReply UNaMenuWidget::ProcMainKeyDown( UNaStateMachine* sm, const FKeyEvent* KeyEvent )
{
	//! 
	enum EPhase
	{
		//! 
		Init,
		//! 
		Start,
		StartWait,
		//! 
		Main,
		//! 
		Decided,
		//! 
		End,
	};
	const FKey	key = KeyEvent->GetKey();

	//! �L�[�C�x���g
	switch ( sm->GetPhase() ){
	case Main:
		if ( key == EKeys::Enter ){
			sm->SetPhase( Decided );
		}
		break;
	}

	return FReply::Handled();
}

//! ���j���[�J�e�S������
void UNaMenuWidget::UpdateMenuItems()
{
	if ( m_LvMenu ){
		FNaListItem	lvi;

		m_LvMenu->DeleteAllItems();

		for ( auto& it : m_MenuItems ){
			const FNaMenuItemDefinition&	def = MenuItemDefinition[int32(it)];

			lvi.Value	= def.DisplayName.ToString();
			lvi.Params.Reset();
			lvi.Params.Add( "icon", def.Icon.ToString() );
			m_LvMenu->AddItem( lvi );
		}
	}
}

//! �p�[�e�B���X�g����
void UNaMenuWidget::UpdatePartyItems()
{
	if ( m_LvParty ){
		FNaEntityProfile*	profile;
		FNaCharacterStatus*	status;
		FNaListItem			lvi;

		m_LvParty->DeleteAllItems();

		for ( auto& it : m_PartyMembers ){
			profile	= it->GetProfile();
			status	= it->GetStatus();

			lvi.Value	= profile->DisplayName;
			lvi.Params.Reset();
			lvi.Params.Add( "hp", FString::FromInt( status->HP ) );
			lvi.Params.Add( "mhp", FString::FromInt( status->HPMax ) );
			m_LvParty->AddItem( lvi );
		}
	}
}
