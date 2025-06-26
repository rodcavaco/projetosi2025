#include <iostream>
#include <string>
#include <cctype>

using namespace std;

// Função para repetir a chave até o tamanho da mensagem
string expandirChave(const string& texto, const string& chave) {
    string chaveExpandida;
    int j = 0;
    for (char c : texto) {
        if (isalpha(c)) {
            chaveExpandida += chave[j % chave.length()];
            j++;
        } else {
            chaveExpandida += c; // mantém espaços e pontuação
        }
    }
    return chaveExpandida;
}

// Função para cifrar
string cifrar(const string& texto, const string& chave) {
    string resultado;
    string chaveExpandida = expandirChave(texto, chave);

    for (size_t i = 0; i < texto.length(); ++i) {
        char letra = texto[i];
        char k = chaveExpandida[i];

        if (isalpha(letra)) {
            bool isUpper = isupper(letra);
            char base = isUpper ? 'A' : 'a';

            int deslocamento = toupper(k) - 'A';
            char cifrado = ((tolower(letra) - 'a' + deslocamento) % 26) + base;

            resultado += cifrado;
        } else {
            resultado += letra; // mantém caracteres especiais
        }
    }

    return resultado;
}

// Função para decifrar
string decifrar(const string& textoCifrado, const string& chave) {
    string resultado;
    string chaveExpandida = expandirChave(textoCifrado, chave);

    for (size_t i = 0; i < textoCifrado.length(); ++i) {
        char letra = textoCifrado[i];
        char k = chaveExpandida[i];

        if (isalpha(letra)) {
            bool isUpper = isupper(letra);
            char base = isUpper ? 'A' : 'a';

            int deslocamento = toupper(k) - 'A';
            char decifrado = ((tolower(letra) - 'a' - deslocamento + 26) % 26) + base;

            resultado += decifrado;
        } else {
            resultado += letra;
        }
    }

    return resultado;
}

int main() {
    system("chcp 65001 > nul");

    string mensagem, chave;

    cout << "Digite a mensagem: ";
    getline(cin, mensagem);

    cout << "Digite a chave (sem espaços): ";
    cin >> chave;

    string mensagemCifrada = cifrar(mensagem, chave);
    string mensagemDecifrada = decifrar(mensagemCifrada, chave);

    cout << "\nMensagem cifrada: " << mensagemCifrada << endl;
    cout << "Mensagem decifrada: " << mensagemDecifrada << endl;

    return 0;
}
