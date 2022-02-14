#include "avlTree.hpp"

namespace tree {

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

}  // namespace tree