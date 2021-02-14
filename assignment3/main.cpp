#include<iostream>                //Şamil Taner CENGİZ
#include<fstream>                 //150170706
#include<string>
#include<time.h>
#include<cstdio>


using namespace std;

struct playerNode {
	int rebound;
	int assist;
	int point;
	
	string name; // holds the key 
	playerNode *parent; // pointer to the parent
	playerNode *left; // pointer to left child
	playerNode *right; // pointer to right child
	
	int color; // 1 = Red, 0 = Black
};

typedef playerNode *NodePtr;

struct maximumNode {				//struct for the maximum values
	int rebound;
	int assist;
	int point;

	string reboundName;
	string assistName;
	string pointName;
};


class RedBlackTree {					//class for the Tree
public:
	NodePtr root;
	NodePtr TNULL;
	
	RedBlackTree() {
		TNULL = new playerNode;
		TNULL->color = 0;
		TNULL->left = NULL;
		TNULL->right = NULL;
		root = TNULL;
	}

	void preOrderHelper(NodePtr node, struct maximumNode* mNode) {   // recursive preorder traversal
								
		if(node->point > mNode->point){
			mNode->point = node->point;
			mNode->pointName = node->name;
		}
		if(node->assist > mNode->assist){
			mNode->assist = node->assist;
			mNode->assistName = node->name;
		}
		if(node->rebound > mNode->rebound){
			mNode->rebound = node->rebound;
			mNode->reboundName = node->name;
		}
		if (node != TNULL) {
			preOrderHelper(node->left, mNode);
			preOrderHelper(node->right, mNode);
		} 
	}

	// preorder traversal
	// playerNode->Left Subtree->Right Subtree
	void printMaximum(){
		struct maximumNode *mNode = new maximumNode;
		mNode->rebound = 0;
		mNode->assist = 0;
		mNode->point = 0;
		
		preOrderHelper(this->root, mNode);
		
		cout<< "Max Points: " << mNode->point << " - Player Name: " << mNode->pointName << endl;
		cout<< "Max Assists: " << mNode->assist << " - Player Name: " << mNode->assistName << endl;
		cout<< "Max Rebs: " << mNode->rebound << " - Player Name: " << mNode->reboundName << endl;
	}

	
	NodePtr searchTreeHelper(NodePtr node, string key) {  	// recursive search for a node to update
		if (node == TNULL) {   //node is TNULL
			return TNULL;
		}
		
		if(key == node->name) {   //key is found
			return node;
		}

		if (key < node->name) {
			return searchTreeHelper(node->left, key);
		} else {
			return searchTreeHelper(node->right, key);
		}
	}

	// search the tree for the key k
	// and return the corresponding node
	NodePtr searchTree(string k) {										//search function
		return searchTreeHelper(this->root, k);
	}

	
	// rotate left at node x
	void leftRotate(NodePtr x) {     ////used in fixing
		NodePtr y = x->right;
		x->right = y->left;
		if (y->left != TNULL) {
			y->left->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == NULL) {
			this->root = y;
		} else if (x == x->parent->left) {
			x->parent->left = y;
		} else {
			x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
	}

	// rotate right at node x
	void rightRotate(NodePtr x) {    //used in fixing
		NodePtr y = x->left;
		x->left = y->right;
		if (y->right != TNULL) {
			y->right->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == NULL) {
			this->root = y;
		} else if (x == x->parent->right) {
			x->parent->right = y;
		} else {
			x->parent->left = y;
		}
		y->right = x;
		x->parent = y;
	}
	
	
	// fix the red black tree after inserting a new node
	void fixInsert(NodePtr k){
		NodePtr u;		//uncle node
		while (k->parent->color == 1) { 				//continue until k's parent is black
			if (k->parent == k->parent->parent->right) {  //if nodes parent is the right child
				u = k->parent->parent->left; // uncle
				if (u->color == 1) {
					// case 1 ->  uncle is red: recolor
					u->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					k = k->parent->parent;
				} else {
					if (k == k->parent->left) {
						// case 2-> uncle is black(triangle): rotate parent
						k = k->parent;
						rightRotate(k);
					}
					// case 3 -> uncle is black(line): rotate grand parent and recolor
					k->parent->color = 0;
					k->parent->parent->color = 1;
					leftRotate(k->parent->parent);
				}
			} else {   //if nodes parent is left child
				u = k->parent->parent->right; // uncle

				if (u->color == 1) {
					// mirror case 1
					u->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					k = k->parent->parent;	
				} else {
					if (k == k->parent->right) {
						// mirror case 2
						k = k->parent;
						leftRotate(k);
					}
					// mirror case 3
					k->parent->color = 0;
					k->parent->parent->color = 1;
					rightRotate(k->parent->parent);
				}
			}
			if (k == root) {
				break;
			}
		}
		root->color = 0;
	}


	// insert the key to the tree in its appropriate position
	// and fix the tree after insertion
	void insert(string key, int rebound, int assist, int point) {   		// insert function

		NodePtr node = new playerNode;
		node->parent = NULL;
		node->rebound = rebound;
		node->assist = assist;
		node->point = point;
		node->name = key;
		node->left = TNULL;
		node->right = TNULL;
		node->color = 1; // new node must be red

		NodePtr y = NULL;
		NodePtr x = this->root;

		while (x != TNULL) {
			y = x;
			if (node->name < x->name) {
				x = x->left;
			} else {
				x = x->right;
			}
		}

		// y is parent of x
		node->parent = y;
		if (y == NULL) {
			root = node;
		} else if (node->name < y->name) {
			y->left = node;
		} else {
			y->right = node;
		}

		// if new node is a root node, simply return
		if (node->parent == NULL){
			node->color = 0;
			return;
		}

		// if the grandparent is null, simply return
		if (node->parent->parent == NULL) {
			return;
		}
		
		// node is in the middle somewhere
		// fix the tree after insertion
		fixInsert(node);
	}

	
	
	void printHelper(NodePtr root, string indent) {			//printing the tree preorder
		// print the tree structure on the screen
	   	if (root != TNULL) {
			cout<<indent;
			indent += "-";
           
			string sColor = root->color?"RED":"BLACK";
			cout << "("<<sColor<<") " << root->name << endl;
			printHelper(root->left, indent);
			printHelper(root->right, indent);
		}
	}
	
	// print the tree structure on the screen
	void printTree() {
	    if (root) {
    		printHelper(this->root, "");
	    }
	}

};


//////////////////////////////////////////////////// main function
int main(int argc, char* argv[]){
		
	ifstream file;
	file.open(argv[1]);
	
	if (!file){
		cerr << "File cannot be opened!";
		return 1;
	}
	
	string line;
	getline(file, line); //this is the header line
	cout<<line<<endl;
	
	//reading first line
	string season;
	getline(file, season, ',');

	string name;
	getline(file, name, ',');

	string team;
	getline(file, team, ',');

	string word;
	int rebound;
	getline(file, word, ',');
	sscanf(word.c_str(), "%d", &rebound);

	int assist;
	getline(file, word, ',');
	sscanf(word.c_str(), "%d", &assist);

	int point;
	getline(file, word);
	sscanf(word.c_str(), "%d", &point);

	
	RedBlackTree rbt;  //creating tree instance
	
	rbt.insert(name, rebound, assist, point);  //first line is inserted to the tree
	
	string previousSeason = season;
	int seasonNumber = 0;
	while(getline(file, season, ',')){		//keeps reading the next lines
		
		if(previousSeason != season){   //end of that season
			seasonNumber++;
			cout<<"End of the " << previousSeason << " Season"<<endl;
			rbt.printMaximum();     //printing maximum values
			if(seasonNumber == 1){		//end of the first season
				rbt.printTree();   //printing the tree
			}
			
		}
		//read the values from csv file
		string name;
		getline(file, name, ',');

		string team;
		getline(file, team, ',');

		string word;
		int rebound;
		getline(file, word, ',');
		sscanf(word.c_str(), "%d", &rebound);
		
		string word2;
		int assist;
		getline(file, word2, ',');
		sscanf(word2.c_str(), "%d", &assist);
		
		string word3;
		int point;
		getline(file, word3);
		sscanf(word3.c_str(), "%d", &point);
		
		//insert the next node if it does not exist, update if it exists
		NodePtr temp;
		temp = rbt.searchTree(name);
		
		if(temp == rbt.TNULL){  //node does not exist, insert it
			rbt.insert(name, rebound, assist, point);
		} else {        //node exists, update the values 
			temp->rebound += rebound;
			temp->assist += assist;
			temp->point += point;
		}
	
	previousSeason = season;     //update the season value
	}
	cout<<"End of the " << previousSeason << " Season"<<endl;
	rbt.printMaximum();
	//rbt.printTree();
	
	
	return 0;
}
