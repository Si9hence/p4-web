#ifndef LIST_H
#define LIST_H
/* List.h
 *
 * doubly-linked, double-ended list with Iterator interface
 * Project UID c1f28c309e55405daf00c565d57ff9ad
 * EECS 280 Project 4
 */

#include <iostream>
#include <cassert> //assert
#include <cstddef> //NULL

template <typename T>
class List {
    // OVERVIEW: a doubly-linked, double-ended list with Iterator interface
public:
    // EFFECTS:  returns true if the list is empty
    bool empty() const;

    // EFFECTS: returns the number of elements in this List
    // HINT:    Traversing a list is really slow.  Instead, keep track of the size
    //          with a private member variable.  That's how std::list does it.
    int size() const;

    // REQUIRES: list is not empty
    // EFFECTS: Returns the first element in the list by reference
    T& front();

    // REQUIRES: list is not empty
    // EFFECTS: Returns the last element in the list by reference
    T& back();

    // EFFECTS:  inserts datum into the front of the list
    void push_front(const T& datum);

    // EFFECTS:  inserts datum into the back of the list
    void push_back(const T& datum);

    // REQUIRES: list is not empty
    // MODIFIES: may invalidate list iterators
    // EFFECTS:  removes the item at the front of the list
    void pop_front();

    // REQUIRES: list is not empty
    // MODIFIES: may invalidate list iterators
    // EFFECTS:  removes the item at the back of the list
    void pop_back();

    // MODIFIES: may invalidate list iterators
    // EFFECTS:  removes all items from the list
    void clear();

    // You should add in a default constructor, destructor, copy constructor,
    // and overloaded assignment operator, if appropriate. If these operations
    // will work correctly without defining these, you can omit them. A user
    // of the class must be able to create, copy, assign, and destroy Lists
    List();
    ~List() {
        clear();
    }

    List& operator=(const List& rhs) {
        if (this != &rhs) {
            clear();
            copy_all(rhs);
        }
        return *this;
    };

    bool operator==(const List& rhs) {
        if (this->first == nullptr && rhs.first == nullptr) {
            return true;
        }
        Node* p = this->front();
        Node* q = rhs->front();
        while (!p) {
            if (*p != *q) {
                return false;
            }
            p = p->next;
            q = q->next;
        }
        if (!q) {
            return true;
        } else {
            return false;
        }
    };

    bool operator!=(const List& rhs) {
        return !(this == rhs);
    }

    List& operator+(const T& datum) {
        this->push_back(datum);
        return this;
    }

private:
    // a private type
    struct Node {
        Node* next;
        Node* prev;
        T datum;
    };

    // REQUIRES: list is empty
    // EFFECTS:  copies all nodes from other to this
    void copy_all(const List<T>& other);

    Node* first; // points to first Node in list, or nullptr if list is empty
    Node* last;  // points to last Node in list, or nullptr if list is empty
    int list_len;
public:
    ////////////////////////////////////////
    class Iterator {
        // OVERVIEW: Iterator interface to List

        // You should add in a default constructor, destructor, copy constructor,
        // and overloaded assignment operator, if appropriate. If these operations
        // will work correctly without defining these, you can omit them. A user
        // of the class must be able to create, copy, assign, and destroy Iterators.

        // Your iterator should implement the following public operators: *,
        // ++ (prefix), default constructor, == and !=.

    public:

        // This operator will be used to test your code. Do not modify it.
        // Requires that the current element is dereferenceable.
        Iterator& operator--() {
            assert(node_ptr);
            node_ptr = node_ptr->prev;
            return *this;
        }
        T operator*() const {
            return node_ptr->datum;
        }
        bool operator==(const Iterator& rhs) const {
            return (node_ptr == rhs.node_ptr);
        }
        bool operator!=(const Iterator& rhs) const {
            return (node_ptr != rhs.node_ptr);
        }
        Iterator& operator++() {
            assert(node_ptr);
            node_ptr = node_ptr->next;
            return *this;
        }

    private:
        Node* node_ptr; // current Iterator position is a List node
        // add any additional necessary member variables here

        // add any friend declarations here
        friend class List;
        // construct an Iterator at a specific position
        Iterator(Node* p):node_ptr(p) {};
    }; // List::Iterator
    ////////////////////////////////////////

    // return an Iterator pointing to the first element
    Iterator begin() const {
        return Iterator(first);
    }

    // return an Iterator pointing to "past the end"
    Iterator end() const {
        return Iterator(last);
    }

    // REQUIRES: i is a valid, dereferenceable iterator associated with this list
    // MODIFIES: may invalidate other list iterators
    // EFFECTS: Removes a single element from the list container
    void erase(Iterator i) {
        if (i.node_ptr->prev) {
            i.node_ptr->prev->next = i.node_ptr->next;
        }
        if (i.node_ptr->next) {
            i.node_ptr->next->prev = i.node_ptr->prev;
        }
        return;
    }

    // REQUIRES: i is a valid iterator associated with this list
    // EFFECTS: inserts datum before the element at the specified position.
    void insert(Iterator i, const T& datum) {
        Node* temp = new Node{ i.node_ptr->next, i.node_ptr->prev, datum };
        if (i.node_ptr->prev) {
            i.node_ptr->prev->next = temp;
        }
        if (i.node_ptr->next) {
            i.node_ptr->next->prev = temp;
        }
        return;
    }

}; // List

////////////////////////////////////////////////////////////////////////////////
// Add your member function implementations below or in the class above
// (your choice). Do not change the public interface of List, although you
// may add the Big Three if needed.  Do add the public member functions for
// Iterator.
template <typename T>
List<T>::List():first(nullptr), last(nullptr), list_len(0) {};

template <typename T>
bool List<T>::empty() const {
    return first == nullptr;
    // assert(false);
}

template <typename T>
int List<T>::size() const {
    return this->list_len;
}

template <typename T>
T& List<T>::front() {
    return first->datum;
}

template <typename T>
T& List<T>::back() {
    return last->datum;
}

template <typename T>
void List<T>::push_front(const T& datum) {
    first = new Node{ first, nullptr, datum };
    if (this->list_len == 0) {
        last = first;
    }
    this->list_len++;
    return;
}

template <typename T>
void List<T>::push_back(const T& datum) {
    if (empty()) {
        Node* new_node = new Node{ first, nullptr, datum };
        last = new_node;
        first = new_node;

    } else {
        Node* new_node = new Node{ nullptr, last, datum };
        // last = last->next = new_node;
        // std::cout << last->next << std::endl;
        last = last->next = new_node;
    }
    this->list_len++;
    return;
}

template <typename T>
void List<T>::pop_front() {
    assert(!empty());
    // Node* temp = first;
    first = first->next;
    this->list_len--;
    // delete temp;
    return;
}

template <typename T>
void List<T>::pop_back() {
    assert(!empty());
    Node* temp = last;
    last = last->prev;
    this->list_len--;
    temp->~();
    return;
}

template <typename T>
void List<T>::clear() {
    while (!empty()) {
        pop_front();
    }
    this->list_len = 0;
    return;
}

template <typename T>
void List<T>::copy_all(const List<T>& other) {
    assert(empty());
    for (Node* node_ptr = other.first;node_ptr;node_ptr = node_ptr->next) {
        push_back(node_ptr->datum);
    }
    return;
}

#endif // Do not remove this. Write all your code above this line.
