// C++ program to insert a node in AVL tree 
#include<bits/stdc++.h> 
using namespace std; 

// An AVL tree for find maximum of a point.
class AVL1{
    public:
    class Node { 
	public: 
	int key; 
	Node *left; 
	Node *right;
    int number; 
    int totalNumber;
	int height; 
    }; 

// height of the tree 
    int height(Node *N) 
    { 
	    if (N == NULL) 
		    return 0; 
	    return N->height; 
    } 
// A utility function to get maximum 
// of two integers 
int max(int a, int b) 
{ 
	return (a > b)? a : b; 
} 

/* Helper function that allocates a 
new node with the given key and 
NULL left and right pointers. */
Node* newNode(int key) 
{ 
	Node* node = new Node(); 
	node->key = key; 
	node->left = NULL; 
	node->right = NULL; 
	node->height = 1; // new node is initially // added at leaf 
    node->number=1; //the number of points with the same key in the node
    node->totalNumber=1; //the number of points in tree with root is node
	return(node); 
} 

//rotate right
Node *rightRotate(Node *y) 
{ 
	Node *x = y->left; 
	Node *T2 = x->right; 

	// Perform rotation 
	x->right = y; 
	y->left = T2; 

	// Update heights 
	y->height = max(height(y->left), 
					height(y->right)) + 1; 
	x->height = max(height(x->left), 
					height(x->right)) + 1; 

	// Return new root 
	return x; 
} 

//rotate left
Node *leftRotate(Node *x) 
{ 
	Node *y = x->right; 
	Node *T2 = y->left; 

	// Perform rotation 
	y->left = x; 
	x->right = T2; 

	// Update heights 
	x->height = max(height(x->left),	 
					height(x->right)) + 1; 
	y->height = max(height(y->left), 
					height(y->right)) + 1; 

	// Return new root 
	return y; 
} 

// Get Balance factor of node N 
int getBalance(Node *N) 
{ 
	if (N == NULL) 
		return 0; 
	return height(N->left) - height(N->right); 
} 

//get N->totalNumber of Node
int getTotalNumber(Node *N){
    if (N==NULL)
        return 0;
    return N->totalNumber;
}
// Update number points of tree with root is node
void UpdateTotalNumber(Node *returnNode){
    if (returnNode->left!=NULL){
        returnNode->left->totalNumber=returnNode->left->number+getTotalNumber(returnNode->left->left)+getTotalNumber(returnNode->left->right);
    }
    if (returnNode->right!=NULL){
        returnNode->right->totalNumber=returnNode->right->number+getTotalNumber(returnNode->right->left)+getTotalNumber(returnNode->right->right);
    }
    returnNode->totalNumber=returnNode->number+getTotalNumber(returnNode->left)+getTotalNumber(returnNode->right);
}
//insert
Node* insert(Node* node, int key) 
{ 
	/* 1. Perform the normal BST insertion */
	if (node == NULL) 
		return(newNode(key)); 

	if (key < node->key) 
		node->left = insert(node->left, key); 
	else if (key > node->key) 
		node->right = insert(node->right, key); 
	else // Equal keys are not allowed in BST 
        {
            node->number=node->number+1; 
            node->totalNumber++;
            return node;
        }
    node->totalNumber=node->number+getTotalNumber(node->left)+getTotalNumber(node->right);
	/* 2. Update height of this ancestor node */
	node->height = 1 + max(height(node->left), 
						height(node->right)); 

	/* 3. Get the balance factor of this ancestor 
		node to check whether this node became 
		unbalanced */
	int balance = getBalance(node); 

	// If this node becomes unbalanced, then 
	// there are 4 cases 


    //update total points of node after rotate
	// Left Left Case 
	if (balance > 1 && key < node->left->key){
        Node* returnNode=rightRotate(node);
        UpdateTotalNumber(returnNode);
        return returnNode;

    } 
		

	// Right Right Case 
	if (balance < -1 && key > node->right->key){
        Node* returnNode=leftRotate(node);
        UpdateTotalNumber(returnNode);
        return returnNode;
    } 
        

	// Left Right Case 
	if (balance > 1 && key > node->left->key) 
	{ 
		node->left = leftRotate(node->left); 
		Node* returnNode=rightRotate(node); 
        UpdateTotalNumber(returnNode);
        return returnNode;
	} 

	// Right Left Case 
	if (balance < -1 && key < node->right->key) 
	{ 
		node->right = rightRotate(node->right); 
		Node* returnNode=leftRotate(node); 
        UpdateTotalNumber(returnNode);
        return returnNode;
	} 

	/* return the (unchanged) node pointer */
	return node; 
} 

//traverse preOrder
void preOrder(Node *root) 
{ 
	if(root != NULL) 
	{ 
		cout << root->key << " "; 
		preOrder(root->left); 
		preOrder(root->right); 
	} 
} 
//minValue of tree with root is node
Node * minValueNode(Node* node)  
{  
    Node* current = node;  
  
    /* loop down to find the leftmost leaf */
    while (current->left != NULL)  
        current = current->left;  
  
    return current;  
}  
  
// Recursive function to delete a node  
// with given key from subtree with  
// given root. It returns root of the  
// modified subtree.  
Node* deleteNode(Node* root, int key)  
{  
      
    // STEP 1: PERFORM STANDARD BST DELETE  
    if (root == NULL)  
        return root;  
  
    // If the key to be deleted is smaller  
    // than the root's key, then it lies 
    // in left subtree  
    if ( key < root->key )  
        root->left = deleteNode(root->left, key);  
  
    // If the key to be deleted is greater  
    // than the root's key, then it lies  
    // in right subtree  
    else if( key > root->key )  
        root->right = deleteNode(root->right, key);  
  
    // if key is same as root's key, then  
    // This is the node to be deleted  
    else
    {  
        // node with only one child or no child
        root->number--;
        if (root->number==0){
            if( (root->left == NULL) || (root->right == NULL) ) {  
                Node *temp = root->left ?  root->left :  root->right;  
  
            // No child case  
                if (temp == NULL)  
                {  
                    temp = root;  
                    root = NULL;  
                }  
                else // One child case  
                    *root = *temp; // Copy the contents of  
                           // the non-empty child  
                delete(temp);
                temp=NULL;  
            }  
            else
            {  
            // node with two children: Get the inorder  
            // successor (smallest in the right subtree)  
                Node* temp = minValueNode(root->right);  
  
            // Copy the inorder successor's  
            // data to this node  
                root->key = temp->key;
                root->number=temp->number;  
                //set temp->number=1 to delete
                temp->number=1;

            // Delete the inorder successor  
                root->right = deleteNode(root->right,  
                                     temp->key);  
            } 
        }  
         
    }  
  
    // If the tree had only one node 
    // then return  
    if (root == NULL)  
    return root;  
    //update total points of tree with root is node
    root->totalNumber=root->number+getTotalNumber(root->left)+getTotalNumber(root->right);
    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE  
    root->height = 1 + max(height(root->left),  
                           height(root->right));  
    
    // STEP 3: GET THE BALANCE FACTOR OF  
    // THIS NODE (to check whether this  
    // node became unbalanced)  
    int balance = getBalance(root);  
  
    // If this node becomes unbalanced,  
    // then there are 4 cases  
    // update total points of tree after rotate
    // Left Left Case  
    if (balance > 1 &&  getBalance(root->left) >= 0){
        Node* returnNode=rightRotate(root);
        UpdateTotalNumber(returnNode);
        return returnNode;
    }  
  
    // Left Right Case  
    if (balance > 1 &&  getBalance(root->left) < 0)  
    {  
        root->left = leftRotate(root->left); 
		Node* returnNode=rightRotate(root); 
        UpdateTotalNumber(returnNode);
        return returnNode;  
    }  
  
    // Right Right Case  
    if (balance < -1 &&  getBalance(root->right) <= 0){
        Node* returnNode=leftRotate(root);
        UpdateTotalNumber(returnNode);
        return returnNode;
    }  
        
  
    // Right Left Case  
    if (balance < -1 &&  getBalance(root->right) > 0)  
    {  
        root->right = rightRotate(root->right); 
		Node* returnNode=leftRotate(root); 
        UpdateTotalNumber(returnNode);
        return returnNode; 
    }  
    return root;  
}  
// get lower bound
int GetIndexLowerBound(Node *node,int So){
    if (node==NULL) return 0;
    if (So==node->key)
        return getTotalNumber(node->left);
    if (So>node->key)
        return getTotalNumber(node->left)+node->number+GetIndexLowerBound(node->right,So);
    return GetIndexLowerBound(node->left,So);
}
// Search key=so
bool Search(Node *root,int so){
    if (root==NULL) return false;
    if (root->key==so) return true;
    if (so>root->key) return Search(root->right,so);
    return Search(root->left,so);
}
// traverse LNR
void LNR(Node *root){
    if (root!=NULL){
        LNR(root->left);
        cout<<root->key<<" "<<root->totalNumber<<endl;
        LNR(root->right);
    }
}
Node *root=NULL; //declare root=NULL
};

////////////////////////////////////
////////////////////////////////////
//////////////////////////
// 
//AVL2 for insert,delete, update
class AVL2{
    public:
    class Node 
{ 
	public: 
	int key; 
	Node *left; 
	Node *right;
    int number; //the number points of node
	int height; //height of node
    int keyMax; //maximum key of tree with root is node
    int keyMIn; ///minimum key of tree with root is node
    int remainder; //remainder for lazy update
    int MaxNumber; //The maximum number of points a node in the tree without remainder
}; 
// height of the tree 
int height(Node *N) 
{ 
	if (N == NULL) 
		return 0; 
	return N->height; 
} 

// A utility function to get maximum 
// of two integers 
int max(int a, int b) 
{ 
	return (a > b)? a : b; 
} 

/* Helper function that allocates a 
new node with the given key and 
NULL left and right pointers. */
Node* newNode(int key,int number=1) 
{ 
	Node* node = new Node(); 
	node->key = key; 
	node->left = NULL; 
	node->right = NULL; 
	node->height = 1; // new node is initially 
					// added at leaf 
    node->number=number;
    node->keyMax=node->key; 
    node->keyMIn=node->key;
    node->remainder=0;
    node->MaxNumber=number;
	return(node); 
} 

// A utility function to right 
// rotate subtree rooted with y 
// See the diagram given above. 
Node *rightRotate(Node *y) 
{ 
	Node *x = y->left; 
	Node *T2 = x->right; 

	// Perform rotation 
	x->right = y; 
	y->left = T2; 

	// Update heights 
	y->height = max(height(y->left), 
					height(y->right)) + 1; 
	x->height = max(height(x->left), 
					height(x->right)) + 1; 

	// Return new root 
	return x; 
} 

// A utility function to left 
// rotate subtree rooted with x 
// See the diagram given above. 
Node *leftRotate(Node *x) 
{ 
	Node *y = x->right; 
	Node *T2 = y->left; 

	// Perform rotation 
	y->left = x; 
	x->right = T2; 

	// Update heights 
	x->height = max(height(x->left),	 
					height(x->right)) + 1; 
	y->height = max(height(y->left), 
					height(y->right)) + 1; 

	// Return new root 
	return y; 
} 

// Get Balance factor of node N 
int getBalance(Node *N) 
{ 
	if (N == NULL) 
		return 0; 
	return height(N->left) - height(N->right); 
} 


//Down remainder to two subtrees
void Down(Node* node){
    if (node==NULL) return;
    node->number+=node->remainder;
    if (node->left!=NULL){
        node->left->remainder+=node->remainder;
    }
    if (node->right!=NULL){
        node->right->remainder+=node->remainder;
    }
    node->remainder=0;
}
// Find maximum key of tree with root is node
int MaxNode(Node* node){
    if (node==NULL) return 0;
    if (node->right!=NULL)
        return node->right->keyMax;
    return node->key;
}
// Find minimum key of tree root
int MinNode(Node* node){
    if (node==NULL) return 0;
    if (node->left!=NULL)
        return node->left->keyMIn;
    return node->key;
}
// Update range 
void UpdateRange(Node* node){
    if (node==NULL) return;
    node->keyMax=MaxNode(node);
    node->keyMIn=MinNode(node);
}
///The maximum number of points a node in the tree with remainder
int MaxNumberOfNode(Node *node){
    if (node==NULL)
        return 0;
    return node->MaxNumber+node->remainder;
}
//Get maximum number of points a node in the tree without remainder
int GetMaxNumber(Node *node){
    return max(node->number,max(MaxNumberOfNode(node->left),MaxNumberOfNode(node->right)));
}
// Update maxnumber
void UpdateNumberMax(Node *node){
    if (node->left!=NULL){
        node->left->MaxNumber=GetMaxNumber(node->left);
    }
    if (node->right!=NULL){
        node->right->MaxNumber=GetMaxNumber(node->right);
    }
    node->MaxNumber=GetMaxNumber(node);

}
// Update range of node after rotate
void UpdateRangeAfterRotate(Node *node){
    if (node==NULL) return;
    if (node->left!=NULL){
        UpdateRange(node->left);
    }
    if (node->right!=NULL){
        UpdateRange(node->right);
    }
    UpdateRange(node);
}
// Insert node
Node* insert(Node* node, int key,int number) 
{ 
	/* 1. Perform the normal BST insertion */
	if (node == NULL) 
		return(newNode(key,number)); 
    if (key!=node->key){
        Down(node);
    }
	if (key < node->key) 
		node->left = insert(node->left, key,number); 
	else if (key > node->key) 
		node->right = insert(node->right, key,number); 
	else // Equal keys are not allowed in BST 
        {
            //node->totalNumber++;
            //AVL2
            node->number=node->number+1;
            node->MaxNumber=GetMaxNumber(node);
            UpdateRange(node);
            return node;
        }
    
    
    //Update
    node->height = 1 + max(height(node->left), height(node->right)); 
    UpdateRange(node);
    node->MaxNumber=GetMaxNumber(node);
	/* 3. Get the balance factor of this ancestor 
		node to check whether this node became 
		unbalanced */
	int balance = getBalance(node); 

	// If this node becomes unbalanced, then 
	// there are 4 cases 
    //Down a node before rotate
    //Update node after rotate
	// Left Left Case 
	if (balance > 1 && key < node->left->key){
        //dow
        Down(node);
        Down(node->left);
        Down(node->left->left);
        Node* returnNode=rightRotate(node);
        UpdateRangeAfterRotate(returnNode);
        UpdateNumberMax(returnNode);
        //UpdateTotalNumber(returnNode);
        return returnNode;

    } 
		

	// Right Right Case 
	if (balance < -1 && key > node->right->key){
        Down(node);
        Down(node->right);
        Down(node->right->right);
        Node* returnNode=leftRotate(node);
        UpdateRangeAfterRotate(returnNode);
        UpdateNumberMax(returnNode);
        //UpdateTotalNumber(returnNode);
        return returnNode;
    } 
        

	// Left Right Case 
	if (balance > 1 && key > node->left->key) 
	{ 
        Down(node);
        Down(node->left);
        Down(node->left->right);
		node->left = leftRotate(node->left); 
		Node* returnNode=rightRotate(node); 
        //UpdateTotalNumber(returnNode);
        UpdateRangeAfterRotate(returnNode);
        UpdateNumberMax(returnNode);
        return returnNode;
	} 

	// Right Left Case 
	if (balance < -1 && key < node->right->key) 
	{ 
        Down(node);
        Down(node->right);
        Down(node->right->left);
		node->right = rightRotate(node->right); 
		Node* returnNode=leftRotate(node); 
        //UpdateTotalNumber(returnNode);
        UpdateRangeAfterRotate(returnNode);
        UpdateNumberMax(returnNode);
        return returnNode;
	} 

	/* return the (unchanged) node pointer */
	return node; 
} 

//traverse preOder
void preOrder(Node *root) 
{ 
	if(root != NULL) 
	{ 
		cout << root->key << " "; 
		preOrder(root->left); 
		preOrder(root->right); 
	} 
} 
//Minimum of node in tree with root is node
Node * minValueNode(Node* node)  
{  
    Node* current = node;  
    /* loop down to find the leftmost leaf */
    while (current->left != NULL){
        Down(current);
        current = current->left;  
    }  
        
  
    return current;  
}  
  
//delete node
Node* deleteNode(Node* root, int key,bool ok)  
{  
    // STEP 1: PERFORM STANDARD BST DELETE  
    if (root == NULL)  
        return root;  
  
    // Down node before traverse
    Down(root);
    if ( key < root->key )  
        root->left = deleteNode(root->left, key,ok);  
  
    // If the key to be deleted is greater  
    // than the root's key, then it lies  
    // in right subtree  
    else if( key > root->key )  
        root->right = deleteNode(root->right, key,ok);  
  
    // if key is same as root's key, then  
    // This is the node to be deleted  
    else
    {  
        // node with only one child or no child
        // if delete node then ok=true
        if (ok==true)
            root->number=0;
        else root->number--;
        if (root->number==0){
            if( (root->left == NULL) || (root->right == NULL) ) {  
                Node *temp = root->left ? root->left :  root->right;  
  
            // No child case  
                Down(temp);
                if (temp == NULL)  
                {  
                    temp = root;  
                    root = NULL;  
                }  
                else // One child case  
                    *root = *temp; // Copy the contents of  
                           // the non-empty child  
                delete(temp);
                temp=NULL;  
            }  
            else
            {  
            // node with two children: Get the inorder  
            // successor (smallest in the right subtree)  
                Node* temp = minValueNode(root->right);  
  
            // Copy the inorder successor's  
            // data to this node  
                Down(temp);
                root->key = temp->key;
                root->number=temp->number;  
                root->right = deleteNode(root->right,temp->key,ok);  
            } 
        }

         
    }  
  
    // If the tree had only one node 
    // then return  
    if (root == NULL)  
    return root;  
    //Update Node
    root->height = 1 + max(height(root->left),  
                           height(root->right));  
    UpdateRange(root);
    root->MaxNumber=GetMaxNumber(root);
    // STEP 3: GET THE BALANCE FACTOR OF  
    // THIS NODE (to check whether this  
    // node became unbalanced)  
    int balance = getBalance(root);  
  
    // If this node becomes unbalanced,  
    // then there are 4 cases  
  
    //Down node before rotate
    //update node after rotate
    // Left Left Case  
    if (balance > 1 &&  getBalance(root->left) >= 0){
        Down(root);
        Down(root->left);
        Down(root->left->left);
        Node* returnNode=rightRotate(root);
        UpdateRangeAfterRotate(returnNode);
        UpdateNumberMax(returnNode);
        //UpdateTotalNumber(returnNode);
        return returnNode;
    }  
  
    // Left Right Case  
    if (balance > 1 &&  getBalance(root->left) < 0)  
    {  
       Down(root);
        Down(root->left);
        Down(root->left->right);
		root->left = leftRotate(root->left); 
		Node* returnNode=rightRotate(root); 
        //UpdateTotalNumber(returnNode);
        UpdateRangeAfterRotate(returnNode);
        UpdateNumberMax(returnNode);
        return returnNode;
    }  
  
    // Right Right Case  
    if (balance < -1 &&  getBalance(root->right) <= 0){
        Down(root);
        Down(root->right);
        Down(root->right->right);
        Node* returnNode=leftRotate(root);
        UpdateRangeAfterRotate(returnNode);
        UpdateNumberMax(returnNode);
        //UpdateTotalNumber(returnNode);
        return returnNode;
    }  
        
  
    // Right Left Case  
    if (balance < -1 &&  getBalance(root->right) > 0)  
    {  
        Down(root);
        Down(root->right);
        Down(root->right->left);
		root->right = rightRotate(root->right); 
		Node* returnNode=leftRotate(root); 
        //UpdateTotalNumber(returnNode);
        UpdateRangeAfterRotate(returnNode);
        UpdateNumberMax(returnNode);
        return returnNode;
    }  
    return root;  
}
// check if         root->keyMin<=l<=r<=root->keyMax
bool isCover(Node *root,int l,int r){
    if (root->keyMax<=r && root->keyMIn>=l)
        return true;
    return false;
}
// check if         root->key in [l,r]
bool isInSide(Node *root,int l,int r){
    if (root->key>=l && root->key<=r)
        return true;
    return false;
}
// Lazy Update
void UpdateSegment(Node *root, int l,int r,int value){
    if (root==NULL) return;
    if (isCover(root,l,r)){
        root->remainder+=value;
    }else if (isInSide(root,l,r)){
        root->number+=value;
        Down(root);
        UpdateSegment(root->left,l,root->key-1,value);
        UpdateSegment(root->right,root->key+1,r,value);
    }else if (root->key<l){
        //key is left segment
        Down(root);
        UpdateSegment(root->right,l,r,value);
    }else if (root->key>r){
        Down(root);
        UpdateSegment(root->left,l,r,value);
    }
    root->MaxNumber=GetMaxNumber(root);
    return;
}
//traverse LNR
void LNR(Node *root){
    if (root!=NULL){
        LNR(root->left);
        //cout<<root->key<<" "<<root->keyMax<<" "<<root->keyMIn<<endl;
        cout<<root->key<<" "<<root->number<<" "<<root->MaxNumber<<" "<<root->remainder<<endl;
        LNR(root->right);
    }
}

Node *root=NULL; //set root=NULL 
};

map<pair<int,int>,bool> Segment;
AVL1 x,y;
AVL2 Result;
int Soluong=0;
void add(int s, int t) {
    //check segment is exist
    if (Segment[pair<int,int>(s,t)]==true) return;
    Segment[pair<int,int>(s,t)]=true;
    Soluong++;
    // Find number of segment cover t-1
    x.root=x.insert(x.root,t-1);
    y.root=y.insert(y.root,s-1);
    int NumberOfAfter=Soluong-x.GetIndexLowerBound(x.root,t-1);
    int NumberOfBefore=Soluong-y.GetIndexLowerBound(y.root,t-1);
    int NumberSegmentShareAPoint=NumberOfAfter-NumberOfBefore;
    int Point=t-1;
    //insert point t-1 into AVL
    Result.root=Result.insert(Result.root,Point,NumberSegmentShareAPoint);
    // Update [s,t-2] increase 1
    if (s<=t-2)
        Result.UpdateSegment(Result.root,s,t-2,1);
}

//remove function
void remove(int s, int t) {
    //check segment is exist
    if (Segment[pair<int,int>(s,t)]==false) return;
    Segment[pair<int,int>(s,t)]=false;
    Soluong--;
    x.root=x.deleteNode(x.root,t-1);
    y.root=y.deleteNode(y.root,s-1);
    //remove point => ok=true
    int Point=t-1;
    bool ok=false;
    if (x.Search(x.root,t-1)==false)
        ok=true;
    //decrease point in node 
    Result.root=Result.deleteNode(Result.root,Point,ok);
    //add -1 for segment [s,t-2]
    if (s<=t-2)
        Result.UpdateSegment(Result.root,s,t-2,-1);
}
    

int minPark() {
    return Result.MaxNumberOfNode(Result.root);
}
int main() 
{   
    clock_t start = clock();
    ofstream output("kq2.txt");
    ifstream input;
    input.open("input.txt");
    while (input.eof()==false){
        int t,x,y;
        input>>t;
        if (t==0){
            input>>x>>y;
            add(x,y);
        }
        else if (t==1){
            input>>x>>y;
            remove(x,y);
        }
        else output<<minPark()<<endl;
        //cout<<minPark();
    }
    
} 

// This code is contributed by 
// rathbhupendra 
