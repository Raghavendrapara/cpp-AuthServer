#pragma once
#include <string>

class User {
public:
    // Constructor: Creates a user object to store data
    User(const std::string& clientID, const std::string& publicKeyHex);
    
    // Persist the user to clients.txt
    void saveToDatabase() const;

    // STATIC function to verify signatures without creating a User object
    static bool verifySignature(const std::string& clientID, const std::string& message, const std::string& signatureHex);

private:
    std::string clientID;
    std::string publicKeyHex;
};