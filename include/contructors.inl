#include "avlTree.hpp"

namespace tree {

template <typename DataType, typename KeyType>
Avl<DataType, KeyType>::Avl(void) : raw_pointer{nullptr}, height_of_tree{0}, number_of_nodes{0} {}

template <typename DataType, typename KeyType>
Avl<DataType, KeyType>::Avl(DataConstReference _data, KeyConstReference _key) : Avl() {
    raw_pointer = new Node(_data, _key);
    height_of_tree = 1;
    number_of_nodes = 1;
}

template <typename DataType, typename KeyType>
typename Avl<DataType, KeyType>::Node* Avl<DataType, KeyType>::freeNode(Node* node) {
    if (node == nullptr) return nullptr;

    node->left = freeNode(node->left);
    node->right = freeNode(node->right);
    delete node;

    return nullptr;
}

template <typename DataType, typename KeyType>
Avl<DataType, KeyType>::~Avl(void) {
    raw_pointer = freeNode(raw_pointer);
}

}