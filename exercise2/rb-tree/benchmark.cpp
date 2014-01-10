#include <iostream>
#include <stdlib.h>
#include <sys/time.h>
#include <vector>
#include <algorithm>
#include <cstdio>

#include "RBTreeNode.h"

using namespace std;

void print_time(string pre, int n, struct timeval before, struct timeval after) {
	long utime = ((after.tv_sec - before.tv_sec) * 1000000) + after.tv_usec - before.tv_usec;
	long stime = ((after.tv_sec - before.tv_sec) * 1000000) + after.tv_usec - before.tv_usec;
	printf("%s\tn: %d\tt: %ld\n", pre.c_str(), n, (utime + stime)/1000);
}

void benchmark_rb() {
	srand(time(nullptr));
	struct timeval before, after;

	// Benchmark RB trees and RB trees
	int max_number_size = 10000000;


	cout << "Timing RB trees sequential insertion" << std::endl;
	{
		RBTree t = RBTree();
		
		gettimeofday(&before, nullptr);
		for (int i = 0; i < max_number_size; i++) {
			if (i % 10000 == 0 && i != 0) {
				// Print time every 10.000th iteration
				gettimeofday(&after, nullptr);
				print_time("RB insert", i, before, after);
			}
			t.insert(rand() % max_number_size);
		}
	}

	cout << "Timing RB trees predecessor search" << std::endl;
	{
		RBTree t = RBTree();
		vector<RBTreeNode*> nodes;
		for (int i = 0; i < max_number_size; i++) {
			if (i % 10000 == 0 && i != 0) {
				int number_of_searches = 1000000;
				// generate 10.000 random numbers to search for
				int rands[number_of_searches];
				for (int q = 0; q < number_of_searches; q++) {
					rands[q] = rand() % nodes.size();
				}

				gettimeofday(&before, nullptr);
				// Make 10.000 predecessor searches
				for (int q = 0; q < number_of_searches; q++) {
					t.predecessor(nodes[rands[q]]);
				}
				gettimeofday(&after, nullptr);
				print_time("RB pred", i, before, after);
			}
			nodes.push_back(t.insert(rand() % max_number_size));
		}
	}

	cout << "Timing RB trees delete" << std::endl;
	for (int i = 100000; i < max_number_size; i += 100000) {
		RBTree t;
		vector<RBTreeNode*> nodes;
		
		for (int q = 0; q < i; q++) {
			nodes.push_back(t.insert(rand() % max_number_size));
		}

		gettimeofday(&before, nullptr);
		for (auto &n : nodes) t.remove(n);
		gettimeofday(&after, nullptr);
		print_time("RB delete", i, before, after);

	}
}

int main() {
	benchmark_rb();
	return 0;
}

