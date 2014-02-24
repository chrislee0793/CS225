#include <iostream>
#include <cmath>
using namespace std;

/**
 * @file list.cpp
 * Doubly Linked List (MP 3).
 *
 * @author Chase Geigle
 * @date (created) Fall 2011
 * @date (modified) Spring 2012, Fall 2012
 *
 * @author Jack Toole
 * @date (modified) Fall 2011
 */

/**
 * Destroys the current List. This function should ensure that
 * memory does not leak on destruction of a list.
 */
template <class T>
List<T>::~List()
{
    /// @todo Graded in MP3.1
    clear();
}

/**
 * Destroys all dynamically allocated memory associated with the current
 * List class.
 */
template <class T>
void List<T>::clear()
{
    /// @todo Graded in MP3.1
    ListNode* next;
    while(head != NULL)
    {
        next = head->next;
        delete head;
        head = next;
    }
}

/**
 * Inserts a new node at the front of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <class T>
void List<T>::insertFront(T const & ndata)
{
    /// @todo Graded in MP3.1
    
    ListNode* newthing = new ListNode(ndata);
    newthing->next = head;
    newthing->prev = NULL;
    if(head != NULL)
        newthing->next->prev = newthing;
    else
    {
        tail = newthing;
        newthing->next = NULL;
    }
    head = newthing;
    length++;
    newthing = NULL;
}

/**
 * Inserts a new node at the back of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <class T>
void List<T>::insertBack( const T & ndata )
{
    /// @todo Graded in MP3.1

    ListNode* newthing = new ListNode(ndata);
    newthing->next = NULL;
    newthing->prev = tail;
    if(tail != NULL)
        newthing->prev->next = newthing;
    else
    {
        head = newthing;
        newthing->prev = NULL;
    }
    tail = newthing;
    length++;
    newthing = NULL;
}


/**
 * Reverses the current List.
 */
template <class T>
void List<T>::reverse()
{
    reverse(head, tail);
}

/**
 * Helper function to reverse a sequence of linked memory inside a List,
 * starting at startPoint and ending at endPoint. You are responsible for
 * updating startPoint and endPoint to point to the new starting and ending
 * points of the rearranged sequence of linked memory in question.
 *
 * @param startPoint A pointer reference to the first node in the sequence
 *  to be reversed.
 * @param endPoint A pointer reference to the last node in the sequence to
 *  be reversed.
 */
template <class T>
void List<T>::reverse( ListNode * & startPoint, ListNode * & endPoint )
{
    //check for stupid shit here
    //null pointers

    if(startPoint == NULL || endPoint == NULL)
        return;

    //seeing if the start is closer to the end than end

    int counter1 = 0;
    int counter2 = 0;

    ListNode* temp = startPoint;

    while(temp->next != NULL)
    {
        counter1++;
        temp = temp->next;
    }

    temp = endPoint;

    while(temp->next != NULL)
    {
        counter2++;
        temp = temp->next;
    }

    if(counter1 < counter2)
        return;
    
    ListNode* working = startPoint;

    while(working != endPoint)
    {
        //don't want to lose this
        temp = working->prev;
        //swap previous and next
        working->prev = working->next;
        working->next = temp;
        //move onto the next (previous?) one and do it again
        working = working->prev;
    }

    /*
    if(&startPoint == &head)    //need to change head reference if we moved it
        head = endPoint;
    if(&endPoint == &tail)      //same with tail
        tail = startPoint;
    */

    //dealing with the end cases
    temp = working->prev;
    working->prev = working->next;
    working->next = temp;

    temp = startPoint->next;
    ListNode* temp2 = startPoint->prev;
    startPoint->next = endPoint->prev;
    endPoint->prev = temp;

    if(endPoint->prev != NULL)
        endPoint->prev->next = endPoint;
    else
        head = endPoint;

    if(startPoint->next != NULL)
        startPoint->next->prev = startPoint;
    else
        tail = startPoint;
        

    temp = startPoint;
    startPoint = endPoint;
    endPoint = temp;
}


/**
 * Reverses blocks of size n in the current List. You should use your
 * reverse( ListNode * &, ListNode * & ) helper function in this method!
 *
 * @param n The size of the blocks in the List to be reversed.
 */
template <class T>
void List<T>::reverseNth( int n )
{
    /// @todo Graded in MP3.1

    //nothing is idiotproof to a sufficiently trained idiot
    if(n <= 0)
        return;

    ListNode* start = head;
    ListNode* stop = head;

    //see how many sections we have to flip, rounding up
    int sections = ceil((double)length / (double)n);

    //cout << sections << endl;

    for(int i = 0; i < sections; i++)
    {
        //setting the end for our flip
        for(int j = 1; j < n; j++)
        {
            //don't want to read after the tail
            if(stop->next == NULL)
                break;
            stop = stop->next;
        }

        //actually reverse it
        reverse(start, stop);

        //get set up for the next one
        start = stop->next;
        stop = start;
    }
}


/**
 * Modifies the List using the waterfall algorithm.
 * Every other node (starting from the second one) is removed from the
 * List, but appended at the back, becoming the new tail. This continues
 * until the next thing to be removed is either the tail (**not necessarily
 * the original tail!**) or NULL.  You may **NOT** allocate new ListNodes.
 * Note that since the tail should be continuously updated, some nodes will
 * be moved more than once.
 */
template <class T>
void List<T>::waterfall()
{
    /// @todo Graded in MP3.1

    ListNode* curr = head->next;
    ListNode* left = curr->prev;
    ListNode* right = curr->next;

    while(right != NULL)
    {
        //sticking curr on the end
        tail->next = curr;
        curr->prev = tail;
        curr->next = NULL;
        tail = curr;

        //patching the hole
        right->prev = left;
        left->next = right;

        //next!
        curr = right->next;
        left = right;
        right = curr->next;
    }
}

/**
 * Splits the given list into two parts by dividing it at the splitPoint.
 *
 * @param splitPoint Point at which the list should be split into two.
 * @return The second list created from the split.
 */
template <class T>
List<T> List<T>::split(int splitPoint)
{
    if (splitPoint > length)
        return List<T>();

    if (splitPoint < 0)
        splitPoint = 0;

    ListNode * secondHead = split(head, splitPoint);

    int oldLength = length;
    if (secondHead == head)
    {
        // current list is going to be empty
        head = NULL;
        tail = NULL;
        length = 0;
    }
    else
    {
        // set up current list
        tail = head;
        while (tail->next != NULL)
            tail = tail->next;
        length = splitPoint;
    }

    // set up the returned list
    List<T> ret;
    ret.head = secondHead;
    ret.tail = secondHead;
    if (ret.tail != NULL)
    {
        while (ret.tail->next != NULL)
            ret.tail = ret.tail->next;
    }
    ret.length = oldLength - splitPoint;
    return ret;
}

/**
 * Helper function to split a sequence of linked memory at the node
 * splitPoint steps **after** start. In other words, it should disconnect
 * the sequence of linked memory after the given number of nodes, and
 * return a pointer to the starting node of the new sequence of linked
 * memory.
 *
 * This function **SHOULD NOT** create **ANY** new List objects!
 *
 * @param start The node to start from.
 * @param splitPoint The number of steps to walk before splitting.
 * @return The starting node of the sequence that was split off.
 */
template <class T>
typename List<T>::ListNode * List<T>::split(ListNode * start, int splitPoint)
{
    /// @todo Graded in MP3.2
    if(start == NULL || splitPoint < 0)
        return NULL;

    ListNode* newstart = start;

    for(int i = 0; i < splitPoint; i++)
        newstart = newstart->next;

    newstart->prev->next = NULL;
    tail = newstart->prev;
    newstart->prev = NULL;
    return newstart;
}

/**
 * Merges the given sorted list into the current sorted list.
 *
 * @param otherList List to be merged into the current list.
 */
template <class T>
void List<T>::mergeWith(List<T> & otherList)
{
    // set up the current list
    head = merge(head, otherList.head);
    tail = head;

    // make sure there is a node in the new list
    if(tail != NULL)
    {
        while (tail->next != NULL)
            tail = tail->next;
    }
    length = length + otherList.length;

    // empty out the parameter list
    otherList.head = NULL;
    otherList.tail = NULL;
    otherList.length = 0;
}

/**
 * Helper function to merge two **sorted** and **independent** sequences of
 * linked memory. The result should be a single sequence that is itself
 * sorted.
 *
 * This function **SHOULD NOT** create **ANY** new List objects.
 *
 * @param first The starting node of the first sequence.
 * @param second The starting node of the second sequence.
 * @return The starting node of the resulting, sorted sequence.
 */
template <class T>
typename List<T>::ListNode * List<T>::merge(ListNode * first, ListNode * second)
{
    /// @todo Graded in MP3.2

    ListNode* temp;
    ListNode* temp2;
	
	if(first == NULL)
	{
		first = second;
	}
	else if(second != NULL && second->data < first->data)
	{
		temp = first;
		first = second;
		second = temp;
	}
	
	ListNode* ret = first;
	
    while(first != NULL && second != NULL)
    {
        if(second->data < first->data)
        {
        	temp = first->prev;
        	temp2 = second;
        	second = second->next;
        	temp->next = temp2;
        	temp2->prev = temp;
        	temp2->next = first;
        	first->prev = temp2;
        }
        else
            first = first->next;
    }
    
    if(second != NULL)
    {
    	first = ret;
    	while(first->next != NULL)
    		first = first->next;
    	first->next = second;
    	second->prev = first;
    }

    return ret;
}

/**
 * Sorts the current list by applying the Mergesort algorithm.
 */
template <class T>
void List<T>::sort()
{
    if (empty())
        return;
    head = mergesort(head, length);
    tail = head;
    while (tail->next != NULL)
        tail = tail->next;
}

/**
 * Sorts a chain of linked memory given a start node and a size.
 * This is the recursive helper for the Mergesort algorithm (i.e., this is
 * the divide-and-conquer step).
 *
 * @param start Starting point of the chain.
 * @param chainLength Size of the chain to be sorted.
 * @return A pointer to the beginning of the now sorted chain.
 */
template <class T>
typename List<T>::ListNode * List<T>::mergesort(ListNode * start, int chainLength)
{
    /// @todo Graded in MP3.2
    if(start == NULL || chainLength < 0)
    	return start;
    
    if(chainLength <= 1)    //a list of length 1 or 0 is technically sorted
        return start;
    ListNode* temp = split(start, chainLength/2);
    ListNode* sorted1 = mergesort(start, chainLength/2);
    ListNode* sorted2 = mergesort(temp, chainLength - (chainLength/2));
    temp = merge(sorted1, sorted2);
    
    return temp;
}
