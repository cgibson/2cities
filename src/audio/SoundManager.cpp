#include "SoundManager.h"

using namespace std;

SoundManager::SoundManager()
{
	memset(buildmusic, 0, sizeof(Mix_Music *) * NUM_BUILD_SONGS);
	memset(carnagemusic, 0, sizeof(Mix_Music *) * NUM_CARNAGE_SONGS);
	memset(buildsfx, 0, sizeof(Mix_Chunk *) * NUM_BUILD_SFX);
	memset(carnagesfx, 0, sizeof(Mix_Chunk *) * NUM_CARNAGE_SFX);
	currbuildmusic = currcarnagemusic =
		currbuildsfx = currcarnagesfx = 0;
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
	//Initialize SDL
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		cerr << Mix_GetError() << endl;
		exit(EXIT_FAILURE);
	}
	//Initialize SDL_mixer
	if (Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096) != 0)
	{
		cerr << Mix_GetError() << endl;
		exit(EXIT_FAILURE);
	}
	loadMusic();
	loadSfx();
}

void SoundManager::tearDownSound()
{
	for(int i = 0; i < NUM_BUILD_SONGS; i++)
    {
		Mix_FreeMusic(buildmusic[i]);
	}
	for(int i = 0; i < NUM_CARNAGE_SONGS; i++)
    {
		Mix_FreeMusic(carnagemusic[i]);
	}
	for(int i = 0; i < NUM_BUILD_SFX; i++)
    {
		Mix_FreeChunk(buildsfx[i]);
	}
	for(int i = 0; i < NUM_CARNAGE_SFX; i++)
    {
		Mix_FreeChunk(carnagesfx[i]);
	}
    Mix_CloseAudio();
}

void SoundManager::loadMusic()
{
	char *buildmusicnames[NUM_BUILD_SONGS] = {
		"resources/sound/DesignTime.ogg"};
		
	char *carnagemusicnames[NUM_CARNAGE_SONGS] = {
	"resources/sound/HopeNeverDiesBrenner.ogg",
	"resources/sound/SupremeLogicianLin.ogg",
	"resources/sound/ProudSoldierGage.ogg",
	"resources/sound/HeroOfLegendForsythe.ogg",
	"resources/sound/FlightOfTheCowardWaylon.ogg"};
	
	//Attempt to read in build state music
	for(int i = 0; i < NUM_BUILD_SONGS; i++)
	{
		if(hasEnding(buildmusicnames[i], ".ogg"))
		{
			buildmusic[i] = Mix_LoadMUS(buildmusicnames[i]);
		}
		else
		{
			cout << "music file" << buildmusicnames[i] << "should be .wav format" << endl;
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
		if(hasEnding(carnagemusicnames[i], ".ogg"))
		{
			carnagemusic[i] = Mix_LoadMUS(carnagemusicnames[i]);
		}
		else
		{
			cout << "music file" << carnagemusicnames[i] << "should be .wav format" << endl;
			exit(EXIT_FAILURE);
		}
		
		if(carnagemusic[i] == NULL)
		{
			cerr << Mix_GetError() << endl;
			exit(EXIT_FAILURE);
		}
	}
}

void SoundManager::loadSfx()
{	
	//Not used for now.
	char *buildsfxnames[NUM_BUILD_SFX];
	
	char *carnagesfxnames[NUM_CARNAGE_SFX] = {
	"resources/sound/28911__junggle__btn101.wav",
	"resources/sound/39457__THE_bizniss__laser_3.wav",
	"resources/sound/73537__Snipperbes__ClassicLaser.wav"};
	
	//Attempt to read in build SFX
	for(int i = 0; i < NUM_BUILD_SFX; i++)
	{
		if(hasEnding(buildsfxnames[i], ".wav"))
		{
			buildsfx[i] = Mix_LoadWAV(buildsfxnames[i]);
		}
		else
		{
			cout << "sound effect file should be .wav" << endl;
			exit(EXIT_FAILURE);
		}
		
		if(buildsfx[i] == NULL)
		{
			cerr << Mix_GetError() << endl;
			exit(EXIT_FAILURE);
		}
	}
	//Attempt to read in carnage SFX
	for(int i = 0; i < NUM_CARNAGE_SFX; i++)
	{
		if(hasEnding(carnagesfxnames[i], ".wav"))
		{
			carnagesfx[i] = Mix_LoadWAV(carnagesfxnames[i]);
		}
		else
		{
			cout << "sound effect file should be .wav" << endl;
			exit(EXIT_FAILURE);
		}
		
		if(carnagesfx[i] == NULL)
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
