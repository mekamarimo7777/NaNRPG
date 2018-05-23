// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaWidget.h"


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

	if ( m_CurrentTrans != m_RequestTrans ){
		if ( OnTransition( m_RequestTrans ) ){
			//! BP
			OnBeginTransition( m_RequestTrans, m_CurrentTrans );

			m_CurrentTrans	= m_RequestTrans;
		}
	}

	m_SM->Execute( InDeltaTime );
}

//! ビューポートへの追加
void UNaWidget::Open( int32 ZOrder )
{
	if ( !IsInViewport() ){
		AddToPlayerScreen( ZOrder );
	}

	EventOpen();
}

//! ビューポートから削除
void UNaWidget::Close()
{
	EventClose();
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
	return false;
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

//!
void UNaWidget::SetResponse( const FString& key, int32 value )
{
	if ( value != 0 ){
		m_Response[key]	= value;
	}
	else {
		m_Response.Remove( key );
	}
}

//!
int32 UNaWidget::GetResponse(  const FString& key ) const
{
	return m_Response[key];
}

//!
int32 UNaWidget::TakeResponse(  const FString& key )
{
	int32*	res = m_Response.Find( key );

	return res ? *res : 0;
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

//! トランジション開始イベント
bool UNaWidget::OnTransition( FName id )
{
	return true;
}
