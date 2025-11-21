#include <iostream>
#include <vector>

int main(){
    //inciar com valores fixos
    std::vector<int> vec{1, 2, 3, 4};
    vec.push_back(5);

    //imprimir
    for (int i = 0; i < vec.size(); i++) {
        std::cout << vec[i] << " ";

    }
    std::cout << "\n";

    //imprimir o vector com um range-based for e auto(automaticamente o tipo, nesse caso 'int')
    for (auto x : vec){
        std::cout << x << " ";
    }
    return 0;
}