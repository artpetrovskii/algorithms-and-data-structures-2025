#include <gtest/gtest.h>
#include <vector>
#include <algorithm>

class BinaryTree {
private:
    struct Node {
        int value;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
        Node(int val) : value(val), left(nullptr), right(nullptr) {}
    };

    std::unique_ptr<Node> root;

    void insert(Node* node, int value) {
        if (value < node->value) {
            if (node->left) {
                insert(node->left.get(), value);
            } else {
                node->left = std::make_unique<Node>(value);
            }
        } else {
            if (node->right) {
                insert(node->right.get(), value);
            } else {
                node->right = std::make_unique<Node>(value);
            }
        }
    }

    bool contains(const Node* node, int value) const {
        if (!node) return false;
        if (value == node->value) return true;
        return value < node->value ? contains(node->left.get(), value) 
                                  : contains(node->right.get(), value);
    }

    void in_order(const Node* node, std::vector<int>& result) const {
        if (!node) return;
        in_order(node->left.get(), result);
        result.push_back(node->value);
        in_order(node->right.get(), result);
    }

public:
    void insert(int value) {
        if (!root) {
            root = std::make_unique<Node>(value);
        } else {
            insert(root.get(), value);
        }
    }

    bool contains(int value) const {
        return contains(root.get(), value);
    }

    std::vector<int> in_order_traversal() const {
        std::vector<int> result;
        in_order(root.get(), result);
        return result;
    }

    size_t size() const {
        return in_order_traversal().size();
    }

    bool is_empty() const {
        return !root;
    }
};

TEST(BinaryTreeTest, IsEmptyOnCreation) {
    BinaryTree tree;
    EXPECT_TRUE(tree.is_empty());
    EXPECT_EQ(tree.size(), 0);
}

TEST(BinaryTreeTest, InsertAndContains) {
    BinaryTree tree;
    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    
    EXPECT_FALSE(tree.is_empty());
    EXPECT_EQ(tree.size(), 3);
    EXPECT_TRUE(tree.contains(50));
    EXPECT_TRUE(tree.contains(30));
    EXPECT_TRUE(tree.contains(70));
    EXPECT_FALSE(tree.contains(100));
}

TEST(BinaryTreeTest, InOrderTraversal) {
    BinaryTree tree;
    std::vector<int> values = {50, 30, 70, 20, 40, 60, 80};
    for (int val : values) {
        tree.insert(val);
    }
    
    std::vector<int> expected = {20, 30, 40, 50, 60, 70, 80};
    EXPECT_EQ(tree.in_order_traversal(), expected);
}

TEST(BinaryTreeTest, LargeTree) {
    BinaryTree tree;
    std::vector<int> values;
    for (int i = 0; i < 1000; ++i) {
        values.push_back(rand() % 10000);
    }
    
    for (int val : values) {
        tree.insert(val);
    }
    
    for (int val : values) {
        EXPECT_TRUE(tree.contains(val));
    }
    
    auto sorted = tree.in_order_traversal();
    EXPECT_TRUE(std::is_sorted(sorted.begin(), sorted.end()));
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}