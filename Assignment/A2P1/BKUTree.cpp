
template <class K, class V>
class BKUTree {
public:
    class AVLTree;
    class SplayTree;
    class Entry {
    public:
        K key;
        V value;
        Entry(K key, V value) : key(key), value(value) {}
    };
public:
    AVLTree* avl;
    SplayTree* splay;
    queue<K> keys;
    int maxNumOfKeys;
public:
    BKUTree(int maxNumOfKeys = 5) {
        splay=new SplayTree();
        avl=new AVLTree();
        this->maxNumOfKeys=maxNumOfKeys;
    }
    ~BKUTree() { this-> clear();}
    void add(K key, V value);
    void remove(K key);
    V search(K key, vector<K>& traversedList);
    void traverseNLROnAVL(void (*func)(K key, V value));
    void traverseNLROnSplay(void (*func)(K key, V value));
    void clear();
    //additional function
    void printQUEUE();
    bool checkQUEUE(K key);
    class SplayTree {
    public:
        class Node {
        public:
            Entry* entry;
            Node* left;
            Node* right;
            typename AVLTree::Node* corr;
            Node(Entry* entry = NULL, Node* left = NULL, Node* right = NULL) {
                this->entry = entry;
                this->left = left;
                this->right = right;
                this->corr = NULL;
            }
            ~Node(){
                this->corr=NULL;
            }
        };
    public:
        Node* root;
        SplayTree(): root(NULL) {};
        ~SplayTree() { this->clear();};
        Node* add(K key, V value);
        Node* add(Entry* entry);
        bool remove(K key);
        V search(K key);
        void traverseNLR(void (*func)(K key, V value));
        void clear();
        //additional function
        Node* Insert(Node *pTrain,Node *newNode);
        Node* Splay(Node *pTrain,Entry entry,int &Count_Splay,bool isOnce);
        Node* rotateRight(Node *pNode);
        Node* rotateLeft(Node *pNode);
        Node *Findmax(Node *pNode);
        void Splay_NLR(void (*func)(K key,V value),Node *pTrain);
        Node* FindKeyOnTheQUEUE(Node *pTrain,K key,vector<K>& traversedList);
        Node* SearchWithoutSplay(Node *pTrain,K key);
        Node* DeleteAllNode(Node *pTrain);
        Node* removeForBKUTree(K key);
        
    };
    class AVLTree {
    public:
        class Node {
        public:
            Entry* entry;
            Node* left;
            Node* right;
            int balance;
            typename SplayTree::Node* corr;
            Node(Entry* entry = NULL, Node* left = NULL, Node* right = NULL) {
                this->entry = entry;
                this->left = left;
                this->right = right;
                this->balance = 0;
                this->corr = NULL;
            }
            ~Node(){
                this->corr=NULL;
            }
        };
    public:
        Node* root;
        AVLTree(): root(NULL) {};
        ~AVLTree() { this->clear(); };
        Node* add(K key, V value);
        Node* add(Entry* entry);
        void remove(K key);
        V search(K key);
        void traverseNLR(void (*func)(K key, V value));
        void clear();
        //additional functioin
        Node* rotateRight(Node *root);
        Node* rotateLeft(Node *root);
        Node* leftBalance(Node *&root,bool &taller);
        Node* rightBalance(Node *&root,bool &taller);
        Node* Insert(Node *&root,Node *newNode,bool &taller);
        void AVL_NLR(void (*func)(K key,V value),Node *pTrain);
        // delete
        Node* deleteRightBalance(Node *&root,bool &shorter);
        Node* deleteLeftBalance(Node *&root,bool &shorter);
        Node* AVLDelete(Node* root, K key,bool &shorter,bool &success);
        //search
        Node* SearchOnAVL(Node *pNode,K key,Node *desNode1, Node *desNode2,vector<K> &traversedList);
        Node* SearchForOnlyAVL(Node *pTrain,K key);
        Node* DeleteAllNode(Node *pTrain);
        void clearForBKUTree();
        Node* DeleteAllNodeForBKU(Node *pTrain);
    };
};  
//SplayTree
//rotate right
template <class K,class V>
typename BKUTree<K,V>::SplayTree::Node* BKUTree<K,V>::SplayTree::rotateRight(SplayTree::Node *pNode){
    SplayTree::Node *tmp=pNode->left;
    pNode->left=tmp->right;
    tmp->right=pNode;
    return tmp;
}
//rotate right
template <class K,class V>
typename BKUTree<K,V>::SplayTree::Node* BKUTree<K,V>::SplayTree::rotateLeft(SplayTree::Node *pNode){
    SplayTree::Node *tmp=pNode->right;
    pNode->right=tmp->left;
    tmp->left=pNode;
    return tmp;
}

//Find Node with largest key
template <class K,class V>
typename BKUTree<K,V>::SplayTree::Node* BKUTree<K,V>::SplayTree::Findmax(SplayTree::Node *pNode){
    if (pNode==NULL) return NULL;
    if (pNode->right==NULL) return pNode;
    return Findmax(pNode->right);
}
// Splay Tree
// isOnce=true: If the tree splay once
// Count_Splay: count number tree splay
template <class K,class V>
typename BKUTree<K,V>::SplayTree::Node* BKUTree<K,V>::SplayTree::Splay(BKUTree<K,V>::SplayTree::Node *pTrain,BKUTree<K,V>::Entry entry,int &Count_Splay,bool isOnce){
    if (pTrain!=NULL){
        if (entry.key<pTrain->entry->key)
            pTrain->left=Splay(pTrain->left,entry,Count_Splay,isOnce);
        else if (entry.key>pTrain->entry->key) 
            pTrain->right=Splay(pTrain->right,entry,Count_Splay,isOnce);
    }
    if (isOnce==true && Count_Splay==1) return pTrain;
    if (pTrain==NULL || pTrain->entry->key==entry.key) return pTrain;
    if (pTrain==this->root && pTrain->left!=NULL && pTrain->left->entry->key==entry.key){
            pTrain=rotateRight(pTrain);
            Count_Splay++;
    }else if (pTrain==this->root && pTrain->right!=NULL && pTrain->right->entry->key==entry.key){ 
            pTrain=rotateLeft(pTrain);
            Count_Splay++;
    }else if (pTrain->left!=NULL && pTrain->left->left!=NULL && pTrain->left->left->entry->key==entry.key){
        pTrain=rotateRight(pTrain);
        pTrain=rotateRight(pTrain);
        Count_Splay++;
    }else if (pTrain->left!=NULL && pTrain->left->right!=NULL && pTrain->left->right->entry->key==entry.key){
        pTrain->left=rotateLeft(pTrain->left);
        pTrain=rotateRight(pTrain);
        Count_Splay++;
    }else if (pTrain->right!=NULL && pTrain->right->right!=NULL && pTrain->right->right->entry->key==entry.key){
        pTrain=rotateLeft(pTrain);
        pTrain=rotateLeft(pTrain);
        Count_Splay++;
    }else if (pTrain->right!=NULL && pTrain->right->left!=NULL && pTrain->right->left->entry->key==entry.key){
        pTrain->right=rotateRight(pTrain->right);
        pTrain=rotateLeft(pTrain);
        Count_Splay++;
    }
    return pTrain;
}
//Insert the node into the Splay Tree
//note: duplicate! and delete node before throw exception
template <class K,class V>
typename BKUTree<K,V>::SplayTree::Node* BKUTree<K,V>::SplayTree::Insert(SplayTree::Node *pTrain,SplayTree::Node *newNode){
    if (pTrain==NULL){
        pTrain=newNode;
        return pTrain;
    }
    //throw exception
    if (newNode->entry->key==pTrain->entry->key) 
        throw "Duplicate key";
    if (newNode->entry->key<pTrain->entry->key)
        pTrain->left=Insert(pTrain->left,newNode);
    else if (newNode->entry->key>pTrain->entry->key)
        pTrain->right=Insert(pTrain->right,newNode);
    return pTrain;
}
//function add the node into the Splay Tree
template <class K,class V>
typename BKUTree<K,V>::SplayTree::Node* BKUTree<K,V>::SplayTree::add(BKUTree<K,V>::Entry* entry){
    //create new node and call function Insert node into the Splay tree (without Splay the tree)
    BKUTree<K,V>::SplayTree::Node *newNode=new SplayTree::Node(entry,NULL,NULL);
    this->root=BKUTree<K,V>::SplayTree::Insert(BKUTree<K,V>::SplayTree::root,newNode);
    //Splay the tree
    BKUTree<K,V>::SplayTree::Node *pTrain=BKUTree<K,V>::SplayTree::root;
    int Count_Splay=0;
    BKUTree<K,V>::SplayTree::root=SplayTree::Splay(pTrain,*entry,Count_Splay,false);
    //reset pTrain=NULL
    pTrain=NULL;
    return newNode;
}
//if parameters are not Entry*, call function add(Entry* entry)
template <class K,class V>
typename BKUTree<K,V>::SplayTree::Node* BKUTree<K,V>::SplayTree::add(K key,V value){
    BKUTree<K,V>::Entry *entry=new BKUTree<K,V>::Entry(key,value);
    return SplayTree::add(entry);
}
//traversal NLR
template <class K,class V>
void BKUTree<K,V>::SplayTree::Splay_NLR(void (*func)(K key,V value),SplayTree::Node *pTrain){
    //check if the tree is empty!
    if (pTrain!=NULL){
        //add must delete
        //cout<<pTrain->corr->entry->key<<endl;
        (*func)(pTrain->entry->key,pTrain->entry->value);
        Splay_NLR(func,pTrain->left);
        Splay_NLR(func,pTrain->right);
    }
}

// call function Splay_NLR
template <class K,class V>
void BKUTree<K,V>::SplayTree::traverseNLR(void (*func)(K , V )){
    BKUTree<K,V>::SplayTree::Splay_NLR(func,BKUTree<K,V>::SplayTree::root);
}
//removing the node from Splay tree
//note exception
template <class K,class V>
bool BKUTree<K,V>::SplayTree::remove(K key){
    //if node is not found, throw "Not found"
    auto *pFind=this->SearchWithoutSplay(this->root,key);
    if (pFind==NULL)
        throw "Not found";
    //delete the node from the tree after bringing the node to the root
    //set pTrain is root
    Node *pTrain=this->root;
    //set the noce is about to delete
    int Count_Splay=0;
    Entry entry(key,0);
    //bring the node to the root
    root=Splay(pTrain,entry,Count_Splay,false);
    //if root is NULL or not found
    //note exception
    
    //delete node
    Count_Splay=0;
    Node *tmp=root,*subRight=root->right;
    if (root->left==NULL && root->right==NULL){
        //if the tree have only root
        if (tmp->entry!=NULL)
            delete tmp->entry;
        tmp->entry=NULL;
        delete tmp;
        tmp=NULL;
        root=NULL;
        return true;
    }else if ((root->left!=NULL) xor (root->right!=NULL)){
        //if right subtree is empty or left subtree is empty
        root=root->left?root->left:root->right;
        if (tmp->entry!=NULL)
            delete tmp->entry;
        tmp->entry=NULL;
        delete tmp;
        tmp=NULL;
        return true;
    }
    //if root have both subtree
    root=root->left;
    if (tmp->entry!=NULL)
        delete tmp->entry;
    tmp->entry=NULL;
    delete tmp;
    tmp=NULL;
    //find the node have max key on the left subtree
    pTrain=root;
    Node *MaxNodeSubRight=Findmax(pTrain);
    //Splay MaxNodeSubright to root
    Count_Splay=0;
    if (pTrain!=NULL)
        root=Splay(pTrain,*(MaxNodeSubRight->entry),Count_Splay,false);
    root->right=subRight;
    //reset pTrain=NULL,tmp=NULL,subRight=NULL
    pTrain=NULL;
    tmp=NULL;
    subRight=NULL;
    MaxNodeSubRight=NULL;
    return true;    
}
template <class K,class V>
typename BKUTree<K,V>::SplayTree::Node* BKUTree<K,V>::SplayTree::removeForBKUTree(K key){
    //if node is not found, throw "Not found"
    auto *pFind=this->SearchWithoutSplay(this->root,key);
    if (pFind==NULL)
        throw "Not found";
    //delete the node from the tree after bringing the node to the root
    //set pTrain is root
    Node *pTrain=this->root;
    //set the noce is about to delete
    int Count_Splay=0;
    Entry entry(key,0);
    //bring the node to the root
    root=Splay(pTrain,entry,Count_Splay,false);
    //if root is NULL or not found
    //note exception
    
    //delete node
    Count_Splay=0;
    Node *tmp=root,*subRight=root->right;
    if (root->left==NULL && root->right==NULL){
        //if the tree have only root
        //if (tmp->entry!=NULL)
          //  delete tmp->entry;
        //tmp->entry=NULL;
        //delete tmp;
        //tmp=NULL;
        this->root=NULL;
        return tmp;
    }else if ((root->left!=NULL) xor (root->right!=NULL)){
        //if right subtree is empty or left subtree is empty
        root=root->left?root->left:root->right;
        //if (tmp->entry!=NULL)
          //  delete tmp->entry;
        //tmp->entry=NULL;
        //delete tmp;
        //tmp=NULL;
        return tmp;
    }
    //if root have both subtree
    root=root->left;
    //if (tmp->entry!=NULL)
    //    delete tmp->entry;
    //tmp->entry=NULL;
    //delete tmp;
    //tmp=NULL;
    //find the node have max key on the left subtree
    pTrain=root;
    Node *MaxNodeSubRight=Findmax(pTrain);
    //Splay MaxNodeSubright to root
    Count_Splay=0;
    if (pTrain!=NULL)
        root=Splay(pTrain,*(MaxNodeSubRight->entry),Count_Splay,false);
    root->right=subRight;
    //reset pTrain=NULL,tmp=NULL,subRight=NULL
    pTrain=NULL;
    //tmp=NULL;
    subRight=NULL;
    MaxNodeSubRight=NULL;
    return tmp;    
}
//if the node have key on the queue, call function FindKeyOnTheQueue to find the node on the tree
template <class K,class V>
typename BKUTree<K,V>::SplayTree::Node* BKUTree<K,V>::SplayTree::FindKeyOnTheQUEUE(Node *pTrain,K key,vector<K>& traversedList){
    if (pTrain==NULL) return NULL;
    if (key==pTrain->entry->key) return pTrain;
    //push key onto traversedList
    traversedList.push_back(pTrain->entry->key);
    if (key<pTrain->entry->key)
        return BKUTree<K,V>::SplayTree::FindKeyOnTheQUEUE(pTrain->left,key,traversedList);
    else return BKUTree<K,V>::SplayTree::FindKeyOnTheQUEUE(pTrain->right,key,traversedList);
};
//using Function SplayTree::Splay to Splay the node is searched to the root
//if the node is not finded, throw "Not Found"
template <class K,class V>
typename BKUTree<K,V>::SplayTree::Node* BKUTree<K,V>::SplayTree::SearchWithoutSplay(Node* pTrain,K key){
    if (pTrain==NULL || pTrain->entry->key==key) return pTrain;
    if (key<pTrain->entry->key)
        return SearchWithoutSplay(pTrain->left,key);
    else return SearchWithoutSplay(pTrain->right,key);    
    return NULL;
}

template <class K,class V>
V BKUTree<K,V>::SplayTree::search(K key){
    BKUTree<K,V>::SplayTree::Node* pNode=SearchWithoutSplay(this->root,key);
    if (pNode==NULL){
        //throw exception
            throw "Not found";
    }
    int Count_Splay=0;
    this->root=this->Splay(this->root,*pNode->entry,Count_Splay,false);
    return pNode->entry->value;   
}
//delete Node in Splay Tree:
template <class K,class V>
typename BKUTree<K,V>::SplayTree::Node* BKUTree<K,V>::SplayTree::DeleteAllNode(BKUTree<K,V>::SplayTree::Node *pTrain){
    if (pTrain==NULL) return pTrain;
    if (pTrain->left)
        pTrain->left=SplayTree::DeleteAllNode(pTrain->left);
    if (pTrain->right) 
        pTrain->right=SplayTree::DeleteAllNode(pTrain->right);
    //delete all pointer in class Splay::Node
    if (pTrain->entry!=NULL)
        delete pTrain->entry;
    pTrain->entry=NULL;
    delete pTrain;
    pTrain=NULL;
    return pTrain;
}
template <class K,class V>
void BKUTree<K,V>::SplayTree::clear(){
    //must be delete
    this->root=SplayTree::DeleteAllNode(this->root);
}
//// AVL Tree

template <class K,class V>
typename BKUTree<K,V>::AVLTree::Node* BKUTree<K,V>::AVLTree::rotateRight(AVLTree::Node *root){
    BKUTree<K,V>::AVLTree::Node *tmpRoot=root->left;
    root->left=tmpRoot->right;
    tmpRoot->right=root;
    return tmpRoot;
}
template <class K,class V>
typename BKUTree<K,V>::AVLTree::Node* BKUTree<K,V>::AVLTree::rotateLeft(AVLTree::Node *root){
    BKUTree<K,V>::AVLTree::Node *tmpRoot=root->right;
    root->right=tmpRoot->left;
    tmpRoot->left=root;
    return tmpRoot;
}
template <class K,class V>
typename BKUTree<K,V>::AVLTree::Node* BKUTree<K,V>::AVLTree::leftBalance(AVLTree::Node *&root,bool &taller){
    BKUTree<K,V>::AVLTree::Node* leftTree=root->left;
    if (leftTree->balance==-1){
        root=rotateRight(root);
        root->balance=0;
        root->right->balance=0;
        taller=0;
    }else{
        Node* rightTree=leftTree->right;
        if (rightTree->balance==-1){
            root->balance=1; 
            leftTree->balance=0;
        }else if (rightTree->balance==0){
            root->balance=0;
            leftTree->balance=0;
        }else{
            root->balance=0;
            leftTree->balance=-1;
        }
        rightTree->balance=0;
        root->left=rotateLeft(leftTree);
        root=rotateRight(root);
        taller=false;
        rightTree=NULL;
    }
    //set leftTree==NULL
    leftTree=NULL;
    return root;
};
template <class K,class V>
typename BKUTree<K,V>::AVLTree::Node* BKUTree<K,V>::AVLTree::rightBalance(AVLTree::Node *&root,bool &taller){
    BKUTree<K,V>::AVLTree::Node *rightTree=root->right;
    if (rightTree->balance==1){
        root=rotateLeft(root);
        root->balance=0;
        root->left->balance=0;
        taller=0;
    }else{
        Node *leftTree=rightTree->left;
        if (leftTree->balance==1){
            root->balance=-1;
            rightTree->balance=0;
        }else if (leftTree->balance==0){
            root->balance=0;
            rightTree->balance=0;
        }else{
            root->balance=0;
            rightTree->balance=1;
        }
        leftTree->balance=0;
        root->right=rotateRight(rightTree);
        root=rotateLeft(root);
        taller=false;
        //set leftTree=NULL
        leftTree=NULL;
    }
    //set rightTree=NULL;
    rightTree=NULL;
    return root;
};
template <class K,class V>
typename BKUTree<K,V>::AVLTree::Node* BKUTree<K,V>::AVLTree::Insert(Node *&root,Node *newNode,bool &taller){
    if (root==NULL){
        root=newNode;
        taller=true;
        return root;
    }
    //if Node is exist, throw "Duplicate key"
    if (newNode->entry->key==root->entry->key)
        throw "Duplicate key";
    if (newNode->entry->key<root->entry->key){
        root->left=Insert(root->left,newNode,taller);
        if (taller){
            if (root->balance==-1)
                root=leftBalance(root,taller);
            else if (root->balance==0)
                root->balance=-1;
            else{
                root->balance=0;
                taller=false;
            }
        }
    }else{
        root->right=Insert(root->right,newNode,taller);
        if (taller){
            if (root->balance==1)
                root=rightBalance(root,taller);
            else if (root->balance==0)
                root->balance=1;
            else{
                root->balance=0;
                taller=false;
            }
        }
    }
    return root;
}
// Add the node into Splay Tree
template <class K,class V>
typename BKUTree<K,V>::AVLTree::Node* BKUTree<K,V>::AVLTree::add(K key,V value){
    AVLTree::Node* pTrain=SearchForOnlyAVL(this->root,key);
    if (pTrain!=NULL)
        throw "Duplicate key";
    Entry *entry=new Entry(key,value);
    AVLTree::Node *newNode=new Node(entry,NULL,NULL);
    bool taller=false;
    this->root=Insert(this->root,newNode,taller);
    return newNode;
}
// call Splay::add(key,value) function
template <class K,class V>
typename BKUTree<K,V>::AVLTree::Node* BKUTree<K,V>::AVLTree::add(Entry *entry){
    AVLTree::Node* pTrain=SearchForOnlyAVL(this->root,entry->key);
    if (pTrain!=NULL)
        throw "Duplicate key";
    AVLTree::Node *newNode=new Node(entry,NULL,NULL);
    bool taller=false;
    this->root=Insert(this->root,newNode,taller);
    return newNode;
}

template <class K,class V>
void BKUTree<K,V>::AVLTree::AVL_NLR(void (*func)(K key,V value),AVLTree::Node *pTrain){
    if (pTrain!=NULL){
        ///must delete
        //cout<<pTrain->corr->entry->key<<endl;
        (*func)(pTrain->entry->key,pTrain->entry->value);
        AVL_NLR(func,pTrain->left);
        AVL_NLR(func,pTrain->right);
    }
}
//preoder
template <class K,class V>
void BKUTree<K,V>::AVLTree::traverseNLR(void (*func)(K , V )){
    BKUTree<K,V>::AVLTree::AVL_NLR(func,BKUTree<K,V>::AVLTree::root);
}

//delete node in AVLtree

template <class K,class V>
typename BKUTree<K,V>::AVLTree::Node* BKUTree<K,V>::AVLTree::deleteRightBalance(BKUTree<K,V>::AVLTree::Node *&root,bool &shorter){
    if (root->balance== -1)
        root->balance= 0;
    else if (root->balance== 0){
        root->balance= 1;
        shorter=false;
    }else{
        Node *rightTree=root->right;
        if (rightTree->balance== -1){
            Node *leftTree=rightTree->left;
            if (leftTree->balance== -1){
                rightTree->balance= 1;
            root->balance= 0;
            }else if (leftTree->balance== 0){
                root->balance= 0;
                rightTree->balance= 0;
            }else{
                root->balance= -1;
                rightTree->balance= 0;
            }
            leftTree->balance= 0;
            root->right=rotateRight(rightTree);
            root=rotateLeft(root);
            //set leftTree=NULL
            leftTree=NULL;
        }else{
            if (rightTree->balance!= 0){
                root->balance= 0;
                rightTree->balance= 0;
            }else{
                root->balance= 1;
                rightTree->balance= -1;
                shorter=false;
            }
            root=rotateLeft(root);
        }
        //set rightTree=NULL
        rightTree=NULL;
    }
    return root;
}
template<class K,class V>
typename BKUTree<K,V>::AVLTree::Node* BKUTree<K,V>::AVLTree::deleteLeftBalance(Node *&root,bool &shorter){
    if (root->balance== 1)
        root->balance= 0;
    else if (root->balance== 0){
        root->balance= -1;
        shorter=false;
    }else{
        Node *leftTree=root-> left;
        if (leftTree->balance== 1){
            Node* rightTree=leftTree->right;
            if (rightTree->balance== 1){
                leftTree->balance= -1;
                root->balance= 0;
            }else if (rightTree->balance==0){
                root->balance= 0;
                leftTree->balance= 0;
            }else{
                root->balance= 1;
                leftTree->balance= 0;
            }
            rightTree->balance= 0;
            root->left=rotateLeft(leftTree);
            root=rotateRight(root);
            //set rightTree=NULL
            rightTree=NULL;
        }else{
            if (leftTree->balance!= 0){
                root->balance= 0;
                leftTree->balance= 0;
            }else{
                root->balance= -1;
                leftTree->balance= 1;
                shorter=false;
            }
            root=rotateRight(root);
        }
        //set leftTree=NULL;
        leftTree=NULL;
    }
    return root;
}
template <class K,class V>
typename BKUTree<K,V>::AVLTree::Node* BKUTree<K,V>::AVLTree::AVLDelete(Node* root, K key,bool &shorter,bool &success){
    if (root==NULL){
        shorter=false;
        success=false;
        return NULL;
    }
    if (key<root->entry->key){
        root->left=AVLDelete(root->left,key,shorter,success);
        if (shorter)
            root=deleteRightBalance(root,shorter);
    }else if (key>root->entry->key){
        root->right=AVLDelete(root->right,key,shorter,success);
        if (shorter){
            root=deleteLeftBalance(root,shorter);
        }
    }else{
        Node* deleteNode=root;
        if (root->right==NULL){
            Node* newRoot=root->left;
            success=true;
            shorter=true;
            if (deleteNode!=NULL){
                if (deleteNode->entry!=NULL)
                    delete deleteNode->entry;
                deleteNode->entry=NULL;
            }
            delete deleteNode;
            deleteNode=NULL;
            return newRoot;
        }else if (root->left==NULL){
            Node* newRoot=root->right;
            success=true;
            shorter=true;
            if (deleteNode!=NULL){
                if (deleteNode->entry!=NULL)
                    delete deleteNode->entry;
                deleteNode->entry=NULL;
            }
            delete deleteNode;
            deleteNode=NULL;
            return newRoot;
        }else{
            Node *exchPtr=root->left;
            while (exchPtr->right!=NULL){
                exchPtr=exchPtr->right;
            }
            //
            Entry *ExtraEntry=root->entry;
            root->entry=exchPtr->entry; //phai gan lai de co het gia tri remember
            exchPtr->entry=ExtraEntry;
            *(exchPtr->entry)=*(root->entry);
            //swap correspond
            //auto tempCorr=root->corr;
            //swao corr
            auto x=root->corr;
            auto y=exchPtr->corr;
            
            root->corr=y;
            if (y!=NULL)
            y->corr=root;

            exchPtr->corr=x;
            if (x!=NULL)
            x->corr=exchPtr;
            //exchPtr->corr=tempCorr;
            //tempCorr=NULL;
            ExtraEntry=NULL;
            x=NULL;
            y=NULL;
            //
            
            root->left=AVLDelete(root->left,exchPtr->entry->key,shorter,success);
            if (shorter){
                root=deleteRightBalance(root,shorter);
            }
            //set exchPtr=NULL
            exchPtr=NULL;
        }
        //set deleteNode=NULL
        deleteNode=NULL;
    }
    return root;
}
//Search only AVL
template <class K,class V>
typename BKUTree<K,V>::AVLTree::Node* BKUTree<K,V>::AVLTree::SearchForOnlyAVL(Node *pTrain,K key){
    if (pTrain==NULL || pTrain->entry->key==key)
        return pTrain;
    if (key<pTrain->entry->key)
        return SearchForOnlyAVL(pTrain->left,key);
    else return SearchForOnlyAVL(pTrain->right,key);    
}
template <class K,class V>
V BKUTree<K,V>::AVLTree::search(K key){
    AVLTree::Node* pTrain=SearchForOnlyAVL(this->root,key);
    if (pTrain==NULL)
        throw "Not found";
    V RES=pTrain->entry->value;
    return RES;
}
//remove node onto the tree
template <class K,class V>
void BKUTree<K,V>::AVLTree::remove(K key){
        bool shorter=false;
        bool success=false;
        this->root=AVLDelete(root,key,shorter,success);
};
//delete all Node in AVLTree:
template <class K,class V>
typename BKUTree<K,V>::AVLTree::Node* BKUTree<K,V>::AVLTree::DeleteAllNode(BKUTree<K,V>::AVLTree::Node *pTrain){
    if (pTrain==NULL) return pTrain;
    if (pTrain->left)
        pTrain->left=AVLTree::DeleteAllNode(pTrain->left);
    if (pTrain->right)
        pTrain->right=AVLTree::DeleteAllNode(pTrain->right);
    //delete all pointer in class AVL::Node
    if (pTrain->entry!=NULL)
        delete pTrain->entry;
    pTrain->entry=NULL;
    delete pTrain;
    pTrain=NULL;
    return pTrain;
}
template <class K,class V>
void BKUTree<K,V>::AVLTree::clear(){
    this->root=AVLTree::DeleteAllNode(this->root);
}
//for BKUTree
template <class K,class V>
typename BKUTree<K,V>::AVLTree::Node* BKUTree<K,V>::AVLTree::DeleteAllNodeForBKU(BKUTree<K,V>::AVLTree::Node *pTrain){
    if (pTrain==NULL) return pTrain;
    if (pTrain->left)
        pTrain->left=AVLTree::DeleteAllNodeForBKU(pTrain->left);
    if (pTrain->right)
        pTrain->right=AVLTree::DeleteAllNodeForBKU(pTrain->right);
    //delete all pointer in class AVL::Node
    pTrain->entry=NULL;
    delete pTrain;
    pTrain=NULL;
    return pTrain;
}
template <class K,class V>
void BKUTree<K,V>::AVLTree::clearForBKUTree(){
    this->root=AVLTree::DeleteAllNodeForBKU(this->root);
}
//BKUTree
//implement
template <class K,class V>
void BKUTree<K,V>::add(K key,V value){
    if (int(keys.size())==maxNumOfKeys) keys.pop();
    keys.push(key);
    //check key duplicate 
    auto *pFind=this->splay->SearchWithoutSplay(this->splay->root,key);
    if (pFind!=NULL)
        throw "Duplicate key";
    Entry *entry=new Entry(key,value);
    auto x=this->splay->add(entry);
    auto y=this->avl->add(entry);
    x->corr=y;
    y->corr=x;
    //set x=NULL,y=NULL
    x=NULL;
    y=NULL;
    pFind=NULL;
}
template <class K,class V>
void BKUTree<K,V>::remove(K key){
    int isExist=false;
    queue<K> tmpkeys;
    while (!keys.empty()){
        if (keys.front()!=key)
            tmpkeys.push(keys.front());
        else isExist=true;
        keys.pop();
    }                                                                                                                                                                                                                                                                                  
    keys=tmpkeys;
    //If the node isn't found
    auto pNode=this->splay->SearchWithoutSplay(this->splay->root,key);
    if (pNode==NULL) throw "Not found";
    //ELSE
    auto tmp=this->splay->removeForBKUTree(key);
    this->avl->remove(key);
    //push root of Splay Tree into keys
    if (isExist && this->splay->root!=NULL){
        if (int(keys.size())==maxNumOfKeys)
            keys.pop();
        keys.push(this->splay->root->entry->key);
    }
    //delete tmp
    tmp->entry=NULL;
    delete tmp;
    tmp=NULL;    
}
template <class K,class V>
bool BKUTree<K,V>::checkQUEUE(K key){
    queue<K> tmp;
    tmp=this->keys;
    while (!tmp.empty()){
        if (tmp.front()==key)
            return true;
        tmp.pop();
    }
    return false;
}
template <class K,class V>
void BKUTree<K,V>::printQUEUE(){
    cout<<"\nprint queue:";
    queue<K> tmp;
    tmp=this->keys;
    while (!tmp.empty()){
        cout<<tmp.front()<<" ";
        tmp.pop();
    }
    cout<<endl;
}
template <class K,class V>
V BKUTree<K,V>::search(K key, vector<K>& traversedList){
    if (this->splay->root==NULL) throw "Not found";
    if (this->splay->root!=NULL && this->splay->root->entry->key==key){
        if (int(keys.size())==this->maxNumOfKeys)
        //push before return
        keys.pop();
        keys.push(key);
        return this->splay->root->entry->value;
    }
    if (this->checkQUEUE(key)){
        auto *pNode=this->splay->FindKeyOnTheQUEUE(this->splay->root,key,traversedList);
        if (pNode==NULL)
            throw "Not found";
        int CountSplay=0;
        auto *pTrain=this->splay->root;
        //if traversed reset then 
        this->splay->root=this->splay->Splay(pTrain,*(pNode->entry),CountSplay,true);
        pTrain=NULL;
        //push before return
        if (int(keys.size())==this->maxNumOfKeys)
            keys.pop();
        keys.push(key);
        V RES=pNode->entry->value;
        pNode=NULL;
        return RES;
    }
    //reference splay->root to AVLTree
    auto *RootRef=this->splay->root->corr;
    //Search on AVL from Rootref
    auto *FindNode=this->avl->SearchOnAVL(RootRef,key,NULL,NULL,traversedList);
    if (FindNode!=NULL) {
        //push before return
        if (int(keys.size())==this->maxNumOfKeys)
            keys.pop();
        keys.push(key);
        //splay once 
        int Count_Splay=0;
        auto *pTrain=this->splay->root;
        this->splay->root=this->splay->Splay(pTrain,*(FindNode->entry),Count_Splay,true);
        //set FindNode=NULL;
        V RES=FindNode->entry->value;
        FindNode=NULL;
        return RES;
    }
    //if node isn't found, Search from root of avl
    auto *pRoot=this->avl->root;
    FindNode=this->avl->SearchOnAVL(pRoot,key,NULL,RootRef,traversedList);
    
    if (FindNode==NULL) throw "Not found";
    int Count_Splay=0;
    auto *pTrain=this->splay->root;
    this->splay->root=this->splay->Splay(pTrain,*(FindNode->entry),Count_Splay,true);
    //
    if (int(keys.size())==this->maxNumOfKeys)
        keys.pop();
    keys.push(key);
    //keys.push(FindNode->entry->key);
    
    V RES=FindNode->entry->value;
    //set NULL
    RootRef=NULL;
    FindNode=NULL;
    pRoot=NULL;
    pTrain=NULL;
    return RES;
};
template <class K,class V>
typename BKUTree<K,V>::AVLTree::Node* BKUTree<K,V>::AVLTree::SearchOnAVL(BKUTree<K,V>::AVLTree::Node *pNode,K key,BKUTree<K,V>::AVLTree::Node *desNode1, BKUTree<K,V>::AVLTree::Node *desNode2,vector<K> &traversedList){
    if (pNode!=NULL && pNode->entry->key!=key) traversedList.push_back(pNode->entry->key);
    if (pNode==desNode1 || desNode2!=NULL && pNode->entry->key==desNode2->entry->key) return NULL;
    if (key<pNode->entry->key)
        return SearchOnAVL(pNode->left,key,desNode1,desNode2,traversedList);
    else if (key>pNode->entry->key)
        return SearchOnAVL(pNode->right,key,desNode1,desNode2,traversedList);
    return pNode;
}
//call function traversal on Splay tree
template <class K,class V>
void  BKUTree<K,V>::traverseNLROnSplay(void (*func)(K key, V value)){
    if (this->splay==NULL) return;
    this->splay->traverseNLR(func);
}
template <class K,class V>
void  BKUTree<K,V>::traverseNLROnAVL(void (*func)(K key, V value)){
    if (this->avl==NULL) return;
    this->avl->traverseNLR(func);
}
template <class K,class V>
void BKUTree<K,V>::clear(){
    this->splay->clear();
    this->avl->clearForBKUTree();
    delete splay;
    splay=NULL;
    delete avl;
    avl=NULL;
}