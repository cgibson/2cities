#include "SoundManager.h"

const int SoundManager::RATE = 22050;
const Uint16 SoundManager::FORMAT = AUDIO_S16SYS;
const int SoundManager::CHANNELS = 2;
const int SoundManager::BUFFER_SIZE = 4096;

const int SoundManager::BG_CHANNEL = 0;

SoundManager::SoundManager()
{
	// nothing yet
}

SoundManager::~SoundManager()
{
	// nothing yet
}

void SoundManager::init()
{
	// initialize SDL_mixer
	if (Mix_OpenAudio(RATE, FORMAT, CHANNELS, BUFFER_SIZE) != 0)
	{
		fprintf(stderr, "Oh noes! Unable to initialize SDL_mixer: %s\n", Mix_GetError());
		exit(EXIT_FAILURE);
	}
	Mix_ReserveChannels(2);

	// load sounds
	_temp_bg_music = Mix_LoadWAV("resources/sound/tempbgmusic.ogg");
	if (_temp_bg_music == 0)
	{
		fprintf(stderr, "Failed to load resources/sound/tempbgmusic.ogg\n");
		fprintf(stderr, "\t%s\n", Mix_GetError());
		exit(EXIT_FAILURE);
	}
}

void SoundManager::play(Sound which)
{
	switch (which)
	{
		case TEMP_BACKGROUND_MUSIC:
			if (Mix_Playing(BG_CHANNEL)) Mix_HaltChannel(BG_CHANNEL);
			Mix_PlayChannel(BG_CHANNEL, _temp_bg_music, 0);
			break;

		default:
			break;
	}
}
