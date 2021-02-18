//
// Created by Артём Черница on 29.01.21.
//

#ifndef LINKEDLIST_IQUEUE_H
#define LINKEDLIST_IQUEUE_H

template <typename T>
class IQueue {
    virtual void offer(T value) = 0;
    virtual T pool() = 0;
    virtual T peek() = 0;
    virtual int size() = 0;
    virtual bool isEmpty() = 0;
};

#endif //LINKEDLIST_IQUEUE_H
