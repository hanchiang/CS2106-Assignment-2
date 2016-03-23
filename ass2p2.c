#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#define NUMELTS 16384
// IMPORTANT: Compile using "gcc assg2p2.c .lm -o assg2p2".
// The "-lm" is important as it brings in the Math library.
// Implements the naive primality test.
// Returns TRUE if n is a prime number
int prime(int n)
{
	int ret=1, i;
	for(i=2; i<=(int) sqrt(n) && ret; i++)
		ret=n % i;
	return ret;
}
int main()
{
	int data[NUMELTS];
	int parentPrimeCount = 0;
	int childPrimeCount = 0;
	int fromChildPrimeCount;
	int fd[2];

	pipe(fd);
	// Declare other variables here.

	// Create the random number list.
	srand(time(NULL));
	int i;
	for(i=0; i<NUMELTS; i++)
		data[i]=(int) (((double) rand() / (double) RAND_MAX) * 10000);
	// Now create a parent and child process.
	//PARENT:
	// Check the 0 to 8191 sub-list
	// Then wait for the prime number count from the child.
	// Parent should then print out the number of primes
	// found by it, number of primes found by the child,
	// And the total number of primes found.
	int pid = fork();
	if (pid) {
		int j;
		for (j = 0; j < 8192; j++) {
			if (prime(data[j])) {
				parentPrimeCount++;
			}
		}

		close(fd[1]);
		read(fd[0], &fromChildPrimeCount, sizeof(fromChildPrimeCount));
		printf("There are %d prime numbers found by parent\n", parentPrimeCount);
		printf("There are %d prime numbers found by child\n", fromChildPrimeCount);
		printf("There are %d prime numbers in total\n", parentPrimeCount+fromChildPrimeCount);

	}

	// CHILD:
	// Check the 8192 to 16383 sub-list.
	// Send # of primes found to the parent.
	else {
		int k;
		for (k = 8192; k < 16384; k++) {
			if (prime(data[k])) {
				childPrimeCount++;
			}
		}
		close(fd[0]);
		write(fd[1], &childPrimeCount, sizeof(childPrimeCount));
	}
}
