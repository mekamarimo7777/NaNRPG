// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaWidget.h"


//////////////////////////////////////////////////
// public methods
//////////////////////////////////////////////////
//! ウィジェット初期化
bool UNaWidget::Initialize()
{
	if ( Super::Initialize() ){
		m_SM	= NewObject<UNaStateMachine>();
		return true;
	}
	else {
		return false;
	}
}

//! 
void UNaWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

//! 更新処理
void UNaWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick( MyGeometry, InDeltaTime );

	//! トランジション変更
	if ( m_CurrentTrans != m_RequestTrans ){
		if ( OnTransition( m_RequestTrans ) ){
			m_CurrentTrans	= m_RequestTrans;
		}
	}

	//! トランジションタスク実行
	ExecTransTask();

	m_SM->Execute( InDeltaTime );
}

//! ビューポートへの追加
void UNaWidget::Open( int32 ZOrder )
{
	if ( !IsInViewport() ){
		AddToPlayerScreen( ZOrder );
	}
}

//! ビューポートから削除
void UNaWidget::Close()
{
}

//! トランジションリクエスト
void UNaWidget::Transition( FName id )
{
	if ( m_CurrentTrans != id && m_RequestTrans != id ){
		m_RequestTrans	= id;
	}
}

//! トランジション判定
bool UNaWidget::IsTransition() const
{
	return m_IsTransition;
}

//!
bool UNaWidget::HasNestFocus() const
{
	TArray<UWidget*>	widgets;

	WidgetTree->GetAllWidgets( widgets );

	for ( auto& it : widgets ){
		if ( it->HasKeyboardFocus() ){
			return true;
		}
	}

	return false;
}

//! フォーカス退避
void UNaWidget::StorePrevFocus()
{
	m_PrevFocus	= FSlateApplication::Get().GetKeyboardFocusedWidget();
}

//! フォーカス復元
void UNaWidget::RestorePrevFocus()
{
	if ( m_PrevFocus.IsValid() ){
		if ( m_PrevFocus == FSlateApplication::Get().GetGameViewport() ){
			APlayerController*	pc = UGameplayStatics::GetPlayerController( this, 0 );
			pc->SetInputMode( FInputModeGameAndUI() );
		}
		FSlateApplication::Get().SetKeyboardFocus( m_PrevFocus );
	}
	else {
		FSlateApplication::Get().SetUserFocusToGameViewport( 0 );
	}
	m_PrevFocus.Reset();
}

//! 
void UNaWidget::SetInputModeUI()
{
	APlayerController*	pc = GetWorld()->GetFirstPlayerController();

	pc->SetInputMode( FInputModeUIOnly() );
}
void UNaWidget::SetInputModeGameAndUI()
{
	APlayerController*	pc = GetWorld()->GetFirstPlayerController();

	pc->SetInputMode( FInputModeGameAndUI() );
}

//! パラメータ設定
void UNaWidget::SetParameter( FName key, const FString& value )
{
	m_Parameters.Add( key, value );
}

//! パラメータ取得
FString UNaWidget::GetParameter( FName key ) const
{
	return m_Parameters.FindRef( key );
}
//! テクスチャパラメータ取得
UTexture* UNaWidget::GetParameterAsTexture( FName key ) const
{
	UTexture*	tex = nullptr;

	//!@todo 今後非同期ロードに変更
	if ( !m_Parameters[key].IsEmpty() ){
		tex	= FindObject<UTexture>( nullptr, *m_Parameters[key] );
		if ( !tex ){
			tex	= LoadObject<UTexture>( nullptr, *m_Parameters[key], nullptr, LOAD_None );
		}
	}

	return tex;
}

//////////////////////////////////////////////////
// protected methods
//////////////////////////////////////////////////
//! トランジション開始イベント
bool UNaWidget::OnTransition( FName id )
{
	return true;
}

//! トランジション開始
void UNaWidget::BeginTransition()
{
	m_IsTransition	= true;
	OnBeginTransition( m_CurrentTrans );
}

//! トランジション完了
void UNaWidget::EndTransition()
{
	OnEndTransition( m_CurrentTrans );
	m_IsTransition	= false;
}

//! 
void UNaWidget::RequestTransition( UNaWidget* widget, FString value )
{
	FNaTransitionTask	task;

	task.Type	= ENaTransitionTask::Transition;
	task.State	= ENaTransitionTaskState::Ready;
	task.Object	= widget;
	task.Params.Add( value );

	m_TransTask.Add( task );
}

//! 
void UNaWidget::RequestAnimation( UWidgetAnimation* anim )
{
	FNaTransitionTask	task;

	task.Type	= ENaTransitionTask::Animation;
	task.State	= ENaTransitionTaskState::Ready;
	task.Object	= anim;

	m_TransTask.Add( task );
}

//!
void UNaWidget::ExecTransTask()
{
	for ( auto& it : m_TransTask ){
		switch ( it.Type ){
		//! トランジション
		case ENaTransitionTask::Transition:
			if ( UNaWidget* widget = Cast<UNaWidget>( it.Object ) ){
				switch ( it.State ){
				case ENaTransitionTaskState::Ready:
					widget->Transition( FName( *it.Params[0] ) );
					it.State	= ENaTransitionTaskState::Processing;
					break;

				case ENaTransitionTaskState::Processing:
					if ( !widget->IsTransition() ){
						it.State	= ENaTransitionTaskState::Completed;
					}
					break;
				}
			}
			else {
				it.State	= ENaTransitionTaskState::Completed;
			}
			break;

		//! アニメーション
		case ENaTransitionTask::Animation:
			if ( UWidgetAnimation* anim = Cast<UWidgetAnimation>( it.Object ) ){
				switch ( it.State ){
				case ENaTransitionTaskState::Ready:
					PlayAnimation( anim, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f );
					it.State	= ENaTransitionTaskState::Processing;
					break;

				case ENaTransitionTaskState::Processing:
					if ( IsAnimationPlaying( anim ) ){
						it.State	= ENaTransitionTaskState::Completed;
					}
					break;
				}
			}
			else {
				it.State	= ENaTransitionTaskState::Completed;
			}
			break;
		}
	}

	//! 削除
	m_TransTask.RemoveAll( []( FNaTransitionTask& p )
	{
		return p.State == ENaTransitionTaskState::Completed;
	});
}
