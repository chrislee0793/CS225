/**
 * @file avltree.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */
 
#include <math.h>
 
template <class K, class V>
V AVLTree<K, V>::find(const K & key) const
{
        return find(root, key);
}
 
template <class K, class V>
V AVLTree<K, V>::find(Node * subtree, const K & key) const
{
        if (subtree == NULL)
                return V();
        else if (key == subtree->key)
                return subtree->value;
        else
        {
                if (key < subtree->key)
                        return find(subtree->left, key);
                else
                        return find(subtree->right, key);
        }
}
 
template <class K, class V>
void AVLTree<K, V>::rotateLeft(Node * & t)
{
        *_out << __func__ << endl; // Outputs the rotation name (don't remove this)
        Node* tr = t->right;
        t->right = tr->left;
        tr->left = t;
        t->height = calculateSubtreeHeight(t);
        tr->height = calculateSubtreeHeight(tr);
        t = tr;
               
}
 
template <class K, class V>
void AVLTree<K, V>::rotateLeftRight(Node * & t)
{
        *_out << __func__ << endl; // Outputs the rotation name (don't remove this)
        // Implemented for you:
        rotateLeft(t->left);
        rotateRight(t);
}
 
template <class K, class V>
void AVLTree<K, V>::rotateRight(Node * & t)
{
        *_out << __func__ << endl; // Outputs the rotation name (don't remove this)
        Node* tl = t->left;
        t->left = tl->right;
        tl->right = t;
        t->height = calculateSubtreeHeight(t);
        tl->height = calculateSubtreeHeight(tl);
        t = tl;
 
}
 
template <class K, class V>
void AVLTree<K, V>::rotateRightLeft(Node * & t)
{
        *_out << __func__ << endl; // Outputs the rotation name (don't remove this)
        rotateRight(t->right);
        rotateLeft(t);
}
 
template <class K, class V>
void AVLTree<K, V>::insert(const K & key, const V & value)
{
        insert(root, key, value);
}
 
template <class K, class V>
void AVLTree<K, V>::insert(Node* & subtree, const K & key, const V & value)
{
        if(subtree == NULL) {
                subtree = new Node(key, value);
                return;
        }      
        if(subtree->key == key) return;
 
        if(key < subtree->key) {
                insert(subtree->left, key, value);
        } else {
                insert(subtree->right, key, value);
        }
        if(heightOrNeg1(subtree->right) - heightOrNeg1(subtree->left) == 2) {
                if(subtree->right != NULL) {
                        if(heightOrNeg1(subtree->right->left) > heightOrNeg1(subtree->right->right)) {
                                rotateRightLeft(subtree);
                        } else if (heightOrNeg1(subtree->right->left) < heightOrNeg1(subtree->right->right)) {
                                rotateLeft(subtree);
                        }
                }
        } else if(heightOrNeg1(subtree->right) - heightOrNeg1(subtree->left) == -2) {
                if(subtree->left != NULL) {
                        if(heightOrNeg1(subtree->left->right) > heightOrNeg1(subtree->left->left)) {
                                rotateLeftRight(subtree);
                        } else if(heightOrNeg1(subtree->left->right) < heightOrNeg1(subtree->left->left)) {
                                rotateRight(subtree);
                        }
                }      
        }
        subtree->height = calculateSubtreeHeight(subtree);
}
 
template <class K, class V>
int AVLTree<K, V>::calculateSubtreeHeight(Node* & node) {
        return 1 + max(heightOrNeg1(node->left), heightOrNeg1(node->right));
}
