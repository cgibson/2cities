#include "SoundManager.h"
#include "sound.h"

using namespace std;
using namespace sound;

SoundManager::SoundManager()
{
	memset(buildmusic, 0, sizeof(Mix_Music *) * NUM_BUILD_SONGS);
	memset(carnagemusic, 0, sizeof(Mix_Music *) * NUM_CARNAGE_SONGS);
	memset(menumusic, 0, sizeof(Mix_Music *) * NUM_MENU_SONGS);
	memset(resultsmusic, 0, sizeof(Mix_Music *) * NUM_RESULTS_SONGS);
	memset(buildsfx, 0, sizeof(Mix_Chunk *) * NUM_BUILD_SFX);
	memset(carnagesfx, 0, sizeof(Mix_Chunk *) * NUM_CARNAGE_SFX);
	memset(uikeysfx, 0, sizeof(Mix_Chunk *) * NUM_UI_KEY_SFX);
	curruichannel = 0;
	currbuildmusic = 0;
	currcarnagemusic = drand48() * (NUM_CARNAGE_SONGS - 1);
	currbuildsfx = 0;
	currcarnagesfx = 0;
}

bool SoundManager::hasEnding(std::string const &musicFile, std::string const &extension)
{
	if (musicFile.length() > extension.length())
	{
		return (musicFile.compare(musicFile.length() -
		extension.length(), extension.length(), extension) == 0);
	}
	return false;
}

void SoundManager::initialize()
{
	//Initialize SDL. Although this may already be done in the main initialize() function.
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		cerr << Mix_GetError() << endl;
		exit(EXIT_FAILURE);
	}
	//Initialize SDL_mixer
	if (Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 512) != 0)
	{
		cerr << Mix_GetError() << endl;
		exit(EXIT_FAILURE);
	}
	loadMusic();
	loadSfx();
	for(int i = 0; i < MAX_KEY_CHANNEL; i++)
	{
		Mix_Volume(i, UI_KEY_VOLUME);
	}
}

void SoundManager::tearDownSound()
{
	for(int i = 0; i < NUM_BUILD_SONGS; i++)
    {
		if(buildmusic[i] != NULL)
		{
			Mix_FreeMusic(buildmusic[i]);
		}
	}
	for(int i = 0; i < NUM_CARNAGE_SONGS; i++)
    {
		if(carnagemusic[i] != NULL)
		{
			Mix_FreeMusic(carnagemusic[i]);
		}
	}
	for(int i = 0; i < NUM_BUILD_SFX; i++)
    {
		if(buildsfx[i] != NULL)
		{
			Mix_FreeChunk(buildsfx[i]);
		}
	}
	for(int i = 0; i < NUM_CARNAGE_SFX; i++)
    {
		if(carnagesfx[i] != NULL)
		{
			Mix_FreeChunk(carnagesfx[i]);
		}
	}
	for(int i = 0; i < NUM_UI_KEY_SFX; i++)
    {
		if(uikeysfx[i] != NULL)
		{
			Mix_FreeChunk(uikeysfx[i]);
		}
	}
    Mix_CloseAudio();
}

void SoundManager::loadMusic()
{	
	//Attempt to read in build state music
	for(int i = 0; i < NUM_BUILD_SONGS; i++)
	{
		if(hasEnding(sound::buildmusicnames[i], ".ogg"))
		{
			buildmusic[i] = Mix_LoadMUS(sound::buildmusicnames[i]);
		}
		else
		{
			cerr << "FATAL ERROR: music file" << sound::buildmusicnames[i] << "should be .wav format" << endl;
			exit(EXIT_FAILURE);
		}
		
		if(buildmusic[i] == NULL)
		{
			cerr << Mix_GetError() << endl;
			exit(EXIT_FAILURE);
		}
	}
	//Attempt to read in carnage state music
	for(int i = 0; i < NUM_CARNAGE_SONGS; i++)
	{
		if(hasEnding(sound::carnagemusicnames[i], ".ogg"))
		{
			carnagemusic[i] = Mix_LoadMUS(sound::carnagemusicnames[i]);
		}
		else
		{
			cout << "music file" << sound::carnagemusicnames[i] << "should be .wav format" << endl;
			exit(EXIT_FAILURE);
		}
		
		if(carnagemusic[i] == NULL)
		{
			cerr << Mix_GetError() << endl;
			exit(EXIT_FAILURE);
		}
	}
	
	//Attempt to read in menu state music
	for(int i = 0; i < NUM_MENU_SONGS; i++)
	{
		if(hasEnding(sound::menumusicnames[i], ".ogg"))
		{
			menumusic[i] = Mix_LoadMUS(sound::menumusicnames[i]);
		}
		else
		{
			cout << "music file" << sound::menumusicnames[i] << "should be .wav format" << endl;
			exit(EXIT_FAILURE);
		}
		
		if(menumusic[i] == NULL)
		{
			cerr << Mix_GetError() << endl;
			exit(EXIT_FAILURE);
		}
	}
	
	//Attempt to read in menu state music
	for(int i = 0; i < NUM_RESULTS_SONGS; i++)
	{
		if(hasEnding(sound::resultsmusicnames[i], ".ogg"))
		{
			resultsmusic[i] = Mix_LoadMUS(sound::resultsmusicnames[i]);
		}
		else
		{
			cout << "music file" << sound::resultsmusicnames[i] << "should be .wav format" << endl;
			exit(EXIT_FAILURE);
		}
		
		if(resultsmusic[i] == NULL)
		{
			cerr << Mix_GetError() << endl;
			exit(EXIT_FAILURE);
		}
	}
}

void SoundManager::loadSfx()
{
	//Attempt to read in carnage SFX
	for(int i = 0; i < NUM_CARNAGE_SFX; i++)
	{
		if(hasEnding(carnagesfxnames[i], ".wav"))
		{
			carnagesfx[i] = Mix_LoadWAV(carnagesfxnames[i]);
		}
		else
		{
			cerr << "sound effect file should be .wav" << endl;
			exit(EXIT_FAILURE);
		}
		
		if(carnagesfx[i] == NULL)
		{
			cerr << Mix_GetError() << endl;
			exit(EXIT_FAILURE);
		}
	}
	
	//Attempt to read in ui key SFX
	for(int i = 0; i < NUM_UI_KEY_SFX; i++)
	{
		if(hasEnding(uikeysfxnames[i], ".wav"))
		{
			uikeysfx[i] = Mix_LoadWAV(uikeysfxnames[i]);
		}
		else
		{
			cerr << "sound effect file should be .wav" << endl;
			exit(EXIT_FAILURE);
		}
		
		if(uikeysfx[i] == NULL)
		{
			cerr << Mix_GetError() << endl;
			exit(EXIT_FAILURE);
		}
	}
}

void SoundManager::playBuildSong()
{
	//Attempt to play the song
	if (Mix_PlayMusic(buildmusic[currbuildmusic], -1) == -1)
	{
		cerr << Mix_GetError() << endl;
		exit(EXIT_FAILURE);
	}
}

void SoundManager::playCarnageSong()
{
	//Attempt to play the song
	if (Mix_PlayMusic(carnagemusic[currcarnagemusic], -1) == -1)
	{
		cerr << Mix_GetError() << endl;
		exit(EXIT_FAILURE);
	}
}

void SoundManager::playMenuSong()
{
	//Attempt to play the song
	if (Mix_PlayMusic(menumusic[0], -1) == -1)
	{
		cerr << Mix_GetError() << endl;
		exit(EXIT_FAILURE);
	}
}
void SoundManager::playResultWinSong()
{
	//Attempt to play the song
	if (Mix_PlayMusic(resultsmusic[0], -1) == -1)
	{
		cerr << Mix_GetError() << endl;
		exit(EXIT_FAILURE);
	}
}
void SoundManager::playResultLoseSong()
{
	//Attempt to play the song
	if (Mix_PlayMusic(resultsmusic[1], -1) == -1)
	{
		cerr << Mix_GetError() << endl;
		exit(EXIT_FAILURE);
	}
}

void SoundManager::playNextBuildSong()
{
	stopPlayingMusic();
	currbuildmusic = (currbuildmusic + 1) % NUM_BUILD_SONGS;
	playBuildSong();
}

void SoundManager::playNextCarnageSong()
{
	stopPlayingMusic();
	currcarnagemusic = (currcarnagemusic + 1) % NUM_CARNAGE_SONGS;
	playCarnageSong();
}

void SoundManager::stopPlayingMusic()
{
	//fades and waits for fade to complete
	while(!Mix_FadeOutMusic(FADE_OUT_MSECS) && Mix_PlayingMusic())
	{
		SDL_Delay(SDL_DELAY_TIME);
	}
}

void SoundManager::playBuildSfx(int buildsfxnum)
{
	if(Mix_PlayChannel(SFX_CHANNEL, buildsfx[buildsfxnum], 0) == -1)
	{
		cerr << Mix_GetError() << endl;
		exit(EXIT_FAILURE);
	}
}

void SoundManager::playCarnageSfx(int carnagesfxnum)
{
	if(Mix_PlayChannel(SFX_CHANNEL, carnagesfx[carnagesfxnum], 0) == -1)
	{
		cerr << Mix_GetError() << endl;
		exit(EXIT_FAILURE);
	}
}

void SoundManager::playUIKey()
{
	int randkey = (drand48() * NUM_UI_KEY_SFX);
	if(Mix_PlayChannel(curruichannel, uikeysfx[randkey], 0) == -1)
	{
		cerr << Mix_GetError() << endl;
		exit(EXIT_FAILURE);
	}
	curruichannel = (curruichannel + 1) % MAX_KEY_CHANNEL;
}

int SoundManager::getCurrBuildMusic()
{
	return currbuildmusic;
}

int SoundManager::getCurrCarnageMusic()
{
	return currcarnagemusic;
}

int SoundManager::getCurrBuildSfx()
{
	return currbuildsfx;
}

int SoundManager::getCurrCarnageSfx()
{
	return currcarnagesfx;
}

void SoundManager::setCurrBuildMusic(int newcurrbuildmusic)
{
	currbuildmusic = newcurrbuildmusic;
}

void SoundManager::setCurrCarnageMusic(int newcurrcarnagemusic)
{
	currcarnagemusic = newcurrcarnagemusic;
}

void SoundManager::setCurrBuildSfx(int newcurrbuildsfx)
{
	currbuildsfx = newcurrbuildsfx;
}

void SoundManager::setCurrCarnageSfx(int newcurrcarnagesfx)
{
	currcarnagesfx = newcurrcarnagesfx;
}
