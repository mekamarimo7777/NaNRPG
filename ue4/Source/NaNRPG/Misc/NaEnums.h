// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM()
namespace EBlockType
{
	enum Type
	{
		Blank,
		Air,

		ENUM_MAX	UMETA(Hidden)
	};
}

//! ブロック面
UENUM()
namespace ENaMapBlockFace
{
	enum Type
	{
		//! 前面（手前）-X
		Front,
		//! 左 -Y
		Left,
		//! 奥 +X
		Back,
		//! 右 +Y
		Right,
		//! 底面 -Z
		Bottom,
		//! 上面 +Z
		Top,

		ENUM_MAX	UMETA(Hidden)
	};
}

// エンティティクラス種別 //
UENUM()
namespace ENaEntity
{
	enum Type
	{
		//
		Character		= 0x00,
		Player,
		Item,

		//
		Block			= 0x20,

		//
		SpawnManager	= 0x40,
		Calendar,
		Weather,
		EventManager,

		ENUM_MAX	UMETA(Hidden)
	};
}

// エンティティ管理レベル //
UENUM()
namespace ENaEntityStage
{
	enum Type
	{
		//! チャンク（チャンク内実体が本体）
		Chunk,
		//! ワールド（ワールド管理のものがマスターデータ）
		World,
		//! 全体（プレイヤーDB内がマスターデータ）
		Global,
	};
}

// マップ方位共通 //
UENUM()
enum class ENaDirection : uint8
{
	West,
	NW,
	North,
	NE,
	East,
	SE,
	South,
	SW,

	ENUM_MAX	UMETA(Hidden),
	Invalid	= 0x7F
};

// アクション効果種別（アイテム、スペルなどの汎用効果。最大128バイトの汎用パラメータを持つ） //
UENUM()
namespace ENaActionEffect
{
	enum Type
	{
		//! 武器として使用できる
		Weapon,
		//！盾として使用できる
		Shield,
		//! 防具としての効果を持つ
		Armor,

		//! 貨幣価値を持つ
		Currency,

		//! 回復（p0：回復対象 p1：強度）
		RecoverHP,

		//! 基礎ステータス増減
		BaseParamBonus,

/*
		Stolen,				// 盗品

		Damage,				// 追加ダメージを与える（属性、ダイス）
		StatusAttack,		// 状態異常を与える（種類、ダイス）
		
		Recoverly,			// 回復（体力、マナ、スタミナetc）

		SummonMosnter,		// 魔物を召喚する
		Calamity,			// 禍いを呼び寄せる
		Unlucky,			// 不運を呼び寄せる（周囲に定期的にトラップを生成する）
		Drain,				// 所有者の生気を吸う
		NoTakeOff,			// 装備から外せない
		NoDrop,				// 手放すことが出来ない*/
	};
}

// アクショントリガ種別 //
UENUM()
namespace ENaActionTrigger
{
	enum Type
	{
		//! トリガなし
		None,

		//! 使う
		Use,
		//! 取る
		Take,
		//! 置く
		Drop,

		//! 食べる
		Eat,
		//! 飲む
		Quaff,
		//! 読む
		Read,
		//! 振る
		Swing,
		//! 投げる
		Throw,
		//! 押す
		Push,
		//! 引く
		Pull,

		//! 開く
		Open,
		//! 閉じる
		Close,

		//! 混ぜる
		Blend,

		ENUM_MAX	UMETA(Hidden)
	};
}

// アイテム効果要因属性 //
UENUM()
namespace ENaItemPropertyFactor
{
	enum Type
	{
		Unique,		// 固有効果
		Enchant,	// エンチャント効果
		Curse,		// 呪いによるもの
		Temporaly,	// 一時的な効果
	};
}

// 装備部位 //
UENUM()
namespace ENaItemEquipPart
{
	enum Type
	{
		//! 利き手
		MainHand,
		//! 手
		SubHand,
		//! 頭
		Head,
		//! 胴体
		Chest,
		//! 腰
		Waist,
		//! 背中
		Back,
		//! 肩
		Shoulder,
		//! 腕
		Arm,
		//！脚
		Leg,
		//! 足
		Foot,
		//! 首
		Neck,
		//! 指
		Finger,
		//! 耳
		Ear,
		//! 顔
		Face,
		//! 尻尾
		Tail,
		//！翼
		Wing,

		ENUM_MAX	UMETA(Hidden)
	};
}

// 魔法属性 //
UENUM()
namespace ENaElemental
{
	enum Type
	{
		None,			// なし

		Fire,			// 火
		Water,			// 水
		Air,			// 風
		Earth,			// 大地
		Light,			// 光
		Darkness,		// 闇

		Thunder,		// 雷
		Ice,			// 氷
		Poison,			// 毒
		Heat,			// 熱
		Acid,			// 酸
		Plant,			// 木
	};
}

// 物理属性 //
UENUM()
namespace ENaPhysics
{
	enum Type
	{
		// なし
		None,
		// 斬撃
		Slash,
		// 刺突
		Thrust,
		// 打撃
		Blow,
	};
}

// アイテム素材傾向 //
UENUM()
namespace ENaItemMaterialTrend
{
	enum Type
	{
		// 金属
		Metals,
		// 卑金属
		BaseMetals,
		// 貴金属
		PreciousMetals,
		// 魔法金属
		MagicMetals,
		// 植物素材
		Plants,
		// 生物素材
		Organic,
	};
}

// アニメーション種別 //
UENUM()
namespace ENaActorAnimation
{
	enum Type
	{
		Idle,
		Walk,
		Run,
		
		Attack,
		UseSkill,
		CastSpell,

		Damage,
		KnockDown,

		ENUM_MAX	UMETA(Hidden)
	};
}

// マップ追加プロセス種別 //
UENUM()
namespace ENaMapProcess
{
	enum Type
	{
		// ブロック埋め込み（設定ブロック） //
		FillBlocks,
		// ブロック変換（変換対象、変換ブロック） //
		ReplaceBlocks,

		// アイテム配置 //
		StrewItem,

		ENUM_MAX	UMETA(Hidden)
	};
}
