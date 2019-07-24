#pragma once 

/* Standard Libraries */
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>

#include "constants.h"

/* Using statements */
using std::string;
using std::endl;
using std::cout;
using std::vector;
using std::unordered_map;

class Node
{
    public:
        Node(int id);
        virtual ~Node();

        int _id;                    // all nodes need this
        int _startIndex;            // all nodes need this
        int _parentEdgeLabelLength;
        int _stringDepth;           // all nodes need this
        Node * _sibling;            // all nodes except root need this
        Node * _parent;             // all nodes need this
        
};

class InternalNode : public Node
{
    public:
        InternalNode(int id);
        Node * _child;                  // only InternalNodes can have children
        InternalNode * _suffixLink;
};

class LeafNode : public Node
{
    public:
        LeafNode(int id);
};

class SuffixTree
{
    public:
        /*
         * Preconditions: assume input ends in '$'
         */
        SuffixTree(string input, string alphabet);

        /*
         * Call recursive function (post-order traversal) to destroy tree.
         */
        ~SuffixTree();

        /*
         * Description: insert suffix into SuffixTree.
         * Return: pointer to leaf node that was just inserted
         */
        Node * insert(int s_starti);

        /*
         * Insert suffix into Suffix Tree using linear time
         * approach (i.e. McCreight's suffix tree construction algorithm).
         */
        void insertLinearTime(int s_starti);

        /*
         * Insert string _input.substr(s_starti) into SuffixTree starting at node u.
         */
        Node * findPath(Node * u, int s_starti, int i);

        /*
         * Starting from v_prime, walk down the tree to node v.
         * We know we've reached node v when beta has been exhausted.
         */
        InternalNode * nodeHops(InternalNode * v_prime, int beta_i, int beta_length);

        /*
         * Display u's children from left to right.
         */
        void displaySubTree(InternalNode * u);

        /*
         * Enumerate nodes using DFS traversal (i.e., visit all children from left to right
         * AFTER visiting the parent).  Note: this will enumerate nodes of the tree in a
         * top-down fashion. As a result of this enumeration, display the STRING DEPTH
         * information from each node. As this display can get too long (== number of nodes
         * in the tree), simply break the output into lines with 10 entries in each. 
         */
        void DFSTraversal();
        void DFSTraversal_no_print();

        /*
         * DFS and return the chars at leaf IDs inorder of traversal as string.
         */
        string getBWT();

        /*
         * Return leaves of Suffix Tree as <id, edgeLabel> pairs
         * in the form of a dictionary.
         * 
         * ~~ FOR TESTING PURPOSES ~~
         */
        unordered_map<int, string> getIDPathLabelPairs();

        /*
         * Report number of nodes/leaves, LRS, BWT.
         */
        void report();

    private:
        /*
         * Recursive function to get Burrows–Wheeler transform and store
         * in the string "transform".
         */
        void rGetBWT(Node * u, string & transform);

        /*
         * Recursively traverse tree in preorder and print relevant info
         * like id and edge label.
         */
        void rDFSTraversal(Node * u);

        /*
         * Recursively traverse tree to find deepest internal node.
         * Record longest repeating sequence.
         */
        void rDFSTraversal_No_Print(Node * u);

        /*
         * Recursively destroy subtree of u and then itself (post-order traversal)
         */        
        void burnTree(Node * u);

        /*
         * Insert child into correct lexicographical order of parent's child list.
         * Assume that child's edge label is correct.
         */
        void insertChild(InternalNode * parent, Node * child);

        /*
         * Assume that parent DOES have a child pointer.
         * Replace appropriate pointer (whether this is parent->child
         * or a sibling of parent->child) with child
         */
        void removeChild(InternalNode * parent, Node * rm);

        /*
         * Return ID/pathlabel pairs of leaves of the suffix tree as a dictionary.
         */
        void rGetIDPathLabelPairs(unordered_map<int, string> & store, Node * u);

        /*
         * Return path label from root to u.
         */
        string getPathLabel(Node * u);

        /*
         * Return parent edge label of Node u
         */
        string getParentEdgeLabel(Node * u);
       
        /* Private attributes. */
        InternalNode * _root;
        Node * _justCreated;        // keep track of leaf just created
        string _input;              // input string
        string _alphabet;
        int _next_id;               // use this field for internal node unique id
        int _match_counter;         // count number of matched character comparisons
        int _num_nodes;             // count number of internal nodes
        int _num_leaves;            // count number of leaves
        string _LRS;
        int _max_string_depth;      // 
        
};

/*
 * Mathematically correct modulo operator.
 * Courtesy of: https://stackoverflow.com/questions/11720656/modulo-operation-with-negative-numbers
 */
int modulo(int x, int N);