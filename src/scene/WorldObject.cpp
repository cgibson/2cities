#include "WorldObject.h"
#include "../system/global.h"
#include "../graphics/graphics.h"
#include "stdio.h"

void WorldObject::draw() {
	Vector pos = getPosition();
	Blueprint blueprint = global::factory->getBlueprint(type);
	
	gfx::materials[blueprint.getMaterial()].applyMaterial();
	
	//gfx::materials[YELLOW_MAT].applyMaterial();
	
	//printf("THIS: %d\nTYPE: %d\nDUMMY_SPHERE: %d\n", type, blueprint.getType(), enumeration::DUMMY_SPHERE);
	
	Vector size = blueprint.getSize();

  //printf("size: %f %f %f\n", size.x(), size.y(), size.z());

  glPushMatrix();
  glTranslatef(pos.x(), pos.y(), pos.z());
  
	switch(blueprint.getShape())
	{
	  case SMALL_CUBE:
	    glScalef(size.x(), size.y(), size.z());
	    glutSolidCube(1);
	    break;
	  case SMALL_SPHERE: default:
      glutSolidSphere(size.mag(),10,10);
      break;
    
  }
  
  glPopMatrix();
}

void WorldObject::update(int elapsedTime) {
/*
	Vector pos = getPosition();
	Vector vel = getVelocity();
	Vector acc = getForce();
	vel = vel + (acc * (elapsedTime/1000.0f));
	pos = pos + (vel * (elapsedTime/1000.0f));

	if(pos.y() > 0) {
		setPosition(pos);
		setVelocity(vel);
	}
	else {
		setPosition(Vector(pos.x(),0,pos.z()));
		setVelocity(Vector(0,0,0));
	}
*/
}

void WorldObject::think(int elapsedTime) {

}
