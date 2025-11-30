#include "User.h"
#include <fstream>
#include <iostream>

// Crypto++ Headers
#include <cryptopp/eccrypto.h>
#include <cryptopp/oids.h>
#include <cryptopp/hex.h>
#include <cryptopp/sha.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>

using namespace std;
using namespace CryptoPP;

// Constructor
User::User(const string& clientID, const string& publicKeyHex) {
    this->clientID = clientID;
    this->publicKeyHex = publicKeyHex;
}

void User::saveToDatabase() const{
    // 1. Check if user already exists
    ifstream readFile("clients.txt");
    string dbID, dbKey;
    while (readFile >> dbID >> dbKey) {
        if (dbID == this->clientID) {
            cout << "[DB] User " << this->clientID << " already exists. Keeping original key." << endl;
            return; // EXIT: Do not save the new (mismatched) key
        }
    }
    readFile.close();

    // 2. Only if not found, append the new user
    ofstream file("clients.txt", ios::app);
    if (file.is_open()) {
        file << this->clientID << " " << this->publicKeyHex << endl;
        file.close();
        cout << "[DB] New User " << this->clientID << " registered." << endl;
    }
}

// Verify ECDSA Signature
bool User::verifySignature(const string& clientID,const string& message,const string& signatureHex) {
    ifstream file("clients.txt");
    string dbID, dbPubKey;
    bool found = false;

    // 1. Find the Public Key for the given Client ID
    while (file >> dbID >> dbPubKey) {
        if (dbID == clientID) {
            found = true;
            break;
        }
    }

    if (!found) { 
        cout << "[AUTH FAIL] Client ID '" << clientID << "' not found." << endl;
        return false; 
    }

    try {
        string binaryKey, binarySignature;
        StringSource(dbPubKey, true, new HexDecoder(new StringSink(binaryKey)));
        StringSource(signatureHex, true, new HexDecoder(new StringSink(binarySignature)));

        ECDSA<ECP, SHA256>::PublicKey publicKey;
        publicKey.Load(StringSource(binaryKey, true).Ref());

        ECDSA<ECP, SHA256>::Verifier verifier(publicKey);

        bool result = verifier.VerifyMessage(
            reinterpret_cast<const CryptoPP::byte*>(message.data()),
            message.size(), 
            reinterpret_cast<const CryptoPP::byte*>(binarySignature.data()),
            binarySignature.size()
        );

        return result;

    } catch (const Exception& e) {
        cerr << "[CRYPTO ERROR] " << e.what() << endl;
        return false;
    }
}