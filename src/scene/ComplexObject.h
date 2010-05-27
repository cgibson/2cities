//Represents an object that has a model of its own, read in from
//a .obj file. Child of WorldObject.

#ifndef _COMPLEX_OBJECT_H_
#define _COMPLEX_OBJECT_H_

#include "WorldObject.h"

using namespace enumeration;

class ComplexObject:public WorldObject
{
	private:
		enumeration::ModelType modeltype;
		
	public:
		ComplexObject(enumeration::ModelType modeltype);
		~ComplexObject();
	
		void draw();
		ModelType getModelType() {return modeltype;}
};

#endif
