#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <random>

class AVLTree {
public:
    struct Node {
        int value;
        int height;
        Node* left;
        Node* right;
        Node(int val) : value(val), height(1), left(nullptr), right(nullptr) {}
    };

    Node* root;

    AVLTree() : root(nullptr) {}
    ~AVLTree() { clear(root); }

    void insert(int value) { root = insert(root, value); }
    bool contains(int value) const { return contains(root, value); }
    void remove(int value) { root = remove(root, value); }
    std::vector<int> inOrder() const { std::vector<int> result; inOrder(root, result); return result; }
    size_t size() const { return inOrder().size(); }
    bool isEmpty() const { return root == nullptr; }

private:
    int height(Node* node) const { return node ? node->height : 0; }
    int balanceFactor(Node* node) const { return height(node->right) - height(node->left); }

    void updateHeight(Node* node) {
        node->height = std::max(height(node->left), height(node->right)) + 1;
    }

    Node* rotateRight(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        x->right = y;
        updateHeight(y);
        updateHeight(x);
        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        y->left = x;
        updateHeight(x);
        updateHeight(y);
        return y;
    }

    Node* balance(Node* node) {
        updateHeight(node);
        int bf = balanceFactor(node);
        
        if (bf == 2) {
            if (balanceFactor(node->right) < 0)
                node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        if (bf == -2) {
            if (balanceFactor(node->left) > 0)
                node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        return node;
    }

    Node* insert(Node* node, int value) {
        if (!node) return new Node(value);
        if (value < node->value)
            node->left = insert(node->left, value);
        else if (value > node->value)
            node->right = insert(node->right, value);
        return balance(node);
    }

    Node* findMin(Node* node) const {
        return node->left ? findMin(node->left) : node;
    }

    Node* removeMin(Node* node) {
        if (!node->left) return node->right;
        node->left = removeMin(node->left);
        return balance(node);
    }

    Node* remove(Node* node, int value) {
        if (!node) return nullptr;
        
        if (value < node->value) {
            node->left = remove(node->left, value);
        } else if (value > node->value) {
            node->right = remove(node->right, value);
        } else {
            Node* left = node->left;
            Node* right = node->right;
            delete node;
            
            if (!right) return left;
            Node* min = findMin(right);
            min->right = removeMin(right);
            min->left = left;
            return balance(min);
        }
        return balance(node);
    }

    bool contains(Node* node, int value) const {
        if (!node) return false;
        if (value == node->value) return true;
        return value < node->value ? contains(node->left, value) 
                                  : contains(node->right, value);
    }

    void inOrder(Node* node, std::vector<int>& result) const {
        if (!node) return;
        inOrder(node->left, result);
        result.push_back(node->value);
        inOrder(node->right, result);
    }

    void clear(Node* node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }
};

// ==================== ТЕСТЫ ====================

TEST(AVLTreeTest, EmptyTree) {
    AVLTree tree;
    EXPECT_TRUE(tree.isEmpty());
    EXPECT_EQ(tree.size(), 0);
    EXPECT_FALSE(tree.contains(42));
}

TEST(AVLTreeTest, SingleInsert) {
    AVLTree tree;
    tree.insert(42);
    EXPECT_FALSE(tree.isEmpty());
    EXPECT_EQ(tree.size(), 1);
    EXPECT_TRUE(tree.contains(42));
    EXPECT_FALSE(tree.contains(43));
}

TEST(AVLTreeTest, MultipleInsertAndBalance) {
    AVLTree tree;
    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(20);
    tree.insert(40);
    tree.insert(60);
    tree.insert(80);

    EXPECT_EQ(tree.size(), 7);
    auto values = tree.inOrder();
    EXPECT_EQ(values, std::vector<int>({20, 30, 40, 50, 60, 70, 80}));
}

TEST(AVLTreeTest, InsertDuplicate) {
    AVLTree tree;
    tree.insert(42);
    tree.insert(42);
    EXPECT_EQ(tree.size(), 1);
}

TEST(AVLTreeTest, RemoveLeaf) {
    AVLTree tree;
    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.remove(30);
    
    EXPECT_EQ(tree.size(), 2);
    EXPECT_FALSE(tree.contains(30));
    auto values = tree.inOrder();
    EXPECT_EQ(values, std::vector<int>({50, 70}));
}

TEST(AVLTreeTest, RemoveNodeWithOneChild) {
    AVLTree tree;
    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(20);
    tree.remove(30);
    
    EXPECT_EQ(tree.size(), 3);
    EXPECT_FALSE(tree.contains(30));
    auto values = tree.inOrder();
    EXPECT_EQ(values, std::vector<int>({20, 50, 70}));
}

TEST(AVLTreeTest, RemoveNodeWithTwoChildren) {
    AVLTree tree;
    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(20);
    tree.insert(40);
    tree.insert(60);
    tree.insert(80);
    tree.remove(50);
    
    EXPECT_EQ(tree.size(), 6);
    EXPECT_FALSE(tree.contains(50));
    auto values = tree.inOrder();
    EXPECT_EQ(values, std::vector<int>({20, 30, 40, 60, 70, 80}));
}

TEST(AVLTreeTest, RemoveRoot) {
    AVLTree tree;
    tree.insert(42);
    tree.remove(42);
    EXPECT_TRUE(tree.isEmpty());
    EXPECT_EQ(tree.size(), 0);
}

TEST(AVLTreeTest, RemoveNonExisting) {
    AVLTree tree;
    tree.insert(42);
    tree.remove(43);
    EXPECT_EQ(tree.size(), 1);
    EXPECT_TRUE(tree.contains(42));
}

TEST(AVLTreeTest, LargeRandomInsertAndRemove) {
    AVLTree tree;
    std::unordered_set<int> control;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10000);

    for (int i = 0; i < 1000; ++i) {
        int val = dis(gen);
        tree.insert(val);
        control.insert(val);
    }

    EXPECT_EQ(tree.size(), control.size());
    for (int val : control) {
        EXPECT_TRUE(tree.contains(val));
    }

    size_t target_size = control.size() / 2;
    while (control.size() > target_size) {
        auto it = control.begin();
        int val = *it;
        tree.remove(val);
        control.erase(it);
    }

    EXPECT_EQ(tree.size(), control.size());
    for (int val : control) {
        EXPECT_TRUE(tree.contains(val));
    }

    auto values = tree.inOrder();
    EXPECT_TRUE(std::is_sorted(values.begin(), values.end()));
}

TEST(AVLTreeTest, BalanceAfterOperations) {
    AVLTree tree;
    for (int i = 0; i < 100; ++i) {
        tree.insert(i);
        auto values = tree.inOrder();
        EXPECT_TRUE(std::is_sorted(values.begin(), values.end()));
    }

    for (int i = 0; i < 50; ++i) {
        tree.remove(i);
        auto values = tree.inOrder();
        EXPECT_TRUE(std::is_sorted(values.begin(), values.end()));
    }
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}