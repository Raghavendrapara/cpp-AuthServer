#pragma once
#include <string>

class User {
public:
    // Constructor: Creates a user object to store data
    User(std::string clientID, std::string publicKeyHex);
    
    // Persist the user to clients.txt
    void saveToDatabase();

    // STATIC function to verify signatures without creating a User object
    static bool verifySignature(std::string clientID, std::string message, std::string signatureHex);

private:
    std::string clientID;
    std::string publicKeyHex;
};