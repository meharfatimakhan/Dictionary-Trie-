#include <iostream>
#include <string>
#include <fstream>
using namespace std;


class node
{
private: 
	friend class trie; //friend class

	//data variables of node class
	char data;
	node *children[26];
	bool endOfWord;

public:

	//default constructor
	node()
	{
		data = '\0'; //set to null

		endOfWord = false;

		for (int i = 0; i < 26; i++) //array of 26 children
			children[i] = 0;  //children set to nullptr
	}
};


class trie
{
public:
	node *root;

	//trie default constructor
	trie()
	{
		root = new node(); //new node creation
	}
	
	// trie destructor
	~trie()
	{
		//calls deletion(root) which deletes the trie starting from the root
		recursiveDelete(root); 
	}

	bool recursiveInsert(node *current, string key, int count)//takes root,key to be inserted, and starting index
	{
		int index = key[count] - 'a'; //converts letter to ascii

		if (count == 0)//base case 1;checks whether a word inside a word is already present in trie or not, checks the whole word from count=0
		{
			node *start = current; //equate current(root) to start

			for (int innerIndex = 0; innerIndex < key.length(); innerIndex++) //total 26 children, so max 26 iterations
			{
				int checkIndex = key[innerIndex] - 'a'; //converts letter to ascii

				if (!start->children[checkIndex]) //if null,then break
					break;

				start = start->children[checkIndex];//else call next child at that node

				if (start->endOfWord == true)//if endOfWord is true,means word already present, then return false
					return false;
			}
		}
		
		if (count == key.length())//base case 2; if count becomes equal to key length
		{
			current->endOfWord = true; //put current's endofword equal to true
			return true;
		}

		if (!current->children[index])//if null(if alphabet not present at index), then create new node at that index
			current->children[index] = new node();

		current->children[index]->data = key[count];//but letter at that index equal to current's child index
		//cout << current->children[index]->data;
		count = count + 1; //increment count

		recursiveInsert(current->children[index], key, count); //sends recursive call to traverse current's children until count is not equal to stringlength
	}

	void loadDictionary()//loads all words from our file in the trie
	{
		string word;
		ifstream read;
		read.open("WordsList.txt");
		while (!read.eof())//reads till end of file
		{
			getline(read, word, '\n');//reads word till it finds end of line
			if (word.length() > 4) //doesnt read words less than equal to 4
			{
				recursiveInsert(root, word, 0);//inserts a word into the trie
			}
		}
		read.close();
	}
	
	void recursivePrint(node *current, string alphabets, int index)//takes root, a string, and starting index
	{
		int length = 0; //initially length is 0

		if (current->endOfWord) //if endofword, then print word(base case)
		{
			alphabets[index] = '\0';//place null at endofword
			cout << alphabets << endl;//print word
			length = alphabets.length();//calculates lengthofword
			return;
		}

		length = alphabets.length();//lengthofword

		for (int innerIndex = 0; innerIndex < 26; innerIndex++)////total 26 children, so max 26 iterations
		{
			if (current->children[innerIndex])//if child at innerindex exists
			{
				alphabets[index] = 'a' + innerIndex;//converting index into alphabet

				index++;// add into index

				recursivePrint(current->children[innerIndex], alphabets, index);//give recursive call;visits children till endofword is not true

				index--;//subtract index

				if (index >= length)//if index is more than actual length of string, then return
					return;
			}
		}
	}

	bool recursiveSearch(node *current, string key,int count)//takes root,key to be inserted, and starting index
	{
		int index = key[count] - 'a'; //converts letter to ascii

		if (count == key.length()) //if count is equal to string length, then proceed
		{
			if (current != nullptr && current->endOfWord) //if current's endofword is true and its not null, then return true, means word found
				return true;
			else //if word not found
				return false;
		}
		else //if count is not equal to string length
		{
			if (!current->children[index])//if current is null at its child's index, then return false
				return false;
			count = count + 1;//if not null, then increment count
			recursiveSearch(current->children[index], key, count);//sends recursive call to traverse current's children until count is not equal to stringlength
		}
	}

	void searchPrefix(string prefix, node *current)//takes a word whose prefix we want to search, and root
	{
		int count = 0; //count is initially 0
		int index = 0;//index is initially 0
		string word = "                         ";//our word string of length 25
		for (int innerIndex = 0; innerIndex < prefix.length() - 1; innerIndex++)//loop works till lengthofword-1
		{
			index = prefix[innerIndex] - 'a';//converts character to its ascii
			if (current->children[index])//if child is not null at that index
			{
				word[count] = 'a' + index;//convert index to alphabet and store in word
				count++;//add count for to reach next position
			}
			current = current->children[index];//child of child
		}
		index = index + 2;//add 2 to traverse the index in recursivePrint
		recursivePrint(current,word,index);//prints all prefixes; is at currentPos,takes our half completed word and index
	}

	void recursiveDelete(node* current)//deletes whole trie
	{
		if (current->endOfWord) //if it's a complete word, then return
			return;

		for (int index = 0; index < 26; index++) //total 26 children, so max 26 iterations
		{
			if (current->children[index])//if child exists
			{
				recursiveDelete(current->children[index]);//recursively reaches children until endOfWord is true
				delete current;//deletes the node at children[index]
				return;//returns after recursion
			}
		}
	}

};

void driver()
{
	trie object;

	object.loadDictionary();//loads all words in our trie

	cout << "Searching Trie:" << endl<<endl;
	if (object.recursiveSearch(object.root, "parent", 0))
		cout << "'parent' found successfully!" << endl;
	else
		cout << "'parent' not found!"<<endl;

	if (object.recursiveSearch(object.root, "parenthetically", 0))
		cout << "'parenthetically' found successfully!" << endl;
	else
		cout << "'parenthetically' not found!"<<endl;

	if (object.recursiveSearch(object.root, "abide", 0))
		cout << "'abide' found successfully!" << endl;
	else
		cout << "'abide' not found!" << endl;

	if (object.recursiveSearch(object.root, "abides", 0))
		cout << "'abides' found successfully!" << endl;
	else
		cout << "'abides' not found!" << endl<<endl;

	cout << "Searching prefixes:" <<endl<< endl;
	cout << "Prefixes of 'parade' are:" << endl;

	object.searchPrefix("parade",object.root);//prints all words with 'parade' as prefix

	cout << endl;

	cout << "Trie(printing): " << endl;
	string words = "                         ";//string of length 25
	object.recursivePrint(object.root, words, 0); //prints all words present in the trie
}

void main()
{
	driver();

	system("pause");
}