#include "avlTree.hpp"

namespace tree {

/*****************************************************************************
 * Constructors and Destructors
 ****************************************************************************/

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

/*****************************************************************************
 * Modifiers Methods (insert, remove and search)
 ****************************************************************************/

template <typename DataType, typename KeyType>
void Avl<DataType, KeyType>::insert(DataConstReference _data, KeyConstReference _key) {
    bool wasInserted = false;
    bool needCheckBalance = false;
    raw_pointer = insert(raw_pointer, _data, _key, wasInserted, needCheckBalance);

    if (wasInserted) {
        height_of_tree = raw_pointer->height;
        number_of_nodes++;
    }
}

template <typename DataType, typename KeyType>
typename Avl<DataType, KeyType>::Node* Avl<DataType, KeyType>::insert(Node* pointer, DataConstReference _data,
                                                                      KeyConstReference _key, bool& wasInserted,
                                                                      bool& needCheckBalance) {
    if (pointer == nullptr) {
        pointer = new Node(_data, _key);
        pointer->left = pointer->right = nullptr;
        wasInserted = true;
        needCheckBalance = true;
    } else if (_key < pointer->key) {
        pointer->left = insert(pointer->left, _data, _key, wasInserted, needCheckBalance);
        
        /**
         * if the insertion is successful, it needs to check if 
         * each node in the insertion path maintains the balance.
         */
        if (needCheckBalance) {
            switch (pointer->balance) {
            case -1:
                pointer->balance = 0;
                needCheckBalance = false;
                break;
            case 0:
                pointer->balance = 1;
                break;
            case 1:
                pointer = chooseRightRotation(pointer);
                needCheckBalance = false;
                break;
            }
        }
    } else if (_key > pointer->key) {
        pointer->right = insert(pointer->right, _data, _key, wasInserted, needCheckBalance);

        /**
         * if the insertion is successful, it needs to check if 
         * each node in the insertion path maintains the balance.
         */
        if (needCheckBalance) {
            switch (pointer->balance) {
            case 1:
                pointer->balance = 0;
                needCheckBalance = false;
                break;
            case 0:
                pointer->balance = -1;
                break;
            case -1:
                pointer = chooseLeftRotation(pointer);
                needCheckBalance = false;
                break;
            }
        }
    }

    pointer->height = getHeight(pointer);

    return pointer;
}

template <typename DataType, typename KeyType>
typename Avl<DataType, KeyType>::Node* Avl<DataType, KeyType>::chooseRightRotation(Node* pointer) {
    if (pointer->left->balance == 1) {
        pointer = singleRightRotate(pointer);
    }
    else {
        pointer = doubleRightRotate(pointer);
    }

    pointer->balance = 0;
    return pointer;
}

template <typename DataType, typename KeyType>
typename Avl<DataType, KeyType>::Node* Avl<DataType, KeyType>::chooseLeftRotation(Node* pointer) {
    if (pointer->right->balance == -1) {
        pointer = singleLeftRotate(pointer);
    }
    else {
        pointer = doubleLeftRotate(pointer);
    }

    pointer->balance = 0;
    return pointer;
}

template <typename DataType, typename KeyType>
typename Avl<DataType, KeyType>::Node* Avl<DataType, KeyType>::singleRightRotate(Node* head) {
    Node* newHead = head->left;
    head->left = newHead->right;
    newHead->right = head;

    head->height = getHeight(head);
    newHead->height = getHeight(newHead);

    size_t heightHeadLeft = head->left == nullptr ? 0 : getHeight(head->left);
    size_t heightHeadRight = head->right == nullptr ? 0 : getHeight(head->right);
    head->balance = (int) heightHeadLeft - heightHeadRight;

    return newHead;
}

template <typename DataType, typename KeyType>
typename Avl<DataType, KeyType>::Node* Avl<DataType, KeyType>::singleLeftRotate(Node* head) {
    Node* newHead = head->right;
    head->right = newHead->left;
    newHead->left = head;

    head->height = getHeight(head);
    newHead->height = getHeight(newHead);

    size_t heightHeadLeft = head->left == nullptr ? 0 : getHeight(head->left);
    size_t heightHeadRight = head->right == nullptr ? 0 : getHeight(head->right);
    head->balance = (int) heightHeadLeft - heightHeadRight;

    return newHead;
}

template <typename DataType, typename KeyType>
typename Avl<DataType, KeyType>::Node* Avl<DataType, KeyType>::doubleRightRotate(Node* head) {
    head->left = singleLeftRotate(head->left);
    return singleRightRotate(head);
}

template <typename DataType, typename KeyType>
typename Avl<DataType, KeyType>::Node* Avl<DataType, KeyType>::doubleLeftRotate(Node* head) {
    head->right = singleRightRotate(head->right);
    return singleLeftRotate(head);
}

template <typename DataType, typename KeyType>
size_t Avl<DataType, KeyType>::getHeight(Node* pointer) {
    size_t height;

    if (pointer->left != nullptr && pointer->right != nullptr)
        height = 1 + std::max(pointer->left->height, pointer->right->height);
        
    else if (pointer->left != nullptr)
        height = 1 + pointer->left->height;

    else if (pointer->right != nullptr)
        height = 1 + pointer->right->height;

    else
        height = 1;

    return height;
}

template <typename DataType, typename KeyType>
void Avl<DataType, KeyType>::remove(KeyConstReference _key) {
    bool wasRemoved = false;
    raw_pointer = remove(raw_pointer, _key, wasRemoved);

    if (wasRemoved) {
        height_of_tree = raw_pointer->height;
        number_of_nodes--;
    }
}

template <typename DataType, typename KeyType>
typename Avl<DataType, KeyType>::Node* Avl<DataType, KeyType>::remove(Node* pointer, KeyConstReference _key,
                                                                      bool& wasRemoved) {
    Node* nodeTemp;
    if (pointer == nullptr) return nullptr;
    if (_key < pointer->key) {
        pointer->left = remove(pointer->left, _key, wasRemoved);

        if (wasRemoved) {
            size_t heightLeft = pointer->left == nullptr ? 0 : getHeight(pointer->left);
            size_t heightRight = pointer->right == nullptr ? 0 : getHeight(pointer->right);
            int newBalance = (int) heightLeft - heightRight;

            if (newBalance == -2) {
                pointer = chooseLeftRotation(pointer);
            }
            else if (newBalance != pointer->balance) {
                pointer->balance -= 1;
            }
        }
    }
    else if (_key > pointer->key) {
        pointer->right = remove(pointer->right, _key, wasRemoved);

        if (wasRemoved) {
            size_t heightLeft = pointer->left == nullptr ? 0 : getHeight(pointer->left);
            size_t heightRight = pointer->right == nullptr ? 0 : getHeight(pointer->right);
            int newBalance = (int) heightLeft - heightRight;

            if (newBalance == 2) {
                pointer = chooseRightRotation(pointer);
            }
            else if (newBalance != pointer->balance) {
                pointer->balance += 1;
            }
        }
    }
    else if (pointer->left && pointer->right) {
        nodeTemp = findMinimumElement(pointer->right);
        pointer->key = nodeTemp->key;
        pointer->data = nodeTemp->data;
        pointer->right = remove(pointer->right, pointer->key, wasRemoved);

        if (wasRemoved) {
            size_t heightLeft = pointer->left == nullptr ? 0 : getHeight(pointer->left);
            size_t heightRight = pointer->right == nullptr ? 0 : getHeight(pointer->right);
            int newBalance = (int) heightLeft - heightRight;

            if (newBalance == 2) {
                pointer = chooseRightRotation(pointer);
            }
            else if (newBalance != pointer->balance) {
                pointer->balance += 1;
            }
        }
    }
    else {
        nodeTemp = pointer;
        if (pointer->left == nullptr)
            pointer = pointer->right;
        else if (pointer->right == nullptr)
            pointer = pointer->left;
        delete nodeTemp;
        wasRemoved = true;
    }

    if (pointer != nullptr) {
        pointer->height = getHeight(pointer);
    }

    return pointer;
}

template <typename DataType, typename KeyType>
typename Avl<DataType, KeyType>::Node* Avl<DataType, KeyType>::findMinimumElement(Node* pointer) {
    if (pointer == nullptr)
        return nullptr;
    else if (pointer->left == nullptr)
        return pointer;
    else
        return findMinimumElement(pointer->left);
}

template <typename DataType, typename KeyType>
bool Avl<DataType, KeyType>::search(KeyConstReference _key) {
    return search(raw_pointer, _key);
}

template <typename DataType, typename KeyType>
bool Avl<DataType, KeyType>::search(Node* pointer, KeyConstReference _key) {
    if (pointer == nullptr) return false;

    if (_key < pointer->key) return search(pointer->left, _key);

    if (_key > pointer->key) return search(pointer->right, _key);

    return true;
}

/*****************************************************************************
 * Access Methods
 ****************************************************************************/

template <typename DataType, typename KeyType>
DataType Avl<DataType, KeyType>::elementInPosition(size_t position) {
    if (position > number_of_nodes) return -1;

    size_t iterator = 1;
    bool var_controle = false;
    DataType key;

    simetricToElement(raw_pointer, iterator, position, var_controle, key);

    return key;
}

template <typename DataType, typename KeyType>
void Avl<DataType, KeyType>::simetricToElement(Node* source, size_t& iteration, size_t position, bool& var_controle,
                                              KeyReference element) {
    if (source == nullptr) return;
    
    this->simetricToElement(source->left, iteration, position, var_controle, element);

    if (iteration == position) {
        var_controle = true;
        element = source->key;
        iteration++;
        return;
    }

    if (var_controle == false) {
        iteration++;
    }

    this->simetricToElement(source->right, iteration, position, var_controle, element);
}

template <typename DataType, typename KeyType>
int Avl<DataType, KeyType>::findPositionOfElement(KeyConstReference _key) {
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
int Avl<DataType, KeyType>::simetric(Node* source, KeyConstReference _key, int& iteration, bool& var_controle) {
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
DataType Avl<DataType, KeyType>::median(void) {
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
void Avl<DataType, KeyType>::simetricToMedian(Node* node, std::vector<Node*>& dados) {
    if (node != nullptr) {
        this->simetricToMedian(node->left, dados);
        dados.push_back(node);
        this->simetricToMedian(node->right, dados);
    }
}

template <typename DataType, typename KeyType>
bool Avl<DataType, KeyType>::isComplete(void) {
    /* It's not necessary to test the last level as
    it may have empty nodes. Hence the height_of_tree - 1 */
    for (size_t level = 1; level <= height_of_tree - 1; level++) {
        if (nodesOnLevel(raw_pointer, 1, level) != std::pow(2, level - 1)) {
            return false;
        }
    }

    return true;
}

template <typename DataType, typename KeyType>
size_t Avl<DataType, KeyType>::nodesOnLevel(Node* _pt, size_t current_level, size_t level) {
    if (current_level == level) {
        return (_pt == nullptr) ? 0 : 1;
    }

    size_t nodes_left = nodesOnLevel(_pt->left, current_level + 1, level);
    size_t node_right = nodesOnLevel(_pt->right, current_level + 1, level);
    return nodes_left + node_right;
}

template <typename DataType, typename KeyType>
bool Avl<DataType, KeyType>::isFull(void) {
    return number_of_nodes == std::pow(2, height_of_tree) - 1;
}

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

}  // namespace tree