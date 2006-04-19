/***************************************************************************
This file is part of MortalEditor, the OpenMortal character editor.
This is an internal tool, no warranty or support. Use at your own risk.

Copyright 2004 UPi upi@sourceforge
 ***************************************************************************/

#ifndef __RLEPACK_H
#define __RLEPACK_H

struct RlePack_P;

class RlePack
{
public:
    RlePack( const char* filename );
    ~RlePack();
	
//    void		offsetSprites( int offset );
//    SDL_Color*	getPalette();
    int		count();
    void		draw( QImage& target, int index, int x, int y );
    void		transferPalette( QImage& target );
    QSize		getSize( int index );
    
private:
    RlePack_P*	p;
};

#endif
