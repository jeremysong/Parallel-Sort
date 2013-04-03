#include <iostream>
#include <string>
#include <cstring>

#include <omp.h>

using namespace std;

#define STRINGMAX 1024
#define FILELINE 10
#define HASHTABLESIZE 10000000

/**
 *  A class to hold each string and correponding frequency
 */
class stringPair {
private:
	int freq;		// frequency of this string
	char str[STRINGMAX];		// string content
	stringPair *next;	// point to next stringPair
				// whose string has same hash value
public:
	stringPair()
	{
		freq = 0;
		next = NULL;
	}

	stringPair(string s)
	{
		freq = 1;
		strcpy(str, s.c_str());
		next = NULL;
	}

	void addFreq() { freq++; }
	int getFreq() { return freq; }
	stringPair * getNext() { return next; }
	void setNext(string s) { next = new stringPair(s); }
	int strCmp(string s) { return strcmp(str, s.c_str()); }
	string getString() { return str; }
	void setString(string s) { strcpy(str, s.c_str()); }
};

stringPair * hashTable[HASHTABLESIZE];

/**
 * A function used to generate hash value of given string
 */
unsigned int hash(string str)
{
	unsigned int hash = 0;
	int tracer = 0;
	for (string::iterator it=str.begin(); it!=str.end(); ++it)
	{
		int char_int = *it;
		hash = char_int*10*tracer+hash;
		tracer++;
	}
	hash = hash % 10000000;
	//cout << str << endl;
	//cout << hash << endl;
	return hash;
}

void addToHashTable(int hash, string str)
{
	if (hashTable[hash] == NULL)
	{
		//cout << str << " added to hashTable" << endl;
		hashTable[hash] = new stringPair(str);
	} else
	{
		stringPair *p = hashTable[hash];
		while(p != NULL)
	       	{
			if(p->strCmp(str) == 0) {
				p->addFreq();
				return;
			}
			if( p->getNext() == NULL )
			{
				p->setNext(str);
				return;
			}
			p = p->getNext();
		}
	}
}

void map(string *content)
{
	for(int i = 0; i < FILELINE; i++)
	{
		string str = content[i];
		addToHashTable(hash(str), str);
	}
}	

/**
 *  Print out the first stringPair of there is one.
 */
void printFirstStringPair()
{
	for(int i = 0; i < HASHTABLESIZE; i++)
	{
		if(hashTable[i] != NULL)
		{
			cout << hashTable[i]->getString() << endl;
		}
	}
}

/**
 *  Print out all the stringPair.
 */
void printAllStringPair()
{
	for(int i = 0; i < HASHTABLESIZE; i++)
	{
		if(hashTable[i] != NULL)
		{
			stringPair *p = hashTable[i];
			while(p != NULL)
			{
				cout << "List:" << p->getString();
				cout << " " << p->getFreq() << endl;
				p = p->getNext();
			}
		}
	}
}


int main(int argc, char **argv)
{
	string content[FILELINE];
	int linenum = 0;
	char line[STRINGMAX];
	while(scanf("%s",line) != EOF)
	{
		content[linenum] = line;
		linenum++;
	}

	map(content);

	// for test only
	//printFirstStringPair();
	printAllStringPair();

	return 0;
}

