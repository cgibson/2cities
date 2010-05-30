#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
	#include <winGL/glut.h>
#else
	#include <GL/glut.h>
#endif
#include "ModelManager.h"
#include "../system/enum.h"

using namespace enumeration;

const char* modelFiles[] = {"models/drillcone.obj"};

//Create a new ModelManager.
ModelManager::ModelManager()
{
	resetLoader();
}

ModelManager::~ModelManager()
{
	for(int i = 0; i < NUM_GAME_MODELS; i++)
	{
		if(modelvertices[i] != NULL)
		{
			delete modelvertices[i];
		}
		if(modelnormals[i] != NULL)
		{
			delete modelnormals[i];
		}
		if(modelcolors[i] != NULL)
		{
			delete modelcolors[i];
		}
	}
}

//Reads in all data from the .obj model files.
void ModelManager::initialize()
{
	//read in each model
	for(int i = 0; i < NUM_GAME_MODELS; i++)
	{
		cout << "Attempting to read model" << modelFiles[i] << endl;
		loader::readFile(modelFiles[i]);
		modelvertices[i] = mdl::vertexarray;
		modelnormals[i] = mdl::normalarray;
		modelcolors[i] = mdl::colorarray;
		modelfaces[i] = mdl::totalfaces;
		modelDLs[i] = glGenLists(i);
		glNewList(modelDLs[i], GL_COMPILE);
		{				
				glEnableClientState(GL_NORMAL_ARRAY);
				glEnableClientState(GL_COLOR_ARRAY);
				glEnableClientState(GL_VERTEX_ARRAY);
				glNormalPointer(GL_FLOAT, 0, getmodelvertices((enumeration::ModelType)i));
				glColorPointer(3, GL_FLOAT, 0, getmodelcolors((enumeration::ModelType)i));
				glVertexPointer(3, GL_FLOAT, 0, getmodelnormals((enumeration::ModelType)i));
				glPushMatrix();
				{
					glDrawArrays(GL_TRIANGLES, 0, getmodelfaces((enumeration::ModelType)i) * 3);
				}
				glPopMatrix();
				glDisableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_COLOR_ARRAY);
				glDisableClientState(GL_NORMAL_ARRAY);
		}
		glEndList();
		resetLoader();
	}
}

void ModelManager::resetLoader()
{
	mdl::Vertices.clear();
	mdl::Normals.clear();
	mdl::vertexarray = NULL;
	mdl::normalarray = NULL;
	mdl::colorarray = NULL;
	mdl::totalfaces = 0;
	
}
