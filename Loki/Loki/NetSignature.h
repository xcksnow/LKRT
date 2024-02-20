//#include <iostream>
//#include <chrono>
//#include <Wincrypt.h>
//#include <Windows.h>
//#include <iomanip>
//#include <sstream>
//
//#pragma comment(lib, "Crypt32.lib")
//
//using namespace std;
//
//
//    static string createTimeStamp() {
//        auto now = std::chrono::system_clock::now();
//        auto now_time = std::chrono::system_clock::to_time_t(now);
//        std::stringstream ss;
//        ss << std::put_time(std::localtime(&now_time), "%Y-%m-%dT%H:%M:%S");
//        return ss.str();
//    }
//
//    static std::string createSignature(const std::string& url) {
//        std::string secret = "433g";
//        std::string method = "POST";
//        std::string timeStamp = createTimeStamp();
//        std::string signature = method + url + timeStamp;
//
//        HCRYPTPROV hProv = 0;
//        if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
//            std::cerr << "Error: CryptAcquireContext failed." << std::endl;
//            return "";
//        }
//
//        HCRYPTHASH hHash = 0;
//        if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)) {
//            std::cerr << "Error: CryptCreateHash failed." << std::endl;
//            CryptReleaseContext(hProv, 0);
//            return "";
//        }
//
//        if (!CryptHashData(hHash, reinterpret_cast<const BYTE*>(signature.c_str()), static_cast<DWORD>(signature.length()), 0)) {
//            std::cerr << "Error: CryptHashData failed." << std::endl;
//            CryptDestroyHash(hHash);
//            CryptReleaseContext(hProv, 0);
//            return "";
//        }
//
//        DWORD hashSize = 0;
//        DWORD dataSize = sizeof(DWORD);
//        if (!CryptGetHashParam(hHash, HP_HASHSIZE, reinterpret_cast<BYTE*>(&hashSize), &dataSize, 0)) {
//            std::cerr << "Error: CryptGetHashParam failed." << std::endl;
//            CryptDestroyHash(hHash);
//            CryptReleaseContext(hProv, 0);
//            return "";
//        }
//
//        std::vector<BYTE> hashBuffer(hashSize, 0);
//        if (!CryptGetHashParam(hHash, HP_HASHVAL, hashBuffer.data(), &hashSize, 0)) {
//            std::cerr << "Error: CryptGetHashParam failed." << std::endl;
//            CryptDestroyHash(hHash);
//            CryptReleaseContext(hProv, 0);
//            return "";
//        }
//
//        CryptDestroyHash(hHash);
//        CryptReleaseContext(hProv, 0);
//
//        std::stringstream signatureHex;
//        for (BYTE byte : hashBuffer) {
//            signatureHex << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
//        }
//
//        return signatureHex.str();
//    }
//};
