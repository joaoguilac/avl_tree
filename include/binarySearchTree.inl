#include "binarySearchTree.hpp"

namespace bst {

/*****************************************************************************
 * Constructors and Destructors
 ****************************************************************************/

template <typename DataType, typename KeyType>
BinarySearchTree<DataType, KeyType>::BinarySearchTree(void) : raw_pointer{nullptr}, height{0}, number_of_nodes{0} {}

template <typename DataType, typename KeyType>
BinarySearchTree<DataType, KeyType>::BinarySearchTree(DataConstReference _data, KeyConstReference _key)
    : BinarySearchTree() {
    raw_pointer = new Node(_data, _key);
}

template <typename DataType, typename KeyType>
typename BinarySearchTree<DataType, KeyType>::Node* BinarySearchTree<DataType, KeyType>::freeNode(Node* node) {
    if (node == nullptr) return nullptr;

    node->left = freeNode(node->left);
    node->right = freeNode(node->right);
    delete node;

    return nullptr;
}

template <typename DataType, typename KeyType>
BinarySearchTree<DataType, KeyType>::~BinarySearchTree(void) {
    raw_pointer = freeNode(raw_pointer);
}

/*****************************************************************************
 * Modifiers Methods (insert, remove and search)
 ****************************************************************************/

template <typename DataType, typename KeyType>
void BinarySearchTree<DataType, KeyType>::insert(DataConstReference _data, KeyConstReference _key) {
    bool wasInserted = false;
    raw_pointer = insert(raw_pointer, _data, _key, wasInserted);

    if (wasInserted) {
        height = get_height(raw_pointer);
        number_of_nodes++;
    }
}

template <typename DataType, typename KeyType>
typename BinarySearchTree<DataType, KeyType>::Node* BinarySearchTree<DataType, KeyType>::insert(
    Node* pointer, DataConstReference _data, KeyConstReference _key, bool& wasInserted) {
    if (pointer == nullptr) {
        pointer = new Node(_data, _key);
        pointer->left = pointer->right = nullptr;
        wasInserted = true;
    } else if (_key < pointer->key) {
        pointer->left = insert(pointer->left, _data, _key, wasInserted);
    } else if (_key > pointer->key) {
        pointer->right = insert(pointer->right, _data, _key, wasInserted);
    }
    return pointer;
}

template <typename DataType, typename KeyType>
typename BinarySearchTree<DataType, KeyType>::Node* BinarySearchTree<DataType, KeyType>::findMinimumElement(
    Node* pointer) {
    if (pointer == nullptr)
        return nullptr;
    else if (pointer->left == nullptr)
        return pointer;
    else
        return findMinimumElement(pointer->left);
}

template <typename DataType, typename KeyType>
void BinarySearchTree<DataType, KeyType>::remove(KeyConstReference _key) {
    bool wasRemoved = false;
    raw_pointer = remove(raw_pointer, _key, wasRemoved);
    if (wasRemoved) {
        height = get_height(raw_pointer);
        number_of_nodes--;
    }
}

template <typename DataType, typename KeyType>
typename BinarySearchTree<DataType, KeyType>::Node* BinarySearchTree<DataType, KeyType>::remove(Node* pointer,
                                                                                                KeyConstReference _key,
                                                                                                bool& wasRemoved) {
    Node* nodeTemp;
    if (pointer == nullptr) return nullptr;
    if (_key < pointer->key)
        pointer->left = remove(pointer->left, _key, wasRemoved);
    else if (_key > pointer->key)
        pointer->right = remove(pointer->right, _key, wasRemoved);
    else if (pointer->left && pointer->right) {
        nodeTemp = findMinimumElement(pointer->right);
        pointer->key = nodeTemp->key;
        pointer->data = nodeTemp->data;
        pointer->right = remove(pointer->right, pointer->key, wasRemoved);
    } else {
        nodeTemp = pointer;
        if (pointer->left == nullptr)
            pointer = pointer->right;
        else if (pointer->right == nullptr)
            pointer = pointer->left;
        delete nodeTemp;
        wasRemoved = true;
    }

    return pointer;
}

template <typename DataType, typename KeyType>
bool BinarySearchTree<DataType, KeyType>::search(KeyConstReference _key) {
    return search(raw_pointer, _key);
}

template <typename DataType, typename KeyType>
bool BinarySearchTree<DataType, KeyType>::search(Node* pointer, KeyConstReference _key) {
    if (pointer == nullptr) return false;

    if (_key < pointer->key) return search(pointer->left, _key);

    if (_key > pointer->key) return search(pointer->right, _key);

    return true;
}

/*****************************************************************************
 * Access Methods
 ****************************************************************************/

template <typename DataType, typename KeyType>
int BinarySearchTree<DataType, KeyType>::simetricToElement(Node* source, int& iteration, int position,
                                                           bool& var_controle, KeyReference element) {
    if (source != nullptr) {
        this->simetricToElement(source->left, iteration, position, var_controle, element);

        if (iteration == position) {
            var_controle = true;
            element = source->key;
            iteration++;
            return iteration;
        }

        if (var_controle == false) {
            iteration++;
        }

        this->simetricToElement(source->right, iteration, position, var_controle, element);
    }
    return 0;
}

template <typename DataType, typename KeyType>
DataType BinarySearchTree<DataType, KeyType>::elementInPosition(int position) {
    if (position > number_of_nodes) return -1;

    int iterator = 1;
    bool var_controle = false;
    DataType key;
    simetricToElement(raw_pointer, iterator, position, var_controle, key);
    return key;
}

template <typename DataType, typename KeyType>
int BinarySearchTree<DataType, KeyType>::simetric(Node* source, KeyConstReference _key, int& iteration,
                                                  bool& var_controle) {
    if (source != nullptr) {
        this->simetric(source->left, _key, iteration, var_controle);

        if (source->key == _key) {
            var_controle = true;
            return iteration;
        }
        if (var_controle == false) {
            iteration++;
        }

        this->simetric(source->right, _key, iteration, var_controle);
    }
    return 0;
}

template <typename DataType, typename KeyType>
int BinarySearchTree<DataType, KeyType>::findPositionOfElement(KeyConstReference _key) {
    int position = 1;
    bool var_controle = false;

    simetric(raw_pointer, _key, position, var_controle);

    if (var_controle == true) {
        return position;
    } else {
        return -1;
    }
}

template <typename DataType, typename KeyType>
void BinarySearchTree<DataType, KeyType>::simetricToMedian(Node* node, std::vector<Node*>& dados) {
    if (node != nullptr) {
        this->simetricToMedian(node->left, dados);
        dados.push_back(node);
        this->simetricToMedian(node->right, dados);
    }
}

template <typename DataType, typename KeyType>
DataType BinarySearchTree<DataType, KeyType>::median(void) {
    // Retrieve all ABB elements by pre-order
    std::vector<Node*> elements;
    simetricToMedian(raw_pointer, elements);
    int vector_size = elements.size();

    /* Median calculation depends on whether
    ABB has an odd or even number of elements */
    int median_ind;
    if (vector_size % 2 == 0) {
        median_ind = (vector_size / 2) - 1;
        return elements[median_ind]->key;
    } else {
        median_ind = vector_size / 2;
        return elements[median_ind]->key;
    }
}

template <typename DataType, typename KeyType>
int BinarySearchTree<DataType, KeyType>::nodesOnLevel(Node* _pt, int current_level, int level) {
    if (current_level == level) {
        return (_pt == nullptr) ? 0 : 1;
    }

    int nodes_left = nodesOnLevel(_pt->left, current_level + 1, level);
    int node_right = nodesOnLevel(_pt->right, current_level + 1, level);
    return nodes_left + node_right;
}

template <typename DataType, typename KeyType>
bool BinarySearchTree<DataType, KeyType>::isComplete(void) {
    /* It's not necessary to test the last level as
    it may have empty nodes. Hence the height-1 */
    for (int level = 1; level <= height - 1; level++) {
        if (nodesOnLevel(raw_pointer, 1, level) != std::pow(2, level - 1)) {
            return false;
        }
    }

    return true;
}

template <typename DataType, typename KeyType>
int BinarySearchTree<DataType, KeyType>::get_height(Node* _pt) {
    if (_pt == nullptr) {
        return 0;
    } else {
        // Calculates the height of each subtree
        int left_height = get_height(_pt->left);
        int right_height = get_height(_pt->right);

        // Returns the highest height between the subtrees
        if (left_height > right_height) {
            return (left_height + 1);
        } else {
            return (right_height + 1);
        }
    }
}

template <typename DataType, typename KeyType>
bool BinarySearchTree<DataType, KeyType>::isFull(void) {
    return number_of_nodes == std::pow(2, height) - 1;
}

template <typename DataType, typename KeyType>
void BinarySearchTree<DataType, KeyType>::toStringHierarchical(const Node* node, bool isLeft, std::stringstream& ss,
                                                               const std::string& prefix) {
    if (node != nullptr) {
        ss << prefix;
        ss << (isLeft ? "├──" : "└──");

        // print the value of the node
        ss << node->key << std::endl;

        // enter the next tree level - left and right branch
        toStringHierarchical(node->left, true, ss, prefix + (isLeft ? "│   " : "    "));
        toStringHierarchical(node->right, false, ss, prefix + (isLeft ? "│   " : "    "));
    }
}

template <typename DataType, typename KeyType>
void BinarySearchTree<DataType, KeyType>::toStringHierarchical(Node* pointer, std::stringstream& ss) {
    ss << std::endl << std::endl;
    toStringHierarchical(pointer, false, ss, "");
}

template <typename DataType, typename KeyType>
void BinarySearchTree<DataType, KeyType>::toStringSorted(Node* pointer, std::stringstream& ss) {
    if (pointer == nullptr) return;
    toStringSorted(pointer->left, ss);
    ss << pointer->key << " ";
    toStringSorted(pointer->right, ss);
}

template <typename DataType, typename KeyType>
void BinarySearchTree<DataType, KeyType>::toStringPerLevel(Node* pointer, std::stringstream& ss) {
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
std::string BinarySearchTree<DataType, KeyType>::toString(std::string type) {
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

}  // namespace bst