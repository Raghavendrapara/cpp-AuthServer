
# C++ gRPC Authentication Service

A secure, high-performance backend using **C++17**, **gRPC**, and **ECDSA** (Elliptic Curve Digital Signatures).

## 📦 Dependencies
You need [vcpkg](https://github.com/microsoft/vcpkg) installed.

```powershell
# Windows (PowerShell)
cd C:\vcpkg
.\vcpkg install grpc:x64-windows protobuf:x64-windows cryptopp:x64-windows
```
# 1. Clone
git clone [https://github.com/Raghavendrapara/cpp-AuthServer.git](https://github.com/Raghavendrapara/cpp-AuthServer.git)
cd cpp-AuthServer

# 2. Build
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build .

# 3. Run Server (Open in Terminal 1)
# Windows:
.\Debug\AuthServer.exe
# Linux/Mac:
./AuthServer

# 4. Run Test Client (Open in Terminal 2)
# Windows:
.\Debug\ClientTool.exe
# Linux/Mac:
./ClientTool