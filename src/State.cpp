/***************************************************************************
                          State.cpp  -  description
                             -------------------
    begin                : Mon Aug 12 2003
    copyright            : (C) 2003 by upi
    email                : upi@apocalypse.rulez.org
 ***************************************************************************/

#include "../config.h"
#include "SDL.h"
#include "common.h"
#include "State.h"

#include <string>
#include <fstream>

#include "Backend.h"
#include "MszPerl.h"

extern PerlInterpreter* my_perl;

SState g_oState;



std::string GetConfigHeader()
{
	std::string sHeader( "Simple config file " );
	sHeader += PACKAGE " " VERSION;
	return sHeader;
}


std::string GetConfigFilename()
{
#ifdef _WINDOWS
	if ( NULL != g_oState.m_pcArgv0 )
	{
		return std::string(g_oState.m_pcArgv0) + ".ini";
	}
	return "c:\\openmortal.ini";	
#else
	return std::string(getenv("HOME")) + "/.openmortalrc";
#endif
}



void SState::ToggleFullscreen()
{
	m_bFullscreen = !m_bFullscreen;
	
	bool bPaletted = ( gamescreen->format->BitsPerPixel <= 8 );
	SDL_Color aoPalette[256];
	int iNumColors = 0;
	
	if ( bPaletted )
	{
		iNumColors = gamescreen->format->palette->ncolors;
		if ( iNumColors > 256 ) iNumColors = 256;
		for ( int i=0; i<iNumColors; ++i )
		{
			aoPalette[i].r = gamescreen->format->palette->colors[i].r;
			aoPalette[i].g = gamescreen->format->palette->colors[i].g;
			aoPalette[i].b = gamescreen->format->palette->colors[i].b;
			aoPalette[i].unused = 0;
		}
	}
	
	gamescreen = SDL_SetVideoMode( gamescreen->w, gamescreen->h,
		gamescreen->format->BitsPerPixel, 
		m_bFullscreen ? SDL_FULLSCREEN : SDL_SWSURFACE );
	
	if ( bPaletted )
	{
		SDL_SetPalette( gamescreen, SDL_LOGPAL | SDL_PHYSPAL, aoPalette, 0, iNumColors );
	}
}



void SState::Load()
{
	std::string sFilename = GetConfigFilename();
	g_oBackend.PerlEvalF( "ParseConfig('%s');", sFilename.c_str() );
	
	SV* poSv;
	poSv = get_sv("GAMETIME", FALSE); if (poSv) m_iGameTime = SvIV( poSv );
	poSv = get_sv("HITPOINTS", FALSE); if (poSv) m_iHitPoints = SvIV( poSv );
	poSv = get_sv("GAMESPEED", FALSE); if (poSv) m_iGameSpeed = SvIV( poSv );
	poSv = get_sv("FULLSCREEN", FALSE); if (poSv) m_bFullscreen = SvIV( poSv );
	poSv = get_sv("CHANNELS", FALSE); if (poSv) m_iChannels = SvIV( poSv );
	poSv = get_sv("MIXINGRATE", FALSE); if (poSv) m_iMixingRate = SvIV( poSv );
	poSv = get_sv("MIXINGBITS", FALSE); if (poSv) m_iMixingBits = SvIV( poSv );
	poSv = get_sv("MUSICVOLUME", FALSE); if (poSv) m_iMusicVolume = SvIV( poSv );
	poSv = get_sv("SOUNDVOLUME", FALSE); if (poSv) m_iSoundVolume = SvIV( poSv );
	
	char pcBuffer[1024];
	for ( int i=0; i<2; ++i )
	{
		for ( int j=0; j<9; ++j )
		{
			sprintf( pcBuffer, "PLAYER%dKEY%d", i, j );
			poSv = get_sv(pcBuffer, FALSE); if (poSv) m_aiPlayerKeys[i][j] = SvIV( poSv );
		}
	}	
}


void SState::Save()
{
	std::string sFilename = GetConfigFilename();
	std::ofstream oStream( sFilename.c_str(), std::ios_base::out | std::ios_base::trunc );
	if ( oStream.rdstate() & std::ios::failbit )
	{
		debug( "Unable to open config file: %s\n", sFilename.c_str() );
		return;
	}
	
	oStream << GetConfigHeader() << '\n';
	oStream << "GAMETIME=" << m_iGameTime << '\n';
	oStream << "HITPOINTS=" << m_iHitPoints << '\n';
	oStream << "GAMESPEED=" << m_iGameSpeed << '\n';
	oStream << "FULLSCREEN=" << m_bFullscreen << '\n';
	oStream << "CHANNELS=" << m_iChannels << '\n';
	oStream << "MIXINGRATE=" << m_iMixingRate << '\n';
	oStream << "MIXINGBITS=" << m_iMixingBits << '\n';
	oStream << "MUSICVOLUME=" << m_iMusicVolume << '\n';
	oStream << "SOUNDVOLUME=" << m_iSoundVolume << '\n';
	
	for ( int i=0; i<2; ++i )
	{
		for ( int j=0; j<9; ++j )
		{
			oStream << "PLAYER" <<i<< "KEY" <<j<< '=' << m_aiPlayerKeys[i][j] << '\n';
		}
	}
	
	oStream.flush();
	oStream.close();
}


