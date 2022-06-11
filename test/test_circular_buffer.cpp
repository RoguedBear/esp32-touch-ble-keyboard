// RUN THESE TESTS IN "native" environment
#include "CircularBuffer.h"

#include <unity.h>

void test_circular_buffer_initialises_and_is_empty() {
    CircularBuffer<int> buffer(16);

    TEST_ASSERT_TRUE(buffer.is_empty());
}

void test_circular_buffer_not_empty() {
    CircularBuffer<int> buffer(16);

    TEST_ASSERT_TRUE(buffer.push(5));
    TEST_ASSERT_FALSE(buffer.is_empty());
}

void test_circular_buffer_push_pop_1_item() {
    CircularBuffer<char> buffer(2);

    TEST_ASSERT_TRUE(buffer.push('a'));
    TEST_ASSERT_EQUAL_CHAR('a', buffer.pop());
}
void test_circular_buffer_push_pop_2_items() {
    CircularBuffer<char> buffer(3); // to hold 2 items

    TEST_ASSERT_TRUE(buffer.push('a'));
    TEST_ASSERT_EQUAL_CHAR('a', buffer.pop());

    TEST_ASSERT_TRUE(buffer.push('b'));
    TEST_ASSERT_EQUAL_CHAR('b', buffer.pop());

    TEST_ASSERT_TRUE(buffer.push('c'));
    TEST_ASSERT_TRUE(buffer.push('d'));
    TEST_ASSERT_FALSE(buffer.push('e'));
    TEST_ASSERT_EQUAL_CHAR('c', buffer.pop());
    TEST_ASSERT_EQUAL_CHAR('d', buffer.pop());
}

void test_circular_buffer_push_when_buffer_full() {
    CircularBuffer<char> buffer(3); // stores 2 items

    TEST_ASSERT_TRUE(buffer.push('a'));
    buffer.print();
    TEST_ASSERT_TRUE(buffer.push('b'));
    buffer.print();
    bool output = buffer.push('c');
    buffer.print();
    TEST_ASSERT_FALSE(output);
}

void test_circular_buffer_pop_when_buffer_empty() {
    CircularBuffer<char> buffer(3); // stores 2 items

    TEST_ASSERT_EQUAL_CHAR('\0', buffer.pop());
}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_circular_buffer_initialises_and_is_empty);
    RUN_TEST(test_circular_buffer_not_empty);
    RUN_TEST(test_circular_buffer_push_pop_1_item);
    RUN_TEST(test_circular_buffer_push_pop_2_items);
    RUN_TEST(test_circular_buffer_push_when_buffer_full);
    RUN_TEST(test_circular_buffer_pop_when_buffer_empty);

    UNITY_END();
}
