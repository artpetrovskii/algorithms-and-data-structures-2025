#include <gtest/gtest.h>
#include <stdexcept>

class Stack {
private:
    int* data;
    size_t capacity;
    size_t size;
    
    void resize() {
        capacity *= 2;
        int* new_data = new int[capacity];
        for (size_t i = 0; i < size; ++i) {
            new_data[i] = data[i];
        }
        delete[] data;
        data = new_data;
    }

public:
    Stack() : capacity(10), size(0) {
        data = new int[capacity];
    }

    ~Stack() {
        delete[] data;
    }

    void push(int value) {
        if (size == capacity) {
            resize();
        }
        data[size++] = value;
    }

    int pop() {
        if (is_empty()) {
            throw std::out_of_range("Stack is empty");
        }
        return data[--size];
    }

    int peek() const {
        if (is_empty()) {
            throw std::out_of_range("Stack is empty");
        }
        return data[size - 1];
    }

    bool is_empty() const {
        return size == 0;
    }

    size_t get_size() const {
        return size;
    }
};

TEST(StackTest, IsEmptyOnCreation) {
    Stack s;
    EXPECT_TRUE(s.is_empty());
    EXPECT_EQ(s.get_size(), 0);
}

TEST(StackTest, PushAndPeek) {
    Stack s;
    s.push(42);
    EXPECT_FALSE(s.is_empty());
    EXPECT_EQ(s.get_size(), 1);
    EXPECT_EQ(s.peek(), 42);
}

TEST(StackTest, PushPop) {
    Stack s;
    s.push(10);
    s.push(20);
    
    EXPECT_EQ(s.pop(), 20);
    EXPECT_EQ(s.pop(), 10);
    EXPECT_TRUE(s.is_empty());
}

TEST(StackTest, ResizeTest) {
    Stack s;
    for (int i = 0; i < 100; ++i) {
        s.push(i);
    }
    
    EXPECT_EQ(s.get_size(), 100);
    for (int i = 99; i >= 0; --i) {
        EXPECT_EQ(s.pop(), i);
    }
}

TEST(StackTest, PopEmptyStack) {
    Stack s;
    EXPECT_THROW(s.pop(), std::out_of_range);
    EXPECT_THROW(s.peek(), std::out_of_range);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}