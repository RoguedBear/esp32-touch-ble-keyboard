#ifndef _CIRCULAR_BUFFER
#define _CIRCULAR_BUFFER

template <class T> class CircularBuffer {
  private:
    int __buffer_size;
    T **__buffer;
    int head, tail;

  public:
    /**
     * @param buffer_size size of the buffer to allocate. reccomended to keep
     * something in power of 2
     */
    CircularBuffer(int buffer_size);

    /**
     * @return boolean if the buffer is empty or has something
     */
    bool    is_empty();
    bool    push(const T &value);
    const T pop();
};
#endif