#ifndef DSETS_H
#define DSETS_H

#include <vector>
#include <queue>

class DisjointSets {

public:
	void addelements(int num);

	int find(int elem);

	void setunion(int a, int b);

	int numroots();

private:
	//int treesize(int a);

	//void testnode(int test, int* connected, std::queue<int>* queue);

	std::vector<int> sets;
};

#endif