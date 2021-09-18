#include <iostream>
#include <iterator>
#include <initializer_list> 

class List {
    private:
        struct node {
            int value;
            node* prev;
            node* next;
            node () {
                prev = next = nullptr;
            }
            explicit node(const int &value_) {
                value = value_;
                prev = next = nullptr;
            }
            node(const node &other) {
                this -> value = value;
                prev = next = nullptr;
            }
        };
        size_t size;
        node *begin_;
        node *end_;
    public:
        class Iterator : std::iterator<std::bidirectional_iterator_tag, int> {
            private:
                node *object;
            public:
                Iterator() {
                    object = nullptr;
                }
                explicit Iterator(node *value) : object(value) {
                }
                node* operator->() {
                    return object;
                }
                int& operator*() {
                    return object -> value;
                }
                int operator*() const{
                    return object ->value;
                }
                Iterator& operator--() {
                    object = object -> prev;
                    return *this;
                }
                Iterator& operator--(int) {
                    auto answer = this;
                    object = object -> prev;
                    return *answer;
                }
                Iterator& operator++() {
                    object = object -> next;
                    return *this;
                }
                Iterator& operator++(int) {
                    auto answer = this;
                    object = object -> next;
                    return *answer;
                }
                bool operator==(const Iterator& other) const {
                    return this -> object == other.object;
                }
                bool operator!=(const Iterator& other) const {
                    return this -> object != other.object;
                }
        };
        Iterator begin() {
            return Iterator(begin_);
        }
        Iterator end() {
            return Iterator(end_);
        }
        List(){
            begin_ = end_ = new node();
            size = 0;
        }
        List(const std::initializer_list<int>& other) : List() {
            size = other.size();
            for (auto &current : other) {
                push_back(current);
            }
        }
        ~List() {
            node* current = begin_;
            while (current != nullptr) {
                node* new_node = current;
                current = current -> next;
                delete new_node;
            }   
        }
        List(List& other) : List() {
            auto &current = other.begin_;
            for (auto &it : other) {
                push_back(it);
            }
        }
        List& operator=(List& other) {
            node* cur = begin_;
            while (cur != nullptr) {
                node* new_node = cur;
                cur = cur -> next;
                delete new_node;
            }
            List();
            for (auto &it : other) {
                push_back(it);
            }
            return *this;
        }
        int front() {
            return begin_ -> value;
        }
        int back() {
            return end_ -> value;
        }
        int length() {
            return size;
        }
        bool empty() {
            return size == 0;
        }
        void push_front(int value) {
            if (begin_ == end_) {
                begin_ = new node(value);
                begin_ -> next = end_;
                end_ -> prev = begin_;
            }
            else {
                node* new_node = new node(value);
                begin_ -> prev = new_node;
                new_node -> next = begin_;
                begin_ = new_node;
            }
            size++;
        }
        void pop_front() {
            if (size == 1) {
                delete begin_;
                begin_ = end_;
            }
            else {
                node* nextnode = begin_ -> next;
                delete begin_; 
                nextnode -> prev = nullptr;
                begin_ = nextnode; 
            }
            size--;
        }
        void push_back(int value) {
            if (begin_ == end_) {
                begin_ = new node(value);
                begin_ -> next = end_;
                end_ -> prev = begin_;
            }
            else {
                node* new_node = new node(value);
                node* end_node = end_ -> prev;
                end_ -> prev = new_node;
                new_node -> next = end_;
                new_node -> prev = end_node;
                end_node -> next = new_node;
            }
            size++;
        }
        void pop_back() {
            if (size == 1) {
                delete begin_;
                begin_ = end_;
            }
            else {
                node* end_node = end_ -> prev;
                node* prev_end_node = end_node -> prev;
                end_ -> prev = prev_end_node;
                prev_end_node -> next = end_;
                delete end_node; 
            }
            size--;
        }
};
int main() {
    List mylist;
    mylist.push_front(4);
    mylist.push_front(3);
    mylist.push_front(2);
    mylist.push_front(1);
    mylist.push_back(4);
    mylist.push_back(3);
    mylist.push_back(2);
    mylist.push_back(1);
    mylist.pop_front();
    mylist.pop_back();
    for (auto it : mylist) {
        std::cout << it << ' ';
    }
    std::cout << "\n";
    auto it1 = mylist.begin();
    auto it2 = mylist.end()--;
    std::cout << (it1 == it2) << "\n";
    std::cout << (it1 != it2) << "\n";
    std::cout << *it1 << ' ' << *it2 << "\n";    

    List mylist2 = {1, 2, 3, 4};
    for (auto it : mylist2) {
        std::cout << it << ' ';
    }
    std::cout << "\n";
    List mylist3 = mylist2;
    for (auto it : mylist3) {
        std::cout << it << ' ';
    }
    std::cout << "\n";
    system("pause");
    return 0;
}   