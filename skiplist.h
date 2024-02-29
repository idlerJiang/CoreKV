#ifndef _COREKV_SKIPLIST_H_
#define _COREKV_SKIPLIST_H_

#include "node.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <mutex>

template<typename KeyType, typename ValueType>
class Skiplist {
private:
    std::mutex mut;     // 信号量实现互斥访问
    int max_level;      // Skiplist 的最高允许高度
    int current_level;  // 当前 Skiplist 的高度
    int node_count;     // Skiplist 的总结点数
    Node<KeyType, ValueType> *head;     // Skiplist 的头节点数组，存储每层的头节点
    std::ifstream file_reader;          // 文件输入流
    std::ofstream file_writer;          // 文件输出流

    int generate_random_level();        // 为新加入的节点生成随机的插入层数

public:
    Skiplist() {};      // 默认构造函数

    Skiplist(int);      // 有参构造函数，参数 int 代表设定的 Skiplist 的最高允许高度

    ~Skiplist();        // 析构函数

    void clear(Node<KeyType, ValueType> *);     // 清除以指定节点为头节点的链表

    Node<KeyType, ValueType> *create_new_node(KeyType, ValueType, int);     // 创建一个新节点

    int insert_element(KeyType, ValueType, bool = false);     // 向 Skiplist 中添加指定键值对

    bool search_element(KeyType, bool = false);   // 在 Skiplist 中查询指定键是否存在

    ValueType get_element(KeyType);   // 在 Skiplist 中获取指定键对应的值

    int remove_element(KeyType, bool = false);    // 从 Skiplist 中移除指定键与其对应的值

    void display();     // 展示 Skiplist 中的所有键值对

    int size();         // 获取当前 Skiplist 的长度

//    void read_from_file(std::string);   // 从指定文件中读入整个 Skiplist

//    void write_to_file(std::string);    // 向指定文件中写入整个 Skiplist
};


template<typename KeyType, typename ValueType>
int Skiplist<KeyType, ValueType>::generate_random_level() {
    int result = 1;
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
    this->current_level = 0;
    this->node_count = 0;
    KeyType key;
    ValueType value;
    this->head = new Node<KeyType, ValueType>(key, value, max_level);
    srand(time(nullptr));
}

template<typename KeyType, typename ValueType>
inline Skiplist<KeyType, ValueType>::~Skiplist() {
    if (file_reader.is_open()) file_reader.close();
    if (file_writer.is_open()) file_writer.close();
    if (head->pointer[0] != nullptr) clear(head->pointer[0]);
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
inline int Skiplist<KeyType, ValueType>::insert_element(const KeyType key, const ValueType value, bool out) {
    mut.lock();
    Node<KeyType, ValueType> *current = this->head;
    Node<KeyType, ValueType> *insert_location[max_level + 1];
    memset(insert_location, 0, sizeof(Node<KeyType, ValueType> *) * (max_level + 1));
    // 找到 Key 在 Skiplist 中每层应该插入的位置
    for (int i = current_level; i >= 0; i--) {
        while (current->pointer[i] != NULL && current->pointer[i]->get_key() < key)
            current = current->pointer[i];
        insert_location[i] = current;
    }
    // 找到 Key 在 Skiplist 中每层应该插入的位置
    current = current->pointer[0];

    // Key 已在 Skiplist 中存在，返回 1
    if (current != NULL && current->get_key() == key) {
        if (out)
            std::cout << "键 " << current->get_key() << " 已存在" << std::endl;
        mut.unlock();
        return 1;
    }

    // Key 不冲突，生成随机插入层数，此时 current == NULL || current->get_key() != key
    int random_level = generate_random_level();
    // 若生成的随机插入层数比当前层数还大，说明 Skiplist 需要增加对应的层数，将每层的头节点设置为 head
    /*
     Level 3    3 (新层，把添加位置的前一位设置为head)
                ↓
     Level 2    3 → → → 6 → → → → → → → 12
                ↓       ↓               ↓
     Level 1    3 → → → 6 → → → 8 → → → 12
                ↓       ↓       ↓       ↓
     Level 0    3 → 4 → 6 → 7 → 8 → 9 → 12
     */
    if (random_level > current_level) {
        for (int i = current_level + 1; i <= random_level; i++)
            insert_location[i] = head;
        current_level = random_level;
    }

    // 生成一个 node 节点并放在指定位置
    Node<KeyType, ValueType> *insert_node = create_new_node(key, value, random_level);
    for (int i = 0; i <= random_level; i++) {
        insert_node->pointer[i] = insert_location[i]->pointer[i];
        insert_location[i]->pointer[i] = insert_node;
    }

    node_count++;
    if (out)
        std::cout << "键值对 " << insert_node->get_key() << " : " << insert_node->get_value() << " 已插入" << std::endl;
    mut.unlock();
    return 0;
}

template<typename KeyType, typename ValueType>
bool Skiplist<KeyType, ValueType>::search_element(KeyType key, bool out) {
    Node<KeyType, ValueType> *current = head;
    for (int i = current_level; i >= 0; i--)
        while (current->pointer[i] != NULL && current->pointer[i]->get_key() < key)
            current = current->pointer[i];
    current = current->pointer[0];
    if (current != NULL && current->get_key() == key) {
        if (out)
            std::cout << "找到键 " << key << " 对应的值为 " << current->get_value() << std::endl;
        return true;
    }
    if (out)
        std::cout << "未找到键 " << key << std::endl;
    return false;
}

template<typename KeyType, typename ValueType>
ValueType Skiplist<KeyType, ValueType>::get_element(KeyType key) {
    Node<KeyType, ValueType> *current = head;
    for (int i = current_level; i >= 0; i--)
        while (current->pointer[i] != NULL && current->pointer[i]->get_key() < key)
            current = current->pointer[i];
    current = current->pointer[0];
    if (current != NULL && current->get_key() == key) {
        return current->get_value();
    }
    return ValueType();
}

template<typename KeyType, typename ValueType>
int Skiplist<KeyType, ValueType>::remove_element(KeyType key, bool out) {
    mut.lock();
    Node<KeyType, ValueType> *current = this->head;
    Node<KeyType, ValueType> *delete_location[max_level + 1];
    memset(delete_location, 0, sizeof(Node<KeyType, ValueType> *) * (max_level + 1));
    for (int i = current_level; i >= 0; i--) {
        while (current->pointer[i] != NULL && current->pointer[i]->get_key() < key)
            current = current->pointer[i];
        delete_location[i] = current;
    }
    current = current->pointer[0];
    if (current != NULL && current->get_key() == key) {
        for (int i = 0; i <= current_level; i++) {
            if (delete_location[i]->pointer[i] != current)
                break;
            delete_location[i]->pointer[i] = current->pointer[i];
        }
        while (current_level > 0 && head->pointer[current_level] == NULL)
            current_level--;
        if (out)
            std::cout << "已删除键为 " << key << " 的数据" << std::endl;
        if (current != NULL)
            delete current;
        node_count--;
    }
    mut.unlock();
    return 0;
}

template<typename KeyType, typename ValueType>
void Skiplist<KeyType, ValueType>::display() {
    for (int i = 0; i <= current_level; i++) {
        Node<KeyType, ValueType> *node = this->head->pointer[i];
        std::cout << "Level " << i << " : ";
        while (node != NULL) {
            std::cout << node->get_key() << ":" << node->get_value() << "; ";
            node = node->pointer[i];
        }
        std::cout << std::endl;
    }
}

template<typename KeyType, typename ValueType>
int Skiplist<KeyType, ValueType>::size() {
    return node_count;
}

#endif