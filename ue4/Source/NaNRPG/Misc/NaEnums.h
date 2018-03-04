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

//! �u���b�N��
UENUM()
namespace ENaMapBlockFace
{
	enum Type
	{
		//! �O�ʁi��O�j-X
		Front,
		//! �� -Y
		Left,
		//! �� +X
		Back,
		//! �E +Y
		Right,
		//! ��� -Z
		Bottom,
		//! ��� +Z
		Top,

		ENUM_MAX	UMETA(Hidden)
	};
}

// �G���e�B�e�B�N���X��� //
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

// �G���e�B�e�B�Ǘ����x�� //
UENUM()
namespace ENaEntityStage
{
	enum Type
	{
		//! �`�����N�i�`�����N�����̂��{�́j
		Chunk,
		//! ���[���h�i���[���h�Ǘ��̂��̂��}�X�^�[�f�[�^�j
		World,
		//! �S�́i�v���C���[DB�����}�X�^�[�f�[�^�j
		Global,
	};
}

// �}�b�v���ʋ��� //
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

// �A�N�V�������ʎ�ʁi�A�C�e���A�X�y���Ȃǂ̔ėp���ʁB�ő�128�o�C�g�̔ėp�p�����[�^�����j //
UENUM()
namespace ENaActionEffect
{
	enum Type
	{
		//! ����Ƃ��Ďg�p�ł���
		Weapon,
		//�I���Ƃ��Ďg�p�ł���
		Shield,
		//! �h��Ƃ��Ă̌��ʂ�����
		Armor,

		//! �ݕ����l������
		Currency,

		//! �񕜁ip0�F�񕜑Ώ� p1�F���x�j
		RecoverHP,

		//! ��b�X�e�[�^�X����
		BaseParamBonus,

/*
		Stolen,				// ���i

		Damage,				// �ǉ��_���[�W��^����i�����A�_�C�X�j
		StatusAttack,		// ��Ԉُ��^����i��ށA�_�C�X�j
		
		Recoverly,			// �񕜁i�̗́A�}�i�A�X�^�~�ietc�j

		SummonMosnter,		// ��������������
		Calamity,			// �Ђ����Ăъ񂹂�
		Unlucky,			// �s�^���Ăъ񂹂�i���͂ɒ���I�Ƀg���b�v�𐶐�����j
		Drain,				// ���L�҂̐��C���z��
		NoTakeOff,			// ��������O���Ȃ�
		NoDrop,				// ��������Ƃ��o���Ȃ�*/
	};
}

// �A�N�V�����g���K��� //
UENUM()
namespace ENaActionTrigger
{
	enum Type
	{
		//! �g���K�Ȃ�
		None,

		//! �g��
		Use,
		//! ���
		Take,
		//! �u��
		Drop,

		//! �H�ׂ�
		Eat,
		//! ����
		Quaff,
		//! �ǂ�
		Read,
		//! �U��
		Swing,
		//! ������
		Throw,
		//! ����
		Push,
		//! ����
		Pull,

		//! �J��
		Open,
		//! ����
		Close,

		//! ������
		Blend,

		ENUM_MAX	UMETA(Hidden)
	};
}

// �A�C�e�����ʗv������ //
UENUM()
namespace ENaItemPropertyFactor
{
	enum Type
	{
		Unique,		// �ŗL����
		Enchant,	// �G���`�����g����
		Curse,		// �􂢂ɂ�����
		Temporaly,	// �ꎞ�I�Ȍ���
	};
}

// �������� //
UENUM()
namespace ENaItemEquipPart
{
	enum Type
	{
		//! ������
		MainHand,
		//! ��
		SubHand,
		//! ��
		Head,
		//! ����
		Chest,
		//! ��
		Waist,
		//! �w��
		Back,
		//! ��
		Shoulder,
		//! �r
		Arm,
		//�I�r
		Leg,
		//! ��
		Foot,
		//! ��
		Neck,
		//! �w
		Finger,
		//! ��
		Ear,
		//! ��
		Face,
		//! �K��
		Tail,
		//�I��
		Wing,

		ENUM_MAX	UMETA(Hidden)
	};
}

// ���@���� //
UENUM()
namespace ENaElemental
{
	enum Type
	{
		None,			// �Ȃ�

		Fire,			// ��
		Water,			// ��
		Air,			// ��
		Earth,			// ��n
		Light,			// ��
		Darkness,		// ��

		Thunder,		// ��
		Ice,			// �X
		Poison,			// ��
		Heat,			// �M
		Acid,			// �_
		Plant,			// ��
	};
}

// �������� //
UENUM()
namespace ENaPhysics
{
	enum Type
	{
		// �Ȃ�
		None,
		// �a��
		Slash,
		// �h��
		Thrust,
		// �Ō�
		Blow,
	};
}

// �A�C�e���f�ތX�� //
UENUM()
namespace ENaItemMaterialTrend
{
	enum Type
	{
		// ����
		Metals,
		// �ڋ���
		BaseMetals,
		// �M����
		PreciousMetals,
		// ���@����
		MagicMetals,
		// �A���f��
		Plants,
		// �����f��
		Organic,
	};
}

// �A�j���[�V������� //
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

// �}�b�v�ǉ��v���Z�X��� //
UENUM()
namespace ENaMapProcess
{
	enum Type
	{
		// �u���b�N���ߍ��݁i�ݒ�u���b�N�j //
		FillBlocks,
		// �u���b�N�ϊ��i�ϊ��ΏہA�ϊ��u���b�N�j //
		ReplaceBlocks,

		// �A�C�e���z�u //
		StrewItem,

		ENUM_MAX	UMETA(Hidden)
	};
}
