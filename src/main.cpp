/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Wed Aug 22 10:18:47 CEST 2001
    copyright            : (C) 2001 by upi
    email                : upi@apocalypse.rulez.org
 ***************************************************************************/


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "PlayerSelect.h"

#include "SDL_video.h"
#include "sge_tt_text.h"
#include "sge_bm_text.h"
#include "sge_surface.h"
#include "SDL.h"
#include "SDL_image.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <string>

#include "common.h"
#include "gfx.h"
#include "Audio.h"
#include "RlePack.h"
#include "Backend.h"
#include "State.h"
#include "FighterStats.h"
#include "MortalNetwork.h"


#ifdef _WINDOWS

#undef DATADIR                // GRRR.. windows keyword...
#include <windows.h>
#define DATADIR "../data"

#endif

_sge_TTFont* inkFont;
_sge_TTFont* impactFont;
_sge_TTFont* titleFont;
_sge_TTFont* chatFont;
sge_bmpFont* fastFont;
sge_bmpFont* creditsFont;
sge_bmpFont* storyFont;
bool bDebug = false;

SDL_Color Colors[] =
{
   { 0,  0,  0,  0 },   { 0,  0,  42, 0 },   { 0,  42, 0,  0 },   { 0,  42, 42, 0 },
   { 42, 0,  0,  0 },   { 42, 0,  42, 0 },   { 63, 42, 0,  0 },   { 42, 42, 42, 0 },
   { 21, 21, 21, 0 },   { 21, 21, 63, 0 },   { 21, 63, 21, 0 },   { 21, 63, 63, 0 },
   { 63, 21, 21, 0 },   { 63, 21, 63, 0 },   { 63, 63, 21, 0 },   { 63, 63, 63, 0 }
};


void Complain( const char* a_pcError )
{
#ifdef _WINDOWS
	::MessageBoxA( 0, a_pcError, "OpenMortal", MB_ICONEXCLAMATION );
#else
	fprintf( stderr, "%s", a_pcError );
#endif
}


_sge_TTFont* LoadTTF( const char* a_pcFilename, int a_iSize )
{
	std::string sPath = std::string(DATADIR) + "/fonts/" + a_pcFilename;
	_sge_TTFont* poFont = sge_TTF_OpenFont( sPath.c_str(), a_iSize );
	
	if ( NULL == poFont )
	{
		Complain( ("Couldn't load font: " + sPath).c_str() );
	}
	
	return poFont;
}


sge_bmpFont* LoadBMPFont( const char* a_pcFilename )
{
	std::string sPath = std::string(DATADIR) + "/fonts/" + a_pcFilename;
	sge_bmpFont* poFont = sge_BF_OpenFont( sPath.c_str(), SGE_BFSFONT | SGE_BFTRANSP );
	if ( NULL == poFont )
	{
		Complain( ("Couldn't load font: " + sPath).c_str() );
	}
	return poFont;
}


int init( int iFlags )
{
	if (SDL_Init(SDL_INIT_VIDEO /*| SDL_INIT_AUDIO*/) < 0)
	{
		fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
		return -1;
	}
	atexit(SDL_Quit);
	
	int d = SDL_VideoModeOK(640, 480, 8, iFlags);
	if (d == 0)
	{
		fprintf(stderr, "requested video mode not available\n");
//		return -1;
	}
	
	gamescreen = SDL_SetVideoMode(640, 480, 8, iFlags);
	if (gamescreen == NULL)
	{
		fprintf(stderr, "failed to set video mode: %s\n", SDL_GetError());
		return -1;
	}
	
	SDL_WM_SetCaption( "OpenMortal", "OpenMortal" );
	std::string sPath = std::string(DATADIR) + "/gfx/icon.png";
	SDL_WM_SetIcon(IMG_Load(sPath.c_str()), NULL);
	SDL_ShowCursor( SDL_DISABLE );

	for ( int i=0; i<16; ++i ) { Colors[i].r *=4; Colors[i].g *=4; Colors[i].b *=4; }
	SDL_SetColors( gamescreen, Colors, 256-16, 16 );

	if ( sge_TTF_Init() )
	{
		fprintf(stderr, "couldn't start ttf engine: %s\n", SDL_GetError());
		return -1;
	}

	sge_TTF_AAOff();
	
	inkFont = LoadTTF( "aardvark.ttf", 20 );
	if ( !inkFont ) return -1;
	impactFont = LoadTTF( "bradybun.ttf", 20 );	// gooddogc.ttf, 20
	if ( !impactFont ) return -1;
	titleFont = LoadTTF( "deadgrit.ttf", 48 );		// deadgrit.ttf, 48
	if ( !titleFont ) return -1;
	chatFont = LoadTTF( "thin.ttf", 20 );		// deadgrit.ttf, 48
	if ( !chatFont ) return -1;

	fastFont = LoadBMPFont( "brandybun3.png" );
	if ( !fastFont ) return -1;
	creditsFont = LoadBMPFont( "CreditsFont2.png" );//"fangfont.png" );
	if ( !creditsFont ) return -1;
	storyFont = LoadBMPFont( "glossyfont.png" );
	if ( !storyFont ) return -1;
	
    return 0;
}


int init2()
{
	if ( !g_oBackend.Construct() )
	{

		fprintf(stderr, "couldn't start backend.\n" );
		return -1;
	}
	return 0;
}





int DrawMainScreen()
{
	SDL_Surface* background = LoadBackground( "Mortal.png", 240 );
	
	DrawTextMSZ( "Version " VERSION "  © 2003-2004 by UPi", inkFont, 320, 430, UseShadow | AlignHCenter, C_WHITE, background, false );
	SDL_Rect r;
	r.x = r.y = 0;
	
	std::string sStaffFilename = DATADIR;
	sStaffFilename += "/characters/STAFF.DAT";
	RlePack pack( sStaffFilename.c_str(), 240 );
	pack.ApplyPalette();
	//SDL_SetColors( gamescreen, pack.getPalette(), 0, 240 );
	SDL_BlitSurface( background, NULL, gamescreen, &r );
	SDL_Flip( gamescreen );

	char* filename[15] = {
		"Jacint.pl", "Jozsi.pl", "Agent.pl", "Mrsmith.pl",
		"Sleepy.pl", "Tejszin.pl",
		"UPi.pl", "Zoli.pl", "Ulmar.pl", "Bence.pl",
		"Descant.pl", "Grizli.pl", "Sirpi.pl", "Macy.pl", "Cumi.pl" };
	int x[14] = {
		0, 26, 67, 125, 159, 209,
		249, 289, 358, 397, 451, 489, 532, 161 };
	int y[14] = {
		5, 4, 5, 5, 5, 7, 
		4, 0, 7, 5, 5, 6, 5, 243 };
		
	int i;

	g_oBackend.PerlEvalF( "eval( \"require '%s/characters/Kinga.pl';\" )", DATADIR );
	g_oBackend.PerlEvalF( "eval( \"require '%s/characters/Ambrus.pl';\" )", DATADIR );
	g_oBackend.PerlEvalF( "eval( \"require '%s/characters/Dani.pl';\" )", DATADIR );

	for ( i=0; i<15; ++i )
	{
		pack.Draw( i, x[i], y[i], false );
		SDL_Flip( gamescreen );
		if ( filename[i] != NULL )
		{
			debug( "Loading fighter %s", filename[i] );
			g_oBackend.PerlEvalF( "eval( \"require '%s/characters/%s';\" )", DATADIR, filename[i] );
		}
	}
	
	int retval = 0;
	i = 0;
	
    SDL_FreeSurface( background );
	return retval;
	
}





int InitJoystick();




/**
The game loop consists of the following events:

\li Player selection
\li DoGame
\li GameOver and FighterStatsDemo (not in network mode)

The loop ends if the game mode changes to a non-game mode (e.g. IN_DEMO or IN_CHAT)
*/

void GameLoop()
{
#define IS_GAME_MODE (g_oState.m_enGameMode != SState::IN_DEMO \
	&& g_oState.m_enGameMode != SState::IN_CHAT \
	&& !g_oState.m_bQuitFlag)
	
	Audio->PlaySample( "car_start.voc" );
	Audio->PlayMusic( "GameMusic" );

	bool bNetworkGame = SState::IN_NETWORK == g_oState.m_enGameMode;

	while ( IS_GAME_MODE )
	{
		g_oPlayerSelect.DoPlayerSelect();
		if ( !IS_GAME_MODE ) break;

		//sprintf( acReplayFile, "/tmp/msz%d.replay", ++iGameNumber );

		int iGameResult = DoGame( NULL, false, bDebug );
		//int iGameResult = DoGame( acReplayFile, false, bDebug );
		//DoGame( acReplayFile, true, bDebug );
		debug ( "iGameResult = %d\n", iGameResult );

		if ( !IS_GAME_MODE ) break;
		
		if ( iGameResult >= 0 && !bNetworkGame )
		{
			GameOver( iGameResult );
			FighterStatsDemo oDemo( g_oPlayerSelect.GetPlayerInfo( iGameResult ).m_enFighter );
			oDemo.Run();
		}
		
		if ( !IS_GAME_MODE ) break;
	}

	if ( bNetworkGame && !g_oState.m_bQuitFlag )
	{
		DrawTextMSZ( "Connection closed.", inkFont, 320, 210, AlignHCenter | UseShadow, C_WHITE, gamescreen );
		DrawTextMSZ( g_poNetwork->GetLastError(), impactFont, 320, 250, AlignHCenter | UseShadow, C_WHITE, gamescreen );
		SDL_Delay( 1000 );
		GetKey( true );
	}

	if ( !g_oState.m_bQuitFlag )
	{
		Audio->PlayMusic( "DemoMusic" );
	}

}



/**
The chat loop consists of:

\li DoOnlineChat
\li GameLoop (if a game was started

The loop ends if DoOnlineChat returns with a quit or disconnect
(not IN_NETWORK mode).
*/

void ChatLoop()
{
	while (1)
	{
		DoOnlineChat();
		
		if ( g_oState.m_bQuitFlag ) break;
		
		if ( SState::IN_NETWORK == g_oState.m_enGameMode )
		{
			GameLoop();
		}
		
		if ( g_oState.m_bQuitFlag ) break;

		g_oState.m_enGameMode = SState::IN_CHAT;
	}
}




int main(int argc, char *argv[])
{
	srand( (unsigned int)time(NULL) );
	if ( 0 != init2() )
	{
		fprintf( stderr, "init2() failed." );
		return -1;
	}
	
	g_oState.m_pcArgv0 = argv[0];
	g_oState.Load();
	CMortalNetwork::Create();
	
	bDebug = false;

	int iFlags = SDL_SWSURFACE | SDL_HWPALETTE;


	if ( g_oState.m_bFullscreen )
	{
		iFlags |= SDL_FULLSCREEN;
	}
	
	int i;
	for ( i=1; i<argc; ++i )
	{
		if ( !strcmp(argv[i], "-debug") )
		{
			bDebug = true;
		}
		else if ( !strcmp(argv[i], "-fullscreen") )
		{
			iFlags |= SDL_FULLSCREEN;
		}
		else if ( !strcmp(argv[i], "-hwsurface") )
		{
			iFlags |= SDL_HWSURFACE;
		}
		else if ( !strcmp(argv[i], "-doublebuf") )
		{
			iFlags |= SDL_DOUBLEBUF;
		}
		else if ( !strcmp(argv[i], "-anyformat") )
		{
			iFlags |= SDL_ANYFORMAT;
		}
		else
		{
			printf( "Usage: %s [-debug] [-fullscreen] [-hwsurface] [-doublebuf] [-anyformat]\n", argv[0] );
			return 0;
		}
	}

	if (init( iFlags )<0)
	{
		return -1;
	}

	InitJoystick();
	
	g_oState.SetLanguage( g_oState.m_acLanguage );
	
	new MszAudio;
//	Audio->LoadMusic( "Last_Ninja_-_The_Wilderness.mid", "DemoMusic" );
	Audio->LoadMusic( "ride.mod", "DemoMusic" );
	Audio->PlayMusic( "DemoMusic" );
	Audio->LoadMusic( "2nd_pm.s3m", "GameMusic" );
	
	DrawMainScreen();
	

	g_oPlayerSelect.SetPlayer( 0, ZOLI );
	g_oPlayerSelect.SetPlayer( 1, SIRPI );

	/*
	int nextFighter = 0;
	int describeOrder[ (int)LASTFIGHTER ];
	
	for ( i=0; i<(int)LASTFIGHTER; ++i ) describeOrder[i] = i;
	for ( i=0; i<100; ++i )
	{
		int j = rand() % (int)LASTFIGHTER;
		int k = rand() % (int)LASTFIGHTER;
		int l;
		l = describeOrder[j];
		describeOrder[j] = describeOrder[k];
		describeOrder[k] = l;
	}
	*/

	/*
	{
		int iGameNumber=0;
		char acReplayFile[1024];
		
		for ( i=0; i<15; ++i )
		{
			sprintf( acReplayFile, DATADIR "/msz%i.replay", i );
			DrawTextMSZ( acReplayFile, impactFont, 10, 10, 0, C_WHITE, gamescreen );
			SDL_Delay(5000 );
			
			DoGame( acReplayFile, true, bDebug );
		}
	}

	*/
	bool bNetworkGame = false;
	
	while ( 1 )
	{
		if ( g_oState.m_bQuitFlag ) break;
		
		switch ( g_oState.m_enGameMode )
		{
		case SState::IN_DEMO:
			DoDemos();
			continue;
			
		case SState::IN_CHAT:
			ChatLoop();
			continue;
			
		default:
			GameLoop();
			continue;
		}

#if 0
		// Remaining are game modes: IN_SINGLE, IN_MULTI, IN_NETWORK
		
		Audio->PlaySample( "car_start.voc" );
		Audio->PlayMusic( "GameMusic" );

		bNetworkGame = false;

		while ( g_oState.m_enGameMode != SState::IN_DEMO
			&& g_oState.m_enGameMode != SState::IN_CHAT
			&& !g_oState.m_bQuitFlag )
		{
			bNetworkGame = SState::IN_NETWORK == g_oState.m_enGameMode;
			
			g_oPlayerSelect.DoPlayerSelect();
			if ( g_oState.m_bQuitFlag || g_oState.m_enGameMode == SState::IN_DEMO  ) break;
			
			//sprintf( acReplayFile, "/tmp/msz%d.replay", ++iGameNumber );
			
			int iGameResult = DoGame( NULL, false, bDebug );
			//int iGameResult = DoGame( acReplayFile, false, bDebug );
			//DoGame( acReplayFile, true, bDebug );
			if ( g_oState.m_bQuitFlag || g_oState.m_enGameMode == SState::IN_DEMO  ) break;
			
			debug ( "iGameResult = %d\n", iGameResult );
			
			if ( iGameResult >= 0 && !bNetworkGame )
			{
				GameOver( iGameResult );


				FighterStatsDemo oDemo( g_oPlayerSelect.GetPlayerInfo( iGameResult ).m_enFighter );
				oDemo.Run();		
			}
			if ( g_oState.m_bQuitFlag || g_oState.m_enGameMode == SState::IN_DEMO  ) break;
		}
		
		if ( bNetworkGame && !g_oState.m_bQuitFlag )
		{
			DrawTextMSZ( "Connection closed.", inkFont, 320, 210, AlignHCenter | UseShadow, C_WHITE, gamescreen );
			DrawTextMSZ( g_poNetwork->GetLastError(), impactFont, 320, 250, AlignHCenter | UseShadow, C_WHITE, gamescreen );
			SDL_Delay( 1000 );
			GetKey( true );
		}
		
		if ( g_oState.m_bQuitFlag ) break;
		Audio->PlayMusic( "DemoMusic" );
#endif
	}
	
	g_oState.Save();
	
	SDL_Quit();
	
	return EXIT_SUCCESS;
}
