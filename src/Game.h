/***************************************************************************
                          Game.h  -  description
                             -------------------
    begin                : Mon Aug 27 2003
    copyright            : (C) 2003 by upi
    email                : upi@apocalypse.rulez.org
 ***************************************************************************/


#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>

struct SDL_Surface;
class Background;


class Game
{
public:
	Game( bool a_bIsReplay, bool a_bDebug );
	~Game();
	int Run();
	std::string& GetReplay();
	void DoReplay( const char* a_pcReplayFile );
	
protected:
	void Draw();
	void DrawHitPointDisplay();
	void DrawBackground();
	void DrawDoodads();
	void DrawPoly( const char* a_pcName, int a_iColor );
	
	void DoOneRound();
	
	void Advance( int a_iNumFrames );
	int ProcessEvents();
	void HurryUp();
	void TimeUp();
	void InstantReplay( int a_iKoAt );

	bool IsNetworkGame();
	
protected:
	
	static int			mg_iBackgroundNumber;

	bool				m_bIsReplay;
	bool				m_bDebug;
	Background*			m_poBackground;
	SDL_Surface*		m_poDoodads;
	
	int					m_aiRoundsWonByPlayer[2];
	int					m_iNumberOfRounds;
	int					m_iFrame;
	
	std::string			m_sReplayString;
	std::vector<int>	m_aReplayOffsets;
	
	enum					// This enum assumes its values during DoOneRound
	{
		Ph_START,			// "Round X" displayed, fighters getting ready
		Ph_NORMAL,			// During the fight
		Ph_TIMEUP,			// Time is up, no KO, no replay.
		Ph_KO,				// There is a KO, forward until the guy is down
		Ph_REWIND,			// There was a KO, rewinding until before the KO
		Ph_SLOWFORWARD,		// Playing back the KO
		
		Ph_REPLAY,			// Replay mode
	}					m_enGamePhase;
};

#endif
