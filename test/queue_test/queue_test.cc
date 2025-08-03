#include <gtest/gtest.h>
#include <stdexcept>
#include <memory>

class Queue {
private:
    struct Node {
        int value;
        std::unique_ptr<Node> next;
        Node(int val) : value(val), next(nullptr) {}
    };

    std::unique_ptr<Node> head;
    Node* tail;
    size_t size;

public:
    Queue() : head(nullptr), tail(nullptr), size(0) {}

    void enqueue(int value) {
        auto new_node = std::make_unique<Node>(value);
        if (!tail) {
            head = std::move(new_node);
            tail = head.get();
        } else {
            tail->next = std::move(new_node);
            tail = tail->next.get();
        }
        size++;
    }

    int dequeue() {
        if (!head) {
            throw std::out_of_range("Queue is empty");
        }
        int value = head->value;
        head = std::move(head->next);
        if (!head) {
            tail = nullptr;
        }
        size--;
        return value;
    }

    int peek() const {
        if (!head) {
            throw std::out_of_range("Queue is empty");
        }
        return head->value;
    }

    bool is_empty() const {
        return size == 0;
    }

    size_t get_size() const {
        return size;
    }
};

TEST(QueueTest, IsEmptyOnCreation) {
    Queue q;
    EXPECT_TRUE(q.is_empty());
    EXPECT_EQ(q.get_size(), 0);
}

TEST(QueueTest, EnqueueAndPeek) {
    Queue q;
    q.enqueue(42);
    EXPECT_FALSE(q.is_empty());
    EXPECT_EQ(q.get_size(), 1);
    EXPECT_EQ(q.peek(), 42);
}

TEST(QueueTest, FIFOOrder) {
    Queue q;
    q.enqueue(10);
    q.enqueue(20);
    q.enqueue(30);
    
    EXPECT_EQ(q.dequeue(), 10);
    EXPECT_EQ(q.dequeue(), 20);
    EXPECT_EQ(q.peek(), 30);
    EXPECT_EQ(q.get_size(), 1);
}

TEST(QueueTest, LargeQueue) {
    Queue q;
    for (int i = 0; i < 1000; ++i) {
        q.enqueue(i);
    }
    
    EXPECT_EQ(q.get_size(), 1000);
    for (int i = 0; i < 1000; ++i) {
        EXPECT_EQ(q.dequeue(), i);
    }
    EXPECT_TRUE(q.is_empty());
}

TEST(QueueTest, DequeueEmptyQueue) {
    Queue q;
    EXPECT_THROW(q.dequeue(), std::out_of_range);
    EXPECT_THROW(q.peek(), std::out_of_range);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}