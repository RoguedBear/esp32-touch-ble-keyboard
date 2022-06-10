#include "CircularBuffer.h"

template <class T> CircularBuffer<T>::CircularBuffer(int buffer_size) {
    __buffer_size = buffer_size;
    __buffer      = new T[__buffer_size];
    head          = 0;
    tail          = 0;
}

template <class T> bool CircularBuffer<T>::is_empty() { return head == tail; }

/**
 * adds item into buffer.
 * @return true if item was added into buffer or discarded
 */
template <class T> bool CircularBuffer<T>::push(const T &value) {
    head = head % __buffer_size; // wrap around buffer length
    if (head != tail) {          // if head isn't same as tail
                                 // aka buffer is not full
        __buffer[head] = value;
        head++;
        return true;
    } else {
        return false;
    }
}

/**
 * @return nullptr if buffer is empty and pop is called
 */
template <class T> const T CircularBuffer<T>::pop() {
    tail = tail % __buffer_size; // wrap around just in case
    if (tail != head) {
        return __buffer[tail++]; // return current tail item, then increment
    } else {
        return nullptr;
    }
}
