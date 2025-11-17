#include "crow.h"
#include "User.h"
#include <iostream>

int main() {
    // Initialize the Crow App
    crow::SimpleApp app;

    // --- ENDPOINT 1: HEALTH CHECK ---
    // GET http://localhost:8080/health
    CROW_ROUTE(app, "/health")([](){
        return "Auth Service is Online";
    });

    // --- ENDPOINT 2: REGISTER ---
    // POST http://localhost:8080/register
    // Body: { "username": "admin", "password": "123" }
    CROW_ROUTE(app, "/register").methods(crow::HTTPMethod::POST)([](const crow::request& req){
        auto x = crow::json::load(req.body);
        
        if (!x) return crow::response(400, "Invalid JSON");
        
        std::string u = x["username"].s();
        std::string p = x["password"].s();

        // Create User and Save (Uses your existing logic!)
        User newUser(u, p);
        newUser.saveToDatabase();

        crow::json::wvalue result;
        result["status"] = "success";
        result["message"] = "User registered successfully";
        return crow::response(201, result);
    });

    // --- ENDPOINT 3: LOGIN (The Core Feature) ---
    // POST http://localhost:8080/login
    // Body: { "username": "admin", "password": "123" }
    CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::POST)([](const crow::request& req){
        auto x = crow::json::load(req.body);
        
        if (!x) return crow::response(400, "Invalid JSON");

        std::string u = x["username"].s();
        std::string p = x["password"].s();

        crow::json::wvalue result;

        // Verify credentials (Uses your existing SHA-256 logic!)
        if (User::verifyCredentials(u, p)) {
            result["status"] = "PASS";
            return crow::response(200, result);
        } else {
            result["status"] = "FAIL";
            return crow::response(401, result);
        }
    });

    // Start the server on port 8080
    std::cout << "Starting Auth Server on port 8080..." << std::endl;
    app.port(8080).multithreaded().run();
}