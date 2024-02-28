#ifndef _COREKV_SKIPLIST_H_
#define _COREKV_SKIPLIST_H_

#include <fstream>
#include "node.h"
template <typename KeyType, typename ValueType>
class Skiplist
{
private:
    int max_level;
    int current_level;
    int node_count;
    Node<KeyType, ValueType> *head;
    std::ifstream file_reader;
    std::ofstream file_writer;

public:
    Skiplist(){};
    Skiplist(int);
    ~Skiplist();
    Node<KeyType, ValueType> *create_new_node(KeyType, ValueType, int);
    int insert_element(KeyType, ValueType);
    bool search_element(KeyType);
    int remove_element(KeyType);
    void display();
    int size();
    void clear(Node<KeyType, ValueType> *);

    void read_from_file(char *);
    void write_to_file(char *);
};

template <typename KeyType, typename ValueType>
inline Skiplist<KeyType, ValueType>::Skiplist(int max_level)
{
    this->max_level = max_level;
}

template <typename KeyType, typename ValueType>
inline Skiplist<KeyType, ValueType>::~Skiplist()
{
}

template <typename KeyType, typename ValueType>
inline Node<KeyType, ValueType> *Skiplist<KeyType, ValueType>::create_new_node(const KeyType key, const ValueType value, int level)
{
    return new Node<KeyType, ValueType>(key, value, level);
}

template <typename KeyType, typename ValueType>
inline int Skiplist<KeyType, ValueType>::insert_element(const KeyType key, const ValueType value)
{
    return 0;
}
#endif