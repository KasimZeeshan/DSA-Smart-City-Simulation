#pragma once
using namespace std;
#include <string>
#include <fstream>
#include <iostream>
#include "Types.h"

// ============================================================
// LinkedList.h
// ------------------------------------------------------------
//   . RouteNode structure
//   . addStop(id)            -- append at end
//   . removeStop(id)         -- remove first occurrence
//   . searchStop(id)         -- find if present
//   . printRoute()           -- print in order
//   . clear()                -- delete entire route
//   . getNextAfter(id)       -- NEW: returns next stop ID after id (or "" if none)
// ============================================================

// ===================== ROUTE NODE ============================
struct RouteNode {
    string stopId;          // ID of bus stop (Stop1, Stop2, ...)
    RouteNode* next;        // next stop

    RouteNode(const string& id) {
        stopId = id;
        next = nullptr;
    }
};

// ===================== LINKED LIST FOR ROUTE =================
class RouteLinkedList {
    public:
    RouteNode* head;        // first stop in route
    RouteNode* tail;        // last stop (for fast append)

public:
    // Constructor
    RouteLinkedList() {
        head = nullptr;
        tail = nullptr;
    }

    // Adds a stop to END of route.
    void addStop(const string& id) {
        RouteNode* node = new RouteNode(id);

        if (head == nullptr) {
            head = node;
            tail = node;
            return;
        }

        tail->next = node;
        tail = node;
    }

    // Removes first occurrence of stopId.
    void removeStop(const string& id) {
        if (!head) return;

        // Case 1: first node
        if (head->stopId == id) {
            RouteNode* temp = head;
            head = head->next;
            if (head == nullptr) tail = nullptr;
            delete temp;
            return;
        }

        // Case 2: middle or last
        RouteNode* curr = head;
        while (curr->next != nullptr && curr->next->stopId != id) {
            curr = curr->next;
        }

        if (curr->next != nullptr) {
            RouteNode* temp = curr->next;
            curr->next = temp->next;
            if (temp == tail) tail = curr;
            delete temp;
        }
    }

    // Returns true if stop exists in route.
    bool searchStop(const string& id) {
        RouteNode* curr = head;
        while (curr != nullptr) {
            if (curr->stopId == id) return true;
            curr = curr->next;
        }
        return false;
    }

    // Prints the full linked-list route.
    void printRoute() {
        cout << "Route: ";
        RouteNode* curr = head;
        while (curr != nullptr) {
            cout << curr->stopId;
            if (curr->next) cout << " -> ";
            curr = curr->next;
        }
        cout << "\n";
    }

    // Deletes entire route.
    void clear() {
        RouteNode* curr = head;
        while (curr != nullptr) {
            RouteNode* temp = curr;
            curr = curr->next;
            delete temp;
        }
        head = nullptr;
        tail = nullptr;
    }

   
    // This is used by the SchoolBusTracker to advance buses along routes.
    string getNextAfter(const string& stopId) {
        RouteNode* curr = head;
        while (curr != nullptr) {
            if (curr->stopId == stopId) {
                if (curr->next != nullptr) return curr->next->stopId;
                else return ""; // last node, no next
            }
            curr = curr->next;
        }
        return ""; // not found
    }

    RouteNode* getHead() {
        return head;
    }

    // Destructor
    ~RouteLinkedList() {
        clear();
    }

    // Copy constructor
    RouteLinkedList(const RouteLinkedList& other) {
        head = nullptr;
        tail = nullptr;

        // Deep copy all nodes
        RouteNode* curr = other.head;
        while (curr != nullptr) {
            addStop(curr->stopId);
            curr = curr->next;
        }
    }

    // Assignment operator
    RouteLinkedList& operator=(const RouteLinkedList& other) {
        if (this != &other) {
            // Clear existing
            clear();

            // Deep copy all nodes
            RouteNode* curr = other.head;
            while (curr != nullptr) {
                addStop(curr->stopId);
                curr = curr->next;
            }
        }
        return *this;
    }

};
