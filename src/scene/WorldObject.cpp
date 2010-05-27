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
	Vector size = blueprint.getSize();

	glPushMatrix();
	glTranslatef(pos.x(), pos.y(), pos.z());
	glRotatef(ori.getK(), ori.getH(), ori.getI(), ori.getJ());
	switch(blueprint.getShape())
	{
		case SMALL_CUBE:
			gfx::modelHandler.drawShadedCube(1);
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
	Vector grav = Vector(0,-10,0);

	if(pos.y() > 1 && abs(vel.y()) > 0.1f)
		vel = vel + (grav * (elapsedTime/1000.0f));
	else
		vel.y(0.0f);

	pos = pos + (vel * (elapsedTime/1000.0f));

	//printf("vel.mag=%f\n",vel.mag());
	if(vel.mag() > (20.0*elapsedTime/1000.0f)) {
		setPosition(pos);
		setVelocity(vel);
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

	currISize = sizeof(ObjectType);
	memcpy(bufPtr + currPos, (void *)&type, currISize); currPos += currISize;

	currISize = sizeof(uint64_t);
	memcpy(bufPtr + currPos, (void *)&timestamp, currISize); currPos += currISize;

	return currPos;
}
int WorldObject::fromBinStream(unsigned char *bufPtr) {
	int currPos = 0;
	int currISize;

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

	currISize = sizeof(ObjectType);
	memcpy((void *)&type, bufPtr + currPos, currISize); currPos += currISize;

	currISize = sizeof(uint64_t);
	memcpy((void *)&timestamp, bufPtr + currPos, currISize); currPos += currISize;

	return currPos;
}
