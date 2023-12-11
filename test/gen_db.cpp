#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#include <cstdlib>

struct _ban {
    std::string ip;
    long int _time;
};

// Funkcija za generiranje pseudoslučajnih IP adresa
std::string generateRandomIP() {
    return std::to_string(rand() % 256) + "." +
           std::to_string(rand() % 256) + "." +
           std::to_string(rand() % 256) + "." +
           std::to_string(rand() % 256);
}

int main() {
    //std::ofstream mydb("datoteka.txt");

    // Postavka generiranja pseudoslučajnih brojeva
    srand(static_cast<unsigned>(time(0)));

    // Generiranje desetaka IP adresa i povezanih vremenskih brojeva
    for (int i = 0; i < 10; ++i) {
        _ban generated_ban;
        generated_ban.ip = generateRandomIP();
        generated_ban._time = static_cast<long int>(time(nullptr) - rand()%100);

        // Ispisivanje u datoteku
        std::cout << generated_ban.ip << "-" << generated_ban._time << std::endl;
    }

    std::cout << "Generiranje završeno." << std::endl;

    return 0;
}