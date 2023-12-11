#ifndef IP_BAN
#define IP_BAN

#include <iostream>
#include <vector>
#include <string>
#include <future>
#include <mutex>
#include <fstream>

#include <time.h>
#include <unistd.h>

#include "../exec/lib/exec.hpp"

using namespace std;

namespace marcelb {

#define BOT_LOOP_TIME 60    // 1 minutes
#define BOT_SLEEP_LOOP_TIME 1 // 1 second

/**
 * Banovani objekt 
 * IP adresa i vrijeme banovanja
*/
struct _ban {
    string ip;
    time_t _time;
};

/**
 * Biblioteka za ban IP adrese kroz UFW vatrozid na određeno vrijeme
 * Automatski uklanja zabranu po isteku vremena
 * Posjeduje vlastiti DB mehanizam za zaštitu od nepovratnog ban-a
*/
class ipban {
    mutex io;
    time_t ban_duration;
    string db_file;
    vector<_ban> banned;
    future<void> unban_bot;
    bool run_unban_bot = true;
    // interface možda bude trebao za ban
    
    /**
     * Metoda učitava banovane IP adrese iz baze
    */
    void load_db();

    /**
     * Metoda ažurira stanja baze sa stanjima iz memorije
    */
    bool update_db();

    /**
     * Metoda uklanja ban za proslijeđeni iterator vektora banned i ažurira bazu
    */
    bool unban(vector<_ban>::iterator ban_itr);

    /**
     * Metoda poziva exec i dodaje pravila u UFW vatrozid
    */
    bool ufw_ban(const string& ip);

    /**
     * Metoda poziva exec i uklanja pravilo u UFW vatrozidu
    */
    bool ufw_unban(const string& ip);

    public:

    /**
     * Konstruktor, prima zadanu vrijednost trajanja ban-a u minutama
     * i putanju datoteke baze podataka
    */
    ipban(const uint& _duration, const string& db_file = "ipban.db");    // u minutama?

    /**
     * Metoda koja banuje proslijeđenu IP adresu, dodaje je u vector banned, ažurira bazu
     * Vraća status operacije
    */
    bool ban(const string& ip);

    /**
     * Destruktor, uklanja sve zabrane.
    */
    ~ipban();

};

/**
 * Funkcija za mirovanje tijeka, koj miruje do isteka vremena ili logičkog stanja uvijeta
 * Prima vrijeme u sekundama, i logički uvijet
*/
static void sleep_if(const uint& _time, const bool& _condition);

}

#endif