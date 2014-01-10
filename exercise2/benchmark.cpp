#include <iostream>
#include <stdlib.h>
#include <sys/time.h>
#include <vector>
#include <algorithm>
#include <cstdio>

#include "veb-heap.cpp"

using namespace std;

void print_time(string pre, int n, struct timeval before, struct timeval after) {
	long utime = ((after.tv_sec - before.tv_sec) * 1000000) + after.tv_usec - before.tv_usec;
	long stime = ((after.tv_sec - before.tv_sec) * 1000000) + after.tv_usec - before.tv_usec;
	printf("%d\t%ld\n", n, (utime + stime));
}

void benchmark_rb() {
	srand(time(nullptr));
	struct timeval before, after;

	// Benchmark RB trees and RB trees
	int max_number_size = 15000000;
	int size = 10000000;

/*
	cout << "Timing vEB trees sequential insertion" << std::endl;
	{
		VebTreeNode<int> t(size);
		
		for (int i = 0; i <= max_number_size; i++) {
			if (i % 100000 == 0) {
				int rands[1000];
				for (int q = 0; q < 1000; q++) {
					rands[q] = rand() % size;
				}
				gettimeofday(&before, nullptr);
				for (int j = 0; j < 1000; j++, i++) {
					int key = rands[j];
					t.insert(key, new VebNode<int>(key, key));
				}
				gettimeofday(&after, nullptr);
				print_time("vEB insert", i-1000, before, after);
			}
			int key = rand() % size;
			t.insert(key, new VebNode<int>(key, key));
		}
	}
*/
/*
	cout << "Timing vEB trees predecessor search" << std::endl;
	{
		VebTreeNode<int> t(size);
		vector<VebNode<int>*> nodes;
		for (int i = 0; i <= max_number_size; i++) {
			if (i % 100000 == 0 && i != 0) {
				int number_of_searches = 1000;
				// generate 10.000 random numbers to search for
				int rands[number_of_searches];
				for (int q = 0; q < number_of_searches; q++) {
					rands[q] = rand() % nodes.size();
				}

				gettimeofday(&before, nullptr);
				// Make 10.000 predecessor searches
				for (int q = 0; q < number_of_searches; q++) {
					t.predecessor(nodes[rands[q]]->getKey());
				}
				gettimeofday(&after, nullptr);
				print_time("vEB pred", i, before, after);
			}
			int key = rand() % size;
			VebNode<int> *n = new VebNode<int>(key, key);
			t.insert(key, n);
			nodes.push_back(n);
		}
	}
*/
	cout << "Timing vEB trees delete" << std::endl;
	{
		VebTreeNode<int> t(size);
		vector<int> nodes;
		for (int i = 0; i <= max_number_size; i++) {
			if (i % 100000 == 0 && i != 0) {
				gettimeofday(&before, nullptr);
				for (int q = 0; q < 1000; q++) {
					delete t.remove(nodes.back());
					nodes.pop_back();
				}
				gettimeofday(&after, nullptr);
				print_time("vEB delete", i, before, after);

				for (int q = 0; q < 1000; q++) {
					int key = rand() % size;
					t.insert(key, new VebNode<int>(key, key));
					nodes.push_back(key);
				}
			}
			int key = rand() % size;
			t.insert(key, new VebNode<int>(key, key));
			nodes.push_back(key);
		}
	}
	
}

int main() {
	benchmark_rb();
	return 0;
}