#include <iostream>

void inverter_frase(std::string& frase)
{
    int tam = frase.length();
    int i = 0;

    while (i < tam){
        if (frase[i] != ' '){
            int left = i;

        while (i < tam && frase[i] != ' '){
            i++;
        }
        int right = i - 1;

        while (left < right){
            std::swap(frase[left], frase[right]);
                left++;
                right--;
            }
        }else{
            i++;
        }
    }
}

int main()
{
    std::string frase = "O mateus é bagre";
    inverter_frase(frase);
    std::cout << frase << std::endl;
    return 0;
}
