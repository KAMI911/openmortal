/***************************************************************************
                          common.cpp  -  description
                             -------------------
    begin                : Fri Aug 24 2001
    copyright            : (C) 2001 by upi
    email                : upi@apocalypse.rulez.org
 ***************************************************************************/

#include <stdio.h>
#include <stdarg.h>

#include "common.h"


SDL_Surface* gamescreen = NULL;

void debug( const char* format, ... )
{
	va_list ap;
	va_start( ap, format );
	vfprintf( stderr, format, ap );
	va_end( ap );
}
