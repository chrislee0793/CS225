/**
 * @file quackfun.cpp
 * This is where you will implement the required functions for the
 *  stacks and queues portion of the lab.
 */

/**
 * Sums items in a stack.
 * @param s A stack holding values to sum.
 * @return The sum of all the elements in the stack, leaving the original
 *  stack in the same state (unchanged).
 *
 * @note You may modify the stack as long as you restore it to its original
 *  values.
 * @note You may use only two local variables of type T in your function.
 *  Note that this function is templatized on the stack's type, so stacks of
 *  objects overloading the + operator can be summed.
 * @note We are using the Standard Template Library (STL) stack in this
 *  problem. Its pop function works a bit differently from the stack we
 *  built. Try searching for "stl stack" to learn how to use it.
 * @hint Think recursively!
 */
template <typename T>
T QuackFun::sum(stack<T> & s)
{
    // Your code here

    T temp, remSum;
    if(s.size() != 0)
    {
        temp = s.top();
        s.pop();
        remSum = sum(s);
    }
    else
        return T();

    s.push(temp);

    return temp + remSum; // stub return value (0 for primitive types). Change this!
                // Note: T() is the default value for objects, and 0 for
                // primitive types
}

/**
 * Reverses even sized blocks of items in the queue. Blocks start at size
 * one and increase for each subsequent block.
 * @param q A queue of items to be scrambled
 *
 * @note Any "leftover" numbers should be handled as if their block was complete.
 * @note We are using the Standard Template Library (STL) queue in this
 *  problem. Its pop function works a bit differently from the stack we
 *  built. Try searching for "stl stack" to learn how to use it.
 * @hint You'll want to make a local stack variable.
 */
template <typename T>
void QuackFun::scramble(queue<T> & q)
{
    stack<T> s;
    queue<T> q2;

    long unsigned int i = 1;

    while(!q.empty())
    {
        if(i % 2 == 0)
        {
            //flip flip flip bear flip
            for(long unsigned int j = 0; j < i; j++)
            {
                if(!q.empty())
                {
                    s.push(q.front());
                    q.pop();
                }
            }
            while(!s.empty())
            {
                q2.push(s.top());
                s.pop();
            }
        }
        else
        {
            //no flip :(
            for(long unsigned int j = 0; j < i; j++)
            {
                if(!q.empty())
                {
                    q2.push(q.front());
                    q.pop();
                }
            }
        }
        i++;
    }

    q = q2;
}

/**
 * This function is NOT part of the standard lab grade, but is extra credit.
 * It was also a previous exam question, and something similar could show up again.
 * 
 * @return true if the parameter stack and queue contain only elements of exactly
 *  the same values in exactly the same order; false, otherwise.
 *
 * @note You may assume the stack and queue contain the same number of items!
 * @note There are restrictions for writing this function.
 * - Your function may not use any loops
 * - In your function you may only declare ONE local boolean variable to use in your return statement,
 *      and you may only declare TWO local variables of parametrized type T to use however you wish.
 *   No other local variables can be used.
 * - After execution of verifySame, the stack and queue must be unchanged. Be sure to comment your code VERY well.
 */
template <typename T>
bool QuackFun::verifySame(stack<T> & s, queue<T> & q)
{
    if(s.empty())
        return true;    //we've gone all the way through
    T stemp = s.top();
    T qtemp = q.front();

    s.pop();
    q.pop();
    q.push(qtemp);  // should put it back in the right order

    if(stemp != qtemp)
    {
        verifySame(s, q);   //need to keep going through to get the queue in the right order again
        s.push(stemp);  //put this back!
        return false;   //this isn't the same, so return false
    }
    else
    {
        bool derp = verifySame(s, q);   //these two are the same, so try the next ones
        s.push(stemp);  //still has to go back
        return derp;    //only true if the rest are too
    }
}

