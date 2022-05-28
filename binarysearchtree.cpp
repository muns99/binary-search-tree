#include "binarysearchtree.h"

Position::Position(Node* v) : v(v) {} // constructor

Elem& BinarySearchTree::Position::operator*() // get element
{
    return v->elt;
}

/*bool BinarySearchTree::Position::operator==() /* Not implemented
{

}

Elem& BinarySearchTree::Position::operator!=() /* Not implemented
{

}
*/
Position Position::left() const // get left child
{
    return Position(v->left);
}

Position Position::right() const // get right child
{
    return Position(v->right);
}

Position Position::parent() const // get parent
{
    return Position(v->par);
}

bool Position::isRoot() const // root of the tree?
{
    return v->par == NULL;
}

bool Position::isExternal() const // an external node?
{
    return v->left == NULL && v->right == NULL;
}

bool Position::hasLeftChild() const // has a left child?
{
    return v->left != NULL;
}

bool Position::hasRightChild() const // has a right child?
{
    return v->right != NULL;
}

BinarySearchTree::BinarySearchTree() // constructor
    : _root(NULL), n(0) { }

int BinarySearchTree::size() const // number of nodes
{
    return n;
}

bool BinarySearchTree::empty() const // is tree empty?
{
    return size() == 0;
}

Position BinarySearchTree::root() const // get the root
{
    return Position(_root);
}

void BinarySearchTree::addRoot() // add root to empty tree
{
    _root = new Node;
    _root->left == NULL;
    _root->right == NULL;
    _root->par == NULL;
    n = 1;
}

void BinarySearchTree::expandExternal(const Position& p)
{
    Node* v = p.v; // p’s node
    v->left = new Node; // add a new left child
    v->left->par = v; // v is its parent
    v->right = new Node; // and a new right child
    v->right->par = v; // v is its parent
    n += 2; // two more nodes
}

// remove p and parent
Position BinarySearchTree::removeAboveExternal(const Position& p)
{
    Node* w = p.v;
    Node* v = w->par; // get p’s node and parent
    Node* sib = (w == v->left ? v->right : v->left);
    if (v == _root)   // child of root?
    {
        _root = sib; // . . .make sibling root
        sib->par = NULL;
    }
    else
    {
        Node* gpar = v->par; // w’s grandparent
        if (v == gpar->left) gpar->left = sib; // replace parent by sib
        else gpar->right = sib;
        sib->par = gpar;
    }
    delete w;
    delete v; // delete removed nodes
    n -= 2; // two fewer nodes
    return Position(sib);
}

PositionList BinarySearchTree::positions() const
{
    PositionList pl;
    inorder(_root, pl); // inorder traversal
    return PositionList(pl); // return resulting list
}


// inorder traversal
void BinarySearchTree::inorder(Position p, PositionList& pl) const
{
    // external nodes are empty
    if (p.isExternal())
    {
        pl.push_back(p);
        return;
    }
    // traverse left subtree
    if(p.left().v != NULL)
        inorder(p.left(), pl);
    // add this node
    pl.push_back(p);
    // traverse right subtree
    if(p.right().v != NULL)
        inorder(p.right(), pl);
}
/* ########################################### */
/*          Implement methods below            */
/* ########################################### */

/* [01]A add node to tree */
void BinarySearchTree::add(Elem data)
{
    if(_root == NULL)
    {
        addRoot();
        _root->elt = data;
        return;
    }
    Node* v =new Node;
    v->elt = data;
    v->left = NULL;
    v->right = NULL;
    Position p = root();
    while(1)
    {
        if(data < *p)
        {
            if(!p.hasLeftChild())
            {
                v->par = p.v;
                p.v->left = v;
                ++n;
                return;
            }
            p = p.left();
        }
        else if(*p < data)
        {
            if(!p.hasRightChild())
            {
                v->par = p.v;
                p.v->right = v;
                ++n;
                return;
            }
            p = p.right();
        }

        else
            return;
    }

}

/* [01]B remove node from tree */
void BinarySearchTree::remove(Elem data)
{
    if(!exist(data)) return;
    Node *lChild, *rChild;
    Position p = getPosition(data,true); //get position of already existing node
    if(p.isExternal())
    {
        if(p.v == p.v->par->left)
            p.v->par->left = NULL;
        else
            p.v->par->right = NULL;

    }

    else if(p.hasRightChild())
    {
        rChild = p.v->right;
        rChild->par = p.v->par;
        if(!p.isRoot())
        {
            if(p.v == p.v->par->left)
                p.v->par->left = rChild;
            else
                p.v->par->right = rChild;
        }
        else _root = rChild;
        if(p.hasLeftChild())
        {
            lChild = p.v->left;
            Position rPChild(rChild);
            while(rPChild.hasLeftChild())
                rPChild = rPChild.left();
            rPChild.v->left = lChild;
            lChild->par = rPChild.v;
        }
    }
    else
    {

        lChild = p.v->left;
        lChild->par = p.v->par;
        if(!p.isRoot())
        {
            if(p.v == p.v->par->left)
                p.v->par->left = lChild;
            else
                p.v->par->right = lChild;
        }
        else _root = lChild;

    }
    delete p.v;
    --n;
    return;
}

/* [02] returns position of the maximum element in the tree */
Position BinarySearchTree::max() const
{
    Position p(_root);
    while(p.hasRightChild())
    {
        p = p.right();
    }
    return p;
}

/* [03] returns position of the minimum element in the tree */
Position BinarySearchTree::min() const
{
    Position p(_root);
    while(p.hasLeftChild())
    {
        p = p.left();
    }
    return p;
}

/* [04] returns true if value is in the tree */
bool BinarySearchTree::exist(Elem val) const
{
    Position p(_root);
    while(1)
    {
        if(val == *p)
            return true;
        else if(val < *p && p.hasLeftChild())
            p = p.left();
        else if(val > *p && p.hasRightChild())
            p =p.right();
        else return false;
    }
}

/* [05] returns position of the largest element less than the value in p */
Position BinarySearchTree::max_below(Position p) const
{
    Position w;
    if(p.hasLeftChild())
    {
        w = p.left();
        while(w.v->right != NULL)
            w = w.right();
        return w;
    }
    else if(p.v->par != NULL)
    {
        w = p.parent();
        if(*w < *p) return w;
        while(w.v->par != NULL)
        {
            if(*w < *p)
                return w;
            w = w.parent();
        }
        return p;
    }
}

/* [06] returns position of the smallest element larger than the value in p */
Position BinarySearchTree::min_above(Position p) const
{
    Position w;
    if(p.hasRightChild())
    {
        w = p.right();
        while(w.v->left != NULL)
            w = w.left();
        return w;
    }
    else if(p.v->par != NULL)
    {
        w = p.parent();
        if(*w > *p) return w;
        if(_root->elt > *p) return root();
        return p;
    }

}


/* [07] count: returns the number of stored elements/nodes (traverse the tree). */
unsigned int BinarySearchTree::count() const
{
    return n;
}

/* [08]/[09] opereator [] positive/negative index */
Elem BinarySearchTree::operator [](int index) const
{
    PositionList p1 = positions();
    if(index < 0) index += p1.size();
    if(index >= p1.size()) return 0;

    int j =0;
    for(std::list<BinarySearchTree::Position>::iterator i = p1.begin(); i != p1.end() ; ++j,++i)
        if(j == index)
            return **i;

}

/* [10] mean of all elements */
double BinarySearchTree::mean() const
{
    Elem res =0;
    PositionList p1 = positions();
    for(std::list<BinarySearchTree::Position>::iterator i = p1.begin(); i != p1.end() ; ++i)
        res += **i;
    return (double)res / p1.size();
}
Position BinarySearchTree :: getPosition(Elem data,bool valid)
{
    if( !valid) if(!exist(data)) return Position(NULL) ;
    Position p = root();
    while(1)
    {
        if(*p == data)
            return p;
        if(data < *p && p.hasLeftChild())
            p = p.left();
        else if(data > *p && p.hasRightChild())
            p = p.right();
        else
            return Position(NULL);

    }
}
