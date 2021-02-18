//
// Created by Артём Черница on 28.01.21.
//

#ifndef L2_ISTACK_H
#define L2_ISTACK_H

template <typename T>
class IStack {
public:
    virtual void push(T value) = 0;
    virtual T pop() = 0;
    virtual T peek() = 0;
    virtual int size() = 0;
    virtual bool isEmpty() = 0;
};

#endif //L2_ISTACK_H
