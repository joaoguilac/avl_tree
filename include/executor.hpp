#ifndef _EXECUTOR_H_
#define _EXECUTOR_H_

#include <fstream>   // std::ifstream
#include <ios>       // ios_base::in
#include <iostream>  // st::cout, std::cin, std::endl
#include <istream>   // std::istringstream
#include <sstream>   // std::stringstream
#include <string>    // std::string

#include "binarySearchTree.hpp"

/**
 * @brief A class to execute tests for a binary search tree.
 *
 * @tparam DataType Type of data the container will store in binary search tree.
 * @tparam KeyType Type of key the container will store in binary search tree.
 */
template <typename DataType, typename KeyType>
class Executor {
    //=== Private members.
   private:
    bst::BinarySearchTree<DataType, KeyType>* tree;
    bool leave = false;

    //=== Private methods.
   private:
    std::string processLine(std::string line);
    void openAndValideFile(std::ifstream& file, std::string fileName, bool mustValidateNumbers = false);
    void processMutipleInserts(std::ifstream& file);

    //=== Public methods.
   public:
    Executor(bst::BinarySearchTree<DataType, KeyType>& tree);
    void start(std::string insertionFileName, std::string commandFileName);
};

#include "executor.inl"
#endif  // _EXECUTOR_H_