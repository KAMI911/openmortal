/***************************************************************************
                          RlePack.h  -  description
                             -------------------
    begin                : Mon Sep 24 2001
    copyright            : (C) 2001 by upi
    email                : upi@apocalypse.rulez.org
 ***************************************************************************/

#ifndef __RLEPACK_H
#define __RLEPACK_H

#include "FighterEnum.h"

struct RlePack_P;


/** 
\class RlePack
\brief RlePack is responsible for loading and drawing "sprites" from a .DAT file.

RlePack loads the sprites from a .DAT file in its constructor. If there is
an error (e.g. file doesn't exist), the number of sprites loaded will be 0.
RlePack is usually used to store the many frames of a fighter in MSZ. It is
also used for the 'cast' in the MainScreenDemo.

The palette by default ranges from 1 to N (the number of colors). This, 
however, can be changed with OffsetSprites(). This is used to make sure that
the two loaded fighters don't overwrite each others palettes or the background
palette.

The 'tint' can be set with the SetTint method. It will not immediately appear
until ApplyPalette() is called. For an explanation about tints, please see the
TintEnum documentation.

\sa TintEnum
*/

class RlePack
{
public:
	RlePack( const char* a_pcFilename, int a_iNumColors );
	~RlePack();

	void		Clear();
	int			LoadFile( const char* a_pcFilename, int a_iNumColors );	
	int			Count();
	void		OffsetSprites( int a_iOffset );
	void		SetTint( TintEnum a_enTint );
	void		ApplyPalette();
	
	int			GetWidth( int a_iIndex );
	int			GetHeight( int a_iIndex );
	void		Draw( int a_iIndex, int a_iX, int a_iY, bool a_bFlipped=false );
	
private:
	RlePack_P*	p;
};

#endif
