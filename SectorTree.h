#pragma once
#include<iostream>
using namespace std;
#include <string>
#include <fstream>
#include "Types.h"

/*====================================================================
    SectorTree.h  (FULL IMPLEMENTATION)

    PURPOSE:
    --------
    This is the N-ARY TREE for Islamabad’s Administrative Layout.

    REQUIRED BY PROJECT:
        City ? Sector ? Street ? House

    WHY N-ARY TREE?
        Because every Sector has MANY streets
        Every Street has MANY houses
        Every house holds 1 family (stored as Family struct)
		Value of N is dynamic (varies per sector/street) e.g G-10 may have 30 streets, F-7 may have 20 streets

    THIS TREE IS USED FOR:
    ----------------------
    . Navigating Islamabad hierarchy
    . Searching by Sector
    . Searching by Street
    . Adding new Streets / Houses dynamically
    . Reports: population density, occupation summary, gender ratio



    CHILD STORAGE FORMAT:
    ---------------------
    Each node has a linked list of children:

        parent -> child -> nextSibling -> nextSibling -> ...

====================================================================*/

/*====================================================================
    Tree Node Structure
====================================================================*/
struct SectorNode {
    string name;                  // Example: "G-10" or "Street 22" or "House 180"

    // If this node is a house-level node -> attach Family info
    bool isHouse;
    Family family;                // only meaningful if isHouse = true

    // Child & sibling pointers (N-ary tree) ---------------------
    SectorNode* firstChild;       // first child in linked-list chain
    SectorNode* nextSibling;      // next node at same level

    // Constructor
    SectorNode(const string& n, bool house = false) {
        name = n;
        isHouse = house;
        firstChild = nullptr;
        nextSibling = nullptr;
    }
};


/*====================================================================
    SectorTree CLASS
    Main Islamabad hierarchical model
====================================================================*/
class SectorTree {
private:
    SectorNode* root;   // The root will be "Islamabad"

    /*------------------------------------------------------------
        findChild(parent, childName)
        Searches parent's linked list for a specific child.
    ------------------------------------------------------------*/
    SectorNode* findChild(SectorNode* parent, const string& childName) {
        SectorNode* curr = parent->firstChild;
        while (curr != nullptr) {
            if (curr->name == childName) return curr;
            curr = curr->nextSibling;
        }
        return nullptr;
    }

    /*------------------------------------------------------------
        addChild(parent, childName, isHouse)
        Adds a new child node to parent's children list.
    ------------------------------------------------------------*/
    SectorNode* addChild(SectorNode* parent, const string& childName, bool isHouse) {
        SectorNode* newNode = new SectorNode(childName, isHouse);

        // Insert at beginning for simplicity
        newNode->nextSibling = parent->firstChild;
        parent->firstChild = newNode;

        return newNode;
    }

    /*------------------------------------------------------------
        recursivePrint(node, depth)
        Prints entire tree structure (for debugging & viva).
    ------------------------------------------------------------*/
    void recursivePrint(SectorNode* node, int depth) {
        if (!node) return;

        // indent based on tree depth
        for (int i = 0; i < depth; i++) cout << "  ";
        cout << "- " << node->name;
        if (node->isHouse) cout << "  (House: " << node->family.name << ")";
        cout << "\n";

        recursivePrint(node->firstChild, depth + 1);
        recursivePrint(node->nextSibling, depth);
    }

public:
    // getter
    SectorNode* getRoot() {
        return root;
    }
    /*------------------------------------------------------------
        Constructor -> Root is always "Islamabad"
    ------------------------------------------------------------*/
    SectorTree() {
        root = new SectorNode("Islamabad", false);
    }

    /*------------------------------------------------------------
        addSector(sectorName)
        Example: "G-10", "F-7", "Blue Area"
    ------------------------------------------------------------*/
    void addSector(const string& sectorName) {
        if (findChild(root, sectorName) == nullptr)
            addChild(root, sectorName, false);
    }

    /*------------------------------------------------------------
        addStreet(sectorName, streetName)
        Example: sector="G-10", street="22"
    ------------------------------------------------------------*/
    void addStreet(const string& sectorName, const string& streetName) {
        SectorNode* sec = findChild(root, sectorName);
        if (!sec) return;

        if (!findChild(sec, streetName))
            addChild(sec, streetName, false);
    }

    /*------------------------------------------------------------
        addHouse(sector, street, houseNo, family)
        Example: street="22", house="180"
    ------------------------------------------------------------*/
    void addHouse(const string& sectorName, const string& streetName,
        const string& houseNo, const Family& fam) {

        SectorNode* sec = findChild(root, sectorName);
        if (!sec) return;

        SectorNode* st = findChild(sec, streetName);
        if (!st) return;

        // House already exists?
        SectorNode* hs = findChild(st, houseNo);
        if (!hs) {
            hs = addChild(st, houseNo, true);
        }

        // attach family
        hs->family = fam;
    }


    /*------------------------------------------------------------
        searchHouse(sector, street, house)
        Returns house node pointer
    ------------------------------------------------------------*/
    SectorNode* searchHouse(const string& sectorName, const string& streetName,
        const string& houseNo) {
        SectorNode* sec = findChild(root, sectorName);
        if (!sec) return nullptr;

        SectorNode* st = findChild(sec, streetName);
        if (!st) return nullptr;

        return findChild(st, houseNo);
    }


    /*------------------------------------------------------------
        printAll()
        Visualize the entire Islamabad tree
    ------------------------------------------------------------*/
    void printAll() {
        recursivePrint(root, 0);
    }

    
};