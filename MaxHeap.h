#pragma once
#include<iostream>
using namespace std;
#include <string>
#include <fstream>

/*============================================================
    MaxHeap.h (FULL IMPLEMENTATION)
    - Stores integer keys (e.g., emergency bed availability)
    - Used for: Hospital Emergency Handling (highest beds first)

    ------------------------------------------------------------
    A MAX HEAP ensures the *largest* value is always at index 0.

    Representation inside an array:

            arr[0]          <- largest element (root)
           /      \
     arr[1]        arr[2]
       /  \         /   \
   arr[3] arr[4] arr[5] arr[6]

    Child formulas:
        leftChild  = 2*i + 1
        rightChild = 2*i + 2

    Parent formula:
        parent = (i - 1) / 2
============================================================*/

const int MAX_HEAP_SIZE = 500;   

class MaxHeap {
private:
    int arr[MAX_HEAP_SIZE];   // raw storage for heap
    int size;                 // current number of elements

    /*--------------------------------------------------------
        swap(a, b)
        Manual swap to avoid using <algorithm> (not allowed).
    --------------------------------------------------------*/
    void swap(int& a, int& b) {
        int temp = a;
        a = b;
        b = temp;
    }

    /*--------------------------------------------------------
        heapifyUp(index)

        Called AFTER inserting a new value at arr[size].
        We compare with parent:
            If parent < child ? swap them.
        Continue until heap property restored.

        This keeps the largest value traveling UP toward root.
    --------------------------------------------------------*/
    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;

            // If child is <= parent, heap property is OK
            if (arr[parent] >= arr[index]) break;

            // Otherwise swap upward
            swap(arr[parent], arr[index]);
            index = parent;
        }
    }

    /*--------------------------------------------------------
        heapifyDown(index)

        Called AFTER removing the max element.

        Steps:
        - Compare parent with both children.
        - Find the LARGEST among them.
        - If child > parent ? swap downward.
        - Continue until heap property is safe.

        This ensures the new root moves DOWN to correct spot.
    --------------------------------------------------------*/
    void heapifyDown(int index) {
        while (true) {
            int left = index * 2 + 1;
            int right = index * 2 + 2;
            int largest = index;

            // Check left child
            if (left < size && arr[left] > arr[largest]) {
                largest = left;
            }

            // Check right child
            if (right < size && arr[right] > arr[largest]) {
                largest = right;
            }

            // If parent already largest ? stop
            if (largest == index) break;

            // Otherwise push parent downward
            swap(arr[index], arr[largest]);
            index = largest;
        }
    }

public:
    /*--------------------------------------------------------
        Constructor
        Simply starts heap with zero elements.
    --------------------------------------------------------*/
    MaxHeap() {
        size = 0;
    }

    /*--------------------------------------------------------
        insert(value)
    --------------------------------------------------------*/
    void insert(int value) {
        if (size >= MAX_HEAP_SIZE) return; // ignore if heap full

        arr[size] = value;       // place value at end
        heapifyUp(size);         // bubble upward
        size++;                  // increase count
    }

    /*--------------------------------------------------------
        isEmpty()
        Returns TRUE if heap has no values.
    --------------------------------------------------------*/
    bool isEmpty() const {
        return size == 0;
    }

    /*--------------------------------------------------------
        peek()
        Returns the CURRENT maximum element WITHOUT removing.
        If heap is empty, return -1 as safe fallback.
    --------------------------------------------------------*/
    int peek() const {
        if (size == 0) return -1;
        return arr[0];           // root contains the maximum
    }

    /*--------------------------------------------------------
        extractMax()

    --------------------------------------------------------*/
    int extractMax() {
        if (size == 0) return -1;       // cannot remove

        int root = arr[0];              // largest value
        arr[0] = arr[size - 1];         // move last to root
        size--;                          // shrink heap

        heapifyDown(0);                 // restore ordering
        return root;
    }

    /*--------------------------------------------------------
        clear()
        Just sets size = 0 (array becomes empty logically).
    --------------------------------------------------------*/
    void clear() {
        size = 0;
    }

    /*--------------------------------------------------------
        getSize()
        Returns number of items in heap.
    --------------------------------------------------------*/
    int getSize() const {
        return size;
    }
};