#include "../lib/ipban.hpp"

using namespace marcelb;

std::string generateRandomIP() {
    return std::to_string(rand() % 256) + "." +
           std::to_string(rand() % 256) + "." +
           std::to_string(rand() % 256) + "." +
           std::to_string(rand() % 256);
}

// time_t random_time (const uint& offset) {
//    time_t _time = time(nullptr);
//    if (_time %2 == 0) {
//         return (time(nullptr) + rand()%offset);
//    }
//    else {
//        return (time(nullptr) - rand()%offset);
//    }
// }

int main() {
    ipban myban(1);
    // myban.ban("192.168.2.74");
    // sleep(300);
    uint i=0;

    while (i<10) {
        string ip = generateRandomIP();
        cout << "Ban " << ip << endl;
        myban.ban(ip);
        sleep(30);
        i++;
    }

    return 0;
}