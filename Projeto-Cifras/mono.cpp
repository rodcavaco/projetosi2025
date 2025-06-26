#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>

std::vector<char> gerarAlfabetoCifradoAleatorio(const std::vector<char>& alfabeto) {
    std::vector<char> alfabetoCifrado = alfabeto;
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(alfabetoCifrado.begin(), alfabetoCifrado.end(), g);
    return alfabetoCifrado;
}

std::string cifrarMensagem(const std::string& mensagem, const std::vector<char>& alfabeto, const std::vector<char>& alfabetoCifrado) {
    std::string resultado = "";

    for (char letra : mensagem) {
        char letraMin = tolower(letra);
        auto it = std::find(alfabeto.begin(), alfabeto.end(), letraMin);

        if (it != alfabeto.end()) {
            int indice = std::distance(alfabeto.begin(), it);
            resultado += alfabetoCifrado[indice];
        } else {
            resultado += letra;
        }
    }

    return resultado;
}

int main() {
    std::vector<char> alfabeto = { 'a','b','c','d','e','f','g','h','i','j','k',
                                   'l','m','n','o','p','q','r','s','t','u','v',
                                   'w','x','y','z' };

    std::vector<char> alfabetoCifrado = gerarAlfabetoCifradoAleatorio(alfabeto);

    std::string mensagem = "ygor eh professor";

    std::string mensagemCifrada = cifrarMensagem(mensagem, alfabeto, alfabetoCifrado);

    std::cout << "Alfabeto original: ";
    for (char c : alfabeto) std::cout << c << " ";
    std::cout << std::endl;

    std::cout << "Alfabeto cifrado : ";
    for (char c : alfabetoCifrado) std::cout << c << " ";
    std::cout << std::endl;

    std::cout << "Mensagem original: " << mensagem << std::endl;
    std::cout << "Mensagem cifrada : " << mensagemCifrada << std::endl;

    return 0;
}
