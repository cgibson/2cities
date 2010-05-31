#ifndef _SOUND_MANAGER_H_
#define _SOUND_MANAGER_H_

#include <SDL/SDL.h>
#include "SDL_mixer.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#define NUM_BUILD_SONGS 1
#define NUM_CARNAGE_SONGS 5
#define NUM_BUILD_SFX 0
#define NUM_CARNAGE_SFX 3
#define MUSIC_CHANNEL 1
#define SFX_CHANNEL 2
#define FADE_OUT_MSECS 1000
#define SDL_DELAY_TIME 100

class SoundManager
{
	private:
		int currbuildmusic;
		int currcarnagemusic;
		int currbuildsfx;
		int currcarnagesfx;
		Mix_Music *buildmusic[NUM_BUILD_SONGS];
		Mix_Music *carnagemusic[NUM_CARNAGE_SONGS];
		Mix_Chunk *buildsfx[NUM_BUILD_SFX];
		Mix_Chunk *carnagesfx[NUM_CARNAGE_SFX];
		
	public:
		SoundManager();
		~SoundManager();
		
		bool hasEnding(std::string const &musicFile, std::string const &extension);
		void initialize();
		void tearDownSound();
		void loadMusic();
		void loadSfx();
		void playBuildSong();
		void playCarnageSong();
		void playNextBuildSong();
		void playNextCarnageSong();
		void stopPlayingMusic();
		void playBuildSfx(int buildsfxnum);
		void playCarnageSfx(int carnagesfxnum);
		int getCurrBuildMusic();
		int getCurrCarnageMusic();
		int getCurrBuildSfx();
		int getCurrCarnageSfx();
		void setCurrBuildMusic(int newcurrbuildmusic);
		void setCurrCarnageMusic(int newcurrcarnagemusic);
		
		//Useful for ammo in particular.
		void setCurrBuildSfx(int newcurrbuildsfx);
		void setCurrCarnageSfx(int newcurrcarnagesfx);
};

#endif
