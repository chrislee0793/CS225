#include "dsets.h"

using namespace std;

void DisjointSets::addelements(int num) {
	for(int i = 0; i < num; i++)
		sets.push_back(-1);
}

int DisjointSets::find(int elem) {
	if(sets[elem] <= -1)
		return elem;
	int end = find(sets[elem]);
	sets[elem] = end;
	return end;
}

void DisjointSets::setunion(int a, int b) {
	int aroot = find(a);
	int broot = find(b);
	if(aroot == broot)	//already in the same set
		return;
	if(sets[aroot] <= sets[broot]) {	//if node is root, it will contain the size of its tree * -1
		sets[aroot] = sets[broot] + sets[aroot];
		sets[broot] = aroot;
	}
	else {
		sets[broot] = sets[aroot] + sets[broot];
		sets[aroot] = broot;
	}
}

int DisjointSets::numroots() {
	int count = 0;
	for(int i = 0; i < sets.size(); i++)
		if(sets[i] < 0)
			count++;
	return count;
}