/***************************************************************************
                          Backend.h  -  description
                             -------------------
    begin                : Sun Dec 8 2002
    copyright            : (C) 2002 by upi
    email                : upi@apocalypse.rulez.org
 ***************************************************************************/


#include <EXTERN.h>
#include <perl.h>
#include <string>

class RlePack;
extern PerlInterpreter *my_perl;


#define PERLEVAL(A) eval_pv(A, TRUE);

#define PERLCALL(PROC,A,B) {							\
    dSP;												\
    ENTER;												\
    SAVETMPS;											\
    PUSHMARK(SP);										\
    XPUSHs(sv_2mortal(newSViv(A)));						\
    XPUSHs(sv_2mortal(newSViv(B)));						\
    PUTBACK ;											\
														\
    call_pv( (PROC),  G_DISCARD );						\
														\
    FREETMPS;											\
    LEAVE;												\
}



#define MAXDOODADS 20
#define MAXSOUNDS 20


class Backend
{
public:

	// Lifecycle
	
	Backend();
	~Backend();
	bool Construct();
	
	// Miscellaneous
	
	void PerlEvalF( const char* a_pcFormat, ... );
	
	// Game data
	
	void AdvancePerl();
	void ReadFromPerl();
	void PlaySounds();
	void WriteToString( std::string& a_rsOutString );
	void ReadFromString( const std::string& a_rsString );
	
public:
	int				m_iGameTime;
	int				m_iGameOver;
	bool			m_bKO;
	int				m_iBgX, m_iBgY;
	int				m_iNumDoodads;
	int				m_iNumSounds;
	
	struct SPlayer
	{
		int m_iX, m_iY, m_iFrame, m_iHitPoints;
	}				m_aoPlayers[2];
	
	struct SDoodad
	{
		int m_iX, m_iY, m_iType, m_iFrame;
		std::string m_sText;
	}				m_aoDoodads[ MAXDOODADS ];

	std::string		m_asSounds[ MAXSOUNDS ];
};

extern Backend g_oBackend;



// FIGHTER ENUMERABLE

enum FighterEnum {
	UNKNOWN = 0,
	
	ULMAR,
	UPI,
	ZOLI,
	CUMI,
	SIRPI,
	MACI,
	BENCE,
	GRIZLI,
	DESCANT,
	SURBA,
	AMBRUS,
	DANI,
	KINGA,
	MISI,
	
	LASTFIGHTER
};


enum TintEnum {
	NO_TINT = 0,
	RANDOM_TINT,
	ZOMBIE_TINT,
	GRAY_TINT,
	DARK_TINT,
	INVERTED_TINT,
};


bool		IsFighterAvailable( FighterEnum a_enFighter );
RlePack*	LoadFighter( FighterEnum fighter, int offset );
void		TintFighter( RlePack* fighter, TintEnum tint, int offset );
int			DoGame( char* replay, bool isReplay, bool bDebug );
void		PlayerSelect();
void		SetPlayer( int player, FighterEnum fighter );



extern FighterEnum Fighter1;
extern FighterEnum Fighter2;
extern TintEnum FingerTint1;
extern TintEnum FingerTint2;
extern RlePack* pack1;
extern RlePack* pack2;

