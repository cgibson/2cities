#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
	#include <winGL/glut.h>
#else
	#include <GL/glut.h>
#endif
#include "ComplexObject.h"
#include "../system/global.h"

using namespace global;

ComplexObject::ComplexObject(E_ModelType modelType)
{
	modeltype = modelType;
}

void ComplexObject::draw()
{
	glPushMatrix();
	{
		Vector scalevec = global::factory->getBlueprint(getType()).getSize();
		glTranslatef(position.x(), position.y(), position.z());
		glRotatef(orientation.getK(), orientation.getH(), orientation.getI(), orientation.getJ());
		glScalef(scalevec.x(), scalevec.y(), scalevec.z());
		glCallList(global::modelManager->getmodelDL(modeltype));
	}
	glPopMatrix();
}
