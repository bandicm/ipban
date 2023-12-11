#ifndef IP_BAN
#define IP_BAN

#include <iostream>
#include <vector>
#include <string>
#include <future>
#include <mutex>

#include <time.h>
#include <unistd.h>

using namespace std;

namespace marcelb {

#define BOT_LOOP_TIME 60    // 1 minutes
#define BOT_SLEEP_LOOP_TIME 1 // 1 second

struct _ban {
    string ip;
    time_t _time;
};

class ipban {
    mutex io;
    time_t ban_duration;     // u sekundama
    vector<_ban> banned;
    future<void> unban_bot;
    bool run_unban_bot = true;
    // interface možda bude trebao za ban

    bool unban(vector<_ban>::iterator ban_itr);
    bool ufw_ban(const string& ip);
    bool ufw_unban(const string& ip);

    public:
    ipban(const uint& _duration);    // u minutama?
    bool ban(const string& ip);
    ~ipban();

};

static void sleep_if(const uint& _time, const bool& _condition);

}

#endif