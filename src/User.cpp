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
User::User(string clientID, string publicKeyHex) {
    this->clientID = clientID;
    this->publicKeyHex = publicKeyHex;
}

// Save Client ID and Public Key to file
void User::saveToDatabase() {
    ofstream file("clients.txt", ios::app);
    if (file.is_open()) {
        file << this->clientID << " " << this->publicKeyHex << endl;
        file.close();
        cout << "[DB] User " << this->clientID << " saved to database." << endl;
    } else {
        cerr << "[ERROR] Could not open clients.txt for writing." << endl;
    }
}

// Verify ECDSA Signature
bool User::verifySignature(string clientID, string message, string signatureHex) {
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

   // ... inside src/User.cpp ...

    try {
        string binaryKey, binarySignature;
        StringSource(dbPubKey, true, new HexDecoder(new StringSink(binaryKey)));
        StringSource(signatureHex, true, new HexDecoder(new StringSink(binarySignature)));

        ECDSA<ECP, SHA256>::PublicKey publicKey;
        publicKey.Load(StringSource(binaryKey, true).Ref());

        ECDSA<ECP, SHA256>::Verifier verifier(publicKey);

        // FIX: Use 'CryptoPP::byte' explicitly in the casts below
        bool result = verifier.VerifyMessage(
            (const CryptoPP::byte*)message.data(), 
            message.size(), 
            (const CryptoPP::byte*)binarySignature.data(), 
            binarySignature.size()
        );

        return result;

    } catch (const Exception& e) {
        cerr << "[CRYPTO ERROR] " << e.what() << endl;
        return false;
    }
}