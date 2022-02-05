#ifndef _BINARY_SEARCH_TREE_H_
#define _BINARY_SEARCH_TREE_H_

#include <cmath>     // std::pow
#include <iostream>  // st::cout, std::cin, std::endl
#include <queue>     // std::queue
#include <sstream>   // std::stringstream
#include <string>    // std::string
#include <vector>    // std::vector

namespace bst {  // binary search tree

/**
 * @brief A class representing a binary search tree.
 *
 * @tparam DataType Type of data the container will store.
 * @tparam KeyType Type of key the container will store.
 */
template <typename DataType, typename KeyType>
class BinarySearchTree {
   private:
    //=== The data node.
    struct Node {
        DataType data;  //!< Information to be stored in the container.
        KeyType key;    //!< Key to my Nome.
        Node* left;     //!< Pointer to left Node.
        Node* right;    //!< Pointer to right Node.

        Node(const DataType& _data = DataType{}, const KeyType& _key = KeyType{}, Node* l = nullptr, Node* r = nullptr)
            : data{_data}, key{_key}, left{l}, right{r} {}
    };

    //=== Some aliases to help writing a clearer code.
   public:
    using DataReference = DataType&;             //!< reference to the data value.
    using DataConstReference = const DataType&;  //!< const reference to the data value.
    using KeyReference = KeyType&;               //!< reference to the key value.
    using KeyConstReference = const KeyType&;    //!< const reference to the key value.

    //=== Private members.
   private:
    Node* raw_pointer;
    int height;
    int number_of_nodes;

    //=== Auxiliaries Methods

    // Release the nodes in the destructor
    Node* freeNode(Node* node);
    // Insert method recursion
    Node* insert(Node* pointer, DataConstReference _data, KeyConstReference _key, bool& wasInserted);
    // Insert method recursion
    Node* remove(Node* pointer, KeyConstReference _key, bool& wasRemoved);
    // Method used to remove a node
    Node* findMinimumElement(Node* pointer);
    // Search method recursion
    bool search(Node* pointer, KeyConstReference _key);
    // Route in symmetrical order to calculate the elementInPosition
    int simetricToElement(Node* source, int& iteration, int position, bool& var_controle, KeyReference element);
    // Route in symmetrical order for findPositionOfElement method
    int simetric(Node* source, KeyConstReference key, int& iteration, bool& var_controle);
    // Route in symmetrical order to calculate the median
    void simetricToMedian(Node* node, std::vector<Node*>& dados);
    // Useful methods for the isComplete and isFull methods
    int nodesOnLevel(Node* _pt, int current_level, int level);
    int get_height(Node* _pt);
    // Methods for the various ways of printing the tree (toString)
    void toStringPerLevel(Node* pointer, std::stringstream& ss);
    void toStringSorted(Node* pointer, std::stringstream& ss);
    void toStringHierarchical(const Node* node, bool isLeft, std::stringstream& ss, const std::string& prefix);
    void toStringHierarchical(Node* pointer, std::stringstream& ss);

   public:
    //=== Public interface

    //=== Constructors and Destructors
    BinarySearchTree(void);
    BinarySearchTree(DataConstReference _data, KeyConstReference _key);
    ~BinarySearchTree(void);

    //=== Modifiers Methods
    void insert(DataConstReference _data, KeyConstReference _key);
    void remove(KeyConstReference _key);
    bool search(KeyConstReference _key);

    //=== Access Methods
    DataType elementInPosition(int position);
    int findPositionOfElement(KeyConstReference _key);
    DataType median(void);
    bool isComplete(void);
    bool isFull(void);
    std::string toString(std::string type);
};

}  // namespace bst

#include "binarySearchTree.inl"
#endif  // _BINARY_SEARCH_TREE_H_