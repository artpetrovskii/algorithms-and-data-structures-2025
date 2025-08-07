#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <random>
#include <queue>

class RBTree {
public:
    RBTree() : root(nullptr) {}
    ~RBTree() { clear(root); }

    void insert(int key) {
        Node* z = new Node(key);
        Node* y = nullptr;
        Node* x = root;

        while (x != nullptr) {
            y = x;
            if (z->key < x->key) {
                x = x->left;
            } else {
                x = x->right;
            }
        }

        z->parent = y;
        if (y == nullptr) {
            root = z;
        } else if (z->key < y->key) {
            y->left = z;
        } else {
            y->right = z;
        }

        fixInsert(z);
    }

    bool contains(int key) const { return search(root, key) != nullptr; }
    
    void remove(int key) {
        Node* z = search(root, key);
        if (z == nullptr) return;

        Node* y = z;
        Node* x;
        Color yOriginalColor = y->color;

        if (z->left == nullptr) {
            x = z->right;
            transplant(z, z->right);
        } else if (z->right == nullptr) {
            x = z->left;
            transplant(z, z->left);
        } else {
            y = minimum(z->right);
            yOriginalColor = y->color;
            x = y->right;

            if (y->parent == z) {
                if (x != nullptr) {
                    x->parent = y;
                }
            } else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        delete z;

        if (yOriginalColor == BLACK) {
            fixDelete(x);
        }
    }

    std::vector<int> inOrder() const {
        std::vector<int> result;
        inOrder(root, result);
        return result;
    }

    size_t size() const { return inOrder().size(); }
    bool isEmpty() const { return root == nullptr; }

    bool validateStructure() const {
        if (!root) return true;
        
        if (root->color != BLACK) return false;

        int blackCount = countBlackNodes(root);

        return validate(root, blackCount, 0);
    }

private:
    enum Color { RED, BLACK };

    struct Node {
        int key;
        Color color;
        Node *left, *right, *parent;

        Node(int key) : key(key), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
    };

    Node* root;

    void clear(Node* node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

    Node* search(Node* node, int key) const {
        if (node == nullptr || node->key == key) {
            return node;
        }
        if (key < node->key) {
            return search(node->left, key);
        }
        return search(node->right, key);
    }

    Node* minimum(Node* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    void transplant(Node* u, Node* v) {
        if (u->parent == nullptr) {
            root = v;
        } else if (u == u->parent->left) {
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }
        if (v != nullptr) {
            v->parent = u->parent;
        }
    }

    void leftRotate(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left != nullptr) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    void rightRotate(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        if (x->right != nullptr) {
            x->right->parent = y;
        }
        x->parent = y->parent;
        if (y->parent == nullptr) {
            root = x;
        } else if (y == y->parent->left) {
            y->parent->left = x;
        } else {
            y->parent->right = x;
        }
        x->right = y;
        y->parent = x;
    }

    void fixInsert(Node* z) {
        while (z != root && z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                Node* y = z->parent->parent->right;
                if (y != nullptr && y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        leftRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            } else {
                Node* y = z->parent->parent->left;
                if (y != nullptr && y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rightRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    void fixDelete(Node* x) {
        while (x != root && (x == nullptr || x->color == BLACK)) {
            if (x == x->parent->left) {
                Node* w = x->parent->right;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    w = x->parent->right;
                }
                if ((w->left == nullptr || w->left->color == BLACK) &&
                    (w->right == nullptr || w->right->color == BLACK)) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->right == nullptr || w->right->color == BLACK) {
                        if (w->left != nullptr) {
                            w->left->color = BLACK;
                        }
                        w->color = RED;
                        rightRotate(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    if (w->right != nullptr) {
                        w->right->color = BLACK;
                    }
                    leftRotate(x->parent);
                    x = root;
                }
            } else {
                Node* w = x->parent->left;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(x->parent);
                    w = x->parent->left;
                }
                if ((w->right == nullptr || w->right->color == BLACK) &&
                    (w->left == nullptr || w->left->color == BLACK)) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->left == nullptr || w->left->color == BLACK) {
                        if (w->right != nullptr) {
                            w->right->color = BLACK;
                        }
                        w->color = RED;
                        leftRotate(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    if (w->left != nullptr) {
                        w->left->color = BLACK;
                    }
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        if (x != nullptr) {
            x->color = BLACK;
        }
    }

    void inOrder(Node* node, std::vector<int>& result) const {
        if (!node) return;
        inOrder(node->left, result);
        result.push_back(node->key);
        inOrder(node->right, result);
    }

    int countBlackNodes(Node* node) const {
        int count = 0;
        while (node != nullptr) {
            count += (node->color == BLACK ? 1 : 0);
            node = node->left;
        }
        return count;
    }

    bool validate(Node* node, int blackCount, int currentBlackCount) const {
        if (node == nullptr) {
            return currentBlackCount == blackCount;
        }

        if (node->color == RED) {
            if ((node->left != nullptr && node->left->color == RED) ||
                (node->right != nullptr && node->right->color == RED)) {
                return false;
            }
        }

        int newCount = currentBlackCount + (node->color == BLACK ? 1 : 0);

        return validate(node->left, blackCount, newCount) && 
               validate(node->right, blackCount, newCount);
    }
};

// ==================== ТЕСТЫ ====================

TEST(RBTreeTest, EmptyTree) {
    RBTree tree;
    EXPECT_TRUE(tree.isEmpty());
    EXPECT_EQ(tree.size(), 0);
    EXPECT_FALSE(tree.contains(42));
    EXPECT_TRUE(tree.validateStructure());
}

TEST(RBTreeTest, SingleInsert) {
    RBTree tree;
    tree.insert(42);
    EXPECT_FALSE(tree.isEmpty());
    EXPECT_EQ(tree.size(), 1);
    EXPECT_TRUE(tree.contains(42));
    EXPECT_FALSE(tree.contains(43));
    EXPECT_TRUE(tree.validateStructure());
}

TEST(RBTreeTest, MultipleInsert) {
    RBTree tree;
    for (int i = 1; i <= 5; ++i) {
        tree.insert(i);
    }
    
    EXPECT_EQ(tree.size(), 5);
    for (int i = 1; i <= 5; ++i) {
        EXPECT_TRUE(tree.contains(i));
    }
    EXPECT_TRUE(tree.validateStructure());
}

TEST(RBTreeTest, InsertDuplicate) {
    RBTree tree;
    tree.insert(42);
    tree.insert(42);
    EXPECT_EQ(tree.size(), 1);
    EXPECT_TRUE(tree.contains(42));
    EXPECT_TRUE(tree.validateStructure());
}

TEST(RBTreeTest, RandomInsertAndValidate) {
    RBTree tree;
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

TEST(RBTreeTest, InsertDescendingOrder) {
    RBTree tree;
    for (int i = 10; i >= 1; --i) {
        tree.insert(i);
    }
    
    EXPECT_EQ(tree.size(), 10);
    EXPECT_TRUE(tree.validateStructure());
    
    auto values = tree.inOrder();
    EXPECT_EQ(values, std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}));
}

TEST(RBTreeTest, StructureIntegrity) {
    RBTree tree;
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

TEST(RBTreeTest, RemoveTest) {
    RBTree tree;
    for (int i = 1; i <= 10; ++i) {
        tree.insert(i);
    }
    
    tree.remove(3);
    tree.remove(7);
    tree.remove(1);
    
    EXPECT_EQ(tree.size(), 7);
    EXPECT_FALSE(tree.contains(3));
    EXPECT_FALSE(tree.contains(7));
    EXPECT_FALSE(tree.contains(1));
    
    for (int i = 2; i <= 10; ++i) {
        if (i != 3 && i != 7) {
            EXPECT_TRUE(tree.contains(i));
        }
    }
    
    EXPECT_TRUE(tree.validateStructure());
}

TEST(RBTreeTest, RemoveAllElements) {
    RBTree tree;
    for (int i = 1; i <= 10; ++i) {
        tree.insert(i);
    }
    
    for (int i = 1; i <= 10; ++i) {
        tree.remove(i);
        EXPECT_TRUE(tree.validateStructure());
    }
    
    EXPECT_TRUE(tree.isEmpty());
    EXPECT_EQ(tree.size(), 0);
    EXPECT_TRUE(tree.validateStructure());
}

TEST(RBTreeTest, RandomInsertAndRemove) {
    RBTree tree;
    std::unordered_set<int> control;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000);

    for (int i = 0; i < 500; ++i) {
        int val = dis(gen);
        tree.insert(val);
        control.insert(val);
    }

    std::vector<int> toRemove(control.begin(), control.end());
    std::shuffle(toRemove.begin(), toRemove.end(), gen);
    toRemove.resize(250);
    
    for (int val : toRemove) {
        tree.remove(val);
        control.erase(val);
        EXPECT_TRUE(tree.validateStructure());
    }

    EXPECT_EQ(tree.size(), control.size());
    for (int val : control) {
        EXPECT_TRUE(tree.contains(val));
    }
    for (int val : toRemove) {
        EXPECT_FALSE(tree.contains(val));
    }
    EXPECT_TRUE(tree.validateStructure());
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}