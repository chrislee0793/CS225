/**
 * @file heap.cpp
 * Implementation of a heap class.
 */
 
#include <algorithm>
#include <math.h>
#include <iostream>
 
template <class T, class Compare>
size_t heap<T, Compare>::root() const {
    /// @todo Update to return the index you are choosing to be your root.
    return 0;
}
 
template <class T, class Compare>
size_t heap<T, Compare>::leftChild( size_t i ) const {
        if(2*i + 1 > _elems.size()) return -1;
        return 2*i + 1;
}
 
template <class T, class Compare>
size_t heap<T, Compare>::rightChild( size_t i ) const {
        if(2*i + 2 > _elems.size()) return -1;
        return 2*i + 2;
}
 
template <class T, class Compare>
size_t heap<T, Compare>::parent( size_t i ) const {
        int is = floor((i-1)/2);
    return is;
}
 
template <class T, class Compare>
bool heap<T, Compare>::hasAChild( size_t currentIdx ) const {
    return _elems.size() - 1 >= currentIdx*2 + 1;
}
 
template <class T, class Compare>
size_t heap<T, Compare>::maxPriorityChild( size_t index ) const {
        if(!hasAChild(index)) return -1;
        if(rightChild(index) == -1) return leftChild(index);
        if(higherPriority(_elems[leftChild(index)], _elems[rightChild(index)])) return leftChild(index);
        return rightChild(index);
}
 
template <class T, class Compare>
void heap<T, Compare>::heapifyDown( size_t index ) {
        if(hasAChild(index)) {
                int mcx = maxPriorityChild(index);
                if(!higherPriority(_elems[index], _elems[mcx])) {
                        std::swap(_elems[index],_elems[mcx]);
                        heapifyDown(leftChild(index));
                        heapifyDown(rightChild(index));
                }
        }
}
 
template <class T, class Compare>
void heap<T, Compare>::heapifyUp( size_t currentIdx ) {
    if( currentIdx == root() )
        return;
    size_t parentIdx = parent( currentIdx );
    if( higherPriority( _elems[ currentIdx ], _elems[ parentIdx ] ) ) {
        std::swap( _elems[ currentIdx ], _elems[ parentIdx ] );
        heapifyUp( parentIdx );
    }
}
 
template <class T, class Compare>
heap<T, Compare>::heap() {
}
 
template <class T, class Compare>
heap<T, Compare>::heap( const std::vector<T> & elems ) {
        _elems = elems;
        for(int i = parent(_elems.size() - 1); i >= 0; i--) heapifyDown(i);
}
 
template <class T, class Compare>
T heap<T, Compare>::pop() {
        T min = _elems[0];
        _elems[0] = _elems[_elems.size() - 1];
        _elems.pop_back();
        if(_elems.size() == 0) return min;
        heapifyDown(0);
    return min;
}
 
template <class T, class Compare>
T heap<T, Compare>::peek() const {
    return _elems[0];
}
 
template <class T, class Compare>
void heap<T, Compare>::push( const T & elem ) {
    _elems.push_back(elem);
        heapifyUp(_elems.size() - 1);
}
 
template <class T, class Compare>
bool heap<T, Compare>::empty() const {
    /// @todo Determine if the heap is empty
    return _elems.size() == 0;
}