#pragma once
#include <string>

class User
{
public:
    // Constructor
    User(std::string username, std::string password);

    // Methods
    bool login(std::string inputPassword);
    std::string getUsername();
    void saveToDatabase();
    static bool verifyCredentials(std::string username, std::string password);

private:
    std::string username;
    std::string password;
    
    // Helper function to scramble/unscramble strings
    static std::string hashPassword(std::string rawPassword);
};