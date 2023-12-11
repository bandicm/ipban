#include "../lib/ipban.hpp"

using namespace marcelb;

int main() {
    ipban myban(1);
    myban.ban("192.168.2.74");
    sleep(80);    
    return 0;
}