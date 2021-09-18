#include <algorithm>
#include <functional>
#include <iostream>
#include <list>

template <typename KeyType, typename ValueType, typename Hasher = std::hash<KeyType>>
class MyHashMap {
    private:
        struct Cell
        {
            std::list<std::pair<const KeyType, ValueType >> chain;
        };
        Hasher hasher_;
        Cell* cells_;
        size_t size_;
        size_t capacity;
        size_t used_cells_;

        void rehash() {
            size_t new_capacity = capacity * 2;
            Cell* new_cells_ = new Cell[new_capacity];
            size_t new_used_cells_ = 0;
            
            for (size_t i = 0; i < capacity; i++) {
                for (auto x : cells_[i].chain) {
                    KeyType key = x.first;
                    ValueType value = x.second;    
                    auto hash = static_cast<size_t>(hasher_(key)) % new_capacity;
                    new_cells_[hash].chain.push_back(std::make_pair(key, value));
                    if (new_cells_[hash].chain.size() == 1) {
                        new_used_cells_++;
                    }
                }
            }

            delete[] cells_;
            cells_ = new_cells_;
            capacity = new_capacity;
            used_cells_ = new_used_cells_;
        }
    public:
        MyHashMap(size_t size = 7) : hasher_() {
            cells_ = new Cell[size];
            capacity = size;
            size_ = 0;
            used_cells_ = 0;
        }
        ~MyHashMap() {
            clear();
            delete[] cells_;
        }
        void clear() {
            for (size_t i = 0; i < capacity; i++) {
                cells_[i].chain.clear();               
            }
            cells_ = new Cell[7];
            size_ = 0;
            capacity = 7;
            used_cells_ = 0;
        }

        size_t size() {
            return size_;
        }
        void insert(KeyType key, ValueType value) {
            auto hash = static_cast<size_t>(hasher_(key)) % capacity;

            if (contains(key)) {
                erase(key);
            }

            cells_[hash].chain.push_back(std::make_pair(key, value));
            size_++;
            if (cells_[hash].chain.size() == 1) {
                used_cells_++;
            }
            if (capacity * 0.6 < used_cells_) {
                rehash();
            }
        }
        void erase(KeyType key) {
            auto hash = static_cast<size_t>(hasher_(key)) % capacity;

            if (contains(key)) {
                for (auto x : cells_[hash].chain) { 
                    if (x.first == key) {
                        cells_[hash].chain.remove(x);
                        break;
                    }
                }

                size_--;
                if (cells_[hash].chain.size() == 1) {
                    used_cells_--;
                }  
            }
        }

        bool contains(KeyType key) {
            auto hash = static_cast<size_t>(hasher_(key)) % capacity;

            for (auto x : cells_[hash].chain) {
                if (x.first == key) return true;
            }

            return false;
        }
        
        ValueType operator[](KeyType key) const {
            auto hash = static_cast<size_t>(hasher_(key)) % capacity;

            if (contains(key)) {
                for (auto x : cells_[hash].chain) { 
                    if (x.first == key) {
                        return x.second;
                    }
                }
            }

            return ValueType();
        }

        ValueType& operator[](KeyType key) {
            auto hash = static_cast<size_t>(hasher_(key)) % capacity;

            if (!contains(key)) {
                insert(key, ValueType());   
            }

            for (auto x : cells_[hash].chain) { 
                if (x.first == key) {
                    auto it = std::find(cells_[hash].chain.begin(), cells_[hash].chain.end(), x); 
                    return it -> second;
                }
            }
        }
};
class MyHasher {
    public:
        size_t operator()(std::string s) {
            size_t hash = 0;
            size_t md = 1e9 + 7;
            for (int i = 0; i < s.size(); i++) {
                hash = (hash * 269 + (int)s[i]) % md;
            }
            return hash;
        }
};
int main() {
    MyHashMap <int, int> map(3);
    map.insert(1, 10);
    map.insert(2, 20);
    map.insert(3, 30);
    map.insert(4, 40);
    map[2] = 100;
    map[1] += 200;
    std::cout << map.size() << '\n'; 
    std::cout << map[1] << '\n'; 
    std::cout << map[2] << '\n';
    map.erase(1);
    map.erase(2);
    std::cout << map.contains(1) << '\n';
    std::cout << map.contains(4) << '\n';
    map.clear();
    std::cout << map.size() << '\n' << '\n';

    MyHashMap <std::string, int, MyHasher> map2;
    map2.insert("I", 10);
    map2.insert("am", 20);
    map2.insert("loxxxxxxxxx", 30);
    map2.insert("dddddddddd", 40);
    map2["I"] = 100;
    map2["am"] += 200;
    std::cout << map2.size() << '\n'; 
    std::cout << map2["I"] << '\n'; 
    std::cout << map2["am"] << '\n';
    map2.erase("I");
    map2.erase("am");
    std::cout << map2.contains("I") << '\n';
    std::cout << map2.contains("dddddddddd") << '\n';
    std::cout << map2.size() << '\n';
    
    return 0;
}