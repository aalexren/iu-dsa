//
// Created by Артём Черница on 29.01.21.
//

#ifndef LINKEDLIST_LINKEDLIST_H
#define LINKEDLIST_LINKEDLIST_H

/*
 * Circular double linked list with a sentinel using pointers
 */

template <typename T>
class Node {
public:
    Node<T> *prev;
    Node<T> *next;
    T value;
};

template <typename T>
class LinkedList {
private:
    Node<T> *_head;
    Node<T> *_tail;
    size_t _length;
public:
    LinkedList() {
        _head = _tail = new Node<T>;
        _length = 0;
    }

    size_t size() {
        return _length;
    }

    bool isEmpty() {
        return _length > 0;
    }

    T get(size_t index) {
        Node<T> *iter = new Node<T>;
        if (index > _length / 2) {
            // backward
            iter = _tail;
            for (int i = 0; i < index; ++i) {
                iter = iter->prev;
            }
        }
        else {
            // forward
            iter = _head;
            for (int i = 0; i < index; ++i) {
                iter = iter->next;
            }
        }

        return iter->value;
    }

    void set(T value, size_t i) {

    }

    void add(T value, size_t i) {

    }

    void remove(size_t i) {

    }

    void addFirst(T value) {
        Node<T> *temp = new Node<T>;
        temp->value = value;

        if (_length == 0) {
            _head = temp;
            _tail = temp;
            _head->next = _tail;
            _head->prev = _tail;
            _tail->next = _head;
            _tail->prev = _head;
        }
        else if (_length == 1) {
            
        }

//        Node<T> *temp = new Node<T>;
//        temp->value = value;
//        temp->next = _head;
//        temp->prev = _tail;
//        _head->prev = temp;
//        _tail->next = temp;
//        _head = temp;

        _length++;
    }

    void addLast(T value) {

    }

    void removeFirst() {

    }

    void removeLast() {

    }
};

#endif //LINKEDLIST_LINKEDLIST_H
