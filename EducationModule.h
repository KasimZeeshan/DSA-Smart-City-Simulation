#pragma once
#include <iostream>
#include <string>

#include "../DataProject/SchoolTree.h"
#include "../DataProject/MaxHeap.h"
#include "../DataProject/Types.h"

using namespace std;

class EducationModule {
	SchoolTree schoolTree;
	MaxHeap RankingSystem;

	struct SchoolRecord {
		string name;
		int scaledRating; // rating * 10 stored as int
		bool used;
		SchoolRecord* next;
		SchoolRecord(const string& n, int r) { name = n; scaledRating = r; used = false; next = nullptr; }
	};

	SchoolRecord* schoolListHead = nullptr;

public:
	EducationModule() {};

	// School system Management
		//// This is very simple, it basically adds a new School
	void addSchool(const string& name, float rating) {
		schoolTree.addSchool(name, rating);
		int scaled = (int)(rating * 10 + 0.5f); // safe rounding
		RankingSystem.insert(scaled);

		// add to linked list
		SchoolRecord* rec = new SchoolRecord(name, scaled);
		rec->next = schoolListHead;
		schoolListHead = rec;

		cout << "Added School: " << name << " (Rating: " << rating << ")" << endl;
	}

	//// Again very simple, just add department in the school
	void addDepartment(const string& schoolName, const string& deptName) {
		schoolTree.addDepartment(schoolName, deptName);
		cout << "Department added " << deptName << "of school " << schoolName << endl;
	};
	//// Again extremely simple, adds new class
	void addClass(const string& schoolName, const string& deptName, const string& className) {
		schoolTree.addClass(schoolName, deptName, className);
		cout << "Added class " << className << " in department " << deptName << " of school " << schoolName << endl;
	};

	// Searching and Printing
	void printAllSchools() {
		// just print all of them out
		cout << "School Structure: " << endl;
		schoolTree.printAll(); // All schools here
	};
	void searchSchool(const string& name) {
		// just find school and call out
		SchoolNode* school = schoolTree.searchSchool(name); // this will find said school
		if (!school) {
			// school was not found, hence nullptr
			cout << "School not found." << endl;
			return;
		}
		cout << "School Found: " << school->name << " with rating " << school->rating << endl;
	};

	// Rankings (FAST No.1 in Pasha Rankings)
	void rankSchools() {
		if (RankingSystem.isEmpty()) {
			cout << "No schools have been added yet." << endl;
			return;
		}
		// reset used flags
		SchoolRecord* r = schoolListHead;
		while (r) { r->used = false; r = r->next; }

		MaxHeap tempHeap = RankingSystem; // copy
		cout << "School Rankings: (High -> Low)" << endl;
		while (!tempHeap.isEmpty()) {
			int scaled = tempHeap.extractMax();
			// find first unused school with this rating
			SchoolRecord* cur = schoolListHead;
			SchoolRecord* match = nullptr;
			while (cur) {
				if (!cur->used && cur->scaledRating == scaled) {
					match = cur;
					break;
				}
				cur = cur->next;
			}
			if (match) {
				match->used = true;
				float rating = match->scaledRating / 10.0f;
				cout << match->name << "  (Rating: " << rating << ")" << endl;
			}
		}
	}

};
