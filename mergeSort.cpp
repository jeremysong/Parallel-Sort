#include <iostream>
#include <omp.h>
#include <cstdlib>

using namespace std;

int SIZE;

/**
 *  Print all the element in array. Used for test only.
 * 	array : the array that will be printed.
 * 	length: the length of the input array.
 */
void printArray( float * array, const int length );

/**
 *  Merge two arrays and arrange all the elements in ascending order.
 *   left  : the pointer of first array to be merged.
 *   right : the pointer of the second array to be merged.
 *   leftl : the last index of the first array.
 *   rightl: the last index of the second array.
 *  return : new array merged with two input arrays.
 */
float * merge( float * left, const int leftl, float * right, const int rightl );

/**
 *  Merge Sort. 
 *  	 array: the pointer of array that will be sorted.
 * 	 begin: the first index of the subarray that will be sorted.
 *	 end  : the last index of the subarray that will be sorted.
 *  return    : new sorted array.
 */
float * merge_sort( float *array, const int begin, const int end, int nthread)
{
	if ( begin == end )
	{
		float *result = new float[1];
		result[0] = array[begin];
		return result;
	}

	int mid = (begin + end) / 2;
	
	float *left = NULL;
	float *right = NULL;
	float *result = NULL;

	//cout << "Total threads: " << omp_get_num_threads() << endl;
	#pragma omp task
	left = merge_sort( array, begin, mid, nthread/2 );

	#pragma omp task
	right = merge_sort( array, mid+1, end, nthread - nthread/2 );

	if(left == NULL)
	{
		cout << "left is null" << endl;
		exit(0);
	}

	#pragma omp taskwait
	result = merge( left, mid-begin, right, end-mid-1 );
	return result;
}

float * merge( float * left, const int leftl, float *right, const int rightl )
{
//	cout << "Thread # in merge function: " << omp_get_thread_num() << endl;

	float *array = new float[leftl + rightl + 2];
	int pos = 0;	// tracer of the result array
	int l = 0;	// tracer of the left array
	int r = 0;	// tracer of the right array

	while ( pos < leftl+rightl+1 )
	{
		while ( (l <= leftl) && (r <= rightl) )
		{
			if ( left[l] < right[r] )
			{
				array[pos] = left[l];
				l++;
				pos++;
			} else
			{
				array[pos] = right[r];
				r++;
				pos++;
			}
		}

		/*
		 * put rest of the elements in left array into result array
		 */
		while ( l <= leftl )
		{
			array[pos] = left[l];
			pos++;
			l++;
		}

		/*
		 * put rest of the elements in right array into result array
		 */
		while ( r <= rightl )
		{
			array[pos] = right[r];
			pos++;
			r++;
		}
	}
	return array;
}

/**
 *  Print all the elements in the array.
 *  This function is for test only.
 */
void printArray( float * array, const int length )
{
	for ( int i = 0; i < length; i++ )
	{
		cout << array[i] << " ";
	}
	cout << "\n";
}

int main(int argc, char **argv) {
	int nthreads = atoi(argv[1]);
	SIZE = atoi(argv[2]);

	float *array = new float[SIZE];
	for(int i = 0; i < SIZE; i++ )
	{
		array[i] = (float)rand()/ (float) RAND_MAX;
	}
	double start = omp_get_wtime();

	float *result;
	#pragma omp parallel num_threads(nthreads)
	{
		#pragma omp single
		result = merge_sort(array, 0,SIZE-1, nthreads);
	}
	double end = omp_get_wtime();

	//printArray(result, SIZE);
	cout << "total running time: " << end-start << endl;

	return 0;
}
