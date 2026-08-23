#pragma once
using namespace std;
#include <iostream>
#include <string>

// ============================================================
// Stack.h (ROUTE HISTORY STACK) — FULL IMPLEMENTATION
// ------------------------------------------------------------
// Required for:
//    Storing route travel history
//    Undo-like behavior
//    Backtracking in navigation
//
// Implementation:
//  
//    Stores simple strings (stop IDs or names)
// ============================================================

// ===================== STACK CLASS ===========================
class RouteStack {
private:
    string* arr;      // dynamic array
    int top;          // index of top element
    int capacity;     // max capacity

public:
    // ------------------------------------------------------------
    // Constructor
    // ------------------------------------------------------------
    RouteStack(int size = 50) {
        capacity = size;
        arr = new string[capacity];
        top = -1;
    }

    // ------------------------------------------------------------
    // Destructor
    // ------------------------------------------------------------
    ~RouteStack() {
        delete[] arr;
    }

    // ------------------------------------------------------------
    // isEmpty()
    // ------------------------------------------------------------
    bool isEmpty() {
        return top == -1;
    }

    // ------------------------------------------------------------
    // isFull()
    // ------------------------------------------------------------
    bool isFull() {
        return top == capacity - 1;
    }

    // ------------------------------------------------------------
    // push(stopId)
    // Add stop to top of stack
    // ------------------------------------------------------------
    bool push(const string& stopId) {
        if (isFull()) {
            cout << "Stack is full, cannot push stop." << endl;
            return false;
        }
        top++;
        arr[top] = stopId;
        return true;
    }

    // ------------------------------------------------------------
    // pop()
    // Removes and returns top value
    // ------------------------------------------------------------
    string pop() {
        if (isEmpty()) {
            cout << "Stack is empty, nothing to pop." << endl;
            return "";
        }
        string value = arr[top];
        top--;
        return value;
    }

    // ------------------------------------------------------------
    // peek()
    // Returns the current top without removing
    // ------------------------------------------------------------
    string peek() {
        if (isEmpty()) {
            cout << "Stack is empty." << endl;
            return "";
        }
        return arr[top];
    }

    // ------------------------------------------------------------
    // printStack()
    // Shows full travel history
    // ------------------------------------------------------------
    void printStack() {
        if (isEmpty()) {
            cout << "Stack is empty." << endl;
            return;
        }

        cout << "Travel History (Top to Bottom):" << endl;
        for (int i = top; i >= 0; i--) {
            cout << "  " << arr[i] << endl;
        }
    }
};