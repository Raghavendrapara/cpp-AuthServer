#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "auth.grpc.pb.h"
#include <cryptopp/eccrypto.h>
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>
#include <cryptopp/oids.h>
#include <cryptopp/filters.h>

using namespace CryptoPP;
using namespace std;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using auth::AuthService;
using auth::RegisterRequest;
using auth::RegisterResponse;
using auth::AuthRequest;
using auth::AuthResponse;

class AuthClient {
public:
    AuthClient(std::shared_ptr<Channel> channel)
        : stub_(AuthService::NewStub(channel)) {
        // Generate Keys for this session
        AutoSeededRandomPool prng;
        privateKey_.Initialize(prng, ASN1::secp256r1());
        privateKey_.MakePublicKey(publicKey_);
    }

    void Register(const string& clientID) {
        RegisterRequest request;
        request.set_client_id(clientID);
        
        string hexKey;
        publicKey_.Save(HexEncoder(new StringSink(hexKey)).Ref());
        request.set_public_key_hex(hexKey);

        RegisterResponse reply;
        ClientContext context;
        Status status = stub_->Register(&context, request, &reply);

        if (status.ok()) cout << "Register: " << reply.message() << endl;
        else cout << "Register Failed." << endl;
    }

    void Login(const string& clientID) {
        AuthRequest request;
        request.set_client_id(clientID);
        string message = "TIMESTAMP_2025"; 
        request.set_timestamp_message(message);

        // Sign the message
        string signature;
        AutoSeededRandomPool prng;
        ECDSA<ECP, SHA256>::Signer signer(privateKey_);
        StringSource s(message, true, new SignerFilter(prng, signer, new HexEncoder(new StringSink(signature))));
        request.set_signature_hex(signature);

        AuthResponse reply;
        ClientContext context;
        Status status = stub_->Authenticate(&context, request, &reply);

        if (status.ok()) cout << "Login: " << reply.message() << endl;
        else cout << "Login Failed." << endl;
    }

private:
    std::unique_ptr<AuthService::Stub> stub_;
    ECDSA<ECP, SHA256>::PrivateKey privateKey_;
    ECDSA<ECP, SHA256>::PublicKey publicKey_;
};

int main() {
    AuthClient client(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    client.Register("TraderBot_01");
    client.Login("TraderBot_01");
    return 0;
}