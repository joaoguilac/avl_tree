#include <iostream>  // cout, cin, endl, cerr

#include "../include/binarySearchTree.hpp"
#include "../include/executor.hpp"

int main(int argc, char *argv[]) {
    // Validação inicial dos argumentos
    if (argv[1] == nullptr || argv[2] == nullptr) {
        std::cerr << "ERRO :: Argumentos com o caminho para os arquivos não foi especificado." << std::endl
                  << std::endl;
        exit(1);
    }

    // Declara a árvore binária e o executor de testes
    bst::BinarySearchTree<int, int> tree;
    Executor<int, int> executor(tree);

    // Inicia a execução
    executor.start(argv[1], argv[2]);

    return 0;
}