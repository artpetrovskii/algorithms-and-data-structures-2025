#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <random>
#include <list>
#include <utility>

using namespace std;

class SequentialTable {
    vector<pair<int, int>> data;

public:
    void insert(int key, int value) {
        for (auto& p : data) {
            if (p.first == key) {
                p.second = value;
                return;
            }
        }
        data.emplace_back(key, value);
    }

    bool contains(int key) const {
        for (const auto& p : data) {
            if (p.first == key) return true;
        }
        return false;
    }

    int get(int key) const {
        for (const auto& p : data) {
            if (p.first == key) return p.second;
        }
        throw out_of_range("Key not found");
    }

    void remove(int key) {
        for (auto it = data.begin(); it != data.end(); ++it) {
            if (it->first == key) {
                data.erase(it);
                return;
            }
        }
    }

    size_t size() const { return data.size(); }
    bool empty() const { return data.empty(); }
};

class OrderedTable {
    vector<pair<int, int>> data;

public:
    void insert(int key, int value) {
        auto it = lower_bound(data.begin(), data.end(), make_pair(key, 0));
        if (it != data.end() && it->first == key) {
            it->second = value;
        } else {
            data.insert(it, make_pair(key, value));
        }
    }

    bool contains(int key) const {
        auto it = lower_bound(data.begin(), data.end(), make_pair(key, 0));
        return it != data.end() && it->first == key;
    }

    int get(int key) const {
        auto it = lower_bound(data.begin(), data.end(), make_pair(key, 0));
        if (it != data.end() && it->first == key) {
            return it->second;
        }
        throw out_of_range("Key not found");
    }

    void remove(int key) {
        auto it = lower_bound(data.begin(), data.end(), make_pair(key, 0));
        if (it != data.end() && it->first == key) {
            data.erase(it);
        }
    }

    size_t size() const { return data.size(); }
    bool empty() const { return data.empty(); }
};

class ChainingHashTable {
    static const size_t DEFAULT_CAPACITY = 16;
    vector<list<pair<int, int>>> table;
    size_t itemCount = 0;

    size_t hash(int key) const {
        return std::hash<int>()(key) % table.size();
    }

    void rehash() {
        if (itemCount < table.size() * 0.75) return;

        vector<list<pair<int, int>>> newTable(table.size() * 2);
        for (auto& bucket : table) {
            for (auto& p : bucket) {
                size_t newHash = std::hash<int>()(p.first) % newTable.size();
                newTable[newHash].push_back(p);
            }
        }
        table = move(newTable);
    }

public:
    ChainingHashTable() : table(DEFAULT_CAPACITY) {}

    void insert(int key, int value) {
        rehash();
        size_t h = hash(key);
        for (auto& p : table[h]) {
            if (p.first == key) {
                p.second = value;
                return;
            }
        }
        table[h].emplace_back(key, value);
        itemCount++;
    }

    bool contains(int key) const {
        size_t h = hash(key);
        for (const auto& p : table[h]) {
            if (p.first == key) return true;
        }
        return false;
    }

    int get(int key) const {
        size_t h = hash(key);
        for (const auto& p : table[h]) {
            if (p.first == key) return p.second;
        }
        throw out_of_range("Key not found");
    }

    void remove(int key) {
        size_t h = hash(key);
        for (auto it = table[h].begin(); it != table[h].end(); ++it) {
            if (it->first == key) {
                table[h].erase(it);
                itemCount--;
                return;
            }
        }
    }

    size_t size() const { return itemCount; }
    bool empty() const { return itemCount == 0; }
};

class OpenAddressingHashTable {
    static const size_t DEFAULT_CAPACITY = 16;
    enum State { EMPTY, OCCUPIED, DELETED };

    struct Entry {
        int key;
        int value;
        State state = EMPTY;
    };

    vector<Entry> table;
    size_t itemCount = 0;

    size_t hash(int key) const {
        return std::hash<int>()(key) % table.size();
    }

    size_t probe(size_t h, size_t i) const {
        return (h + i) % table.size();
    }

    void rehash() {
        if (itemCount < table.size() * 0.5) return;

        vector<Entry> newTable(table.size() * 2);
        for (auto& entry : table) {
            if (entry.state == OCCUPIED) {
                size_t h = hash(entry.key);
                size_t i = 0;
                while (newTable[h].state == OCCUPIED) {
                    i++;
                    h = probe(h, i);
                }
                newTable[h] = entry;
            }
        }
        table = move(newTable);
    }

public:
    OpenAddressingHashTable() : table(DEFAULT_CAPACITY) {}

    void insert(int key, int value) {
        rehash();
        size_t h = hash(key);
        size_t i = 0;
        size_t firstDeleted = table.size();

        while (i < table.size()) {
            size_t pos = probe(h, i);
            if (table[pos].state == EMPTY || 
                (table[pos].state == OCCUPIED && table[pos].key == key)) {
                if (table[pos].state == EMPTY && firstDeleted < table.size()) {
                    pos = firstDeleted;
                }
                table[pos].key = key;
                table[pos].value = value;
                table[pos].state = OCCUPIED;
                itemCount++;
                return;
            } else if (table[pos].state == DELETED && firstDeleted == table.size()) {
                firstDeleted = pos;
            }
            i++;
        }

        throw runtime_error("Hash table overflow");
    }

    bool contains(int key) const {
        size_t h = hash(key);
        size_t i = 0;
        while (i < table.size()) {
            size_t pos = probe(h, i);
            if (table[pos].state == EMPTY) break;
            if (table[pos].state == OCCUPIED && table[pos].key == key) return true;
            i++;
        }
        return false;
    }

    int get(int key) const {
        size_t h = hash(key);
        size_t i = 0;
        while (i < table.size()) {
            size_t pos = probe(h, i);
            if (table[pos].state == EMPTY) break;
            if (table[pos].state == OCCUPIED && table[pos].key == key) {
                return table[pos].value;
            }
            i++;
        }
        throw out_of_range("Key not found");
    }

    void remove(int key) {
        size_t h = hash(key);
        size_t i = 0;
        while (i < table.size()) {
            size_t pos = probe(h, i);
            if (table[pos].state == EMPTY) return;
            if (table[pos].state == OCCUPIED && table[pos].key == key) {
                table[pos].state = DELETED;
                itemCount--;
                return;
            }
            i++;
        }
    }

    size_t size() const { return itemCount; }
    bool empty() const { return itemCount == 0; }
};

// ==================== ТЕСТЫ ====================

template <typename Table>
class TableTest : public testing::Test {
protected:
    Table table;
};

using TableTypes = testing::Types<
    SequentialTable,
    OrderedTable,
    ChainingHashTable,
    OpenAddressingHashTable
>;

TYPED_TEST_SUITE(TableTest, TableTypes);

TYPED_TEST(TableTest, EmptyTable) {
    EXPECT_TRUE(this->table.empty());
    EXPECT_EQ(this->table.size(), 0);
    EXPECT_FALSE(this->table.contains(42));
}

TYPED_TEST(TableTest, SingleInsert) {
    this->table.insert(42, 100);
    EXPECT_FALSE(this->table.empty());
    EXPECT_EQ(this->table.size(), 1);
    EXPECT_TRUE(this->table.contains(42));
    EXPECT_EQ(this->table.get(42), 100);
}

TYPED_TEST(TableTest, MultipleInsert) {
    for (int i = 1; i <= 5; ++i) {
        this->table.insert(i, i*10);
    }
    
    EXPECT_EQ(this->table.size(), 5);
    for (int i = 1; i <= 5; ++i) {
        EXPECT_TRUE(this->table.contains(i));
        EXPECT_EQ(this->table.get(i), i*10);
    }
}

TYPED_TEST(TableTest, InsertDuplicate) {
    this->table.insert(42, 100);
    this->table.insert(42, 200);
    EXPECT_EQ(this->table.size(), 1);
    EXPECT_TRUE(this->table.contains(42));
    EXPECT_EQ(this->table.get(42), 200);
}

TYPED_TEST(TableTest, RemoveTest) {
    for (int i = 1; i <= 5; ++i) {
        this->table.insert(i, i*10);
    }
    
    this->table.remove(3);
    this->table.remove(1);
    
    EXPECT_EQ(this->table.size(), 3);
    EXPECT_FALSE(this->table.contains(3));
    EXPECT_FALSE(this->table.contains(1));
    
    EXPECT_TRUE(this->table.contains(2));
    EXPECT_TRUE(this->table.contains(4));
    EXPECT_TRUE(this->table.contains(5));
}

TYPED_TEST(TableTest, RandomOperations) {
    mt19937 gen(42);
    uniform_int_distribution<int> dist(1, 100);
    unordered_map<int, int> reference;
    
    for (int i = 0; i < 1000; ++i) {
        int key = dist(gen);
        int value = dist(gen);
        
        if (i % 3 == 0) {
            this->table.remove(key);
            reference.erase(key);
        } else {
            this->table.insert(key, value);
            reference[key] = value;
        }
        
        if (i % 100 == 0) {
            EXPECT_EQ(this->table.size(), reference.size());
            for (const auto& p : reference) {
                EXPECT_TRUE(this->table.contains(p.first));
                EXPECT_EQ(this->table.get(p.first), p.second);
            }
        }
    }
    
    EXPECT_EQ(this->table.size(), reference.size());
    for (const auto& p : reference) {
        EXPECT_TRUE(this->table.contains(p.first));
        EXPECT_EQ(this->table.get(p.first), p.second);
    }
}

TYPED_TEST(TableTest, StressTest) {
    const int TEST_SIZE = 10000;
    mt19937 gen(42);
    uniform_int_distribution<int> dist(1, TEST_SIZE * 10);
    
    for (int i = 0; i < TEST_SIZE; ++i) {
        int key = dist(gen);
        int value = dist(gen);
        this->table.insert(key, value);
    }
    
    EXPECT_GE(this->table.size(), TEST_SIZE * 0.9);
    
    for (int i = 0; i < 100; ++i) {
        int key = dist(gen);
        if (this->table.contains(key)) {
            EXPECT_NO_THROW(this->table.get(key));
        } else {
            EXPECT_THROW(this->table.get(key), out_of_range);
        }
    }
}

TEST(HashTableTest, ChainingHashTableRehash) {
    ChainingHashTable table;
    const int TEST_SIZE = 100;
    
    for (int i = 1; i <= TEST_SIZE; ++i) {
        table.insert(i, i*10);
    }
    
    EXPECT_EQ(table.size(), TEST_SIZE);
    for (int i = 1; i <= TEST_SIZE; ++i) {
        EXPECT_TRUE(table.contains(i));
        EXPECT_EQ(table.get(i), i*10);
    }
}

TEST(HashTableTest, OpenAddressingHashTableRehash) {
    OpenAddressingHashTable table;
    const int TEST_SIZE = 100;
    
    for (int i = 1; i <= TEST_SIZE; ++i) {
        table.insert(i, i*10);
    }
    
    EXPECT_EQ(table.size(), TEST_SIZE);
    for (int i = 1; i <= TEST_SIZE; ++i) {
        EXPECT_TRUE(table.contains(i));
        EXPECT_EQ(table.get(i), i*10);
    }
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}