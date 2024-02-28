#ifndef _COREKV_SKIPLIST_H_
#define _COREKV_SKIPLIST_H_

#include "node.h"
#include <fstream>

template<typename KeyType, typename ValueType>
class Skiplist {
private:
    int max_level;
    int current_level;
    int node_count;
    Node<KeyType, ValueType> *head;
    std::ifstream file_reader;
    std::ofstream file_writer;

    int generate_random_level();

public:
    Skiplist() {};

    Skiplist(int);

    ~Skiplist();

    void clear(Node<KeyType, ValueType> *);

    Node<KeyType, ValueType> *create_new_node(KeyType, ValueType, int);

    int insert_element(KeyType, ValueType);

    bool search_element(KeyType);

    int remove_element(KeyType);

    void display();

    int size();

    void read_from_file(std::string);

    void write_to_file(std::string);
};


template<typename KeyType, typename ValueType>
int Skiplist<KeyType, ValueType>::generate_random_level() {
    int result = 0;
    srand(time(nullptr));
    while (rand() % 2) {
        result++;
        if (result == max_level)
            break;
    }
    return result;
}

template<typename KeyType, typename ValueType>
inline Skiplist<KeyType, ValueType>::Skiplist(int max_level) {
    this->max_level = max_level;
}

template<typename KeyType, typename ValueType>
inline Skiplist<KeyType, ValueType>::~Skiplist() {
    if (file_reader.is_open()) file_reader.close();
    if (file_writer.is_open()) file_writer.close();
    if (head->pointer[0] != nullptr) clear(head);
    delete head;
}

template<typename KeyType, typename ValueType>
void Skiplist<KeyType, ValueType>::clear(Node<KeyType, ValueType> *pointer) {
    if (pointer->pointer[0] != nullptr)
        clear(pointer->pointer[0]);
    delete pointer;
}

template<typename KeyType, typename ValueType>
inline Node<KeyType, ValueType> *
Skiplist<KeyType, ValueType>::create_new_node(const KeyType key, const ValueType value, int level) {
    return new Node<KeyType, ValueType>(key, value, level);
}

template<typename KeyType, typename ValueType>
inline int Skiplist<KeyType, ValueType>::insert_element(const KeyType key, const ValueType value) {
    int level = generate_random_level();
    return 0;
}

template<typename KeyType, typename ValueType>
bool Skiplist<KeyType, ValueType>::search_element(KeyType) {
    return false;
}

template<typename KeyType, typename ValueType>
int Skiplist<KeyType, ValueType>::remove_element(KeyType) {
    return 0;
}

template<typename KeyType, typename ValueType>
void Skiplist<KeyType, ValueType>::display() {

}

template<typename KeyType, typename ValueType>
int Skiplist<KeyType, ValueType>::size() {
    return 0;
}

template<typename KeyType, typename ValueType>
void Skiplist<KeyType, ValueType>::read_from_file(std::string) {

}

template<typename KeyType, typename ValueType>
void Skiplist<KeyType, ValueType>::write_to_file(std::string) {

}

#endif