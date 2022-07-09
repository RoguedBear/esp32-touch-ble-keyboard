#ifndef _CIRCULAR_BUFFER
#define _CIRCULAR_BUFFER

#ifdef DEBUG_CODE
#include "TouchKey.h"

#include <WString.h>
#endif

template <class T> class CircularBuffer {
  private:
    int __buffer_size;
    T * __buffer;
    int head, tail;

  public:
    /**
     * Circular Buffer, stores (size - 1) items
     * @param buffer_size size of the buffer to allocate. reccomended to keep
     * something in power of 2
     */
    CircularBuffer(int buffer_size);

    bool    is_empty();
    void    clear();
    bool    push(const T &value);
    const T pop();
    const T poll();
#ifdef DEBUG_CODE
    String print();
#endif
#ifdef NATIVE_TEST
    void print(); // for testing purposes only
#endif
};

// =======================================================================

template <class T> CircularBuffer<T>::CircularBuffer(int buffer_size) {
    __buffer_size = buffer_size;
    __buffer      = new T[__buffer_size];
    head          = 0;
    tail          = 0;
}

/**
 * @return boolean if the buffer is empty aka has zero items
 */
template <class T> bool CircularBuffer<T>::is_empty() { return head == tail; }

/**
 * clears the buffer
 */
template <class T> void CircularBuffer<T>::clear() { head = tail; }

/**
 * adds item into buffer.
 * @return true if item was added into buffer or discarded
 */
template <class T> bool CircularBuffer<T>::push(const T &value) {
    int next_head = (head + 1) % __buffer_size; // wrap around buffer length
    if (next_head != tail) {                    // if head isn't same as tail
                                                // aka buffer is not full
        __buffer[head] = value;
        head           = next_head;
        return true;
    } else {
        return false;
    }
}

/**
 * @return null object of the type initialised in template, if buffer is empty
 */
template <class T> const T CircularBuffer<T>::pop() {
    int next_tail = (tail + 1) % __buffer_size; // wrap around just in case
    if (!this->is_empty()) {                    // if not empty
        T _val = __buffer[tail];
        tail   = next_tail;
        return _val;
    } else {
        return T();
    }
}

/**
 * returns the value from buffer without removing it
 * @return null object of the type initialised in template, if buffer is empty
 */
template <class T> const T CircularBuffer<T>::poll() {
    if (!this->is_empty()) {
        T _val = __buffer[tail];
        return _val;
    } else {
        return T();
    }
}

#ifdef DEBUG_CODE
template <class T> String CircularBuffer<T>::print() {

    int    head_copy      = head;
    int    tail_copy      = tail;
    int    head_copy_copy = head_copy;
    int    tail_copy_copy = tail_copy;
    String output("[ ");
    while (tail_copy_copy != head_copy) {
        output += String(((key_press_timestamp_t *)&__buffer[tail_copy_copy])
                             ->obj->letter_to_press) +
                  String(' ');
        tail_copy_copy = (tail_copy_copy + 1) % __buffer_size;
    }
    output += String("]    ");

    output +=
        String("\tt=") + String(tail_copy) + String(" h=") + String(head_copy);

    // output += String(" [ ");
    // while (head_copy_copy != tail_copy) {
    //     output += String(((key_press_timestamp_t *)&__buffer[head_copy_copy])
    //                          ->obj->letter_to_press) +
    //               String(' ');
    //     head_copy_copy = (head_copy_copy + 1) % __buffer_size;
    // }
    // output += String("]");

    return output;
}
#endif

/** For testing purposes only */
#ifdef NATIVE_TEST
#include <cstdio>
#include <stdbool.h>
#include <stdint.h>
template <class T> void CircularBuffer<T>::print() {
    printf("[head=%d,tail=%d]", head, tail);
    printf("[ ");
    for (int i = 0; i < __buffer_size; i++) {
        printf("%c ", __buffer[i]);
    }
    printf("]"); // apparently escape characters are not processed in
                 // platformio's output
}
#endif

#endif