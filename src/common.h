/***************************************************************************
                          common.h  -  description
                             -------------------
    begin                : Fri Aug 24 2001
    copyright            : (C) 2001 by upi
    email                : upi@apocalypse.rulez.org
 ***************************************************************************/

#ifndef __COMMON_H
#define __COMMON_H



struct SDL_Surface;

#define SCREENWIDTH 640
#define SCREENHEIGHT 480
#define GAMEBITS 3
#define GAMEBITS2 (1<<GAMEBITS)

void debug( const char* format, ... );
#define ABS(A) ( (A>=0) ? (A) : -(A) )

#ifdef _SDL_video_h

extern SDL_Surface* gamescreen;


void DoMenu( bool a_bDrawBackground );
void GameOver( int a_iPlayerWon );
void DoDemos();


#define C_BLACK			240
#define C_BLUE			241
#define C_GREEN			242
#define C_CYAN			243

#define C_RED			244
#define C_MAGENTA		245
#define C_ORANGE		246
#define C_LIGHTGRAY		247

#define C_DARKGRAY		248
#define C_LIGHTBLUE		249
#define C_LIGHTGREEN	250
#define C_LIGHTCYAN		251

#define C_LIGHTRED		252
#define C_LIGHTMAGENTA	253
#define C_YELLOW		254
#define C_WHITE			255


#endif

#endif
