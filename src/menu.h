/***************************************************************************
                          menu.h  -  description
                             -------------------
    begin                : Sun Aug 3 2003
    copyright            : (C) 2003 by upi
    email                : upi@apocalypse.rulez.org
 ***************************************************************************/


#ifndef MENU_H
#define MENU_H


#include <string>
#include <vector>

class MenuItem;
class EnumMenuItem;
class TextMenuItem;

class Menu
{
public:
	Menu( const char* a_pcTitle );
	virtual ~Menu();

	virtual MenuItem* AddMenuItem( const char* a_pcUtf8Text, SDLKey a_tShortcut = SDLK_UNKNOWN, int a_iCode = 0 );
	virtual EnumMenuItem* AddEnumMenuItem( const char* a_pcUtf8Text, int a_iInitialValue, 
		const char** a_ppcNames, const int* a_piValues, int a_iCode = 0 );
	virtual TextMenuItem* AddTextMenuItem( const char* a_pcTitle, const char* a_pcValue, int a_iCode = 0 );
	virtual MenuItem* AddMenuItem( MenuItem* a_poItem );
	virtual void AddOkCancel( int a_iOkCode = 0 );
	virtual void ItemActivated( int a_iItemCode, MenuItem* a_poMenuItem );
	virtual void ItemChanged( int a_iItemCode, int a_iValue, MenuItem* a_poMenuItem );
	virtual int Run();

	virtual void Draw();
	virtual void Clear();
	virtual void EnterName( const char* a_pcTitle, std::string& a_rsTarget, TextMenuItem* a_poMenuItem, int a_iMaxlen );

protected:

	virtual void FocusNext();
	virtual void FocusPrev();
	virtual void InvokeSubmenu( Menu* a_poSubmenu );

	typedef std::vector<MenuItem*> ItemList;
	typedef ItemList::iterator ItemIterator;

	
	std::string			m_sTitle;
	ItemList			m_oItems;
	int					m_iCurrentItem;
	int					m_iReturnCode;
	bool				m_bDone;
};



class MenuItem
{
public:
	MenuItem( Menu* a_poMenu, const char* a_pcUtf8Text, int a_iCode = -1 );
	virtual ~MenuItem();

	virtual void Draw();
	virtual void Clear();
	virtual void Activate();
	virtual void Increment() {};
	virtual void Decrement() {};
	
	virtual void SetText( const char* a_pcUtf8Text, bool a_bCenter );
	virtual void SetPosition( const SDL_Rect& a_roPosition );
	virtual void SetActive( bool a_bActive );
	virtual void SetEnabled( bool a_bEnabled );
	
	virtual bool GetEnabled() { return m_bEnabled; }
	
protected:
	Menu*			m_poMenu;

	// appearance
	std::string		m_sUtf8Text;
	SDL_Rect		m_oPosition;
	bool			m_bCenter;
	Uint32			m_iHighColor;
	Uint32			m_iLowColor;
	Uint32			m_iInactiveColor;
	Uint32			m_iBackgroundColor;

	// data content
	int				m_iCode;
	bool			m_bActive;
	bool			m_bEnabled;
};



class EnumMenuItem: public MenuItem
{
public:
	EnumMenuItem(  Menu* a_poMenu, int a_iInitialValue, const char* a_pcUtf8Text, int a_iCode = -1 );
	virtual ~EnumMenuItem();
	
	int GetCurrentValue();
	const char* GetCurrentText();
	virtual void Draw();
	virtual void Increment();
	virtual void Decrement();
	
	virtual void SetEnumValues( const char ** a_ppcNames, const int * a_piValues );
	
protected:
	int				m_iValue;
	int				m_iMax;
	std::string		m_sUtf8Title;
	const char**	m_ppcNames;
	const int*		m_piValues;
};



class TextMenuItem: public MenuItem
{
public:
	TextMenuItem( Menu* a_poMenu, const char* a_pcInitialValue, const char* a_pcUtf8Title, int a_iCode );
	virtual ~TextMenuItem();

	virtual void Draw();
	virtual void SetValue( const char* a_pcValue );
	
protected:
	std::string		m_sTitle;
	std::string		m_sValue;
};




class CNetworkMenu: public Menu
{
public:
	CNetworkMenu();
	~CNetworkMenu();
	
	void Connect();
	
	void ItemActivated( int a_iItemCode, MenuItem* a_poMenuItem );
	void ItemChanged( int a_iItemCode, int a_iValue, MenuItem* a_poMenuItem );

protected:
	bool			m_bOK;
	bool			m_bServer;
	std::string		m_sHostname;
	std::string		m_sNick;

	TextMenuItem*	m_poServerMenuItem;
	TextMenuItem*	m_poNickMenuItem;
};




void DoMenu();
void DoMenu( Menu& a_roMenu );

#endif
