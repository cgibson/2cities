#ifndef _MODEL_MANAGER_H_
#define _MODEL_MANAGER_H_

#include "loader.h"
#include "../system/enum.h"

#define NUM_GAME_MODELS 7

using namespace mdl;
using namespace loader;
using namespace enumeration;

extern const char* modelFiles[];

//In charge of reading each model into the game and holding all models.
class ModelManager
{
	private:
		float *modelvertices[NUM_GAME_MODELS];
		float *modelnormals[NUM_GAME_MODELS];
		float *modelcolors[NUM_GAME_MODELS];
		int modelfaces[NUM_GAME_MODELS];
		GLuint modelDLs[NUM_GAME_MODELS];
		
	public:
		ModelManager();
		~ModelManager();
		
		void initialize();
		void resetLoader();
		float *getmodelvertices(enumeration::ModelType type) {return modelvertices[type];}
		float *getmodelnormals(enumeration::ModelType type) {return modelnormals[type];}
		float *getmodelcolors(enumeration::ModelType type) {return modelcolors[type];}
		int getmodelfaces(enumeration::ModelType type) {return modelfaces[type];}
		int getmodelDL(enumeration::ModelType type) {return modelDLs[type];}
};

#endif
