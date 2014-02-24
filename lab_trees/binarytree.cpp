/**
 * @file binarytree.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */

/**
 * @return The height of the binary tree. Recall that the height of a binary tree
 *  is just the length of the longest path from the root to a leaf, and that the
 *  height of an empty tree is -1.
 */
template <typename T>
int BinaryTree<T>::height() const
{
	// Call recursive helper function on root
	return height(root);
}

/**
 * Private helper function for the public height function.
 * @param subRoot
 * @return The height of the subtree
 */
template <typename T>
int BinaryTree<T>::height(const Node * subRoot) const
{
	// Base case
	if (subRoot == NULL)
        return -1;
	
	// Recursive definition
	return 1 + max(height(subRoot->left), height(subRoot->right));
}

/**
 * Prints out the values of the nodes of a binary tree in order.
 * That is, everything to the left of a node will be printed out before that node
 *  itself, and everything to the right of a node will be printed out after that node.
 */
template <typename T>
void BinaryTree<T>::printLeftToRight() const
{
	// Call recursive helper function on the root
	printLeftToRight(root);

	// Finish the line
	cout << endl;
}

/**
 * Private helper function for the public printLeftToRight function.
 * @param subRoot
 */
template <typename T>
void BinaryTree<T>::printLeftToRight(const Node * subRoot) const
{
	// Base case - null node
	if (subRoot == NULL)
		return;

	// Print left subtree
	printLeftToRight(subRoot->left);

	// Print this node
	cout << subRoot->elem << ' ';

	// Print right subtree
	printLeftToRight(subRoot->right);
}

/**
 * Flips the tree over a vertical axis, modifying the tree itself
 *  (not creating a flipped copy).
 */
template <typename T>
void BinaryTree<T>::mirror()
{
	// your code here
	recursiveReverse(root);
}

template <typename T>
void BinaryTree<T>::recursiveReverse(Node* target)
{
	if(target == NULL)
		return;
	Node* temp = target->left;
	target->left = target->right;
	target->right = temp;
	recursiveReverse(target->left);
	recursiveReverse(target->right);
}

/**
 * @return True if an in-order traversal of the tree would produce a nondecreasing list
 *  output values, and false otherwise. This is also the criterion for a binary tree to be
 *  a binary search tree.
 */
template <typename T>
bool BinaryTree<T>::isOrdered() const
{
    // your code here

	if(root == NULL)
		return true;
	return recursiveOrdered(root);
}

template <typename T>
bool BinaryTree<T>::recursiveOrdered(Node* target) const
{
	if(target == NULL || (target->right == NULL && target->left == NULL))
		return true;
	if(target->left == NULL && target->right != NULL && target->elem < target->right->elem)
	{
		Node* next = target->right;
		while(next->left != NULL)
			next = next->left;
		if(target->elem < next->elem)	//for nearly ordered trees
			return recursiveOrdered(target->right);
		return false;
	}
	if(target->right == NULL && target->left != NULL && target->left->elem < target->elem)
	{
		Node* prev = target->left;
		while(prev->right != NULL)
			prev = prev->right;
		if(prev->elem < target->elem)	//for nearly ordered trees
			return recursiveOrdered(target->left);
		return false;
	}
	if((target->left->elem < target->elem) && (target->elem < target->right->elem))
	{
		Node* prev = target->left;
		while(prev->right != NULL)
			prev = prev->right;
		
		Node* next = target->right;
		while(next->left != NULL)
			next = next->left;

		if(prev->elem < target->elem && target->elem < next->elem)	//for nearly ordered trees
			return recursiveOrdered(target->left) && recursiveOrdered(target->right);
		return false;
	}
	return false;
}

/**
 * Prints out all the possible paths from the root of the tree to any leaf node.
 * That is, all sequences starting at the root node and continuing downwards, ending at a
 *  leaf node. Paths ending in a left node should be printed before paths ending in a node
 *  further to the right.
 */
template <typename T>
void BinaryTree<T>::printPaths() const
{
    // your code here
    recursivePaths("Path: " + TToString(root->elem), root->left);
    recursivePaths("Path: " + TToString(root->elem), root->right);
}

template <typename T>
void BinaryTree<T>::recursivePaths(string prefix, Node* top) const
{
	if(top == NULL)
		cout << prefix << endl;
	else
	{
		if(top->left != NULL)
			recursivePaths(prefix + " " + TToString(top->elem), top->left);

		if(top->right != NULL)
			recursivePaths(prefix + " " + TToString(top->elem), top->right);
		if(top->right == NULL && top->left == NULL)
			cout << prefix << " " << TToString(top->elem) << endl;
	}
}

//because converting Ts to strings is a pain in the ass
template <typename T>
string BinaryTree<T>::TToString(T input) const
{
	string output;
	ostringstream convert;
	convert << input;
	output = convert.str();
	return output;
}

/**
 * Each node in a tree has a distance from the root node - the depth of that node,
 *  or the number of edges along the path from that node to the root. This function returns
 *  the sum of the distances of all nodes to the root node (the sum of the depths of all
 *  the nodes). Your solution should take O(n) time, where n is the number of nodes in the tree.
 * @return The sum of the distances of all nodes to the root
 */
template <typename T>
int BinaryTree<T>::sumDistances() const
{
    // your code here
    return recursiveDistance(0, root);
}

template <typename T>
int BinaryTree<T>::recursiveDistance(int length, Node* top) const
{
	if(top == NULL)
		return 0;
	int left = 0;
	int right = 0;
	if(top->left != NULL)
		left = recursiveDistance(length + 1, top->left);

	if(top->right != NULL)
		right = recursiveDistance(length + 1, top->right);

	return length + left + right;
}