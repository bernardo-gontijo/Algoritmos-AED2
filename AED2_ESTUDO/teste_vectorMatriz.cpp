#include <iostream>
#include <vector>

int main() {
    std::vector<std::vector<int>> matriz = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    //imprimir
    std::cout << "Matriz 3x3:" << std::endl;
    for (const auto& lin : matriz){
        for(const auto& elem : lin){
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}