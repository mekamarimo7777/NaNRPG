// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UMG.h"

#include "Utility/Components/NaStateMachine.h"

#include "NaWidget.generated.h"

//! 汎用リザルト
UENUM()
namespace EUIResult
{
	enum Type
	{
		//! 処理中
		None		= 0,
		//! 決定
		Decided		= 1,
		//! キャンセル
		Canceled	= 2
	};
}

UCLASS(abstract)
class NANRPG_API UNaWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//! ウィジェット初期化
	virtual bool	Initialize() override;
	//! 
	virtual void	NativeConstruct() override;
	//!
	virtual void	NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//! ウィジェットオープン（ビューポートへの追加）
	virtual void	Open( int32 ZOrder = 0 );
	//! ウィジェットクローズ（ビューポートからの削除）
	virtual void	Close();

	//! 状態変更
	UFUNCTION(BlueprintCallable, Category = "Transition")
	void	Transition( FName id );
	//! トランジション判定
	bool	IsTransition() const;

	//! 
	bool	HasNestFocus() const;

	//! フォーカス退避
	void	StorePrevFocus();
	//! フォーカス復元
	void	RestorePrevFocus();

	//! 入力モード切り替え
	void	SetInputModeUI();
	void	SetInputModeGameAndUI();

	//! パラメータ設定
	UFUNCTION(BlueprintCallable, Category = "Parameters")
	void		SetParameter( FName key, const FString& value );
	//! パラメータ取得
	UFUNCTION(BlueprintCallable, Category = "Parameters")
	FString		GetParameter( FName key ) const;
	UFUNCTION(BlueprintCallable, Category = "Parameters")
	int32		GetParameterAsInt( FName key ) const		{ return FCString::Atoi( *GetParameter( key ) ); }
	UFUNCTION(BlueprintCallable, Category = "Parameters")
	float		GetParameterAsFloat( FName key ) const		{ return FCString::Atof( *GetParameter( key ) ); }
	UFUNCTION(BlueprintCallable, Category = "Parameters")
	FVector		GetParameterAsVector( FName key ) const		{ FVector v; v.InitFromString( GetParameter( key ) ); return v; }
	UFUNCTION(BlueprintCallable, Category = "Parameters")
	UTexture*	GetParameterAsTexture( FName key ) const;

protected:
	//! トランジション開始イベント
	UFUNCTION(BlueprintImplementableEvent, Category = "Transition")
	void	OnBeginTransition( FName id );



	//!@obsolete
	UFUNCTION(BlueprintImplementableEvent, Category = "Widget")
	void	EventOpen();
	//!@obsolete
	UFUNCTION(BlueprintImplementableEvent, Category = "Widget")
	void	EventClose();

	//!@obsolete
	UFUNCTION(BlueprintCallable, Category = "Response")
	void	SetResponse( const FString& key, int32 value );
	//!@obsolete
	int32	GetResponse(  const FString& key ) const;
	//!@obsolete
	int32	TakeResponse(  const FString& key );

protected:
	//! ステートマシン
	UPROPERTY(Transient)
	UNaStateMachine*		m_StateMachine;

	//! 汎用パラメータ
	UPROPERTY()
	TMap<FName, FString>	m_Parameters;

	//! 
	UPROPERTY(Transient)
	FName	m_CurrentTrans;

	//! @obsolete BP側レスポンス
	UPROPERTY()
	TMap<FString, int32>	m_Response;

	//! 直前のフォーカススレート
	TSharedPtr<SWidget>		m_PrevFocus;
};
