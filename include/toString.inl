#include "avlTree.hpp"

namespace tree {

template <typename DataType, typename KeyType>
std::string Avl<DataType, KeyType>::toString(std::string type) {
    std::stringstream ss;
    if (type == "EM NIVEL") {
        toStringPerLevel(raw_pointer, ss);
    } else if (type == "SIMETRICA") {
        toStringSorted(raw_pointer, ss);
    } else if (type == "HIERARQUICA") {
        toStringHierarchical(raw_pointer, ss);
    }

    // If the tree has no elements
    if (ss.str().empty()) {
        return "Árvore vazia";
    } else {
        return ss.str();
    }
}

template <typename DataType, typename KeyType>
void Avl<DataType, KeyType>::toStringPerLevel(Node* pointer, std::stringstream& ss) {
    std::queue<Node*> elements_per_level;  // queue is empty

    // Base Case
    if (pointer == nullptr) {
        return;
    } else {
        elements_per_level.push(pointer);  // add root node to queue
    }

    while (!elements_per_level.empty()) {
        // Add the front of queue to ss and remove it from queue
        Node* current_node = elements_per_level.front();
        ss << current_node->key << " ";
        elements_per_level.pop();

        // Put the children in queue
        if (current_node->left != nullptr) {
            elements_per_level.push(current_node->left);
        }
        if (current_node->right != nullptr) {
            elements_per_level.push(current_node->right);
        }
    }
}

template <typename DataType, typename KeyType>
void Avl<DataType, KeyType>::toStringSorted(Node* pointer, std::stringstream& ss) {
    if (pointer == nullptr) return;
    toStringSorted(pointer->left, ss);
    ss << pointer->key << " ";
    toStringSorted(pointer->right, ss);
}

template <typename DataType, typename KeyType>
void Avl<DataType, KeyType>::toStringHierarchical(Node* pointer, std::stringstream& ss) {
    ss << std::endl << std::endl;
    toStringHierarchical(pointer, false, ss, "");
}

template <typename DataType, typename KeyType>
void Avl<DataType, KeyType>::toStringHierarchical(const Node* node, bool isLeft, std::stringstream& ss,
                                                  const std::string& prefix) {
    if (node != nullptr) {
        ss << prefix;
        ss << (isLeft ? "├──" : "└──");

        ss << node->key << std::endl;

        toStringHierarchical(node->left, true, ss, prefix + (isLeft ? "│   " : "    "));
        toStringHierarchical(node->right, false, ss, prefix + (isLeft ? "│   " : "    "));
    }
}

}