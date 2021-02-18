//
// Created by Артём Черница on 28.01.21.
//

#ifndef L2_ARRAYLIST_H
#define L2_ARRAYLIST_H

#include "IStack.h"

template <typename T>
class ArrayList : public IStack<T> {
private:
    T *_stack;
    int _size;
    int _index;
public:
    ArrayList(int size);

    void push(T value) override;

    T pop() override;

    T peek() override;

    int size() override;

    bool isEmpty() override;
};

template <class T>
ArrayList<T>::ArrayList(int size) {
    _stack = new T[size * sizeof(T)];
    _size = size;
    _index = -1;
}

template <class T>
void ArrayList<T>::push(T value) {
    if (_index >= _size) {
        T *temp = new T[_size * 2];
        std::copy_n(_stack, _size, temp);
        *_stack = *temp;
        _size = _size * 2;
    }
    _index++;
    _stack[_index] = value;
}

template <class T>
T ArrayList<T>::pop() {
    if (_index < 0)
        throw std::out_of_range("index out of range error");
    // remove last element
    // *actually just decrease the index of last element
    _index -= 1;
    return _stack[_index + 1];
}

template <class T>
T ArrayList<T>::peek() {
    return _stack[_index];
}

template <class T>
int ArrayList<T>::size() {
    return _index + 1;
}

template <class T>
bool ArrayList<T>::isEmpty() {
    return _index < 0;
}

#endif //L2_ARRAYLIST_H
