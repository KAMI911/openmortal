/***************************************************************************
                          Audio.cpp  -  description
                             -------------------
    begin                : Sat Jul 26 2003
    copyright            : (C) 2003 by upi
    email                : upi@apocalypse.rulez.org
 ***************************************************************************/

#include "Audio.h"
#include "State.h"
#include "common.h"

#include "SDL_mixer.h"

#include <string>
#include <map>


typedef std::map<std::string,Mix_Chunk*>	SampleMap;
typedef std::map<std::string,Mix_Music*>	MusicMap;
typedef SampleMap::iterator			SampleMapIterator;
typedef MusicMap::iterator			MusicMapIterator;


MszAudio* Audio = NULL;


class MszAudioPriv
{
public:
	bool		m_bAudioOk;
	int		m_iNumChannels;
	SampleMap	m_oSamples;
	MusicMap	m_oMusics;
};



#define SELF (*m_poPriv)
#define CHECKOK if ( ! m_poPriv->m_bAudioOk ) return;


MszAudio::MszAudio()
{
	Audio = this;
	m_poPriv = new MszAudioPriv;
	m_poPriv->m_bAudioOk = false;
	m_poPriv->m_iNumChannels = 0;

	SDL_version compile_version;
	const SDL_version *link_version;
	MIX_VERSION(&compile_version);
	debug( "compiled with SDL_mixer version: %d.%d.%d\n",
		compile_version.major, compile_version.minor, compile_version.patch);
		
	link_version=Mix_Linked_Version();
	debug("running with SDL_mixer version: %d.%d.%d\n",
		link_version->major, link_version->minor, link_version->patch);
	if ( Mix_OpenAudio( MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,
		2 /*stereo*/, 1024*2 /*chunksize*/ ) < 0 )
	{
		debug("Mix_OpenAudio: %s\n", Mix_GetError());
		SELF.m_bAudioOk = false;
	}
	else
	{
		SELF.m_bAudioOk = true;
	}

	m_poPriv->m_iNumChannels = Mix_AllocateChannels(8);
}


MszAudio::~MszAudio()
{
	delete m_poPriv;
	m_poPriv = NULL;
}


void MszAudio::LoadSample( const char* a_pcFilename, const char* a_pcSampleName )
{
	CHECKOK;

	std::string sSampleName( a_pcSampleName ? a_pcSampleName : a_pcFilename );
	std::string sSampleFile = DATADIR;
	sSampleFile += "/sound/";
	sSampleFile += a_pcFilename;

	if ( m_poPriv->m_oSamples.count( sSampleName ) )
	{
		debug( "Key %s already loaded", a_pcSampleName );
		return;
	}
	
	Mix_Chunk *poSample;
	poSample=Mix_LoadWAV(sSampleFile.c_str());
	if(!poSample)
	{
		debug("Mix_LoadWAV: %s\n", Mix_GetError());
		return;
	}

	m_poPriv->m_oSamples[ sSampleName ] = poSample;
}



void MszAudio::UnloadSample( const char* a_pcSampleName )
{
	SampleMapIterator it = m_poPriv->m_oSamples.find( a_pcSampleName );
	if ( m_poPriv->m_oSamples.end() == it )
	{
		debug( "UnloadSample: sample %s not found", a_pcSampleName );
		return;
	}

	Mix_FreeChunk( (*it).second );
	m_poPriv->m_oSamples.erase( it );
}


void MszAudio::PlaySample( const char* a_pcSampleName )
{
	Mix_Chunk* poSample;
	SampleMapIterator it = m_poPriv->m_oSamples.find(a_pcSampleName);
	if ( m_poPriv->m_oSamples.end() == it )
	{
		// Try to load the sample..
		LoadSample( a_pcSampleName );
		
		it = m_poPriv->m_oSamples.find(a_pcSampleName);
		if ( m_poPriv->m_oSamples.end() == it )
		{
			debug( "PlaySample: sample %s not found", a_pcSampleName );
			return;
		}
	}

	poSample = (*it).second;
	
	int iVolume = g_oState.m_iSoundVolume * 128 / 100;
	if ( poSample->volume != iVolume )
	{
		Mix_VolumeChunk( poSample, iVolume );
	}

	if ( -1 == Mix_PlayChannel( -1, poSample, 0 ) )
	{
		debug( "PlaySample: Mix_PlayChannel: %s\n",Mix_GetError());
	}
}


void MszAudio::PlayFile( const char* a_pcFileName )
{
	Mix_Chunk *poSample;
	poSample=Mix_LoadWAV("sample.wav");
	if(!poSample)
	{
		debug("Mix_LoadWAV: %s\n", Mix_GetError());
		return;
	}
	if ( -1 == Mix_PlayChannel( -1, poSample, 0 ) )
	{
		debug( "PlaySample: Mix_PlayChannel: %s\n",Mix_GetError());
	}
	Mix_FreeChunk( poSample );
}


void MszAudio::LoadMusic( const char* a_pcFilename, const char* a_pcMusicName )
{
	CHECKOK;

	std::string sMusicName( a_pcMusicName ? a_pcMusicName : a_pcFilename );
	std::string sMusicFile = DATADIR;
	sMusicFile += "/sound/";
	sMusicFile += a_pcFilename;

	if ( m_poPriv->m_oMusics.count( sMusicName ) )
	{
		debug( "Key %s already loaded", a_pcMusicName );
		return;
	}

	Mix_Music *poMusic;
	poMusic=Mix_LoadMUS(sMusicFile.c_str());
	if(!poMusic)
	{
		debug("Mix_LoadMUS: %s\n", Mix_GetError());
		return;
	}

	m_poPriv->m_oMusics[ sMusicName ] = poMusic;
}


void MszAudio::UnloadMusic( const char* a_pcMusicName )
{
	MusicMapIterator it = m_poPriv->m_oMusics.find( a_pcMusicName );
	if ( m_poPriv->m_oMusics.end() == it )
	{
		debug( "UnloadMusic: music %s not found", a_pcMusicName );
		return;
	}

	Mix_FreeMusic( (*it).second );
	m_poPriv->m_oMusics.erase( it );
}


void MszAudio::PlayMusic( const char* a_pcMusicName )
{
	MusicMapIterator it = m_poPriv->m_oMusics.find( a_pcMusicName );
	if ( m_poPriv->m_oMusics.end() == it )
	{
		debug( "PlayMusic: music %s not found", a_pcMusicName );
		return;
	}

	SetMusicVolume( g_oState.m_iMusicVolume );
	//Mix_PlayMusic( (*it).second, -1 );
	Mix_FadeInMusic( (*it).second, -1, 100 );
}


void MszAudio::FadeMusic( int a_iMilliSec )
{
	Mix_FadeOutMusic( a_iMilliSec );
}


void MszAudio::SetMusicVolume( int a_iVolume )
{
	Mix_VolumeMusic( a_iVolume * 128 / 100 );
}


void MszAudio::StopMusic()
{
	Mix_HaltMusic();
}


bool MszAudio::IsMusicPlaying()
{
	return Mix_PlayingMusic();
}







