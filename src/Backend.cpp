/***************************************************************************
                          Backend.cpp  -  description
                             -------------------
    begin                : Mon Dec 9 2002
    copyright            : (C) 2002 by upi
    email                : upi@apocalypse.rulez.org
 ***************************************************************************/


#include "common.h"
#include "Backend.h"
#include "Audio.h"

#include <string>
#include <stdarg.h>


/***************************************************************************
                     PUBLIC EXPORTED VARIABLES
***************************************************************************/

PerlInterpreter*	my_perl;
Backend				g_oBackend;


/***************************************************************************
                     PRIVATE VARIABLES (perl variable space)
***************************************************************************/

SV
	*perl_bgx, *perl_bgy,
	*perl_p1x, *perl_p1y, *perl_p1f, *perl_p1h,
	*perl_p2x, *perl_p2y, *perl_p2f, *perl_p2h,
	*perl_time, *perl_over, *perl_ko;

SV
	*perl_doodad_x, *perl_doodad_y,
	*perl_doodad_t, *perl_doodad_f,
	*perl_doodad_text;

SV
	*perl_sound;



/***************************************************************************
                  		BACKEND CLASS IMPLEMENTATION
***************************************************************************/

Backend::Backend()
{
	m_iBgX = m_iBgY = 0;
	m_iNumDoodads = m_iNumSounds = 0;
	for ( int i=0; i<2; ++i )
	{
		m_aoPlayers[i].m_iX = m_aoPlayers[i].m_iY = 0;
		m_aoPlayers[i].m_iFrame = 0;
		m_aoPlayers[i].m_iHitPoints = 0;
	}
}


Backend::~Backend()
{
	if ( NULL != my_perl )
	{
		perl_destruct( my_perl );
		perl_free( my_perl );
		my_perl = NULL;
	}
}


bool Backend::Construct()
{
	if ( my_perl != NULL )
	{
		// Already inited
		return false;
	}
	
	perl_bgx = NULL;
	perl_doodad_x = NULL;
	
	std::string sFileName = DATADIR;
	sFileName += "/script";
	
	chdir( sFileName.c_str() );
	
	char *perl_argv[] = {"", "Backend.pl"};
	my_perl = perl_alloc();
	if ( my_perl == NULL )
	{
		return false;
	}
	
	perl_construct( my_perl );
	if ( perl_parse( my_perl, NULL, 2, perl_argv, (char**)NULL ) )
	{
		char *error = SvPV_nolen(get_sv("@", FALSE));
		fprintf( stderr, "%s", error );
		return false;
	}
	
	if ( perl_run( my_perl ) )
	{
		char *error = SvPV_nolen(get_sv("@", FALSE));
		fprintf( stderr, "%s", error );
		return false;
	}

	return true;
}


void Backend::PerlEvalF( const char* a_pcFormat, ... )
{
	va_list ap;
	va_start( ap, a_pcFormat );
	
	char acBuffer[1024];
	vsnprintf( acBuffer, 1023, a_pcFormat, ap );
	acBuffer[1023] = 0;
	PERLEVAL(acBuffer);
	
	va_end( ap );
}



void Backend::AdvancePerl()
{
	PERLEVAL("GameAdvance();");
}



void Backend::ReadFromPerl()
{
	if ( perl_bgx == NULL )
	{
		perl_bgx = get_sv("bgx", TRUE);
		perl_bgy = get_sv("bgy", TRUE);
		perl_p1x = get_sv("p1x", TRUE);
		perl_p1y = get_sv("p1y", TRUE);
		perl_p1f = get_sv("p1f", TRUE);
		perl_p1h = get_sv("p1h", TRUE);
		perl_p2x = get_sv("p2x", TRUE);
		perl_p2y = get_sv("p2y", TRUE);
		perl_p2f = get_sv("p2f", TRUE);
		perl_p2h = get_sv("p2h", TRUE);
		perl_time= get_sv("time", TRUE);
		perl_over= get_sv("over", TRUE);
		perl_ko = get_sv("ko", TRUE);
	}

	m_iBgX = SvIV( perl_bgx );
	m_iBgY = SvIV( perl_bgy );
	m_aoPlayers[0].m_iX = SvIV( perl_p1x );
	m_aoPlayers[0].m_iY = SvIV( perl_p1y );
	m_aoPlayers[0].m_iFrame = SvIV( perl_p1f );
	m_aoPlayers[0].m_iHitPoints = SvIV( perl_p1h ) / 10;
	m_aoPlayers[1].m_iX = SvIV( perl_p2x );
	m_aoPlayers[1].m_iY = SvIV( perl_p2y );
	m_aoPlayers[1].m_iFrame = SvIV( perl_p2f );
	m_aoPlayers[1].m_iHitPoints = SvIV( perl_p2h ) / 10;
	m_iGameTime = SvIV( perl_time );
	m_iGameOver = SvIV( perl_over );
	m_bKO = SvIV( perl_ko );
	
	// READ DOODAD DATA
	
	if ( perl_doodad_x == NULL )
	{
		perl_doodad_x = get_sv("doodad_x", TRUE);
		perl_doodad_y = get_sv("doodad_y", TRUE);
		perl_doodad_t = get_sv("doodad_t", TRUE);
		perl_doodad_f = get_sv("doodad_f", TRUE);
		perl_doodad_text = get_sv("doodad_text", TRUE);
	}
	
	for ( m_iNumDoodads=0; m_iNumDoodads<MAXDOODADS; ++m_iNumDoodads )
	{
		PERLEVAL("GetNextDoodadData();");
		
		SDoodad& oDoodad = m_aoDoodads[m_iNumDoodads];
		
		oDoodad.m_iType = SvIV(perl_doodad_t);
		
		if ( oDoodad.m_iType < 0 )
		{
			break;
		}
		
		oDoodad.m_iX = SvIV(perl_doodad_x);
		oDoodad.m_iY = SvIV(perl_doodad_y);
		oDoodad.m_iFrame = SvIV(perl_doodad_f);

		if ( oDoodad.m_iType == 0 )
		{
			oDoodad.m_sText = SvPV_nolen(perl_doodad_text);
		}
		else
		{
			oDoodad.m_sText.clear();
		}
	}
	
	
	// READ SOUND DATA
	
	if ( perl_sound == NULL )
	{
		perl_sound = get_sv("sound", TRUE);
	}
	
	for ( m_iNumSounds=0; m_iNumSounds<MAXSOUNDS; ++m_iNumSounds )
	{
		PERLEVAL("GetNextSoundData();");
		const char* pcSound = SvPV_nolen(perl_sound);
	
		if ( NULL == pcSound
			|| 0 == *pcSound )
		{
			break;
		}
		
		m_asSounds[ m_iNumSounds ] = pcSound;
		//Audio->PlaySample( pcSound );
	}
}


void Backend::PlaySounds()
{
	for ( int i=0; i<m_iNumSounds; ++i )
	{
		Audio->PlaySample( m_asSounds[i].c_str() );
	}
}



/***************************************************************************
                  	PLAYBACK STRING CONVERSION ROUTINES
***************************************************************************/


void Backend::WriteToString( std::string& a_rsOutString )
{
	char acBuffer[2048];
	int iNumChars = sprintf( acBuffer, "%d %d  %d %d %d %d  %d %d %d %d  %d  ",
		m_iBgX, m_iBgY,
		m_aoPlayers[0].m_iX, m_aoPlayers[0].m_iY, m_aoPlayers[0].m_iFrame, m_aoPlayers[0].m_iHitPoints,
		m_aoPlayers[1].m_iX, m_aoPlayers[1].m_iY, m_aoPlayers[1].m_iFrame, m_aoPlayers[1].m_iHitPoints,
		m_iNumDoodads );
	
	int i;
	for ( i = 0; i<m_iNumDoodads; ++i )
	{
		SDoodad& roDoodad = m_aoDoodads[i];
		iNumChars += sprintf( acBuffer+iNumChars, "%d %d %d %d %d %s  ",
			roDoodad.m_iX, roDoodad.m_iY, roDoodad.m_iType, roDoodad.m_iFrame,
			roDoodad.m_sText.size(), roDoodad.m_sText.c_str() );
	}
	
	iNumChars += sprintf( acBuffer+iNumChars, "%d ", m_iNumSounds );
	for ( i = 0; i<m_iNumSounds; ++i )
	{
		iNumChars += sprintf( acBuffer+iNumChars, " %d %s",
			m_asSounds[i].size(), m_asSounds[i].c_str() );
	}
	
	// debug( "Frame: '%s'\n", acBuffer );
	a_rsOutString = acBuffer;
}


void Backend::ReadFromString( const std::string& a_rsString )
{
	const char* pcBuffer = a_rsString.c_str();
	
	int iNumMatches;
	int iOffset, iTotal;
	iNumMatches = sscanf( pcBuffer, "%d %d %d %d %d %d %d %d %d %d %d%n",
		&m_iBgX, &m_iBgY,
		&m_aoPlayers[0].m_iX, &m_aoPlayers[0].m_iY, &m_aoPlayers[0].m_iFrame, &m_aoPlayers[0].m_iHitPoints,
		&m_aoPlayers[1].m_iX, &m_aoPlayers[1].m_iY, &m_aoPlayers[1].m_iFrame, &m_aoPlayers[1].m_iHitPoints,
		&m_iNumDoodads, &iTotal );
	
	if ( m_iNumDoodads > MAXDOODADS )
	{
		m_iNumDoodads = m_iNumSounds = 0;
		return;
	}
	
	int i, j;
	for ( i=0; i<m_iNumDoodads; ++i )
	{
		SDoodad& roDoodad = m_aoDoodads[i];
		iNumMatches += sscanf( pcBuffer+iTotal, "%d %d %d %d %d %n",
			&roDoodad.m_iX, &roDoodad.m_iY, &roDoodad.m_iType, &roDoodad.m_iFrame,
			&j, &iOffset );
		iTotal += iOffset;
		roDoodad.m_sText.assign( pcBuffer + iTotal, j );
		iTotal += j;
	}
	
	iNumMatches += sscanf( pcBuffer + iTotal, "%d%n",
		&m_iNumSounds, &iOffset );
		
	if ( m_iNumSounds > MAXSOUNDS )
	{
		m_iNumSounds = 0;
		return;
	}
	
	iTotal += iOffset;
	
	for ( i=0; i<m_iNumSounds; ++i )
	{
		iNumMatches += sscanf( pcBuffer+iTotal, "%d %n",
			&j, &iOffset );
		iTotal += iOffset;
		m_asSounds[i].assign( pcBuffer + iTotal, j );
		iTotal += j;
	}
}

