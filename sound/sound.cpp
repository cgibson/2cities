#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "SDL_mixer.h"
#include <iostream>
#include <string>

using namespace std;

//prototypes
void sigfunc(void);

//globals
Mix_Music* music = NULL;
string input = "";

void sigfunc(int sig)
{
	printf("Thanks for listening!\n");
	Mix_FreeMusic(music);
	Mix_CloseAudio();
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
	signal(SIGINT, sigfunc);
	if(argc != 2)
	{
		cout << "Format is ./program <sound_file>" << endl;
		exit(EXIT_FAILURE);
	}

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
	
	//Attempt to read in mp3
	music = Mix_LoadMUS(argv[1]);
	if(music == NULL)
	{
		cerr << Mix_GetError() << endl;
		exit(EXIT_FAILURE);
	}
	//Attempt to play the song
	if (Mix_PlayMusic(music, -1) == -1)
	{
		cerr << Mix_GetError() << endl;
		exit(EXIT_FAILURE);
	}
	printf("Playing %s\n(Press any key + Enter, or use ctrl+c to quit)\n", argv[1]);

	getline(cin, input);

    printf("Thanks for listening!\n");
	Mix_FreeMusic(music);
	Mix_CloseAudio();
	exit(EXIT_SUCCESS);
}
