#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

class WorldObjectID {
	int id;
public:
	WorldObjectID(int nid) {id = nid;}
	int getID() {return id;}

};

void printSortDetails(vector<WorldObjectID *> *objVec, int locBeg, int locMid, int locEnd) {
	printf("bi%i(%i) mi%i(%i) ei%i(%i)\n",locBeg, (*objVec)[locBeg]->getID(),locMid, (*objVec)[locMid]->getID(),locEnd, (*objVec)[locEnd]->getID());
}

void updateObjectVector(vector<WorldObjectID *> *objVec, WorldObjectID *objPtr) {
	// *********************************************
    // Sorted, Linear Search Update Code
	int locBeg = 0;
	int locEnd = objVec->size() - 1;
	printf("size = %i\n", locEnd);
	int locMid = (locBeg + locEnd) / 2;
	printf("***Inserting %i\n",objPtr->getID());
	//printSortDetails(objVec, locBeg, locMid, locEnd);
	while(locBeg < locEnd) {
		// Check if found object
		if ((*objVec)[locMid]->getID() == objPtr->getID()) {
			locBeg = locMid;	// Found, break out!
			break;
		}
		// Must be in last half
		else if ((*objVec)[locMid]->getID() < objPtr->getID()) {
			locBeg = locMid + 1;
		}
		// Must be in first half
		else {
			locEnd = locMid - 1;
		}
		locMid = (locBeg + locEnd) / 2;
		printSortDetails(objVec, locBeg, locMid, locEnd);
	}
	if(objVec->size() != 0 && (*objVec)[locBeg]->getID() < objPtr->getID()) locBeg++;

	unsigned int i = locBeg;

	// Find Location in main Object vector
	//while (i < objVec->size() && (*objVec)[i]->getID() < objPtr->getID()) { i++; }

	// If at end, must not have found... push to back
	if (i == objVec->size()) {
		objVec->push_back(objPtr);
	}
	// if found, replace pointer with newer one
	else if((*objVec)[i]->getID() == objPtr->getID()) {
		// Check if incoming packet is newer data
//		if (objPtr->getTimeStamp() > (*objVec)[i]->getTimeStamp()) {
			WorldObjectID *oldObjPtr = (*objVec)[i];
			(*objVec)[i] = objPtr;
			delete oldObjPtr;
//		}
	}
	// Else, not found (but at insert location)
	else {
//		printf("new item after %i! ID=%i\n", (*it)->getID(), objPtr->getID());
		objVec->insert(objVec->begin() + i, objPtr);
	}
	// *********************************************
}

void printVec(vector<WorldObjectID *> *objVec) {
	printf("Items in Vector:");
	for(int i = 0; i < objVec->size(); i++)
		printf("%i ", (*objVec)[i]->getID());
	printf("\n");
}

int main(int argc, char *argv[]) {
	vector<WorldObjectID *> testVec;
	//testVec.push_back(new WorldObjectID(11));
	//printVec(&testVec);
	for(int i=0; i<5; i++) {
		updateObjectVector(&testVec, new WorldObjectID(i));
		printVec(&testVec);
	}
	for(int i=0; i<5; i++) {
		updateObjectVector(&testVec, new WorldObjectID(i));
		printVec(&testVec);
	}
}
