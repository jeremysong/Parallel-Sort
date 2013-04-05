#include <iostream>
#include <string>
#include <cstring>
#include <iomanip>

#include <omp.h>

using namespace std;

#define STRINGMAX 1024
#define FILELINE 10000
#define HASHTABLESIZE 10000000

/**
 *  A class to hold each string and correponding frequency
 */
class stringPair {
private:
	int freq;		// frequency of this string
	char str[STRINGMAX];	// string content
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

/*
 *  array to store all the unique string and frequency.
 *  Use linked list to solve colision.
 */
stringPair * hashTable[HASHTABLESIZE];

/*
 *  group lock to control each memory location of hashTable
 *
 */
int lock[HASHTABLESIZE];

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
	hash = hash % HASHTABLESIZE;
	//cout << str << endl;
	//cout << hash << endl;
	return hash;
}

void addToHashTable(int hash, string str)
{
	/*
	 *  Busy waiting if the corresponding memory location is locked
	 */
	while ( lock[hash] == 1 );

#pragma omp atomic
	lock[hash] = 1;
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
#pragma omp atomic
	lock[hash] = 0;
}

/*
 *  Map all the string to hashtable
 */
void map(string *content, int nthreads)
{
    #pragma omp parallel num_threads(nthreads)
    {
	int i = 0;
	#pragma omp for private(i)
	for(i = 0; i < FILELINE; i++)
	{
		if (content[i].length() != 0)
		{
			string str = content[i];
			/*
			 *  Use critical here to prevent that more than
			 *  one threads access the same memory location
			 */
			#pragma omp critical
			addToHashTable(hash(str), str);
		}
	}
    }
}	

/*
 *  Reduce: Print all the unique string and frequency
 */
void reduce(int nthreads)
{
    #pragma omp parallel num_threads(nthreads)
    {
	int i = 0;
	#pragma omp for private(i)
	for(i = 0; i < HASHTABLESIZE; i++)
	{
		stringPair *p = hashTable[i];
		while(p != NULL)
		{
			cout << setw(15) << left <<  p->getString() << p->getFreq() << endl;
			p = p->getNext();
		}
	}
    }

}

/**
 *  Print out the first stringPair of there is one.
 *  For test only
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
 *  For test only
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
	int nthreads = atoi(argv[1]);

	string content[FILELINE];
	int linenum = 0;
	char line[STRINGMAX];
	while(scanf("%s",line) != EOF)
	{
		content[linenum] = line;
		linenum++;
	}

	for(int i = 0; i < HASHTABLESIZE; i++)
	{
		lock[i] = 0;
	}

	double start = omp_get_wtime();

	map(content, nthreads);
	reduce(nthreads);

	double end = omp_get_wtime();

	cout << "Total running time: " << end-start << endl;

	return 0;
}

