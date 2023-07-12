#include <iostream>
#include <random>
#include <string>
#include <cmath>
#include <vector>

bool isPrime(int num) {
    if (num < 2)
        return false;
    
    if (num == 2)
        return true;
    
    if (num % 2 == 0)
        return false;
    
    for (int i = 3; i * i <= num; i += 2) {
        if (num % i == 0)
            return false;
    }
    
    return true;
}

int generatePrime(int minValue, int maxValue) {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(minValue, maxValue);

    int prime = distribution(generator);
    while (!isPrime(prime)) {
        prime = distribution(generator);
    }

    return prime;
}

int generateE(int minValue, int maxValue) {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(minValue, maxValue);
    int e = distribution(generator);
    return e;
}

int modInverse(int e, int totientEuler) {
    for (int i = 3; i < totientEuler; i++) {

        if ((i*e)%totientEuler == 1) {
            return i;
        }
    }
    throw std::runtime_error("Modular inverse does not exist");
}


void encodeMessage(const std::string& message, std::vector<int>& messageEncoded) {
    for (char ch : message) {
        messageEncoded.push_back(static_cast<int>(ch));
    }
}

int modularExponentiation(int base, int exponent, int modulus) {
    int result = 1;
    base = base % modulus;

    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result = (result * base) % modulus;
        }
        base = (base * base) % modulus;
        exponent /= 2;
    }

    return result;
}

void encryptMessage(const std::vector<int>& messageEncoded, std::vector<int>& cipherText, int e, int n) {
    for (int ch : messageEncoded) {
        int ciphext = modularExponentiation(ch, e, n);
        cipherText.push_back(ciphext);
    }
}


void decryptMessage(const std::vector<int>& cipherText, std::vector<int>& plainText, int d, int n) {
    for (int ch : cipherText) {
        int plaint = modularExponentiation(ch, d, n);
        plainText.push_back(plaint);
    }
}


void decodeMessage(const std::vector<int>& plainText, std::string decodedMessage) {

    for (int ch : plainText) {
        decodedMessage += static_cast<char>(ch);
    }
    std::cout << decodedMessage;
}



int main() {
    int p = generatePrime(1, 100);
    int q = generatePrime(1, 100);
    while (p == q){
        q = generatePrime(1, 100);
    }

    int n = p * q;
    int totientEuler = (p-1)*(q-1);
    int e = generateE(3, totientEuler - 1);

    while (std::gcd(e,totientEuler) != 1){
        e = generateE(3, totientEuler - 1);
    }

    int d = modInverse(e, totientEuler);

    std::string message = "Secret";
    std::vector<int> messageEncoded;
    std::vector<int> cipherText;
    std::vector<int> plainText;
    std::string decodedMessage;

    std::cout << "This is your secret message: " << message;

    encodeMessage(message, messageEncoded);
    //std::cout << "This is your encoded message: " << messageEncoded;

    encryptMessage(messageEncoded, cipherText, e, n);
    //std::cout << "This is your encrypted message: " << cipherText;

    decryptMessage(cipherText, plainText, d, n);
    //cout << "This is your decrypted message: " << plainText;

    decodeMessage(plainText, decodedMessage);
    std::cout << "This is your original message: " << decodedMessage;

    return 0;
}