#include <iostream>
#include <iterator>
#include <vector>
#include <initializer_list> 

class Iterator;

class Deque {
    private:
        class Block {
            public:
                int* array;

                Block() {
                    array = new int[BLOCK_SIZE];
                }
                ~Block() {
                    delete[] array;
                }
        };
        const static int BLOCK_SIZE = 2;
        int block_begin_;
        int block_end_;
        int begin_;
        int end_; 
        size_t size;
        size_t capacity;
        size_t total_blocks;
        Block** block_ptrs;
        void clear_blocks(){
            for (int i = 0; i < total_blocks; i++) {
                delete block_ptrs[i];
            }
            delete [] block_ptrs;
        }
        void change_length() { 
            this -> capacity = size / BLOCK_SIZE * BLOCK_SIZE * 2;
            size_t old_total_blocks = total_blocks;
            this -> total_blocks = capacity / BLOCK_SIZE;
            Block** new_block_ptrs = new Block*[total_blocks];

            int index = 0;
            while (block_begin_ != block_end_) {
                new_block_ptrs[index] = block_ptrs[block_begin_];
                block_begin_++;
                if (block_begin_ == old_total_blocks) {
                    block_begin_ = 0;
                }
                index++;
            }
            new_block_ptrs[index] = block_ptrs[block_begin_];
            block_begin_ = 0;
            block_end_ = index;
            index++;
            delete[] block_ptrs;
            while (index != total_blocks) {
                new_block_ptrs[index] = new Block();
                index++;
            }
    
            block_ptrs = new_block_ptrs;
        }

    public:
        class Iterator : std::iterator<std::random_access_iterator_tag, int> {
            private:
                Deque* deque_pointer;
                int index;
            public:
                Iterator() {
                    deque_pointer = nullptr;
                }
                Iterator(Deque* deque_pointer, int index) {
                    this -> deque_pointer = deque_pointer;
                    this -> index = index;
                }
                int operator*() {
                    int x = (*deque_pointer)[index];
                    return x;
                }
                bool operator==(const Iterator& other) const {
                    return this -> index == other.index;
                }
                bool operator!=(const Iterator& other) const {
                    return !(this -> index == other.index);
                }
                bool operator<(const Iterator& other) const {
                    return this -> index < other.index;
                }
                bool operator>(const Iterator& other) const {
                    return this -> index > other.index;
                }
                bool operator<=(const Iterator& other) const {
                    return this -> index <= other.index;
                }
                bool operator>=(const Iterator& other) const {
                    return this -> index >= other.index;
                }
                Iterator& operator--(int) {
                    auto old_deque = new Iterator(deque_pointer, index); 
                    index++;    
                    return *old_deque;
                }
                Iterator& operator--() {
                    index--;
                    return *this;
                }
                Iterator& operator++() {
                    index++;
                    return *this;
                }
                Iterator& operator++(int) {
                    auto old_deque = new Iterator(deque_pointer, index); 
                    index++;
                    return *old_deque;
                }
                Iterator& operator+=(int x) {
                    index += x;         
                    return *this;
                }
                Iterator& operator-=(int x) {
                    index -= x;                                
                    return *this;
                }
                Iterator& operator-(int x) {
                    auto new_iterator = new Iterator(deque_pointer, index);
                    new_iterator -> index -= x;
                    return *new_iterator;
                }
                Iterator& operator+(int x) {
                    auto new_iterator = new Iterator(deque_pointer, index);
                    new_iterator -> index += x;
                    return *new_iterator;
                }
                Iterator& operator=(Iterator& x) {
                    this -> deque_pointer = x.deque_pointer;
                    this -> index = x.index;
                    return *this;
                }
                void swap(Iterator& x, Iterator& y) {
                    std::swap(x.deque_pointer, y.deque_pointer);
                    std::swap(x.index, y.index);
                }
                int operator-(Iterator& x) {
                    auto y = *this;
                    int answer = 0;
                    if (x > y) swap(x, y);
                    while (x != y) {
                        x++;
                        answer++;
                    }
                    return answer;
                }      
        };

        Deque() {
            this -> size = 0;
            this -> capacity = BLOCK_SIZE * 4;
            this -> block_ptrs = new Block*[4];
            this -> total_blocks = 4;
            for (int i = 0; i < total_blocks; i++) {
                block_ptrs[i] = new Block();
            }

            this -> block_begin_  = 0;
            this -> block_end_  = 0;
            this -> begin_ = 0;
            this -> end_ = 0;
        }
        Deque(size_t size) {
            int number_of_blocks = size / BLOCK_SIZE + 4;
            this -> size = 0;
            this -> capacity =  number_of_blocks * BLOCK_SIZE;
            this -> block_ptrs = new Block*[number_of_blocks];
            this -> total_blocks = number_of_blocks;

            for (int i = 0; i < number_of_blocks; i++) {
                block_ptrs[i] = new Block();
            }
    
            for (int i = 0; i < size; i++) {
                push_back(0);
            }
        }
        Deque(const std::initializer_list<int> &list) : Deque() {
            for (auto &current : list) {
                push_back(current);
            }
        }
        ~Deque() {
            clear_blocks();
        }
        Deque(const Deque& other) {
            this -> size = other.size;
            this -> capacity = other.capacity;
            
            this -> block_begin_ = other.block_begin_;
            this -> block_end_ = other.block_end_;
            this -> begin_ = other.begin_;
            this -> end_ = other.end_;

            this -> total_blocks = other.total_blocks;
            this -> block_ptrs = new Block*[total_blocks];
            for (int i = 0; i < total_blocks; i++) {
                block_ptrs[i] = other.block_ptrs[i];
            }
        }
        Deque& operator = (const Deque& other) {
            this -> size = other.size;
            this -> capacity = other.capacity;
            this -> block_begin_ = other.block_begin_;
            this -> block_end_ = other.block_end_;
            this -> begin_ = other.begin_;
            this -> end_ = other.end_;

            this -> total_blocks = other.total_blocks;
            this -> block_ptrs = new Block*[total_blocks];
            for (int i = 0; i < total_blocks; i++) {
                block_ptrs[i] = other.block_ptrs[i];
            }
            return *this;
        }
        int& operator[](int index) {
            if (begin_ + index < BLOCK_SIZE) {
                return (*block_ptrs[block_begin_]).array[begin_ + index];
            }
            int k = BLOCK_SIZE - begin_;
            index -= k;
            int current_block = block_begin_ + (index + BLOCK_SIZE) / BLOCK_SIZE;
            index -= index / BLOCK_SIZE * BLOCK_SIZE;
            if (current_block >= total_blocks) {
                current_block -= total_blocks;
            }
            return (*block_ptrs[current_block]).array[index];
        }

        void resize(int new_size) {
            while (size < new_size) {
                if (size == capacity || (block_begin_ == block_end_ && size > BLOCK_SIZE)) {
                    change_length();
                }
                push_back(0);
            }
            while (size > new_size) {
                if (total_blocks > 4 && size * 3 < capacity) {
                    change_length();
                }
                pop_back();
            }
        }

        void push_back(int value) {
            if (size + 1 == capacity) {
                change_length();
            }
            int next_block = block_end_;
            int next_end = end_ + 1;
            if (next_end == BLOCK_SIZE) {
                next_block++;
                next_end = 0;
            
                if (next_block == total_blocks) {
                    next_block = 0;
                }
                if (next_block == block_begin_) {
                    change_length();
                    next_block = block_end_ + 1;
                } 
            }

            (*block_ptrs[block_end_]).array[end_] = value; 
            block_end_ = next_block;
            end_ = next_end;
            size++;
        }

        void pop_back() {
            if (this -> size == 0) {
                std::cout << "Non-existent index\n";
            }
            else {
                size--;
                end_--;
                if (end_ == -1) {
                    block_end_--;
                    if (block_end_ == -1) {
                        block_end_ = total_blocks;
                        end_ = BLOCK_SIZE - 1;
                    }
                }
                if (total_blocks > 4 && size * 3 < capacity) {
                    change_length();
                }
            }
        }

        void push_front(int value) {
            if (size + 1 == capacity) {
                change_length();
            }
            if (begin_ == 0) {
                int prev_block = block_begin_;
                prev_block--;
                if (prev_block == -1) {
                    prev_block = total_blocks - 1;
                }
                if (prev_block == block_end_) {
                    change_length();
                    prev_block = total_blocks - 1;
                } 
                block_begin_ = prev_block;
                begin_ = BLOCK_SIZE - 1; 
            } else {
                begin_--;
            }
            (*block_ptrs[block_begin_]).array[begin_] = value; 
            size++;
        }

        void pop_front() {
            if (this -> size == 0) {
                std::cout << "Non-existent index\n";
            }
            else {
                size--;
                begin_++;
                if (begin_ == BLOCK_SIZE) {
                    block_begin_++;
                    if (block_begin_ == total_blocks) {
                        block_begin_ = 0;
                        begin_ = 0;
                    }
                }
                if (total_blocks > 4 && size * 3 < capacity) {
                    change_length();
                }
            }
        }

        void clear() {
            clear_blocks();
            Deque();
        }
        bool empty() {
            return this -> size == 0 ? true : false;
        }
        int length() {
            return this -> size;
        }
        Iterator begin() {
            return Iterator(this, 0);
        }
        Iterator end() {
            return Iterator(this, size);  
        }
};
int main() {
    Deque Mydeque = {1, 2, 3, 4, 5};
    auto it1 = Mydeque.begin();
    auto it2 = it1 + 1;
    auto it3 = it2 + 1;
    auto it4 = it3 + 1;
    std::cout << *it1 << ' ' << *it2 << ' ' << *it3 << ' ' << *it4 << '\n';
    for (int i = 100; i <= 110; i++){
        Mydeque.push_back(i);
    }
    std::cout << *it1 << ' ' << *it2 << ' ' << *it3 << ' ' << *it4 << '\n';
    for (auto x : Mydeque) { 
        std::cout << x << ' ';
    }
    std::cout << '\n';
    for (int i = 100; i <= 110; i++){
        Mydeque.push_front(i);
    }
    for (auto x : Mydeque) { 
        std::cout << x << ' ';
    }
    return 0;
}