// Fill out your copyright notice in the Description page of Project Settings.

#include "NaNRPG.h"
#include "NaLogDefinition.h"



//! ���O�e�L�X�g����
const FNaLogMessage* UNaLogDefinition::FindMessage( FName name ) const
{
	const FNaLogMessage*	ref;

	ref	= Messages.FindByPredicate( [name]( const FNaLogMessage& p )
	{
		return p.Name == name;
	});

	return ref;
}
