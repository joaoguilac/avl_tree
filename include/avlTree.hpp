#ifndef _AVL_TREE_
#define _AVL_TREE_

#include <cmath>     // std::pow
#include <iostream>  // st::cout, std::cin, std::endl
#include <queue>     // std::queue
#include <sstream>   // std::stringstream
#include <string>    // std::string
#include <vector>    // std::vector

namespace tree {

/**
 * @brief A class representing a avl tree.
 *
 * @tparam DataType Type of data the container will store.
 * @tparam KeyType Type of key the container will store.
 */
template <typename DataType, typename KeyType>
class Avl {
   private:
    //=== The data node.
    struct Node {
        DataType data;  //!< Information to be stored in the container.
        KeyType key;    //!< Key to my Nome.
        Node* left;     //!< Pointer to left Node.
        Node* right;    //!< Pointer to right Node.
        size_t height;  //!< Height of Node.

        Node(const DataType& _data, const KeyType& _key, const size_t& h, Node* l = nullptr, Node* r = nullptr)
            : data{_data}, key{_key}, left{l}, right{r}, height{h} {}
    };

    //=== Some aliases to help writing a clearer code.
   public:
    using DataReference = DataType&;             //!< reference to the data value.
    using DataConstReference = const DataType&;  //!< const reference to the data value.
    using KeyReference = KeyType&;               //!< reference to the key value.
    using KeyConstReference = const KeyType&;    //!< const reference to the key value.
    using SizeConstReference = const size_t&;    //!< const reference to the size value.

    //=== Private members.
   private:
    Node* raw_pointer;
    int height;
    int number_of_nodes;

    //=== Auxiliaries Methods

    // Release the nodes in the destructor
    Node* freeNode(Node* node);
    // Insert method recursion
    Node* insert(Node* pointer, DataConstReference _data, KeyConstReference _key, bool& wasInserted,
                 SizeConstReference height);
    // Remove method recursion
    Node* remove(Node* pointer, KeyConstReference _key, bool& wasRemoved);
    // Find minimum element
    Node* findMinimumElement(Node* pointer);
    // Search method recursion
    bool search(Node* pointer, KeyConstReference _key);
    // Route in symmetrical order to calculate the elementInPosition
    int simetricToElement(Node* source, int& iteration, int position, bool& var_controle, KeyReference element);
    // Route in symmetrical order for findPositionOfElement method
    int simetric(Node* source, KeyConstReference key, int& iteration, bool& var_controle);
    // Route in symmetrical order to calculate the median
    void simetricToMedian(Node* node, std::vector<Node*>& dados);
    // Auxiliar method for the isComplete method
    int nodesOnLevel(Node* _pt, int current_level, int level);
    // Returns height of tree
    int getHeight(Node* _pt);
    // Methods for the various ways of printing the tree (toString)
    void toStringPerLevel(Node* pointer, std::stringstream& ss);
    void toStringSorted(Node* pointer, std::stringstream& ss);
    void toStringHierarchical(const Node* node, bool isLeft, std::stringstream& ss, const std::string& prefix);
    void toStringHierarchical(Node* pointer, std::stringstream& ss);
    // Choose Rotation
    Node* chooseRotation(Node* pointer);
    // Rotation methods
    Node* singleRightRotate(Node* head);
    Node* singleLeftRotate(Node* head);
    Node* doubleRightRotate(Node* head);
    Node* doubleLeftRotate(Node* head);

   public:
    //=== Public interface

    //=== Constructors and Destructors
    Avl(void);
    Avl(DataConstReference _data, KeyConstReference _key);
    ~Avl(void);

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

    //=== Printing Method
    std::string toString(std::string type);
};

}  // namespace tree

#include "avlTree.inl"
#endif  // _AVL_TREE_