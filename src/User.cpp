#include "User.h"
#include <fstream>
#include <iostream>

// Crypto++ Headers
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>

using namespace std;
using namespace CryptoPP;

// Constructor
User::User(string username, string password) {
    this->username = username;
    this->password = password;
}

// --- THE SHARED LOGIC ---
string User::hashPassword(string rawPassword) {
    SHA256 hash;
    string digest;

    StringSource s(rawPassword, true, 
        new HashFilter(hash,
            new HexEncoder(
                new StringSink(digest)
            )
        )
    );
    return digest;
}

// Register
void User::saveToDatabase() {
    ofstream file("database.txt", ios::app);
    if (file.is_open()) {
        // 1. Hash it
        string encryptedPass = hashPassword(this->password);
        // 2. Save it
        file << this->username << " " << encryptedPass << endl;
        file.close();
        cout << "[DEBUG] User registered. Saved Hash: " << encryptedPass.substr(0, 10) << "..." << endl;
    }
}

// Login
bool User::verifyCredentials(string username, string inputPassword) {
    ifstream file("database.txt");
    string dbUser, dbPass;

    if (!file.is_open()) return false;

    // 1. Hash the input immediately
    string hashedInput = hashPassword(inputPassword);

    cout << "\n--- DEBUGGING LOGIN ---" << endl;
    cout << "Attempting to match User: " << username << endl;
    cout << "Hash of Input Password:   " << hashedInput.substr(0, 10) << "..." << endl;

    while (file >> dbUser >> dbPass) {
        if (dbUser == username) {
            // Found the user, now checking password
            cout << "Found User in File!" << endl;
            cout << "Hash stored in File:    " << dbPass.substr(0, 10) << "..." << endl;
            
            if (dbPass == hashedInput) {
                return true;
            } else {
                cout << "MISMATCH! Hashes do not match." << endl;
            }
        }
    }
    return false;
}