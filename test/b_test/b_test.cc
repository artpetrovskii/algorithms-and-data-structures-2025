#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <random>
#include <queue>

class BTree {
public:
    BTree(int degree) : t(degree), root(nullptr) {}
    ~BTree() { clear(root); }

    void insert(int key) {
        if (!root) {
            root = new Node(true);
            root->keys.push_back(key);
            return;
        }

        if (root->keys.size() == 2*t-1) {
            Node* new_root = new Node(false);
            new_root->children.push_back(root);
            splitChild(new_root, 0);
            root = new_root;
        }
        insertNonFull(root, key);
    }

    bool contains(int key) const { return contains(root, key); }
    void remove(int key) { remove(root, key); }

    std::vector<int> inOrder() const {
        std::vector<int> result;
        inOrder(root, result);
        return result;
    }

    size_t size() const { return inOrder().size(); }
    bool isEmpty() const { return root == nullptr; }

    bool validateStructure() const {
        if (!root) return true;
        return validateNode(root, true);
    }

private:
    struct Node {
        bool leaf;
        std::vector<int> keys;
        std::vector<Node*> children;

        Node(bool is_leaf) : leaf(is_leaf) {}
    };

    Node* root;
    const int t;

    void clear(Node* node) {
        if (!node) return;
        for (Node* child : node->children) {
            clear(child);
        }
        delete node;
    }

    void splitChild(Node* parent, int index) {
        Node* y = parent->children[index];
        Node* z = new Node(y->leaf);
        
        z->keys.assign(y->keys.begin() + t, y->keys.end());
        y->keys.resize(t-1);
        
        if (!y->leaf) {
            z->children.assign(y->children.begin() + t, y->children.end());
            y->children.resize(t);
        }
        
        parent->keys.insert(parent->keys.begin() + index, y->keys[t-1]);
        parent->children.insert(parent->children.begin() + index + 1, z);
    }

    void insertNonFull(Node* node, int key) {
        int i = node->keys.size() - 1;
        
        if (node->leaf) {
            node->keys.push_back(0);
            while (i >= 0 && key < node->keys[i]) {
                node->keys[i+1] = node->keys[i];
                i--;
            }
            node->keys[i+1] = key;
        } else {
            while (i >= 0 && key < node->keys[i])
                i--;
            i++;
            
            if (node->children[i]->keys.size() == 2*t-1) {
                splitChild(node, i);
                if (key > node->keys[i])
                    i++;
            }
            insertNonFull(node->children[i], key);
        }
    }

    bool contains(Node* node, int key) const {
        if (!node) return false;
        
        auto it = std::lower_bound(node->keys.begin(), node->keys.end(), key);
        if (it != node->keys.end() && *it == key)
            return true;
        
        if (node->leaf)
            return false;
            
        int index = it - node->keys.begin();
        return contains(node->children[index], key);
    }

    void remove(Node* node, int key) {}

    void inOrder(Node* node, std::vector<int>& result) const {
        if (!node) return;
        
        for (size_t i = 0; i < node->keys.size(); ++i) {
            if (!node->leaf)
                inOrder(node->children[i], result);
            result.push_back(node->keys[i]);
        }
        
        if (!node->leaf)
            inOrder(node->children.back(), result);
    }

    bool validateNode(Node* node, bool is_root) const {
        if (!is_root && (node->keys.size() < t-1 || node->keys.size() > 2*t-1))
            return false;
        
        if (!std::is_sorted(node->keys.begin(), node->keys.end()))
            return false;
        
        if (!node->leaf) {
            if (node->children.size() != node->keys.size() + 1)
                return false;
            
            for (Node* child : node->children) {
                if (!validateNode(child, false))
                    return false;
            }
            
            for (size_t i = 0; i < node->keys.size(); ++i) {
                int max_left = findMax(node->children[i]);
                int min_right = findMin(node->children[i+1]);
                if (max_left >= node->keys[i] || min_right <= node->keys[i])
                    return false;
            }
        }
        
        return true;
    }

    int findMin(Node* node) const {
        while (!node->leaf)
            node = node->children.front();
        return node->keys.front();
    }

    int findMax(Node* node) const {
        while (!node->leaf)
            node = node->children.back();
        return node->keys.back();
    }
};

// ==================== ТЕСТЫ ====================

TEST(BTreeTest, EmptyTree) {
    BTree tree(3);
    EXPECT_TRUE(tree.isEmpty());
    EXPECT_EQ(tree.size(), 0);
    EXPECT_FALSE(tree.contains(42));
    EXPECT_TRUE(tree.validateStructure());
}

TEST(BTreeTest, SingleInsert) {
    BTree tree(3);
    tree.insert(42);
    EXPECT_FALSE(tree.isEmpty());
    EXPECT_EQ(tree.size(), 1);
    EXPECT_TRUE(tree.contains(42));
    EXPECT_FALSE(tree.contains(43));
    EXPECT_TRUE(tree.validateStructure());
}

TEST(BTreeTest, MultipleInsertNoSplit) {
    BTree tree(3);
    for (int i = 1; i <= 5; ++i) {
        tree.insert(i);
    }
    
    EXPECT_EQ(tree.size(), 5);
    for (int i = 1; i <= 5; ++i) {
        EXPECT_TRUE(tree.contains(i));
    }
    EXPECT_TRUE(tree.validateStructure());
}

TEST(BTreeTest, InsertWithSplit) {
    BTree tree(2);
    
    for (int i = 1; i <= 5; ++i) {
        tree.insert(i);
    }
    
    EXPECT_EQ(tree.size(), 5);
    for (int i = 1; i <= 5; ++i) {
        EXPECT_TRUE(tree.contains(i));
    }
    EXPECT_TRUE(tree.validateStructure());
    
    auto values = tree.inOrder();
    EXPECT_EQ(values, std::vector<int>({1, 2, 3, 4, 5}));
}

TEST(BTreeTest, InsertDuplicate) {
    BTree tree(3);
    tree.insert(42);
    tree.insert(42);
    EXPECT_EQ(tree.size(), 1);
    EXPECT_TRUE(tree.contains(42));
    EXPECT_TRUE(tree.validateStructure());
}

TEST(BTreeTest, RandomInsertAndValidate) {
    BTree tree(3);
    std::unordered_set<int> control;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10000);

    for (int i = 0; i < 1000; ++i) {
        int val = dis(gen);
        tree.insert(val);
        control.insert(val);
        
        if (i % 100 == 0) {
            EXPECT_TRUE(tree.validateStructure());
        }
    }

    EXPECT_EQ(tree.size(), control.size());
    for (int val : control) {
        EXPECT_TRUE(tree.contains(val));
    }
    EXPECT_TRUE(tree.validateStructure());
    
    auto values = tree.inOrder();
    EXPECT_TRUE(std::is_sorted(values.begin(), values.end()));
}

TEST(BTreeTest, DifferentDegrees) {
    for (int t = 2; t <= 5; ++t) {
        BTree tree(t);
        for (int i = 1; i <= 50; ++i) {
            tree.insert(i);
        }
        
        EXPECT_EQ(tree.size(), 50);
        EXPECT_TRUE(tree.validateStructure());
        
        auto values = tree.inOrder();
        EXPECT_EQ(values.size(), 50);
        EXPECT_TRUE(std::is_sorted(values.begin(), values.end()));
    }
}

TEST(BTreeTest, InsertDescendingOrder) {
    BTree tree(3);
    for (int i = 10; i >= 1; --i) {
        tree.insert(i);
    }
    
    EXPECT_EQ(tree.size(), 10);
    EXPECT_TRUE(tree.validateStructure());
    
    auto values = tree.inOrder();
    EXPECT_EQ(values, std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}));
}

TEST(BTreeTest, StructureIntegrity) {
    BTree tree(3);
    std::vector<int> numbers;
    for (int i = 1; i <= 100; ++i) {
        numbers.push_back(i);
    }
    
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(numbers.begin(), numbers.end(), g);
    
    for (int num : numbers) {
        tree.insert(num);
        EXPECT_TRUE(tree.validateStructure());
    }
    
    EXPECT_EQ(tree.size(), 100);
    auto values = tree.inOrder();
    EXPECT_EQ(values.size(), 100);
    EXPECT_TRUE(std::is_sorted(values.begin(), values.end()));
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}