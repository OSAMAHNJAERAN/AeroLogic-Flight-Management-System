/**
 * ============================================================================
 * CUSTOM DATA STRUCTURES IMPLEMENTATION
 * ============================================================================
 * * Includes:
 * 1. LinkedList (Doubly Linked) - For dynamic passenger storage
 * 2. Stack - For "Undo Last Booking" feature
 * 3. Queue - For "Waitlist" on full flights
 * 4. HashTable - For O(1) Passenger Search
 *
 * ============================================================================
 */

#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include "Passenger.h"
#include "Utility.h"
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

// Generic Node Structure
struct Node {
  Passenger data;
  Node *next;
  Node *prev; // Useful for Doubly Linked List

  Node(Passenger p) : data(p), next(nullptr), prev(nullptr) {}
};

// ============================================================================
// 1.5. QUEUE (FIFO) - For Flight Waitlist
// ============================================================================
class WaitlistQueue {
private:
  struct QueueNode {
    Passenger data;
    QueueNode *next;
    QueueNode(Passenger p) : data(p), next(nullptr) {}
  };
  QueueNode *front, *rear;
  int count;

public:
  WaitlistQueue() : front(nullptr), rear(nullptr), count(0) {}

  void enqueue(Passenger p) {
    QueueNode *newNode = new QueueNode(p);
    if (rear == nullptr) {
      front = rear = newNode;
    } else {
      rear->next = newNode;
      rear = newNode;
    }
    count++;
  }

  bool dequeue(Passenger &p) {
    if (front == nullptr)
      return false;
    QueueNode *temp = front;
    p = temp->data;
    front = front->next;
    if (front == nullptr)
      rear = nullptr;
    delete temp;
    count--;
    return true;
  }

  bool isEmpty() { return front == nullptr; }
  int getCount() { return count; }

  ~WaitlistQueue() {
    while (front) {
      QueueNode *temp = front;
      front = front->next;
      delete temp;
    }
  }
};

struct FlightStruct {
  int id;
  std::string destination;
  std::string date;
  std::string time;
  double price;
  double firstClassPrice;
  std::string cabin[ROWS][COLS];
  WaitlistQueue waitlist;

  FlightStruct() : id(0), price(0.0), firstClassPrice(0.0) {
    for (int i = 0; i < ROWS; i++)
      for (int j = 0; j < COLS; j++)
        cabin[i][j] = "";
  }
};

struct FlightNode {
  FlightStruct data;
  FlightNode *next;
  FlightNode(FlightStruct f) : data(f), next(nullptr) {}
};

class FlightList {
private:
  FlightNode *head;
  FlightNode *tail;
  int size;

public:
  FlightList() : head(nullptr), tail(nullptr), size(0) {}

  void insert(FlightStruct f) {
    FlightNode *newNode = new FlightNode(f);
    if (!head) {
      head = tail = newNode;
    } else {
      tail->next = newNode;
      tail = newNode;
    }
    size++;
  }

  FlightStruct *search(int id) {
    FlightNode *current = head;
    while (current) {
      if (current->data.id == id) {
        return &current->data;
      }
      current = current->next;
    }
    return nullptr;
  }

  FlightNode *getHead() const { return head; }
  int getSize() const { return size; }

  ~FlightList() {
    while (head) {
      FlightNode *temp = head;
      head = head->next;
      delete temp;
    }
  }
};

// ============================================================================
// 1. LINKED LIST (Doubly Linked)
// ============================================================================
class PassengerList {
private:
  Node *head;
  Node *tail;
  int size;

public:
  PassengerList() : head(nullptr), tail(nullptr), size(0) {}

  void insert(Passenger p) {
    Node *newNode = new Node(p);
    if (!head) {
      head = tail = newNode;
    } else {
      tail->next = newNode;
      newNode->prev = tail;
      tail = newNode;
    }
    size++;
  }

  // Remove by unique Name
  bool remove(string name) {
    if (!head)
      return false;

    Node *current = head;
    string upperSearch = toUpperCase(name);

    while (current) {
      if (toUpperCase(current->data.name) == upperSearch) {
        if (current == head && current == tail) {
          head = tail = nullptr;
        } else if (current == head) {
          head = head->next;
          head->prev = nullptr;
        } else if (current == tail) {
          tail = tail->prev;
          tail->next = nullptr;
        } else {
          current->prev->next = current->next;
          current->next->prev = current->prev;
        }
        delete current;
        size--;
        return true;
      }
      current = current->next;
    }
    return false;
  }

  Passenger *find(string name) {
    Node *current = head;
    string upperSearch = toUpperCase(name);
    while (current) {
      if (toUpperCase(current->data.name) == upperSearch) {
        return &current->data;
      }
      current = current->next;
    }
    return nullptr;
  }

  // Convert to array for display/processing if needed, or iterate
  int getSize() const { return size; }

  Node *getHead() const { return head; }

  // Merge Sort for Linked List
  Node *merge(Node *first, Node *second) {
    if (!first)
      return second;
    if (!second)
      return first;

    if (toUpperCase(first->data.name) < toUpperCase(second->data.name)) {
      first->next = merge(first->next, second);
      first->next->prev = first;
      first->prev = nullptr;
      return first;
    } else {
      second->next = merge(first, second->next);
      second->next->prev = second;
      second->prev = nullptr;
      return second;
    }
  }

  Node *split(Node *head) {
    Node *fast = head;
    Node *slow = head;
    while (fast->next && fast->next->next) {
      fast = fast->next->next;
      slow = slow->next;
    }
    Node *temp = slow->next;
    slow->next = nullptr;
    return temp;
  }

  Node *mergeSort(Node *node) {
    if (!node || !node->next)
      return node;
    Node *second = split(node);
    node = mergeSort(node);
    second = mergeSort(second);
    return merge(node, second);
  }

  void sort() {
    head = mergeSort(head);
    // Fix tail pointer after sort
    Node *temp = head;
    while (temp && temp->next)
      temp = temp->next;
    tail = temp;
  }

  void display() {
    Node *current = head;
    int i = 1;
    cout << "\n    +----+----------------------------+--------+-------------+"
         << endl;
    cout << "    | #  | Name                       | Seat   | Class       |"
         << endl;
    cout << "    +----+----------------------------+--------+-------------+"
         << endl;
    while (current) {
      cout << "    | " << right << setw(2) << i++ << " | " << left << setw(26)
           << current->data.name << " | " << setw(6) << current->data.seatNumber
           << " | " << setw(11) << current->data.classType << " |" << endl;
      current = current->next;
    }
    cout << "    +----+----------------------------+--------+-------------+"
         << endl;
  }

  ~PassengerList() {
    while (head) {
      Node *temp = head;
      head = head->next;
      delete temp;
    }
  }
};

// ============================================================================
// 2. STACK (LIFO) - For Undo Operations
// ============================================================================
class BookingStack {
private:
  struct StackNode {
    Passenger data;
    StackNode *next;
    StackNode(Passenger p) : data(p), next(nullptr) {}
  };
  StackNode *top;

public:
  BookingStack() : top(nullptr) {}

  void push(Passenger p) {
    StackNode *newNode = new StackNode(p);
    newNode->next = top;
    top = newNode;
  }

  bool pop(Passenger &p) {
    if (isEmpty())
      return false;
    StackNode *temp = top;
    p = temp->data;
    top = top->next;
    delete temp;
    return true;
  }

  bool isEmpty() { return top == nullptr; }

  ~BookingStack() {
    while (top) {
      StackNode *temp = top;
      top = top->next;
      delete temp;
    }
  }
};

// ============================================================================
// 3. QUEUE (FIFO) - For Flight Waitlist
// ============================================================================

// ============================================================================
// 4. HASH TABLE - For O(1) Searching
// ============================================================================
const int TABLE_SIZE = 1543; // Prime number > 1440

class PassengerHashTable {
private:
  Node *table[TABLE_SIZE]; // Array of pointers to Nodes (Chaining)

  int hashFunction(string key) {
    int sum = 0;
    for (char c : key)
      sum += toupper(c);
    return sum % TABLE_SIZE;
  }

public:
  PassengerHashTable() {
    for (int i = 0; i < TABLE_SIZE; i++)
      table[i] = nullptr;
  }

  void insert(Passenger p) {
    int index = hashFunction(p.name);
    Node *newNode = new Node(p);
    // Insert at beginning of chain
    newNode->next = table[index];
    table[index] = newNode;
  }

  bool remove(string name) {
    int index = hashFunction(name);
    Node *current = table[index];
    Node *prev = nullptr;
    string upperSearch = toUpperCase(name);

    while (current) {
      if (toUpperCase(current->data.name) == upperSearch) {
        if (prev == nullptr) {
          table[index] = current->next;
        } else {
          prev->next = current->next;
        }
        delete current;
        return true;
      }
      prev = current;
      current = current->next;
    }
    return false;
  }

  Passenger *search(string name) {
    int index = hashFunction(name);
    Node *current = table[index];
    string upperSearch = toUpperCase(name);

    while (current) {
      if (toUpperCase(current->data.name) == upperSearch) {
        return &current->data;
      }
      current = current->next;
    }
    return nullptr;
  }

  ~PassengerHashTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
      Node *current = table[i];
      while (current) {
        Node *temp = current;
        current = current->next;
        delete temp;
      }
    }
  }
};

#endif