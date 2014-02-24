/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */
 
#include <math.h>
#include <complex>
#include <algorithm>
template<int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim> & first, const Point<Dim> & second, int curDim) const
{
 
        if(first[curDim] < second[curDim]) return true;
        if(first[curDim] > second[curDim]) return false;
        return first < second;
}
 
 
template<int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim> & target, const Point<Dim> & currentBest, const Point<Dim> & potential) const
{
        if(distance(target, currentBest) < distance(target, potential)) {
                return false;
        } else if(distance(target, currentBest) > distance(target, potential)) {
                return true;
        }
    return potential < currentBest;
}
 
template<int Dim>
int KDTree<Dim>::distance(const Point<Dim> & first, const Point<Dim> & second) const {
        int total = 0;
        for(int i = 0; i < Dim; i++) {
                total += pow(first[i] - second[i], 2);
        }
        return sqrt(total);
}
 
template<int Dim>
KDTree<Dim>::KDTree(const vector< Point<Dim> > & newPoints) {
        if(newPoints.size() == 0) return;
        vector< Point<Dim> > list(newPoints);
        buildTree(list, 0);
}
 
template<int Dim>
void KDTree<Dim>::buildTree(vector< Point<Dim> > & newPoints, int axis) {
        if(newPoints.size() == 0) return;
        int nextAxis = (axis + 1) % Dim;
        Point<Dim> midpt = median(newPoints, axis);
        vector< Point<Dim> > before;
        vector< Point<Dim> > after;
        bool flipflop = false;
        for(int i = 0; i < newPoints.size(); i++) {
                if(midpt == newPoints[i]) {
                        flipflop = true;
                } else if(flipflop) {
                        after.push_back(newPoints[i]);
                } else {
                        before.push_back(newPoints[i]);
                }
        }
        buildTree(before, nextAxis);
        points.push_back(midpt);
        buildTree(after, nextAxis);
                       
}
 
template<int Dim>
Point<Dim> KDTree<Dim>::median(vector< Point<Dim> > & newPoints, int axis) {
        int n = ceil((newPoints.size() - 1)/2.0);
        if(newPoints.size() % 2 == 0) n--;
        return select(newPoints, 0, newPoints.size() - 1, n + 1, axis);
}
 
template<int Dim>
int KDTree<Dim>::partition(vector< Point<Dim> > & list, int left, int right, int pivotIndex, int axis){
        Point<Dim> pivotVal = list[pivotIndex];
        list[pivotIndex] = list[right];
        list[right] = pivotVal;
        int storeIndex = left;
        for(int i = left; i < right; i++) {
                if(smallerDimVal(list[i], pivotVal, axis)) {
                        Point<Dim> curr = list[storeIndex];
                        list[storeIndex] = list[i];
                        list[i] = curr;
                        storeIndex++;
                }
        }
 
        Point<Dim> curr = list[storeIndex];
        list[storeIndex] = list[right];
        list[right] = curr;
        return storeIndex;
}
 
template<int Dim>
Point<Dim> KDTree<Dim>::select(vector< Point<Dim> > & list, int left, int right, int k, int axis) {
        if(left == right) return list[left];
        int newPivot = partition(list, left, right, (left + right)/2, axis);
        int pivotDistance = newPivot - left + 1;
        if(pivotDistance == k) {
                return list[newPivot];
        } else if(k < pivotDistance) {
                return select(list, left, newPivot - 1, k, axis);
        } else {
                return select(list, newPivot + 1, right, k - pivotDistance, axis);
        }
}
template<int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(int low, int high, int axis, const Point<Dim> & query) const {
        if(low >= high) return points[low];
        int midPt = floor((double)(low + high)/2);
        int newAxis = (axis + 1) % Dim;
        Point<Dim> current;
        bool lowSearch = true;
        if(smallerDimVal(points[midPt], query, axis)) {
                current = findNearestNeighbor(midPt + 1, high, newAxis, query);
                lowSearch = false;
        } else {
                current = findNearestNeighbor(low, midPt - 1, newAxis, query);
        }
 
        if(shouldReplace(query, current, points[midPt])) current = points[midPt];
 
        if(abs(points[midPt][axis] - query[axis]) <= abs(current[axis] - query[axis])) {
                Point<Dim> potential;
                if(lowSearch) {
                        potential = findNearestNeighbor(midPt + 1, high, newAxis, query);
                } else {
                        potential = findNearestNeighbor(low, midPt - 1, newAxis, query);
                }
               
                if(shouldReplace(query, current, potential)) return potential;
        }
 
        return current;
}
 
template<int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim> & query) const {
 
   return findNearestNeighbor(0, points.size() - 1, 0, query);
}