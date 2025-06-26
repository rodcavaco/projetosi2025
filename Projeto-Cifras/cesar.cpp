#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

std::vector<char> gerarAlfabetoCifrado(const std::vector<char>& alfabeto, int deslocamento) {
    std::vector<char> alfabetoCifrado(alfabeto.size());

    int tamanho = alfabeto.size();
    for (int i = 0; i < tamanho; ++i) {
        int novaPos = (i + deslocamento + tamanho) % tamanho;
        alfabetoCifrado[i] = alfabeto[novaPos];
    }

    return alfabetoCifrado;
}

std::string cifraCesarMapeando(const std::string& mensagem, const std::vector<char>& alfabeto, const std::vector<char>& alfabetoCifrado) {
    std::string resultado;

    for (char letra : mensagem) {
        char letraMin = std::tolower(letra);

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
    std::vector<char> alfabeto = {
        'a','b','c','d','e','f','g','h','i','j','k','l','m',
        'n','o','p','q','r','s','t','u','v','w','x','y','z'
    };

    int deslocamento = 3;
    std::vector<char> alfabetoCifrado = gerarAlfabetoCifrado(alfabeto, deslocamento);

    std::string mensagem = "ygor eh professor";
    std::string mensagemCifrada = cifraCesarMapeando(mensagem, alfabeto, alfabetoCifrado);

    std::cout << "Mensagem original : " << mensagem << std::endl;
    std::cout << "Mensagem cifrada  : " << mensagemCifrada << std::endl;

    // Descriptografar
    deslocamento = -3;
    alfabetoCifrado = gerarAlfabetoCifrado(alfabeto, deslocamento);
    std::string mensagemDecifrada = cifraCesarMapeando("bjru hk surihvvru", alfabeto, alfabetoCifrado);

    std::cout << "Mensagem descriptografada: " << mensagemDecifrada << std::endl;

    return 0;
}
