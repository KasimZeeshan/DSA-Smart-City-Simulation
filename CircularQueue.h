#pragma once
using namespace std;
#include <iostream>
#include <string>

// ============================================================
// CircularQueue.h- array based circular queue
// ------------------------------------------------------------
// Required for:
//    Passenger queue handling
//    Any first-come first-serve simulation
//    Implement using circular array
//
// Behavior:
//   front  = index of first element
//   rear   = index where next element will be inserted
//   count  = number of elements in queue
// ============================================================

// ===================== PASSENGER DATA =======================
// You can store anything here. For now, simple passenger record.
struct Passenger {
    string name;
    string destinationStop;
};


// ===================== CIRCULAR QUEUE CLASS =================
class CircularQueue {
private:
    Passenger* arr;   // dynamic array
    int capacity;     // max size
    int front;        // index of first element
    int rear;         // index of next insertion
    int count;        // current size

public:
    // ------------------------------------------------------------
    // Constructor
    // ------------------------------------------------------------
    CircularQueue(int size = 20) {
        capacity = size;
        arr = new Passenger[capacity];
        front = 0;
        rear = 0;
        count = 0;
    }

    // ------------------------------------------------------------
    // Destructor
    // ------------------------------------------------------------
    ~CircularQueue() {
        delete[] arr;
    }

    // ------------------------------------------------------------
    // isEmpty()
    // ------------------------------------------------------------
    bool isEmpty() {
        return count == 0;
    }

    // ------------------------------------------------------------
    // isFull()
    // ------------------------------------------------------------
    bool isFull() {
        return count == capacity;
    }

    // ------------------------------------------------------------
    // enqueue(p)
    // Add passenger to queue (rear)
    // ------------------------------------------------------------
    bool enqueue(const Passenger& p) {
        if (isFull()) {
            cout << "Queue is full, cannot add passenger.\n";
            return false;
        }

        arr[rear] = p;
        rear = (rear + 1) % capacity;
        count++;
        return true;
    }

    // ------------------------------------------------------------
    // dequeue()
    // Remove AND return front passenger
    // ------------------------------------------------------------
    Passenger dequeue() {
        Passenger temp;
        temp.name = "";
        temp.destinationStop = "";

        if (isEmpty()) {
            cout << "Queue is empty, nothing to remove.\n";
            return temp;
        }

        temp = arr[front];
        front = (front + 1) % capacity;
        count--;
        return temp;
    }

    // ------------------------------------------------------------
    // peek()
    // Returns front passenger without removing
    // ------------------------------------------------------------
    Passenger peek() {
        Passenger temp;
        temp.name = "";
        temp.destinationStop = "";

        if (isEmpty()) {
            cout << "Queue is empty.\n";
            return temp;
        }

        return arr[front];
    }

    // ------------------------------------------------------------
    // printQueue()
    // Displays all passengers in queue order
    // ------------------------------------------------------------
    void printQueue() {
        if (isEmpty()) {
            cout << "Queue is empty.\n";
            return;
        }

        cout << "Passengers in queue:\n";
        int idx = front;
        for (int i = 0; i < count; i++) {
            cout << "  Name: " << arr[idx].name
                << " | Destination: " << arr[idx].destinationStop << "\n";
			idx = (idx + 1) % capacity; //circular increment
        }
    }
};