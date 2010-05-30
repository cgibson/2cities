#ifndef _SOUNDMANAGER_H_
#define _SOUNDMANAGER_H_

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "SDL_mixer.h"

class SoundManager {
public:
	SoundManager();
	~SoundManager();

	typedef enum E_SOUNDS
	{
		TEMP_BACKGROUND_MUSIC
	} Sound;

	// initialize and load sounds
	void init();

	// update the camera every frame
	void play(Sound which);

	static const int RATE;
	static const Uint16 FORMAT;
	static const int CHANNELS;
	static const int BUFFER_SIZE;

	static const int BG_CHANNEL;

private:
	Mix_Chunk *_temp_bg_music;
};

#endif
