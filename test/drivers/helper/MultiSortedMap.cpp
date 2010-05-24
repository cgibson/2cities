#include "../../../src/helper/MultiSortedMap.h"

class testClass {
public:
	int item1;
	int item2;
	int item3;

	testClass() {item1 = item2 = item3 = 0;}
	testClass(int i1, int i2, int i3) {
		item1 = i1;
		item2 = i2;
		item3 = i3;
	}
	testClass(const testClass& newItem) {
		item1 = newItem.item1;
		item2 = newItem.item2;
		item3 = newItem.item3;
	}
};

bool testClass_sort_i1 (testClass tc1, testClass tc2) {
	return (tc1.item1 < tc2.item1);
}

bool testClass_sort_i2 (testClass tc1, testClass tc2) {
	return (tc1.item2 < tc2.item2);
}

bool testClass_sort_i3 (testClass tc1, testClass tc2) {
	return (tc1.item3 < tc2.item3);
}

bool float_sort_rev(float item1, float item2) {
	return item1 > item2;
}

bool float_sort_fwd(float item1, float item2) {
	return item1 < item2;
}

int main(int argc, char *argv[]) {
	printf("Hey, you opened!\n");
	int r;

	printf("\n\n***FLOAT OBJECTS***\n\n");
	MultiSortedMap<float> f1;
	r = f1.addSorter(float_sort_rev);
	printf("added sorter %i\n",r);
	r = f1.addSorter(float_sort_fwd);
	printf("added sorter %i\n",r);

	f1.update(1, 1.0f);
	for (int i=0; i<5; i++) {
		if(f1.found(i))
			printf("index %i = %f\n",i, f1.find(i));
	}
	f1.update(4, 2.2f);
	for (int i=0; i<5; i++) {
		if(f1.found(i))
			printf("index %i = %f\n",i, f1.find(i));
	}
	f1.update(2, 3.1f);

	for (int i=0; i<5; i++) {
		if(f1.found(i))
			printf("index %i = %f\n",i, f1.find(i));
	}

	printf("\n**** TESTING VECTOR ****\n");
	vector<float> floatVector = f1.getVector();
	for(int i=0;i<floatVector.size();i++)
		printf("vector has %f at %i\n", floatVector[i], i);

	printf("\n**** TESTING SORTED LISTS ****\n");
	list<float>::iterator it;
	list<float> floatList;

	floatList = f1[0];
	printf("Reverse Sorted List");
	printf("sizeof floatList = %i\n",floatList.size());
	for (it=floatList.begin();it != floatList.end(); it++) {
		printf("index = %f\n", *it);
	}

	floatList = f1[1];
	printf("Forward Sorted List");
	printf("sizeof floatList = %i\n",floatList.size());
	for (it=floatList.begin();it != floatList.end(); it++) {
		printf("index = %f\n", *it);
	}

	printf("\n**** TESTING UPDATE/SORTED LISTS ****\n");

	f1.update(4, 10.1f);

	floatList = f1[0];
	printf("Reverse Sorted List");
	printf("sizeof floatList = %i\n",floatList.size());
	for (it=floatList.begin();it != floatList.end(); it++) {
		printf("index = %f\n", *it);
	}

	floatList = f1[1];
	printf("Forward Sorted List");
	printf("sizeof floatList = %i\n",floatList.size());
	for (it=floatList.begin();it != floatList.end(); it++) {
		printf("index = %f\n", *it);
	}

	printf("\n\n***TESTCLASS OBJECTS***\n\n");
	MultiSortedMap<testClass> tc1;
	r = tc1.addSorter(testClass_sort_i1);
	printf("added sorter %i\n",r);
	r = tc1.addSorter(testClass_sort_i2);
	printf("added sorter %i\n",r);
	r = tc1.addSorter(testClass_sort_i3);
	printf("added sorter %i\n",r);

	tc1.update(0, testClass(1,10, 13));
	tc1.update(1, testClass(2, 9, 12));
	tc1.update(2, testClass(3, 8, 15));
	tc1.update(3, testClass(4, 7, 11));
	tc1.update(4, testClass(5, 6, 14));
	for (int i=0; i<5; i++) {
		if(tc1.found(i)) {
			testClass tmpObj = tc1.find(i);
			printf("index %i = <%i, %i, %i>\n",i, tmpObj.item1, tmpObj.item2, tmpObj.item3);
		}
	}
	printf("\n**** TESTCLASS SORTED LISTS ****\n");
	list<testClass>::iterator it2;
	list<testClass> testList;

	testList = tc1[0];
	printf("Item1 Sorted List");
	printf("sizeof testList = %i\n",testList.size());
	for (it2=testList.begin();it2 != testList.end(); it2++) {
		testClass tmpObj = *it2;
		printf("<%i, %i, %i>\n", tmpObj.item1, tmpObj.item2, tmpObj.item3);
	}

	testList = tc1[1];
	printf("Item2 Sorted List");
	printf("sizeof testList = %i\n",testList.size());
	for (it2=testList.begin();it2 != testList.end(); it2++) {
		testClass tmpObj = *it2;
		printf("<%i, %i, %i>\n", tmpObj.item1, tmpObj.item2, tmpObj.item3);
	}

	testList = tc1[2];
	printf("Item3 Sorted List");
	printf("sizeof testList = %i\n",testList.size());
	for (it2=testList.begin();it2 != testList.end(); it2++) {
		testClass tmpObj = *it2;
		printf("<%i, %i, %i>\n", tmpObj.item1, tmpObj.item2, tmpObj.item3);
	}

	printf("\n**** TESTCLASS UPDATE/SORTED LISTS ****\n");

	tc1.update(4, testClass(0, 0, 0));
	for (int i=0; i<5; i++) {
		if(tc1.found(i)) {
			testClass tmpObj = tc1.find(i);
			printf("index %i = <%i, %i, %i>\n",i, tmpObj.item1, tmpObj.item2, tmpObj.item3);
		}
	}

	testList = tc1[0];
	printf("Item1 Sorted List");
	printf("sizeof testList = %i\n",testList.size());
	for (it2=testList.begin();it2 != testList.end(); it2++) {
		testClass tmpObj = *it2;
		printf("<%i, %i, %i>\n", tmpObj.item1, tmpObj.item2, tmpObj.item3);
	}

	testList = tc1[1];
	printf("Item2 Sorted List");
	printf("sizeof testList = %i\n",testList.size());
	for (it2=testList.begin();it2 != testList.end(); it2++) {
		testClass tmpObj = *it2;
		printf("<%i, %i, %i>\n", tmpObj.item1, tmpObj.item2, tmpObj.item3);
	}

	testList = tc1[2];
	printf("Item3 Sorted List");
	printf("sizeof testList = %i\n",testList.size());
	for (it2=testList.begin();it2 != testList.end(); it2++) {
		testClass tmpObj = *it2;
		printf("<%i, %i, %i>\n", tmpObj.item1, tmpObj.item2, tmpObj.item3);
	}

	printf("\n**** TESTING VECTOR ****\n");
	vector<testClass> testVector = tc1.getVector();
	for(int i=0;i<testVector.size();i++)
		printf("<%i, %i, %i> at %i\n", testVector[i].item1, testVector[i].item2, testVector[i].item3, i);

	tc1.remove(0);
	printf("\n**** TESTING VECTOR AFTER DELETE ****\n");
	testVector = tc1.getVector();
	for(int i=0;i<testVector.size();i++)
		printf("<%i, %i, %i> at %i\n", testVector[i].item1, testVector[i].item2, testVector[i].item3, i);

	printf("\n\nGoodbye!\n");
	return 0;
}
