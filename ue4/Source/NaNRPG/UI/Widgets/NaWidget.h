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

//! トランジションタスク種別
UENUM()
enum class ENaTransitionTask
{
	//! 子ウィジェットトランジション
	Transition	= 0,
	//! アニメーション
	Animation	= 1,
};
//! トランジションタスク状態
UENUM()
enum class ENaTransitionTaskState
{
	//! 開始待ち
	Ready		= 0,
	//! 実行中
	Processing	= 1,
	//! 完了
	Completed	= 2,
};

//! トランジションタスク
USTRUCT()
struct FNaTransitionTask
{
	GENERATED_BODY()

	//! 種別
	UPROPERTY()
	ENaTransitionTask		Type;
	//! 状態
	UPROPERTY()
	ENaTransitionTaskState	State;

	//! 対象オブジェクト
	UPROPERTY()
	UObject*		Object;
	//! 
	UPROPERTY()
	TArray<FString>	Params;
};

UCLASS(abstract)
class NANRPG_API UNaWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	
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

	//! トランジションリクエスト
	UFUNCTION(BlueprintCallable, Category = "Transition")
	void	Transition( FName id );
	//! トランジション中判定
	bool	IsTransition() const;

	//! フォーカス退避
	void	StorePrevFocus();
	//! フォーカス復元
	void	RestorePrevFocus();
	//! フォーカス判定
	bool	HasNestFocus() const;

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
	//! トランジションイベント（内部通知）
	virtual bool	OnTransition( FName id );
	//! トランジション開始
	void			BeginTransition();
	//! トランジション完了
	void			EndTransition();
	//! トランジション開始イベント（BP）
	UFUNCTION(BlueprintImplementableEvent, Category = "Transition")
	bool			OnBeginTransition( FName id );
	//! トランジション終了イベント（BP）
	UFUNCTION(BlueprintImplementableEvent, Category = "Transition")
	bool			OnEndTransition( FName id );

	//! 
	UFUNCTION(BlueprintCallable, Category = "Transition")
	void	RequestTransition( UNaWidget* widget, FString value );
	//! 
	UFUNCTION(BlueprintCallable, Category = "Transition")
	void	RequestAnimation( UWidgetAnimation* anim );
	//!
	void	ExecTransTask();
	//! 
	bool	HasTransTask() const	{ return m_TransTask.Num() > 0; }

protected:
	//! ステートマシン
	UPROPERTY(Transient)
	UNaStateMachine*		m_SM;

	//! 汎用パラメータ
	UPROPERTY(Transient)
	TMap<FName, FString>	m_Parameters;

	//! 現在表示
	UPROPERTY(Transient)
	FName	m_CurrentTrans;
	//! トランジションリクエスト
	UPROPERTY(Transient)
	FName	m_RequestTrans;
	//! 
	UPROPERTY(Transient)
	bool	m_IsTransition;
	//! トランジションタスク
	UPROPERTY(Transient)
	TArray<FNaTransitionTask>	m_TransTask;

	//! 直前のフォーカススレート
	TSharedPtr<SWidget>		m_PrevFocus;
};
