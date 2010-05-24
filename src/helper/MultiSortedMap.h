#ifndef _MULTISORTEDMAP_H_
#define _MULTISORTEDMAP_H_

#define MULTISORTEDMAP_SIZE 5

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <list>

using namespace std;

template<class T> class MultiSortedMap_SortList;

/***********************************
 *      MAIN DATA STRUCTURE
 **********************************/
template <class T>
class MultiSortedMap {
private:
	map<int, T> DataMap;       // Used for key/value lookup
	unsigned int SortSize;

	T lastUpdated;

	// Array to store sorted pointer lists that refer to data items
	MultiSortedMap_SortList<T> SortLists[MULTISORTEDMAP_SIZE];

public:
	MultiSortedMap(unsigned int SortedListsSize = MULTISORTEDMAP_SIZE);
	~MultiSortedMap();

	int  addSorter(bool ( *newFunc ) ( T, T ));
	void removeSorter(int sorterID);

	void insert(int newObjID, T newObj);
	T    update(int newObjID, T newObj);

	T    remove(int ObjID);
	void clear();
	
	int  size() { return DataMap.size(); };

	T    find(int ObjID);
	bool found(int objID);

	T    getLast();

	vector<T> getVector();
	list<T> getList();

	list<T> operator[](int SortListIndex);
};

/***********************************
 *      HELPER DATA STRUCTURE
 **********************************/
template<class T>
class MultiSortedMap_SortList {
private:
	list<int> DataIndex;    // sort is stored as

public:
	map<int, T> *dataPtr;   // Callback pointers to data source
	bool ( *SorterFunc ) ( T, T );

	MultiSortedMap_SortList();
	MultiSortedMap_SortList(map<int, T> *newDataPtr, bool ( *newSorterFunc ) ( T, T ));

	// Function that redirects int sort to desired T type SorterFunc
	bool SorterFuncRedirect(int item1, int item2);

	void addObject(int objKey);
	void removeObject(int objKey);
	void clear();

	// iterator through and add items to list.
	list<T> getList();
};

/***********************************
 *  MAIN DATA STRUCTURE FUNCTIONS
 **********************************/
template <class T>
inline MultiSortedMap<T>::MultiSortedMap(unsigned int SortedListsSize) {
	SortSize = MULTISORTEDMAP_SIZE;
	// SorterList takes care of its defaults

	printf("MultiSortedMap Created\n");
}

template <class T>
inline MultiSortedMap<T>::~MultiSortedMap() {}

template <class T>
inline int MultiSortedMap<T>::addSorter(bool ( *newFunc ) ( T, T )) {
	// find first open sorter location
	int i=0;
	while(i < SortSize && SortLists[i].SorterFunc != NULL) i++;

//	printf("SortSize is %i and stopped at index %i\n",SortSize, i);
	// check if no more room
	if(i == SortSize)
		return -1;

	SortLists[i].SorterFunc = newFunc;
	SortLists[i].dataPtr = &DataMap;

	printf("Added New SorterFunc %i\n", i);
	return i;
}

template <class T>
inline void MultiSortedMap<T>::removeSorter(int sorterID) {
	SortLists[sorterID].clear();
}

template <class T>
inline void MultiSortedMap<T>::insert(int newObjID, T newObj) {
	update(newObjID, newObj);
}

// Will return the old copy (if it exists) so it can be free'd (if a pointer.)
// If it doesn't exist, 0 will be returned
template <class T>
inline T MultiSortedMap<T>::update(int newObjID, T newObj) {
	// check if newObj is in the data set (DataMap)
	typename map<int, T>::iterator it = DataMap.find(newObjID);
	T oldObj;

	// if not in DataMap, push_back & add ID to default DataMap
	if(it == DataMap.end()) {
//		printf("MultiSortedMap<T>::update> Adding Item #%i\n",newObjID);
		DataMap[newObjID] = newObj;
		oldObj = T();
	}
	// if in KeyMap, simply replace the item
	else {
//		printf("MultiSortedMap<T>::update> Replacing Item #%i\n",newObjID);
		oldObj = it->second;
		DataMap[newObjID] = newObj;

		// iterator through sorted lists and remove item
		for(int i=0; i < SortSize; i++) {
			SortLists[i].removeObject(newObjID);
		}
	}

	// iterator through sorted lists and insert
	for(int i=0; i < SortSize; i++) {
		SortLists[i].addObject(newObjID);
	}

	lastUpdated = newObj;
	return oldObj;
}

// Will return the old copy (if it exists) so it can be free'd (if a pointer.)
// If it doesn't exist, 0 will be returned
template <class T>
inline T MultiSortedMap<T>::remove(int ObjID) {
	// check if not in DataMap
	typename map<int, T>::iterator it = DataMap.find(ObjID);

	// if not, assume not in data and return
	if(it == DataMap.end())
		return T();

	T oldObj = (*it).second;

	// cycle SortedLists and remove entry
	for(int i=0; i < SortSize; i++) {
		SortLists[i].removeObject(ObjID);
	}

	// remove from DataMap
	DataMap.erase(ObjID);

	return oldObj;
}

// NOTE: assumes user has already iterated list and delete pointer items
template <class T>
inline void MultiSortedMap<T>::clear() {
	for(int i=0; i < SortSize; i++) {
		SortLists[i].clear();
	}

	DataMap.clear();
}

template <class T>
inline bool MultiSortedMap<T>::found(int ObjID) {
	// check if not in DataMap
	typename map<int, T>::iterator it = DataMap.find(ObjID);

	// if not, assume not in data and return
	if(it == DataMap.end())
		return false;
	else
		return true;
}

template <class T>
inline T MultiSortedMap<T>::find(int ObjID) {
	// check if not in DataMap
	typename map<int, T>::iterator it = DataMap.find(ObjID);

	// if not, assume not in data and return
	if(it == DataMap.end())
		return T();

	return it->second;
}

template <class T>
inline T MultiSortedMap<T>::getLast() {
	return lastUpdated;
}

template <class T>
inline vector<T> MultiSortedMap<T>::getVector() {
	vector<T> objVector;
	typename map<int, T>::iterator it;
	for(it=DataMap.begin(); it != DataMap.end(); it++) {
		objVector.push_back((*it).second);
	}
	return objVector;
}

template <class T>
inline list<T> MultiSortedMap<T>::getList() {
	list<T> objList;
	typename map<int, T>::iterator it;
	for(it=DataMap.begin(); it != DataMap.end(); it++) {
		objList.push_back((*it).second);
	}
	return objList;
}

template <class T>
inline list<T> MultiSortedMap<T>::operator[](int SortListIndex) {
//	printf("Attempting to retrieve list %i!\n", SortListIndex);
	if(SortListIndex < 0 || SortListIndex >= SortSize) {
		printf("SortListIndex out of bounds! Returning Default List!\n");
		exit(0);
		return getList();
	}
	else if(SortLists[SortListIndex].SorterFunc == NULL) {
		printf("No SorterFunc in place! Returning Default List!\n");
		exit(0);
		return getList();
	}
	else {
//		printf("Calling/Returning SortListIndex's getList!\n");
		return SortLists[SortListIndex].getList();
	}
}

/***********************************
 * HELPER DATA STRUCTURE FUNCTIONS
 * ********************************/
template <class T>
inline MultiSortedMap_SortList<T>::MultiSortedMap_SortList() {
	SorterFunc = NULL;
	dataPtr = NULL;
}

template <class T>
inline MultiSortedMap_SortList<T>::MultiSortedMap_SortList(map<int, T> * newDataPtr, bool ( *newSorterFunc ) ( T, T )) {
	dataPtr = newDataPtr;
	SorterFunc = newSorterFunc;

	// Insert all items already in dataPtr
	typename map<int, T>::iterator it;
	for(it=(*dataPtr).begin(); it != (*dataPtr).end(); it++) {
		addObject((*it).first);
	}
}

template <class T>
inline bool MultiSortedMap_SortList<T>::SorterFuncRedirect(int item1, int item2) {
	typename map<int, T>::iterator it1 = (*dataPtr).find(item1);
	typename map<int, T>::iterator it2 = (*dataPtr).find(item2);

	return SorterFunc(it1->second, it2->second);
}

template <class T>
inline void MultiSortedMap_SortList<T>::addObject(int objKey) {
	if(SorterFunc != NULL) {
		// TODO change Linear Location Finding to Binary Location
		list<int>::iterator it = DataIndex.begin();
		while(it != DataIndex.end() && SorterFuncRedirect((*it), objKey)) it++;

		if(it == DataIndex.begin()) {
			DataIndex.push_front(objKey);
		}
		else if(it == DataIndex.end()) {
			DataIndex.push_back(objKey);
		}
		else {
			DataIndex.insert(it, objKey);
		}
	}
}

template <class T>
inline void MultiSortedMap_SortList<T>::removeObject(int objKey) {
	if(SorterFunc != NULL) {
		DataIndex.remove(objKey);
	}
}

template <class T>
inline void MultiSortedMap_SortList<T>::clear() {
	DataIndex.clear();
//	SorterFunc = NULL;
}

template <class T>
inline list<T> MultiSortedMap_SortList<T>::getList() {
	list<T> objList;

	// Add objects to a list
	list<int>::iterator it;
	for(it=DataIndex.begin(); it != DataIndex.end(); it++) {
//		printf("item => %i => %f\n", *it, (*dataPtr)[*it]);
		objList.push_back((*dataPtr)[(*it)]);
	}

	return objList;
}

#endif
