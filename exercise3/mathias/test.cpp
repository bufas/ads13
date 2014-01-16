
#import <iostream>
#import "full-retro.h"

using namespace std;

int main() {
	FullRetro fr;

	for (int i = 0; i < 1000; i++) {
		fr.Insert(INSERT, i);
	}

	// fr.Insert(INSERT, 10);
	// fr.print();
	// fr.Insert(INSERT, 20);
	// fr.print();
	// fr.Insert(INSERT, 40);
	// fr.print();
	// fr.Insert(DELETE, 20);
	// fr.print();
	// fr.Insert(INSERT, 30);
	// fr.print();

	// TODO this does not work. We need to handle insertion between elements in a node
	//fr.Insert(0, INSERT, 1);

	fr.print();
	
	cout << "Tests finished." << endl;
}