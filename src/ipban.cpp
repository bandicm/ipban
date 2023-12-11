#include "../lib/ipban.hpp"

marcelb::ipban::ipban(const uint& _duration) {
    ban_duration = _duration*60;

    unban_bot = async(launch::async, [&]() {
        while (run_unban_bot) {
            sleep_if(BOT_LOOP_TIME, run_unban_bot);
            cout << "Sleep" << endl;
            io.lock();
            for (uint i=0; i<banned.size(); i++) {
                if (difftime(time(NULL), banned[i]._time) >= ban_duration) {
                    unban(banned.begin() + i);
                }
            }
            io.unlock();
        }
        return;
    });

}

marcelb::ipban::~ipban() {
    run_unban_bot = false;
    unban_bot.get();
    for (uint i=0; i<banned.size(); i++) {
        unban(banned.begin() + i);
    }
}

bool marcelb::ipban::ban(const string& ip) {
    ufw_ban(ip);
    io.lock();
    banned.push_back({ip, time(NULL)});
    io.unlock();
}

bool marcelb::ipban::unban(vector<_ban>::iterator ban_itr) {
    ufw_unban(ban_itr->ip);
    io.lock();
    banned.erase(ban_itr);
    io.unlock();
}

bool marcelb::ipban::ufw_ban(const string& ip) {
    cout << "UFW ban IP: " << ip << endl;
}


bool marcelb::ipban::ufw_unban(const string& ip) {
    cout << "UFW unban IP: " << ip << endl;
}

static void marcelb::sleep_if(const uint& _time, const bool& _condition) {
    time_t start_time = time(NULL);
    do {
        sleep(BOT_SLEEP_LOOP_TIME);
    } while (difftime(time(NULL), start_time) < _time && _condition);
}
