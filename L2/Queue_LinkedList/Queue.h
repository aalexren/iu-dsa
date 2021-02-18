//
// Created by Артём Черница on 29.01.21.
//

#ifndef LINKEDLIST_QUEUE_H
#define LINKEDLIST_QUEUE_H

#include "LinkedList.h"
#include "IQueue.h"

template <typename T>
class Queue : public IQueue<T> {
    LinkedList<T> queue;
};

#endif //LINKEDLIST_QUEUE_H
