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
	} m_enGameMode;


	bool	m_bQuitFlag;		// true if quit event came
	const char* m_pcArgv0;		// Set by main to argv[0]
	
	// VARIABLES DURING GAMEPLAY (OR REPLAY)
	
	bool	m_bGameOver;		// true if the game is over
	bool	m_bIsReplay;		// true if in replay mode
	FILE*	m_poReplayFile;		// the file to load/save replay from/to, or NULL
	
	
	// CONFIGURATION VARIABLES
	
	int		m_iGameTime;		// Time of rounds in seconds.
	int		m_iHitPoints;		// The initial number of hit points.
	
	bool	m_bFullscreen;		// True in fullscreen mode.
	
	int		m_iChannels;		// 1: mono, 2: stereo
	int		m_iMixingRate;		// The mixing rate, in kHz
	int		m_iMixingBits;		// 1: 8bit, 2: 16bit
	int		m_iMusicVolume;		// Volume of music; 0: off, 100: max
	int		m_iSoundVolume;		// Volume of sound effects; 0: off, 100: max
	
	int		m_aiPlayerKeys[2][9];	// Player keysyms
	
	SState()
	{
		m_enGameMode = IN_DEMO;
		
		m_bQuitFlag = false;
		m_pcArgv0 = NULL;
		
		m_bGameOver = true;
		m_bIsReplay = false;
		m_poReplayFile = NULL;
		
		m_iGameTime = 60;
		m_iHitPoints = 100;
		
		#ifdef _WINDOWS
			#ifdef _DEBUG
				m_bFullScreen = false;
			#else
				m_bFullscreen = true;
			#endif
		#else
			m_bFullscreen = false;
		#endif
		
		m_iChannels = 2;
		m_iMixingRate = MIX_DEFAULT_FREQUENCY;
		m_iMixingBits = 2;
		m_iMusicVolume = 50;
		m_iSoundVolume = 100;
		
		static const int aiDefaultKeys[2][9] = {
      		{ SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_PAGEDOWN,
                SDLK_DELETE, SDLK_INSERT, SDLK_END, SDLK_HOME },
      		{ SDLK_w, SDLK_s, SDLK_a, SDLK_d, SDLK_x,
                SDLK_f, SDLK_r, SDLK_g, SDLK_t }
		};
		
		for ( int i=0; i<2; ++i )
			for ( int j=0; j<9; ++j )
				m_aiPlayerKeys[i][j] = aiDefaultKeys[i][j];

	};
	
	void Load();
	void Save();
	void ToggleFullscreen();
};


extern SState g_oState;

#endif
