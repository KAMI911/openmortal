/***************************************************************************
                          State.h  -  description
                             -------------------
    begin                : Mon Aug 12 2003
    copyright            : (C) 2003 by upi
    email                : upi@apocalypse.rulez.org
 ***************************************************************************/


#ifndef STATE_H
#define STATE_H

#include "SDL_mixer.h"
#include "SDL_keysym.h"

struct SState
{
	enum TGameMode {
		IN_DEMO,
		IN_SINGLE,
		IN_MULTI,
		IN_NETWORK,
		IN_CHAT,
	} m_enGameMode;
	
	bool	m_bQuitFlag;		// true if quit event came
	const char* m_pcArgv0;		// Set by main to argv[0]
	

	// CONFIGURATION VARIABLES
	
	int		m_iGameTime;		// Time of rounds in seconds.
	int		m_iHitPoints;		// The initial number of hit points.
	int		m_iGameSpeed;		// The speed of the game (fps = 1000/GameSpeed)
	
	bool	m_bFullscreen;		// True in fullscreen mode.
	
	int		m_iChannels;		// 1: mono, 2: stereo
	int		m_iMixingRate;		// The mixing rate, in kHz
	int		m_iMixingBits;		// 1: 8bit, 2: 16bit
	int		m_iMusicVolume;		// Volume of music; 0: off, 100: max
	int		m_iSoundVolume;		// Volume of sound effects; 0: off, 100: max
	
	int		m_aiPlayerKeys[2][9];	// Player keysyms
	char	m_acLanguage[10];	// Language ID (en,hu,fr,es,..)
	int		m_iLanguageCode;	// Non-persistend language code (set by backend based on the language)
	
	char	m_acLatestServer[256];	// Last server
	bool	m_bServer;			// We were server in the last network game
	char	m_acNick[128];		// The user name on the last server.
	
	SState();
	
	void Load();
	void Save();
	void ToggleFullscreen();
	void SetLanguage( const char* a_pcLanguage );
	void SetServer( const char* a_pcServer );
};


extern SState g_oState;

#endif
