#pragma once
using namespace std;
#include <string>
#include <fstream>
#include <iostream>
#include "Types.h"

// ============================================================
// FamilyTree.h 
// ------------------------------------------------------------
// Purpose:
//   This tree stores INDIVIDUALS inside a FAMILY.
//   It is a simple N-ary tree where:
//       Root = Family
//       Children = Individuals
//
// Why a tree?
//   - project requirement: Family -> Individuals
//   - Easy traversal for reports (age, occupation, gender ratio)

// ============================================================

// ======================= INDIVIDUAL NODE ======================
struct IndividualNode {
    Individual data;             // name, age, occupation, gender
    IndividualNode* next;        // next individual in the list

    IndividualNode(const Individual& i) {
        data = i;
        next = nullptr;
    }
};


// ======================= FAMILY TREE CLASS =====================
class FamilyTree {
private:
    Family family;               // stores family data and name
    IndividualNode* head;        // head of linked list of individuals

public:
    // ------------------------------------------------------------
    // Constructor
    // ------------------------------------------------------------
    FamilyTree() {
        family = Family();
        head = nullptr;
    }

    // ------------------------------------------------------------
    // setFamilyName(name)
    // Sets name of the family (usually head of household)
    // ------------------------------------------------------------
    void setFamilyName(const string& name) {
        family.name = name;
    }

    // ------------------------------------------------------------
    // addIndividual(ind)
    // Inserts an individual at the FRONT of linked list.
    // ------------------------------------------------------------
    void addIndividual(const Individual& ind) {
        if (family.memberCount >= MAX_MEMBERS) return; // array limit safety

        // Store inside array for reporting
        family.members[family.memberCount] = ind;
        family.memberCount++;

        // Also maintain pointer-based list for traversal
        IndividualNode* node = new IndividualNode(ind);
        node->next = head;
        head = node;
    }

    // ------------------------------------------------------------
    // printFamily()
    // Prints family name and all individuals.
    // ------------------------------------------------------------
    void printFamily() {
        cout << "Family Name: " << family.name << "\n";
        cout << "Members (" << family.memberCount << "):\n";

        IndividualNode* curr = head;
        while (curr != nullptr) {
            cout << "  - " << curr->data.name
                << ", Age: " << curr->data.age
                << ", Occupation: " << curr->data.occupation
                << ", Gender: " << curr->data.gender
                << "\n";
            curr = curr->next;
        }
    }

    // ------------------------------------------------------------
    // getFamily()
    // Returns the Family struct by value.
    // ------------------------------------------------------------
    Family getFamily() const {
        return family;
    }

    // ------------------------------------------------------------
    // countByGender(gender)
    // Helps build gender ratio reports.
    // ------------------------------------------------------------
    int countByGender(const string& g) {
        int cnt = 0;
        IndividualNode* curr = head;
        while (curr != nullptr) {
            if (curr->data.gender == g) cnt++;
            curr = curr->next;
        }
        return cnt;
    }

    // ------------------------------------------------------------
    // countByOccupation(occ)
    // Helps build occupation summary reports.
    // ------------------------------------------------------------
    int countByOccupation(const string& occ) {
        int cnt = 0;
        IndividualNode* curr = head;
        while (curr != nullptr) {
            if (curr->data.occupation == occ) cnt++;
            curr = curr->next;
        }
        return cnt;
    }
};