/***************************************************************************
                          menu.cpp  -  description
                             -------------------
    begin                : Sun Aug 3 2003
    copyright            : (C) 2003 by upi
    email                : upi@apocalypse.rulez.org
 ***************************************************************************/

#include "SDL.h"
#include "SDL_video.h"

#include "menu.h"
#include "gfx.h"
#include "State.h"
#include "common.h"
#include "Audio.h"
#include "Backend.h"


enum 
{
/* Master menu structure:
MAIN MENU
*/
MENU_UNKNOWN,
	MENU_SURRENDER,
	MENU_SINGLE_PLAYER,
		MENU_EASY,
		MENU_MEDIUM,
		MENU_HARD,
	MENU_MULTI_PLAYER,
	MENU_OPTIONS,
		MENU_GAME_TIME,			//	( :30 - 5:00 )
		MENU_TOTAL_HIT_POINTS,	// ( 25 - 1000 )
		MENU_SOUND,
			MENU_CHANNELS,		// MONO / STEREO
			MENU_MIXING_RATE,	// 11kHz / 22kHz / 44.1 kHz
			MENU_BITS,			// 8 bit / 16 bit
			MENU_MUSIC_VOLUME,	// (0% - 100%)
			MENU_SOUND_VOLUME,	// (0% - 100%)
			MENU_SOUND_OK,
		MENU_FULLSCREEN,
		MENU_KEYS_RIGHT,
		MENU_KEYS_LEFT,
		MENU_OPTIONS_OK,
	MENU_INFO,
	MENU_QUIT,					// (confirm)
};


const char* g_ppcGameTime[] = { "0:30", "0:45", "1:00", "1:15", "1:30", "1:45", "2:00", "3:00", "5:00", NULL };
const int g_piGameTime[] = { 30, 45, 60, 75, 90, 105, 120, 180, 300 };
const char* g_ppcHitPoints[] = { "BABY", "VERY LOW", "LOW", "NORMAL", "HIGH", "VERY HIGH", "NEAR IMMORTAL", NULL };
const int g_piHitPoints[] = { 1, 10, 50, 100, 150, 200, 500 };
const char* g_ppcChannels[] = { "MONO", "STEREO", NULL };
const int g_piChannels[] = { 1, 2 };
const char* g_ppcMixingRate[] = { "LOW", "MEDIUM", "HIGH", NULL };
const int g_piMixingRate[] = { 1, 2, 3 };
const char* g_ppcMixingBits[] = { "8 bit", "16 bit", NULL };
const int g_piMixingBits[] = { 1, 2 };
const char* g_ppcVolume[] = { "OFF", "10%", "20%", "30%", "40%", "50%", "60%", "70%", "80%", "90%", "100%", NULL };
const int g_piVolume[] = { 0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };



SDL_Surface* poBackground = NULL;



void InputKeys( int a_iPlayerNumber )
{
	SDL_BlitSurface( poBackground, NULL, gamescreen, NULL );
	DrawGradientText( "Input keys", titleFont, 20, gamescreen );
	SDL_Flip( gamescreen );
	
	static const char* apcKeyNames[9] = { "up", "down", "left", "right", "block", 
		"low punch", "high punch", "low kick", "high kick" };
	char acBuffer[1024];
	int iY = 70;
	
	for ( int i=0; i<9; ++i )
	{
		sprintf( acBuffer, "%s player-'%s'?", a_iPlayerNumber ? "Left" : "Right", apcKeyNames[i] );
		DrawTextMSZ( acBuffer, inkFont, 10, iY, UseShadow, C_WHITE, gamescreen );
		
		SDLKey enKey = GetKey();
		
		if ( SDLK_ESCAPE == enKey )
		{
			SDL_BlitSurface( poBackground, NULL, gamescreen, NULL );
			SDL_Flip( gamescreen );
			return;
		}
		g_oBackend.PerlEvalF( "GetKeysym(%d);", enKey );
		DrawTextMSZ( g_oBackend.GetPerlString("keysym"), inkFont, 530, iY, UseShadow, C_WHITE, gamescreen );
		g_oState.m_aiPlayerKeys[a_iPlayerNumber][i] = enKey;
		iY += 35;
	}
	
	DrawTextMSZ( "Thanks!", inkFont, 320, iY + 20, UseShadow | AlignCenter, C_WHITE, gamescreen );
	GetKey();
	SDL_BlitSurface( poBackground, NULL, gamescreen, NULL );
	SDL_Flip( gamescreen );
}



/***************************************************************************
							MENUITEM DEFINITION
 ***************************************************************************/
 
 

MenuItem::MenuItem( Menu* a_poMenu, const char* a_pcUtf8Text, int a_iCode )
: m_sUtf8Text( a_pcUtf8Text )
{
	m_poMenu = a_poMenu;
	m_iCode = a_iCode;

	m_oPosition.x = m_oPosition.y = 100;
	m_oPosition.w = m_oPosition.h = 100;
	m_bCenter = true;
	m_iHighColor = C_WHITE;
	m_iLowColor = C_LIGHTGRAY;
	m_iInactiveColor = C_DARKGRAY;
	m_iBackgroundColor = C_BLACK;
	m_bActive = false;
	m_bEnabled = true;
}

	
MenuItem::~MenuItem()
{
}

void MenuItem::Draw()
{
	if ( NULL != poBackground )
	{
		SDL_BlitSurface( poBackground, &m_oPosition, gamescreen, &m_oPosition );
	}
	else
	{
		SDL_FillRect( gamescreen, &m_oPosition, 0 );
	}
	
	int iX = m_oPosition.x;
	int iY = m_oPosition.y;

	if ( m_bCenter )
	{
		iX += m_oPosition.w / 2;
	}
	
	DrawTextMSZ( m_sUtf8Text.c_str(), inkFont, iX, iY,
		UseTilde | UseShadow | (m_bCenter ? AlignHCenter : 0),
		m_bEnabled ? (m_bActive ? m_iHighColor : m_iLowColor) : m_iInactiveColor,
		gamescreen );
	
	SDL_UpdateRect( gamescreen, m_oPosition.x, m_oPosition.y, m_oPosition.w, m_oPosition.h );	
}


void MenuItem::Clear()
{
//	debug( "Clear: %d:%d %dx%d\n", m_oPosition.x, m_oPosition.y, m_oPosition.w, m_oPosition.h );
	if (poBackground )
	{
		SDL_Rect oDest = m_oPosition;
		SDL_BlitSurface( poBackground, &m_oPosition, gamescreen, &oDest );
	}
	else
	{
		SDL_FillRect( gamescreen, &m_oPosition, C_WHITE );
	}

	SDL_UpdateRect( gamescreen, m_oPosition.x, m_oPosition.y, m_oPosition.w, m_oPosition.h );	
}


void MenuItem::Activate()
{
	if ( m_poMenu )
	{
		m_poMenu->ItemActivated( m_iCode, this );
	}
}
	
	
void MenuItem::SetText( const char* a_pcUtf8Text, bool a_bCenter )
{
	m_sUtf8Text = a_pcUtf8Text;
	m_bCenter = a_bCenter;
	Draw();
}

void MenuItem::SetPosition( const SDL_Rect& a_roPosition )
{
	m_oPosition = a_roPosition;
}

void MenuItem::SetActive( bool a_bActive )
{
	if ( m_bActive == a_bActive )
	{
		return;
	}
	m_bActive = a_bActive;
	Draw();
}
	
void MenuItem::SetEnabled( bool a_bEnabled )
{
	if ( m_bEnabled == a_bEnabled )
	{
		return;
	}
	m_bEnabled = a_bEnabled;
	Draw();
}




/***************************************************************************
							ENUMMENUITEM DEFINITION
 ***************************************************************************/
 
 

EnumMenuItem::EnumMenuItem( Menu* a_poMenu, int a_iInitialValue, const char* a_pcUtf8Text, int a_iCode )
: MenuItem( a_poMenu, a_pcUtf8Text, a_iCode )
{
	m_sUtf8Title = a_pcUtf8Text;
	m_iMax = -1;
	m_iValue = a_iInitialValue;
}

EnumMenuItem::~EnumMenuItem()
{
}
	

void EnumMenuItem::Draw()
{
	m_sUtf8Text = m_sUtf8Title;
	if ( m_iValue <= m_iMax )
	{
		m_sUtf8Text += m_ppcNames[m_iValue];
	}
	if ( m_iValue > 0 )
	{
		m_sUtf8Text = "< " + m_sUtf8Text;
	}
	if ( m_iValue < m_iMax )
	{
		m_sUtf8Text += " >";
	}
	
	MenuItem::Draw();
}


void EnumMenuItem::Increment()
{
	if ( m_iValue < m_iMax )
	{
		++m_iValue;
		Draw();
		m_poMenu->ItemChanged( m_iCode, m_piValues[m_iValue], this );
		Audio->PlaySample( "ding.voc" );
	}
}


void EnumMenuItem::Decrement()
{
	if ( m_iValue > 0 )
	{
		--m_iValue;
		Draw();
		m_poMenu->ItemChanged( m_iCode, m_piValues[m_iValue], this );
		Audio->PlaySample( "ding.voc" );
	}
}

	
void EnumMenuItem::SetEnumValues( const char ** a_ppcNames, const int * a_piValues )
{
	m_ppcNames = a_ppcNames;
	m_piValues = a_piValues;
	
	int i;
	bool bFoundValue = false;
	
	for ( i=0; NULL != a_ppcNames[i]; ++i )
	{
		if ( !bFoundValue &&
			m_iValue == a_piValues[i] )
		{
			bFoundValue = true;
			m_iValue = i;
		}
	}
	
	if ( !bFoundValue )
	{
		m_iValue = 0;
	}
	
	m_iMax = i-1;
}






/***************************************************************************
							MENU DEFINITION
 ***************************************************************************/
 

Menu::Menu( const char* a_pcTitle )
: m_sTitle( a_pcTitle )
{
	m_iCurrentItem = 0;
	m_iReturnCode = -1;
	m_bDone = false;
}


Menu::~Menu()
{
	ItemIterator it;
	for ( it = m_oItems.begin(); it != m_oItems.end(); ++it )
	{
		delete *it;
	}
}


MenuItem* Menu::AddMenuItem( const char* a_pcUtf8Text, SDLKey a_tShortcut, int a_iCode )
{
	MenuItem* poItem = new MenuItem( this, a_pcUtf8Text, a_iCode );
	return AddMenuItem( poItem );
}



EnumMenuItem* Menu::AddEnumMenuItem( const char* a_pcUtf8Text, int a_iInitialValue, 
	const char** a_ppcNames, const int* a_piValues, int a_iCode )
{
	EnumMenuItem* poItem = new EnumMenuItem( this, a_iInitialValue, a_pcUtf8Text, a_iCode );
	poItem->SetEnumValues( a_ppcNames, a_piValues );
	
	AddMenuItem( poItem );
	return poItem;
}



MenuItem* Menu::AddMenuItem( MenuItem* a_poItem )
{
	m_oItems.push_back( a_poItem );
	
	SDL_Rect oRect;
	oRect.x = 0; oRect.w = gamescreen->w;
	oRect.y = m_oItems.size() * 45 + 100;
	oRect.h = 45;

	a_poItem->SetPosition( oRect );
	
	return a_poItem;
}


void Menu::AddOkCancel( int a_iOkCode )
{
	SDL_Rect oRect;
	oRect.x = 0; oRect.w = 150;
	oRect.y = gamescreen->h - 50; oRect.h = 30;

	MenuItem* poItem = AddMenuItem( "~OK", SDLK_o, a_iOkCode );
	poItem->SetPosition( oRect );
	
//	poItem = AddMenuItem( "Cancel", SDLK_UNKNOWN, 0 );
	oRect.x = gamescreen->w - 150;
	poItem->SetPosition( oRect );
}


void Menu::InvokeSubmenu( Menu* a_poMenu )
{
	Audio->PlaySample( "strange_button.voc" );
	Clear();
	m_iReturnCode = a_poMenu->Run();
	if ( m_iReturnCode < 0 )
	{
		Audio->PlaySample( "pop.voc" );
		Draw();
	}
	else
	{
		m_iReturnCode --;
		m_bDone = true;
	}
}


void Menu::ItemActivated( int a_iItemCode, MenuItem* a_poMenuItem )
{
	debug( "Menu::ItemActivated( %d )\n", a_iItemCode );
	
	switch ( a_iItemCode )
	{
		case MENU_QUIT:
			m_bDone = true;
			m_iReturnCode = 100;
			g_oState.m_bQuitFlag = true;
			break;
		
		case MENU_SURRENDER:
			m_bDone = true;
			m_iReturnCode = 100;
			g_oState.m_enGameMode = SState::IN_DEMO;
			break;
			
		case MENU_MULTI_PLAYER:
			m_bDone = true;
			m_iReturnCode = 100;
			g_oState.m_enGameMode = SState::IN_MULTI;
			break;
		
		case MENU_FULLSCREEN:
			Audio->PlaySample( "strange_button.voc" );
			g_oState.ToggleFullscreen();
			if ( NULL != poBackground )
			{
				SDL_BlitSurface( poBackground, NULL, gamescreen, NULL );
			}
			else
			{
				SDL_FillRect( gamescreen, NULL, 0 );
			}
			a_poMenuItem->SetText( g_oState.m_bFullscreen ? "~FULLSCREEN ON" : "~FULLSCREEN OFF", true );
			Draw();
			break;
		
		case MENU_OPTIONS_OK:
			m_bDone = true;
			m_iReturnCode = -1;
			break;
		
		case MENU_OPTIONS:
		{
			Menu* poMenu = new Menu( "Options" );
			poMenu->AddEnumMenuItem( "GAME TIME: ", g_oState.m_iGameTime, g_ppcGameTime, g_piGameTime, MENU_GAME_TIME );
			poMenu->AddEnumMenuItem( "HIT POINTS: ", g_oState.m_iHitPoints, g_ppcHitPoints, g_piHitPoints, MENU_TOTAL_HIT_POINTS );
			poMenu->AddMenuItem( "~SOUND", SDLK_s, MENU_SOUND );
			poMenu->AddMenuItem( g_oState.m_bFullscreen ? "~FULLSCREEN ON" : "~FULLSCREEN OFF", SDLK_f, MENU_FULLSCREEN );
			poMenu->AddMenuItem( "~RIGHT PLAYER KEYS", SDLK_r, MENU_KEYS_RIGHT );
			poMenu->AddMenuItem( "~LEFT PLAYER KEYS", SDLK_l, MENU_KEYS_LEFT );
			poMenu->AddOkCancel( MENU_OPTIONS_OK );
			
			InvokeSubmenu( poMenu );
			delete poMenu;
			break;
		}
		
		case MENU_SOUND:
		{
			Menu* poMenu = new Menu( "Sound" );
			poMenu->AddEnumMenuItem( "CHANNELS: ", 1, g_ppcChannels, g_piChannels, MENU_CHANNELS )->SetEnabled(false);
			poMenu->AddEnumMenuItem( "SOUND QUALITY: ", 2, g_ppcMixingRate, g_piMixingRate, MENU_MIXING_RATE )->SetEnabled(false);
			poMenu->AddEnumMenuItem( "SOUND FIDELITY: ", 2, g_ppcMixingBits, g_piMixingBits, MENU_BITS )->SetEnabled(false);
			poMenu->AddEnumMenuItem( "MUSIC VOLUME: ", g_oState.m_iMusicVolume, g_ppcVolume, g_piVolume, MENU_MUSIC_VOLUME );
			poMenu->AddEnumMenuItem( "EFFECTS VOLUME: ", g_oState.m_iSoundVolume, g_ppcVolume, g_piVolume, MENU_SOUND_VOLUME );
			poMenu->AddOkCancel( MENU_SOUND_OK );
			
			InvokeSubmenu( poMenu );
			delete poMenu;
			break;
		}
		
		case MENU_SOUND_OK:
			m_bDone = true;
			m_iReturnCode = -1;
			break;
		
		case MENU_KEYS_LEFT:
			InputKeys(1);
			Draw();
			break;
		
		case MENU_KEYS_RIGHT:
			InputKeys(0);
			Draw();
			break;
		
		default:
			break;
	}
}



void Menu::ItemChanged( int a_iItemCode, int a_iValue, MenuItem* a_poMenuItem )
{
	debug( "Menu::ItemActivated( %d )\n", a_iItemCode );
	
	switch ( a_iItemCode )
	{
		case MENU_MUSIC_VOLUME:
			g_oState.m_iMusicVolume = a_iValue;
			Audio->SetMusicVolume( a_iValue );
			break;
			
		case MENU_SOUND_VOLUME:
			g_oState.m_iSoundVolume = a_iValue;
			break;
			
		case MENU_GAME_TIME:
			g_oState.m_iGameTime = a_iValue;
			break;
			
		case MENU_TOTAL_HIT_POINTS:
			g_oState.m_iHitPoints = a_iValue;
			break;
	} // end of switch a_iItemCode
}


/** Run executes the menus, maybe invoking submenus as well. The
menus modify the global game state.

Returns 0, or the number of parent menus that should be cleared. */

int Menu::Run()
{
	if ( m_oItems[m_iCurrentItem]->GetEnabled() )
	{
		m_oItems[m_iCurrentItem]->SetActive(true);
	}
	else
	{
		FocusNext();
	}

	Draw();

	while ( !m_bDone )
	{
		SDLKey enKey = GetKey();
		
		if ( g_oState.m_bQuitFlag ||
			SDLK_ESCAPE == enKey )
		{
			m_bDone = true;
			m_iReturnCode = -1;
			break;
		}
		
		switch ( enKey )
		{
			case SDLK_UP:
			{
				FocusPrev();
				break;
			} // end of SDLK_UP
			

			case SDLK_DOWN:
			{
				FocusNext();
				break;
			} // end of SDLK_DOWN
			
			case SDLK_LEFT:
			{
				MenuItem* poItem = m_oItems[m_iCurrentItem];
				poItem->Decrement();
				break;
			}
			
			case SDLK_RIGHT:
			{
				MenuItem* poItem = m_oItems[m_iCurrentItem];
				poItem->Increment();
				break;
			}
			
			case SDLK_RETURN:
			{
				MenuItem* poItem = m_oItems[m_iCurrentItem];
				if ( poItem->GetEnabled() )
				{
					poItem->Activate();
				}
			}
			
			default:
				break;
		} // end of switch
	}
	
	Clear();
	
	return m_iReturnCode;
}


void Menu::Draw()
{
	DrawGradientText( m_sTitle.c_str(), titleFont, 20, gamescreen );

	for ( ItemIterator it=m_oItems.begin(); it!=m_oItems.end(); ++it )
	{
		(*it)->Draw();
	}

	SDL_Flip( gamescreen );

}



void Menu::FocusNext()
{
	MenuItem* poItem = NULL;
	int iNextItem;
	
	for ( iNextItem = m_iCurrentItem+1; iNextItem < (int) m_oItems.size(); ++iNextItem )
	{
 		poItem = m_oItems[iNextItem];
		if ( poItem->GetEnabled() )
		{
			break;
		}
		poItem = NULL;
	}
	
	if ( NULL != poItem )
	{
		Audio->PlaySample("strange_quack.voc");
		m_oItems[m_iCurrentItem]->SetActive(false);
		m_oItems[iNextItem]->SetActive(true);
		m_iCurrentItem = iNextItem;
	}
}



void Menu::FocusPrev()
{
	MenuItem* poItem = NULL;
	int iPrevItem;
	
	for ( iPrevItem = m_iCurrentItem-1; iPrevItem >= 0; --iPrevItem )
	{
		poItem = m_oItems[iPrevItem];
		if ( poItem->GetEnabled() )
		{
			break;
		}
		poItem = NULL;
	}
	
	if ( NULL != poItem )
	{
		Audio->PlaySample("strange_quack.voc");
		m_oItems[m_iCurrentItem]->SetActive(false);
		m_oItems[iPrevItem]->SetActive(true);
		m_iCurrentItem = iPrevItem;
	}
}



void Menu::Clear()
{
	if (poBackground)
	{
		SDL_BlitSurface( poBackground, 0, gamescreen, 0 );
	}
	else
	{
		SDL_FillRect( gamescreen, NULL, 0 );
	}
}


void DoMenu( bool a_bDrawBackground )
{
	Audio->PlaySample( "crashhh.voc" );
	
	poBackground = SDL_ConvertSurface( gamescreen, gamescreen->format, SDL_SWSURFACE );
	if ( NULL == poBackground )
	{
		debug( "DoMenu: Couldn't allocate background.\n" );
	}
	else
	{
		int i;
		SDL_Rect oRect;
		oRect.x = 0; oRect.w = poBackground->w; oRect.h = 1;
		
		for ( i=0; i<poBackground->h; i += 2 )
		{
			oRect.y = i;
			SDL_FillRect( poBackground, &oRect, C_BLACK );	
		}
		
		oRect.w = 1; oRect.y = 0; oRect.h = poBackground->h;
		
		for ( i=0; i<poBackground->w; i+=2 )
		{
			oRect.x = i;
			SDL_FillRect(poBackground, &oRect, C_BLACK );
		}
		
		SDL_BlitSurface( poBackground, 0, gamescreen, 0 );
		SDL_Flip( gamescreen );
	}

	Menu oMenu( "Main Menu" );
	
	if ( SState::IN_DEMO == g_oState.m_enGameMode )
	{
		oMenu.AddMenuItem( "~SINGLE PLAYER GAME", SDLK_s, MENU_SINGLE_PLAYER )->SetEnabled(false);
		oMenu.AddMenuItem( "~MULTI PLAYER GAME", SDLK_m, MENU_MULTI_PLAYER );
	}
	else
	{
		oMenu.AddMenuItem( "~SURRENDER GAME", SDLK_s, MENU_SURRENDER );
	}
	oMenu.AddMenuItem( "~OPTIONS", SDLK_o, MENU_OPTIONS );
	oMenu.AddMenuItem( "~INFO", SDLK_i, MENU_INFO )->SetEnabled(false);
	oMenu.AddMenuItem( "QUIT", SDLK_UNKNOWN, MENU_QUIT );
	
	oMenu.Run();
	
	Audio->PlaySample("shades_rollup.voc");
	
	//SDL_BlitSurface( poBackground, 0, gamescreen, 0 );
	//SDL_Flip( gamescreen );
	SDL_FreeSurface( poBackground );
	poBackground = NULL;
}


