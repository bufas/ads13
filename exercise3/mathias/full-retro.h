
#include <vector>
#include <map>

#include "retro.h"
#include "timeline.h"
#include "rb-tree/RBTree.h"
#include "rb-tree/RBTreeNode.h"

using namespace std;

class FullRetro : public Retro {

	void insert_into_timeline(int t, Leaf *l) {
		if (timeline.size() == 0) {
			timeline.push_back(l);
			return;
		}

		timeline.push_back(timeline[timeline.size()-1]);
		for (int i = timeline.size()-2; i > t; i--) {
			timeline[i] = timeline[i-1];
		}
		timeline[t] = l;
	}

	int member_search_now(int key) {
		return (now.find(key) != now.end()) ? 1 : 0;
	}

public:

	std::map<int, Leaf*> now; // Map is implemented as a binary search tree
	Tree history;
	vector<Leaf*> timeline;

	int currentTime;

	FullRetro() : currentTime(0) {}

	// insert an element at the current time
	void Insert(Op o, int key) {
		Insert(currentTime, o, key);
	}

	void Insert(int t, Op o, int key) {
		Leaf *n = history.insert(t, o, key);

		switch(o) {
			case INSERT:
				// Insert into now
				now[key] = n;
				history.insert_interval_to_end(n, key);
				break;
			case DELETE:
				// Remove from now and insert interval in history
				Leaf *l = now[key];
				n->insert_leaf = l;
				history.stop_interval(n, key);
				now.erase(key);
				break;
		}

		insert_into_timeline(t, n);
		currentTime++;
	}

	void Delete(int t) {
		if (t > timeline.size()) {
			cout << "Querying the future is not allowed" << endl;
			return;
		}

		Leaf n = *timeline[t];
		switch (n.o) {
			case INSERT:
				// Remove the node from now, and invalidate the leaf in history
				now.erase(n.x);
				n.invalidate();
				history.remove_interval_from(&n, n.x);
				break;
			case DELETE:
				history.restore_interval_from(&n, n.x);
				now[n.x] = n.insert_leaf;
				break;
		}

		// Remove the deleted time from the timeline (takes linear time, bad doge)
		bool reached = false;
		for (int i = 0; i < timeline.size(); i++) {
			if (reached) timeline[i-1] = timeline[i];
			else if (timeline[i] == &n) reached = true;
		}

		currentTime--;
	}

	bool Query(int key) {
		return Query(currentTime, key);
	}

	bool Query(int t, int key) {
		if (t >= currentTime) {
			return Query(key);
		}

		// For retroactive queries we need to check both the history
		// and the now structures. It will reside in history if it has
		// been deleted, and in now if it has not. The problem is that
		// we can't search in the now structure because it also contains
		// elements that might be created after t. We need to do a linear
		// search through with this implementation.
		// This problem can be fixed by not having the now structure. This
		// will make the insert_interval() and remove_interval() methods
		// harder to implement but it can be done. Then we can just do
		// a normal search in the history structure, and everybody is
		// happy!
		return timeline[currentTime]->member_search(key);
	}

	void print() {
		cout << endl << "=============================\nPrinting history\n=============================" << endl;
		history.print();
		// cout << "=============================\nPrinting now\n=============================" << endl;
		// for (auto it=now.begin(); it!=now.end(); ++it) {
		// 	cout << it->first << " ";
		// }

		// cout << endl << endl << "=============================\nPrinting timeline\n=============================" << endl;
		// for (int i = 0; i < timeline.size(); i++) printf("%p\n", timeline[i]);
		// cout << endl;
	}

};



// TODO:
// A leaf should be able to find its own time in the timeline
// by running to the root, adding the sizes of its left subtrees
// However, here it is done by scanning through the timeline, 
// which is easier.
