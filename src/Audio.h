/***************************************************************************
                          Audio.h  -  description
                             -------------------
    begin                : Sat Jul 26 2003
    copyright            : (C) 2003 by upi
    email                : upi@apocalypse.rulez.org
 ***************************************************************************/


#ifndef OPENMORTAL_AUDIO_H
#define OPENMORTAL_AUDIO_H


#ifndef NULL
#define NULL 0
#endif


class COpenMortalAudioPriv;

/**
\class COpenMortalAudio
\brief Audio services for OpenMortal (sound and music).
\ingroup Media

This class is a wrapper around SDL_Mixer
*/

class COpenMortalAudio
{
public:
	COpenMortalAudio();
	~COpenMortalAudio();
	void LoadSampleMap();

public:
	// Sample related methods
	
	void LoadSample( const char* a_pcFilename, const char* a_pcSampleName=NULL );
	void UnloadSample( const char* a_pcSampleName );
	void PlaySample( const char* a_pcSampleName );
	void PlayFile( const char* a_pcFilename );

	// Music related methods

	void LoadMusic( const char* a_pcFilename, const char* a_pcMusicName=NULL );
	void UnloadMusic( const char* a_pcMusicName );
	void PlayMusic( const char* a_pcMusicName );
	void FadeMusic( int a_iMilliSec );
	void SetMusicVolume( int a_iVolume );
	void StopMusic();
	bool IsMusicPlaying();

protected:
	// Attributes

	COpenMortalAudioPriv* m_poPriv;
};


extern COpenMortalAudio* Audio;

#endif // ifdef OPENMORTAL_AUDIO_H
