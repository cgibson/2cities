#include "WorldObject.h"
#include "../system/global.h"
#include "stdio.h"

#ifdef CLIENT
	#include "../graphics/graphics.h"
#endif

void WorldObject::draw() {
#ifdef CLIENT
	Vector pos = getPosition();
	Quaternion ori = getOrientation();
	Blueprint blueprint = global::factory->getBlueprint(type);

	gfx::materials[blueprint.getMaterial()].applyMaterial();
	
	Vector size = blueprint.getSize();

	glPushMatrix();
	glTranslatef(pos.x(), pos.y(), pos.z());
	glRotatef(ori.getK(), ori.getH(), ori.getI(), ori.getJ());
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
#endif
}

void WorldObject::update(int elapsedTime) {

	Vector pos = getPosition();
	Vector vel = getVelocity();
	Vector acc = Vector(0,-8,0);

	vel = vel + (acc * (elapsedTime/1000.0f));
	pos = pos + (vel * (elapsedTime/1000.0f));

	if(pos.y() > 1) {
		setPosition(pos);
		setVelocity(vel);
	}
	else {
	}

}

void WorldObject::think(int elapsedTime) {

}

void WorldObject::print() {
/*	Items in WorldObjects (for reference)
 *
 * 	unsigned int id;
 * 	unsigned int playerid;
 * 	Vector position;
 * 	Vector velocity;
 * 	Vector force;
 * 	Quaternion orientation;
 * 	ObjectType type; //SMALL_SPHERE, MEDIUM_SPHERE, LARGE_SPHERE, SMALL_CUBE, etc
 * 	PhysicsState physics; //NONE, DYNAMIC, STATIC
 * 	uint64_t timestamp;
 */
    printf("***Object Details***\n"
    		" ID       : %i\n"
    		" PlayerID : %i\n"
    		" position : %s\n"
    		" velocity : %s\n"
    		" force    : %s\n"
    		"*******************\n",
    		id, playerid, position.str(), velocity.str(), force.str());
}
