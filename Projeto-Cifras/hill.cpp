#include <iostream>
#include <limits>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <algorithm>

using namespace std;

// Converte letra maiúscula para número (A=0...Z=25)
int charToNum(char c) {
    return toupper(c) - 'A';
}

// Converte número para letra maiúscula
char numToChar(int n) {
    return 'A' + (n % 26);
}

// Calcula o inverso modular de 'a' mod 'm' (apenas se existir)
int modInverse(int a, int m) {
    a %= m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1)
            return x;
    }
    return -1;
}

// Determinante da matriz
int determinant(const vector<vector<int>>& mat, int n) {
    if (n == 1) return mat[0][0];
    if (n == 2) return (mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0]);

    int det = 0;
    for (int x = 0; x < n; x++) {
        vector<vector<int>> submat(n - 1, vector<int>(n - 1));
        for (int i = 1; i < n; i++) {
            int colIdx = 0;
            for (int j = 0; j < n; j++) {
                if (j == x) continue;
                submat[i - 1][colIdx++] = mat[i][j];
            }
        }
        int sign = (x % 2 == 0) ? 1 : -1;
        det += sign * mat[0][x] * determinant(submat, n);
    }
    return det;
}

// Matriz adjunta
vector<vector<int>> adjoint(const vector<vector<int>>& mat, int n) {
    vector<vector<int>> adj(n, vector<int>(n));
    if (n == 1) {
        adj[0][0] = 1;
        return adj;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            vector<vector<int>> submat(n - 1, vector<int>(n - 1));
            int r = 0;
            for (int row = 0; row < n; row++) {
                if (row == i) continue;
                int c = 0;
                for (int col = 0; col < n; col++) {
                    if (col == j) continue;
                    submat[r][c++] = mat[row][col];
                }
                r++;
            }
            int sign = ((i + j) % 2 == 0) ? 1 : -1;
            adj[j][i] = (sign * determinant(submat, n)) % 26;
            if (adj[j][i] < 0) adj[j][i] += 26;
        }
    }
    return adj;
}

// Inversa modular da matriz
vector<vector<int>> inverseMatrix(const vector<vector<int>>& mat, int n) {
    int det = determinant(mat, n) % 26;
    if (det < 0) det += 26;

    int invDet = modInverse(det, 26);
    if (invDet == -1)
        throw runtime_error("Matriz não invertível módulo 26.");

    vector<vector<int>> adj = adjoint(mat, n);
    vector<vector<int>> inv(n, vector<int>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            inv[i][j] = (adj[i][j] * invDet) % 26;
    return inv;
}

// Multiplicação matriz × vetor
vector<int> multiplyMatrixVector(const vector<vector<int>>& mat, const vector<int>& vec) {
    int m = vec.size();
    vector<int> result(m, 0);
    for (int i = 0; i < m; i++)
        for (int j = 0; j < m; j++)
            result[i] = (result[i] + mat[i][j] * vec[j]) % 26;
    return result;
}

// Gera matriz chave m×m válida
vector<vector<int>> generateKeyMatrix(int m) {
    vector<vector<int>> mat(m, vector<int>(m));
    srand(time(nullptr));
    while (true) {
        for (int i = 0; i < m; i++)
            for (int j = 0; j < m; j++)
                mat[i][j] = rand() % 26;

        try {
            inverseMatrix(mat, m);
            return mat;
        } catch (...) {
            continue;
        }
    }
}

// Remove espaços e preenche com 'X' até múltiplo de m
string preprocess(string text, int m) {
    string clean = "";
    for (char c : text)
        if (isalpha(c)) clean += toupper(c);
    while (clean.size() % m != 0)
        clean += 'X';
    return clean;
}

// Criptografia
string hillEncrypt(string text, const vector<vector<int>>& key, int m) {
    text = preprocess(text, m);
    if (text.size() % m != 0)
        throw runtime_error("Texto inválido após preprocessamento.");

    string encrypted = "";
    for (size_t i = 0; i < text.size(); i += m) {
        vector<int> block(m);
        for (int j = 0; j < m; j++)
            block[j] = charToNum(text[i + j]);

        vector<int> res = multiplyMatrixVector(key, block);
        for (int val : res)
            encrypted += numToChar(val);
    }
    return encrypted;
}

// Descriptografia
string hillDecrypt(string text, const vector<vector<int>>& invKey, int m) {
    if (text.size() % m != 0)
        throw runtime_error("Texto cifrado não é múltiplo do bloco.");

    string decrypted = "";
    for (size_t i = 0; i < text.size(); i += m) {
        vector<int> block(m);
        for (int j = 0; j < m; j++)
            block[j] = charToNum(text[i + j]);

        vector<int> res = multiplyMatrixVector(invKey, block);
        for (int val : res)
            decrypted += numToChar(val);
    }
    return decrypted;
}

// Imprime matriz
void printMatrix(const vector<vector<int>>& mat) {
    for (auto row : mat) {
        for (int val : row)
            cout << val << "\t";
        cout << endl;
    }
}

int main() {
    int m;
    cout << "Digite o tamanho do bloco (m >= 2): ";
    if (!(cin >> m)) {
        cout << "Erro: entrada inválida. Digite um número inteiro." << endl;
        return 1;
    }
    if (m < 2 || m > 5) {
        cout << "Por enquanto, use blocos entre 2 e 5 por segurança." << endl;
        return 1;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string plaintext = "ygor eh professor";
    cout << "\nTexto original: " << plaintext << endl;

    try {
        vector<vector<int>> key = generateKeyMatrix(m);
        vector<vector<int>> invKey = inverseMatrix(key, m);

        cout << "\nMatriz chave (" << m << "x" << m << "):\n";
        printMatrix(key);

        string encrypted = hillEncrypt(plaintext, key, m);
        cout << "\nTexto criptografado: " << encrypted << endl;

        cout << "\nMatriz inversa:\n";
        printMatrix(invKey);

        string decrypted = hillDecrypt(encrypted, invKey, m);
        cout << "\nTexto decifrado: " << decrypted << endl;

    } catch (const exception& e) {
        cerr << "\nErro: " << e.what() << endl;
        return 1;
    }

    return 0;
}
