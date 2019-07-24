#include "suffix_tree.h"
#include <stdio.h>
using namespace std;

Node::Node(int id)
{
    this->_id = id;
    this->_startIndex = 0;
    this->_parentEdgeLabelLength = 0;
    this->_stringDepth = 0;
    this->_sibling = nullptr;
    this->_parent = nullptr;
}

Node::~Node() { }

InternalNode::InternalNode(int id) : Node(id)
{
    this->_child = nullptr;
    this->_suffixLink = nullptr;
}

LeafNode::LeafNode(int id) : Node(id) { }

SuffixTree::SuffixTree(string input, string alphabet)
{
    /* Ensure input ends in '$' */
    this->_input = (input[input.length() - 1] != '$') ? input + '$' : input;

    /* Set important counters and auxiliary variables. */
    this->_next_id = this->_input.length();
    this->_match_counter = 0;
    this->_num_nodes = 0;
    this->_num_leaves = 0;
    this->_max_string_depth = 0;
    this->_LRS = "";

    /*
     * Leaves will have IDs 0 through this->_input.length() - 1
     *      (since this->_input includes the extra '$' and thus its
     *      length is the number of suffixes)
     * For internal nodes, use this->next_id for the id. Increment whenever
     * we create a new internal node.
     */
    this->_root = new InternalNode(this->_next_id++);
    // set parent to itself and suffix link to itself
    this->_root->_parent = this->_root->_suffixLink = this->_root;
    this->_justCreated = this->_root;
    this->_num_nodes++;

    /* Insert all the suffixes. */
    for (int i = 0; i < this->_input.length(); i++) {
        if (DEBUG_MODE) cout << "Suffix " << i << ": " << this->_input.substr(i) << endl;
        if (SUFFIX_LINK_MODE) {
            this->insertLinearTime(i);
        } else {
            this->insert(i);
        }
       
    }
}

SuffixTree::~SuffixTree()
{
    this->burnTree(this->_root);
}

Node * SuffixTree::insert(int s_starti)
{
    return this->findPath(this->_root, s_starti, 0);
}

void SuffixTree::insertLinearTime(int s_starti)
{
    InternalNode * u = nullptr, * u_prime = nullptr;

    // Establish u <- parent of leaf i-1
    u = dynamic_cast<InternalNode *>(this->_justCreated->_parent);

    // Establish u_prime <- parent of u
    u_prime = dynamic_cast<InternalNode *>(u->_parent);

    // Get the case...
    int suffixLinkCase = 0;

    // suffix link is known
    if (u->_suffixLink != nullptr)
    {
        suffixLinkCase = (this->_root == u) ? CASE_1B : CASE_1A;
    }
    // suffix link is unknown
    else
    {
        suffixLinkCase = (this->_root == u_prime) ? CASE_2B : CASE_2A;
    }

    InternalNode * v = nullptr, * v_prime = nullptr;
    int beta_i = 0, beta_length = 0, beta_prime_i = 0, beta_prime_length = 0,
        alpha_prime_i = 0, alpha_prime_length = 0;
    
    switch (suffixLinkCase)
    {
        case CASE_1A:
            // 1. Go to u
            // 2. Take SL(u) to v.
            v = u->_suffixLink;
            
            // 3. FindPath(v, s[i + alpha.length()])
            this->findPath(v, s_starti, v->_stringDepth);
            break;
        case CASE_1B:
            // 1. Go to u.
            // 2. Take SL(u) which is the root to v which is the root.
            v = u->_suffixLink;

            // 3. FindPath(v, s)
            this->findPath(v, s_starti, 0);
            break;
        case CASE_2A:
            // 1. Go to u_prime.
            // 2. Let beta = u->parentEdgeLabel and alpha_prime = u_prime->parentEdgeLabel.substr(1)
            beta_i = u->_startIndex;
            beta_length = u->_parentEdgeLabelLength;

            // 3. Take SL(u_prime) to v_prime.
            v_prime = u_prime->_suffixLink;

            // 4. NodeHop(v_prime, s[i + alpha_prime.length()], beta)
            v = this->nodeHops(v_prime, beta_i, beta_length);

            // 5. Set SL(u) to v
            u->_suffixLink = v;

            // 6. FindPath(v, s[i + alpha.length()])
            this->findPath(v, s_starti, v->_stringDepth);
            break;
        case CASE_2B:
            // 1. Go to u_prime
            // 2. Take SL(u_prime) to v_prime which is the root.
            v_prime = u_prime->_suffixLink;

            // 3. Let beta_prime = u->parentEdgeLabel.substr(1)
            beta_prime_i = u->_startIndex + 1;
            beta_prime_length = u->_parentEdgeLabelLength - 1;

            // 4. NodeHop(v_prime/root, s[i], beta_prime)
            v = this->nodeHops(v_prime, beta_prime_i, beta_prime_length);

            // 5. Set SL(u) to v
            u->_suffixLink = v;

            // 6. FindPath(v, s[i + beta_prime.length()])
            this->findPath(v, s_starti, v->_stringDepth);
            break;
        default:
            cout << "SuffixTree::insertLinearTime -- invalid case" << endl;
            exit(1);
    }
}

Node * SuffixTree::findPath(Node * u, int s_starti, int i)
{
    // Assume u is not a nullptr; cast u to InternalNode *
    InternalNode * in_ptr = dynamic_cast<InternalNode*>(u);
    
    // p is head of child list of u
    Node * p = in_ptr->_child;

    int j = 0, alpha_i, alpha_length;

    // pick branch out of u that starts with s[i] by looking at path labels of children
    while (p != nullptr) {
        if (this->_input.at(p->_startIndex) == this->_input.at(s_starti + i)) {
            alpha_i = p->_startIndex;
            alpha_length = p->_parentEdgeLabelLength;
            break;
        }
        p = p->_sibling;
    }

    // if p is null, must insert new child in child list
    if (p == nullptr) {
        // this means no such branch exists, so create node
        LeafNode * lf = new LeafNode(s_starti);
        lf->_startIndex = lf->_id + i;
        lf->_parentEdgeLabelLength = this->_input.length() - (i + s_starti);
        lf->_stringDepth = in_ptr->_stringDepth + this->_input.length() - (i + s_starti);
        this->insertChild(in_ptr, lf);
        this->_justCreated = lf;
        this->_num_leaves++;
        return lf;
    }
    int slength = this->_input.length() - s_starti;
    while (i < slength && j < alpha_length) {
        // check for character matches
        // alpha.at(j) == input at alpha_i + j
        if (this->_input.at(s_starti + i) == this->_input.at(alpha_i + j))
        {
            i++;
            j++;
            this->_match_counter++;
            continue;
        }
        /*
         * Create a new internal node "newIt". Replace p (whether it's
         * in_ptr->child or a sibling of in_ptr->child) with newIt.
         * We can do this by simply removing p and then adding newIt since
         * the order will be maintained (no duplicates allowed).
         */

        // first, ensure p is gone from parent
        this->removeChild(in_ptr, p);

        // next, create new internal node
        InternalNode * newIt = new InternalNode(this->_next_id++);
        newIt->_stringDepth = in_ptr->_stringDepth + j;
        newIt->_startIndex = p->_startIndex;
        newIt->_parentEdgeLabelLength = j; 
        this->_num_nodes++;
        this->insertChild(in_ptr, newIt);

        // create new leaf and add to newIt's child list
        LeafNode * lf = new LeafNode(s_starti);
        // input at s_starti + in_ptr->_stringDepth + j
        lf->_stringDepth = newIt->_stringDepth + this->_input.length() - (s_starti + in_ptr->_stringDepth + j);
        lf->_startIndex = in_ptr->_stringDepth + j + lf->_id;
        lf->_parentEdgeLabelLength = this->_input.length() - (s_starti + in_ptr->_stringDepth + j);
        this->_num_leaves++;
        this->insertChild(newIt, lf);
        this->_justCreated = lf;

        // p should now be in newIt's child list
        p->_stringDepth = newIt->_stringDepth + alpha_length - j;
        p->_startIndex = newIt->_startIndex + j;
        p->_parentEdgeLabelLength = alpha_length - j;
        this->insertChild(newIt, p);

        return lf;
    }
    /* This code only executes when alpha has been exhausted. */
    u = p;
    return this->findPath(u, s_starti, i);    
}

InternalNode * SuffixTree::nodeHops(InternalNode * v_prime, int beta_i, int beta_length)
{
    /* Base Case: beta is exhausted */
    if (beta_length <= 0) return v_prime;

    // Assume v_prime is not a nullptr; cast v_prime to InternalNode *
    InternalNode * in_ptr = dynamic_cast<InternalNode*>(v_prime);
    
    // p is head of child list of v_prime
    Node * p = in_ptr->_child;
    int alpha_i, alpha_length;

    // pick branch out of u that starts with beta[0] by looking at path labels of children
    while (p != nullptr) {
        if (this->_input.at(p->_startIndex) == this->_input.at(beta_i)) {
            alpha_i = p->_startIndex;
            alpha_length = p->_parentEdgeLabelLength;
            break;
        }
        p = p->_sibling;
    }
    /*
     * if == lengths, there must already be a node there
     * but if beta < alpha, must create new internal node
     * & split alpha (edge from p to in_ptr)
     */
    if (beta_length < alpha_length) {
        // first, ensure p is gone from parent
        this->removeChild(in_ptr, p);

        // next, create new internal node
        InternalNode * newIt = new InternalNode(this->_next_id++);
        newIt->_stringDepth = in_ptr->_stringDepth + beta_length;
        newIt->_startIndex = p->_startIndex;
        newIt->_parentEdgeLabelLength = beta_length;
        this->_num_nodes++;
        this->insertChild(in_ptr, newIt);

        // p should now be in newIt's child list
        p->_stringDepth = newIt->_stringDepth + alpha_length - beta_length;
        p->_startIndex = newIt->_startIndex + beta_length;
        p->_parentEdgeLabelLength = alpha_length - beta_length;
        this->insertChild(newIt, p);
        return newIt;
    }
    // recurse with starting index of beta within input string
    return this->nodeHops(dynamic_cast<InternalNode*>(p), beta_i + alpha_length, beta_length - alpha_length);
}

void SuffixTree::displaySubTree(InternalNode * u)
{
    Node * n = u->_child;
    cout << "Child List Node ID" << u->_id << endl;
    while (n != nullptr) {
        cout << n->_id << "|" << this->getParentEdgeLabel(n) << " ";
        n = n->_sibling;
    }
    cout << endl;
}

void SuffixTree::DFSTraversal()
{
    this->rDFSTraversal(this->_root);
}

void SuffixTree::DFSTraversal_no_print()
{
     this->rDFSTraversal_No_Print(this->_root);
}

string SuffixTree::getBWT()
{
    string result = "";
    result.reserve(this->_input.length());
    this->rGetBWT(this->_root, result);
    return result;
}

unordered_map<int, string> SuffixTree::getIDPathLabelPairs()
{
    unordered_map<int, string> result;
    this->rGetIDPathLabelPairs(result, this->_root);
    return result;
}

void SuffixTree::report()
{
   fstream file;
   ofstream afile;
  
    this->DFSTraversal_no_print();
    cout << "Number of Internal Nodes: " << this->_num_nodes << endl;
    cout << "Number of Leaves: " << this->_num_leaves << endl;
    cout << "Longest Repeated Substring is: " << this->_LRS << endl;
    cout << "Length of LRS is: " << this->_LRS.length() << endl;
    cout << "Starting Coordinates of LRS are: ";
    size_t found = this->_input.find(this->_LRS);
    while (found != string::npos)
    {
        cout << found;
        found = this->_input.find(this->_LRS, found + 1);
        cout << ((found != string::npos) ? ", " : "");
    }
    string result = this->getBWT();
     afile.open("bwtIndex.txt");

    for(auto c: result){
        afile << c << endl;
    }
    afile.close();
    cout << endl;
}

void SuffixTree::rGetBWT(Node * u, string & transform)
{
    if (u != nullptr) {
        InternalNode * in = dynamic_cast<InternalNode*>(u);
        LeafNode * lf = dynamic_cast<LeafNode*>(u);

        if (in != nullptr)
        {
            this->rGetBWT(in->_child, transform);
            this->rGetBWT(in->_sibling, transform);
        }
        else
        {
            transform.push_back(this->_input.at(
                modulo(lf->_id - 1, this->_input.length())
            ));
            this->rGetBWT(lf->_sibling, transform);
        }
    }
}

void SuffixTree::rDFSTraversal(Node * u)
{
    if (u != nullptr) {
        InternalNode * in = dynamic_cast<InternalNode*>(u);
        LeafNode * lf = dynamic_cast<LeafNode*>(u);

        if (in != nullptr)
        {
            cout << "edge label for id=" << in->_id << " is " << this->getParentEdgeLabel(in) << ". String depth is: " << in->_stringDepth<< endl;
            if(in->_stringDepth > this->_max_string_depth) {
                this->_max_string_depth = in->_stringDepth;
                this-> _LRS = this->getPathLabel(in);
            }
            this->rDFSTraversal(in->_child);
            this->rDFSTraversal(in->_sibling);
        }
        else
        {
            cout << "edge label for id=" << lf->_id << " is " << this->getParentEdgeLabel(lf) << ". String depth is: " << lf->_stringDepth << endl;
            this->rDFSTraversal(lf->_sibling);
        }
    }
    
}

void SuffixTree::rDFSTraversal_No_Print(Node * u)
{
    if (u != nullptr) {
        InternalNode * in = dynamic_cast<InternalNode*>(u);
        LeafNode * lf = dynamic_cast<LeafNode*>(u);

        if (in != nullptr)
        {
            if(in->_stringDepth > this->_max_string_depth){
                _max_string_depth = in->_stringDepth;
                _LRS = this->getPathLabel(in);
            }
            this->rDFSTraversal_No_Print(in->_child);
            this->rDFSTraversal_No_Print(in->_sibling);
        }
        else
        {
            
            this->rDFSTraversal_No_Print(lf->_sibling);
        }
    }
    
}

void SuffixTree::burnTree(Node * u)
{
    if (u != nullptr) {
        InternalNode * in = dynamic_cast<InternalNode*>(u);
        LeafNode * lf = dynamic_cast<LeafNode*>(u);
        int nodeid;

        if (in != nullptr)
        {
            this->burnTree(in->_child);
            this->burnTree(in->_sibling);
            nodeid = in->_id;
        }
        else
        {
            nodeid = lf->_id;
            this->burnTree(lf->_sibling);
        }
        if (DEBUG_MODE) cout << "killing node id: " << nodeid << endl;
        delete u;
    }
}

void SuffixTree::insertChild(InternalNode * parent, Node * child)
{
    Node * curr = parent->_child, * prev = nullptr;

    /* No matter what, child's parent is parent. */
    child->_parent = parent;

    /* If no children, simply insert as head. */
    if (parent->_child == nullptr) {
        parent->_child = child;
        return;
    }

    /* Maintain lexicographical ordering */
    while (curr != nullptr && this->_input.at(child->_startIndex) > this->_input.at(curr->_startIndex)) {
        prev = curr;
        curr = curr->_sibling;
    }

    /* If prev is still null, we're inserting at head. */
    if (prev == nullptr) {
        parent->_child = child;
        child->_sibling = curr;
    }
    /* Inserting in middle/end. Treat the same. */
    else
    {
        prev->_sibling = child;
        child->_sibling = curr;     // set sibling to prev's old sibling (curr)
    }
}

void SuffixTree::removeChild(InternalNode * parent, Node * rm)
{
    Node * cur = parent->_child;

    if (cur == nullptr)
    {
        printf("removeChild -- ERROR no children\n");
        return;
    }

    /* Delete head. */
    if (cur == rm)
    {
        parent->_child = cur->_sibling;
        rm->_parent = nullptr;
        rm->_sibling = nullptr;
        return;
    }

    while (cur->_sibling != rm)
    {
        cur = cur->_sibling;    
    }

    cur->_sibling = cur->_sibling->_sibling;
    rm->_parent = nullptr;
    rm->_sibling = nullptr;
}

void SuffixTree::rGetIDPathLabelPairs(unordered_map<int, string> & store, Node * u)
{
    if (u != nullptr) {
        InternalNode * in = dynamic_cast<InternalNode*>(u);
        LeafNode * lf = dynamic_cast<LeafNode*>(u);

        if (in != nullptr)
        {
            this->rGetIDPathLabelPairs(store, in->_child);
            this->rGetIDPathLabelPairs(store, in->_sibling);
        }
        else
        {
            this->rGetIDPathLabelPairs(store, lf->_sibling);
            store[u->_id] = this->getParentEdgeLabel(u);
        }
    }
}

string SuffixTree::getPathLabel(Node * u)
{
    string s = "";

    /* Look while u is not root. */
    while (u != this->_root)
    {
        s = this->getParentEdgeLabel(u) + s;
        u = u->_parent;
    }
    return s;
}

string SuffixTree::getParentEdgeLabel(Node * u)
{
    return this->_input.substr(u->_startIndex, u->_parentEdgeLabelLength);
}

int modulo(int x, int N)
{
    return (x % N + N) %N;
}
