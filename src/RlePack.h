/***************************************************************************
                          RlePack.h  -  description
                             -------------------
    begin                : Mon Sep 24 2001
    copyright            : (C) 2001 by upi
    email                : upi@apocalypse.rulez.org
 ***************************************************************************/

#ifndef __RLEPACK_H
#define __RLEPACK_H

struct RlePack_P;

class RlePack
{
public:
	RlePack( const char* filename );
	~RlePack();
	
	void		offsetSprites( int offset );
	SDL_Color*	getPalette();
	void		draw( int index, int x, int y, bool flipped=false );
	
private:
	RlePack_P*	p;
};

#endif
