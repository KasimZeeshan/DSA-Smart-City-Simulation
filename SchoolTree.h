#pragma once
using namespace std;
#include <string>
#include <fstream>
#include "Types.h"

/*====================================================================
    SchoolTree.h 

    REQUIRED BY PROJECT:
        A STRICT 3-LEVEL TREE:
            School -> Department ? Class

    This structure represents:
        . Schools in Islamabad
        . Their departments (CS, Math, Physics, etc.)
        . Classes (e.g., 10th-A, 5th-B, BSCS-1A)

    WHY A TREE?
        - Hierarchical academic model
        - Fast traversal
        - Clear separation of levels

    CHILD STORAGE FORMAT:
        node->firstChild -> nextSibling -> nextSibling …
        ( N-ary linked children list)
====================================================================*/

/*====================================================================
    Node Structure (Shared for School, Department, and Class)
====================================================================*/
struct SchoolNode {
    string name;           // e.g., "City School", "Math Dept", "10th-A"
    float rating;          // meaningful only at school-level

    // Linked children & siblings
    SchoolNode* firstChild;
    SchoolNode* nextSibling;

    // Constructor
    SchoolNode(const string& n, float r = 0.0f) {
        name = n;
        rating = r;
        firstChild = nullptr;
        nextSibling = nullptr;
    }
};


/*====================================================================
    SCHOOL TREE CLASS
====================================================================*/
class SchoolTree {
private:
    SchoolNode* root;   // Root = "Schools" container node

    /*------------------------------------------------------------
        findChild(parent, childName)
        Searches for a specific child node.
    ------------------------------------------------------------*/
    SchoolNode* findChild(SchoolNode* parent, const string& childName) {
        SchoolNode* curr = parent->firstChild;
        while (curr != nullptr) {
            if (curr->name == childName) return curr;
            curr = curr->nextSibling;
        }
        return nullptr;
    }

    /*------------------------------------------------------------
        addChild(parent, name, rating)
        Adds a new node to parent's children at HEAD of list.
    ------------------------------------------------------------*/
    SchoolNode* addChild(SchoolNode* parent, const string& name, float rating) {
        SchoolNode* newNode = new SchoolNode(name, rating);
        newNode->nextSibling = parent->firstChild;
        parent->firstChild = newNode;
        return newNode;
    }

    /*------------------------------------------------------------
        recursivePrint display helper for viva/debug
    ------------------------------------------------------------*/
    void recursivePrint(SchoolNode* node, int depth) {
        if (!node) return;

        for (int i = 0; i < depth; i++)
            cout << "  ";

        cout << "- " << node->name;
        if (depth == 1) cout << "  (Rating: " << node->rating << ")";
        cout << "\n";

        recursivePrint(node->firstChild, depth + 1);
        recursivePrint(node->nextSibling, depth);
    }

public:
    /*------------------------------------------------------------
        Constructor -> Root = "Schools"
    ------------------------------------------------------------*/
    SchoolTree() {
        root = new SchoolNode("Schools");
    }

    /*------------------------------------------------------------
        addSchool(name, rating)
        Creates level-1 node.
    ------------------------------------------------------------*/
    void addSchool(const string& schoolName, float rating) {
        if (!findChild(root, schoolName)) {
            addChild(root, schoolName, rating);
        }
    }

    /*------------------------------------------------------------
        addDepartment(school, dept)
        Level-2 child node.
    ------------------------------------------------------------*/
    void addDepartment(const string& schoolName, const string& deptName) {
        SchoolNode* school = findChild(root, schoolName);
        if (!school) return;

        if (!findChild(school, deptName)) {
            addChild(school, deptName, 0.0f);
        }
    }

    /*------------------------------------------------------------
        addClass(school, department, className)
        Level-3 node.
    ------------------------------------------------------------*/
    void addClass(const string& schoolName, const string& deptName,
        const string& className) {
        SchoolNode* school = findChild(root, schoolName);
        if (!school) return;

        SchoolNode* dept = findChild(school, deptName);
        if (!dept) return;

        if (!findChild(dept, className)) {
            addChild(dept, className, 0.0f);
        }
    }

    /*------------------------------------------------------------
        searchSchool() -> returns pointer
    ------------------------------------------------------------*/
    SchoolNode* searchSchool(const string& schoolName) {
        return findChild(root, schoolName);
    }

    /*------------------------------------------------------------
        searchDepartment()
    ------------------------------------------------------------*/
    SchoolNode* searchDepartment(const string& schoolName, const string& deptName) {
        SchoolNode* school = findChild(root, schoolName);
        if (!school) return nullptr;

        return findChild(school, deptName);
    }

    /*------------------------------------------------------------
        searchClass()
    ------------------------------------------------------------*/
    SchoolNode* searchClass(const string& schoolName, const string& deptName,
        const string& className) {
        SchoolNode* dept = searchDepartment(schoolName, deptName);
        if (!dept) return nullptr;

        return findChild(dept, className);
    }

    /*------------------------------------------------------------
        printAll() -> complete 3-level tree
    ------------------------------------------------------------*/
    void printAll() {
        recursivePrint(root, 0);
    }
};