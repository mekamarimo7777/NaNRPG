// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//! イベントトリガ
UENUM()
enum class ENaEventTrigger
{
	//! 即時実行
	Instant,
	//! 自ターン開始時
	BeginTurn,
	//! 干渉時
	Interacted,
	//! 被ダメージ時
	
	ENUM_MAX	UMETA(Hidden)
};

//! イベントコード
UENUM()
enum class ENaEventCode
{
	//! メッセージ表示
	Message,
	//! 選択肢表示
	Selection,
	//! 共通ショップ処理
	Shop,

	//! フラグ操作
	Flag,
	//! 変数操作
	Variable,
	//! 汎用演算
	Calc,

	//! 無条件ジャンプ
	Jump,
	//! 条件ジャンプ
	Branch,
	//! ラベル
	Label,

	//! 時間待ち
	Wait,

	//! エンティティ生成
	SpawnEntity,
	//! インベントリ生成
	//! ワールド生成
	GenerateWorld,

	//! マップ移動
	TransLevel,

	ENUM_MAX	UMETA(Hidden)
};
