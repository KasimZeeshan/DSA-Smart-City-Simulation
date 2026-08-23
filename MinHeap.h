#pragma once
#include<iostream>
using namespace std;
#include <string>
#include <fstream>

/*============================================================
    MinHeap.h (FULL IMPLEMENTATION)
    - Stores integer keys (distances, priorities)
    - Used for: Hospital emergency ranking, path optimization
============================================================*/

const int HEAP_MAX = 500;

class MinHeap {
private:
    int arr[HEAP_MAX];    // raw array storage
    int size;             // current number of elements

    // --- Helper: swap two elements ---
    void swap(int& a, int& b) {
        int temp = a;
        a = b;
        b = temp;
    }

    // --- Heapify upwards after insert ---
    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (arr[parent] <= arr[index]) break;
            swap(arr[parent], arr[index]);
            index = parent;
        }
    }

    // --- Heapify downwards after removal ---
    void heapifyDown(int index) {
        while (true) {
            int left = index * 2 + 1;
            int right = index * 2 + 2;
            int smallest = index;

            if (left < size && arr[left] < arr[smallest]) smallest = left;
            if (right < size && arr[right] < arr[smallest]) smallest = right;

            if (smallest == index) break;

            swap(arr[index], arr[smallest]);
            index = smallest;
        }
    }

public:
    MinHeap() {
        size = 0;
    }

    // --- Insert a new value into heap ---
    void insert(int value) {
        if (size >= HEAP_MAX) return; // heap full
        arr[size] = value;
        heapifyUp(size);
        size++;
    }

    // --- Returns true if empty ---
    bool isEmpty() const {
        return size == 0;
    }

    // --- Get min element (but do NOT remove it) ---
    int peek() const {
        if (size == 0) return -1;
        return arr[0];
    }

    // --- Remove and return min element ---
    int extractMin() {
        if (size == 0) return -1;

        int root = arr[0];
        arr[0] = arr[size - 1];
        size--;

        heapifyDown(0);
        return root;
    }

    // --- Clear heap ---
    void clear() {
        size = 0;
    }

    // --- Get number of items ---
    int getSize() const {
        return size;
    }
};