#include "WorldObject.h"
#include "../system/global.h"
#include "stdio.h"
#include "../system/enum.h"

using namespace enumeration;

#ifdef CLIENT
	#include "../graphics/graphics.h"
#endif

void WorldObject::draw() {
#ifdef CLIENT
//	Vector pos = getPosition();
//	Quaternion ori = getOrientation();
	
	Blueprint blueprint = global::factory->getBlueprint(type);
	Vector size = blueprint.getSize();
	glPushMatrix();
    glTranslatef(position.x(), position.y(), position.z());
    glRotatef(orientation.getK(), orientation.getH(),
              orientation.getI(), orientation.getJ());
    glScalef(size.x(), size.y(), size.z());
    Shape sh = blueprint.getShape();
    
    if (sh >= SHAPE_BOX_MIN && sh < SHAPE_BOX_MAX)
    {
      gfx::modelHandler.drawShadedCube(1.0);
    }
    else if (sh <= SHAPE_SPHERE_MIN && sh < SHAPE_SPHERE_MAX)
    {
      glutSolidSphere(1.0, 10, 10);
    }
    else if (sh == SMALL_CONE)
    {
      glTranslatef(0, 0, -0.5);
      glutSolidCone(1, 1, 10, 10);
    }
    else
    {
      printf("Unrecognized Shape value %d. Object unrendered\n", sh);
    }
	glPopMatrix();
#endif
}

void WorldObject::update(int elapsedTime) {
	if(global::interpolateObjects) {
		Vector pos = getPosition();
		Vector vel = getVelocity();
		Vector grav = Vector(0,-10,0);
		float timeFrac = (elapsedTime/1000.0f);

		if(vel.mag() > (90.0 * timeFrac)) {
			if(pos.y() > 1 && abs(vel.y()) > 70.0f * timeFrac)
				vel = vel + (grav * timeFrac);
			else
				vel.y(0.0f);

			pos = pos + (vel * timeFrac);

			setPosition(pos);
			setVelocity(vel);
		}
	}
}

void WorldObject::interpolate(int elapsedTime) {
	Vector pos = getPosition();
	Vector vel = getVelocity();
	Vector grav = Vector(0,-10,0);
	float timeFrac = (elapsedTime/1000.0f);

	if(pos.y() > 1)
		vel = vel + (grav * timeFrac);
	else
		vel.y(0.0f);

	pos = pos + (vel * timeFrac);

	setPosition(pos);
	setVelocity(vel);
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
 *
    printf("***Object Details***\n"
    		" ID       : %i\n"
    		" PlayerID : %i\n"
    		" position : %s\n"
    		" velocity : %s\n"
    		" force    : %s\n"
    		"*******************\n",
    		id, playerid, position.str(), velocity.str(), force.str());
    		*/
   printf("***Object Details***\n");
   printf(" ID       : %i\n", id);
   printf(" PlayerID : %i\n", playerid);
   printf(" position : %s\n", position.str());
   printf(" velocity : %s\n", velocity.str());
   printf(" force    : %s\n", force.str());
   printf(" orient   : %s\n", orientation.str());
   printf("*******************\n");
}

int WorldObject::makeBinStream(unsigned char *bufPtr) {
	int currPos = 0;
	int currISize;

	currISize = sizeof(ObjectType);
	memcpy(bufPtr + currPos, (void *)&type, currISize); currPos += currISize;

	currISize = sizeof(unsigned int);
	memcpy(bufPtr + currPos, (void *)&id       , currISize); currPos += currISize;
	memcpy(bufPtr + currPos, (void *)&playerid , currISize); currPos += currISize;
	memcpy(bufPtr + currPos, (void *)&bldgid   , currISize); currPos += currISize;

	double tmpD;
	currISize = sizeof(double);
	tmpD = position.x(); memcpy(bufPtr + currPos, &tmpD, currISize); currPos += currISize;
	tmpD = position.y(); memcpy(bufPtr + currPos, &tmpD, currISize); currPos += currISize;
	tmpD = position.z(); memcpy(bufPtr + currPos, &tmpD, currISize); currPos += currISize;

	tmpD = velocity.x(); memcpy(bufPtr + currPos, &tmpD, currISize); currPos += currISize;
	tmpD = velocity.y(); memcpy(bufPtr + currPos, &tmpD, currISize); currPos += currISize;
	tmpD = velocity.z(); memcpy(bufPtr + currPos, &tmpD, currISize); currPos += currISize;

	tmpD = force.x(); memcpy(bufPtr + currPos, &tmpD, currISize); currPos += currISize;
	tmpD = force.y(); memcpy(bufPtr + currPos, &tmpD, currISize); currPos += currISize;
	tmpD = force.z(); memcpy(bufPtr + currPos, &tmpD, currISize); currPos += currISize;

	float tmpF;
	currISize = sizeof(float);
	tmpF = orientation.getH(); memcpy(bufPtr + currPos, &tmpF, currISize); currPos += currISize;
	tmpF = orientation.getI(); memcpy(bufPtr + currPos, &tmpF, currISize); currPos += currISize;
	tmpF = orientation.getJ(); memcpy(bufPtr + currPos, &tmpF, currISize); currPos += currISize;
	tmpF = orientation.getK(); memcpy(bufPtr + currPos, &tmpF, currISize); currPos += currISize;

	currISize = sizeof(uint64_t);
	memcpy(bufPtr + currPos, (void *)&timestamp, currISize); currPos += currISize;

	return currPos;
}
int WorldObject::fromBinStream(unsigned char *bufPtr) {
	int currPos = 0;
	int currISize;

	currISize = sizeof(ObjectType);
	memcpy((void *)&type, bufPtr + currPos, currISize); currPos += currISize;

	currISize = sizeof(unsigned int);
	memcpy((void *)&id       , bufPtr + currPos, currISize); currPos += currISize;
	memcpy((void *)&playerid , bufPtr + currPos, currISize); currPos += currISize;
	memcpy((void *)&bldgid   , bufPtr + currPos, currISize); currPos += currISize;

	double tmpD;
	currISize = sizeof(double);
	memcpy(&tmpD , bufPtr + currPos, currISize); tmpD = position.x(); currPos += currISize;
	memcpy(&tmpD , bufPtr + currPos, currISize); tmpD = position.y(); currPos += currISize;
	memcpy(&tmpD , bufPtr + currPos, currISize); tmpD = position.z(); currPos += currISize;

	memcpy(&tmpD , bufPtr + currPos, currISize); tmpD = velocity.x(); currPos += currISize;
	memcpy(&tmpD , bufPtr + currPos, currISize); tmpD = velocity.y(); currPos += currISize;
	memcpy(&tmpD , bufPtr + currPos, currISize); tmpD = velocity.z(); currPos += currISize;

	memcpy(&tmpD , bufPtr + currPos, currISize); tmpD = force.x(); currPos += currISize;
	memcpy(&tmpD , bufPtr + currPos, currISize); tmpD = force.y(); currPos += currISize;
	memcpy(&tmpD , bufPtr + currPos, currISize); tmpD = force.z(); currPos += currISize;

	float tmpF;
	currISize = sizeof(float);
	memcpy(&tmpF, bufPtr + currPos, currISize); tmpF = orientation.getH(); currPos += currISize;
	memcpy(&tmpF, bufPtr + currPos, currISize); tmpF = orientation.getI(); currPos += currISize;
	memcpy(&tmpF, bufPtr + currPos, currISize); tmpF = orientation.getJ(); currPos += currISize;
	memcpy(&tmpF, bufPtr + currPos, currISize); tmpF = orientation.getK(); currPos += currISize;

	currISize = sizeof(uint64_t);
	memcpy((void *)&timestamp, bufPtr + currPos, currISize); currPos += currISize;

	return currPos;
}
