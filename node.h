#ifndef _COREKV_NODE_H_
#define _COREKV_NODE_H_

template<typename KeyType, typename ValueType>
class Node {
private:
    KeyType key;
    ValueType value;

public:
    int node_level;
    Node<KeyType, ValueType> **pointer;

    Node() {}

    Node(KeyType, ValueType, int);

    ~Node();

    KeyType get_key() const;

    ValueType get_value() const;

    void set_value(const ValueType);
};

template<typename KeyType, typename ValueType>
inline Node<KeyType, ValueType>::Node(const KeyType key, const ValueType value, int node_level) {
    this->key = key;
    this->value = value;
    this->node_level = node_level;
    this->pointer = new Node<KeyType, ValueType> *[node_level + 1];
    memset(this->pointer, 0, sizeof(Node<KeyType, ValueType> *) * (node_level + 1));
}

template<typename KeyType, typename ValueType>
inline Node<KeyType, ValueType>::~Node() {
    for (int i = 0; i <= node_level; ++i) {
        if (pointer[i] != nullptr) {
            delete[] pointer[i];
            pointer[i] = nullptr;
        }
    }
    delete[] pointer;
    pointer = nullptr;
}

template<typename KeyType, typename ValueType>
inline KeyType Node<KeyType, ValueType>::get_key() const {
    return key;
}

template<typename KeyType, typename ValueType>
inline ValueType Node<KeyType, ValueType>::get_value() const {
    return value;
}

template<typename KeyType, typename ValueType>
inline void Node<KeyType, ValueType>::set_value(const ValueType value) {
    this->value = value;
}

#endif