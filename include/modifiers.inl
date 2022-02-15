#include "avlTree.hpp"

namespace tree {

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
            pointer = switchBalanceToRight(pointer, needCheckBalance);
        }
    } else if (_key > pointer->key) {
        pointer->right = insert(pointer->right, _data, _key, wasInserted, needCheckBalance);

        /**
         * if the insertion is successful, it needs to check if
         * each node in the insertion path maintains the balance.
         */
        if (needCheckBalance) {
            pointer = switchBalanceToLeft(pointer, needCheckBalance);
        }
    }

    pointer->height = getHeight(pointer);

    return pointer;
}

template <typename DataType, typename KeyType>
typename Avl<DataType, KeyType>::Node* Avl<DataType, KeyType>::switchBalanceToRight(Node* pointer, 
                                                                                    bool& needCheckBalance) {
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

    return pointer;
}

template <typename DataType, typename KeyType>
typename Avl<DataType, KeyType>::Node* Avl<DataType, KeyType>::switchBalanceToLeft(Node* pointer, 
                                                                                   bool& needCheckBalance) {
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

    return pointer;
}

template <typename DataType, typename KeyType>
typename Avl<DataType, KeyType>::Node* Avl<DataType, KeyType>::chooseRightRotation(Node* pointer) {
    if (pointer->left->balance == 1) {
        pointer = singleRightRotate(pointer);
    } else {
        pointer = doubleRightRotate(pointer);
    }

    pointer->balance = 0;
    return pointer;
}

template <typename DataType, typename KeyType>
typename Avl<DataType, KeyType>::Node* Avl<DataType, KeyType>::chooseLeftRotation(Node* pointer) {
    if (pointer->right->balance == -1) {
        pointer = singleLeftRotate(pointer);
    } else {
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
    head->balance = (int)heightHeadLeft - heightHeadRight;

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
    head->balance = (int)heightHeadLeft - heightHeadRight;

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
            int newBalance = (int)heightLeft - heightRight;

            if (newBalance == -2) {
                pointer = chooseLeftRotation(pointer);
            } else if (newBalance != pointer->balance) {
                pointer->balance -= 1;
            }
        }
    } else if (_key > pointer->key) {
        pointer->right = remove(pointer->right, _key, wasRemoved);

        if (wasRemoved) {
            size_t heightLeft = pointer->left == nullptr ? 0 : getHeight(pointer->left);
            size_t heightRight = pointer->right == nullptr ? 0 : getHeight(pointer->right);
            int newBalance = (int)heightLeft - heightRight;

            if (newBalance == 2) {
                pointer = chooseRightRotation(pointer);
            } else if (newBalance != pointer->balance) {
                pointer->balance += 1;
            }
        }
    } else if (pointer->left && pointer->right) {
        nodeTemp = findMinimumElement(pointer->right);
        pointer->key = nodeTemp->key;
        pointer->data = nodeTemp->data;
        pointer->right = remove(pointer->right, pointer->key, wasRemoved);

        if (wasRemoved) {
            size_t heightLeft = pointer->left == nullptr ? 0 : getHeight(pointer->left);
            size_t heightRight = pointer->right == nullptr ? 0 : getHeight(pointer->right);
            int newBalance = (int)heightLeft - heightRight;

            if (newBalance == 2) {
                pointer = chooseRightRotation(pointer);
            } else if (newBalance != pointer->balance) {
                pointer->balance += 1;
            }
        }
    } else {
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

}  // namespace tree