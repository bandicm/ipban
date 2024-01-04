#include "../lib/ipban.hpp"

marcelb::ipban::ipban(const uint& _duration, const uint& _fail_interval, const uint& _fail_limit, const string& _db_file) {
    ban_duration = _duration*60;
    fail_interval = _fail_interval*60;
    fail_limit = _fail_limit;
    db_file = _db_file;
    load_db();

    unban_bot = async(launch::async, [&]() {
        while (run_unban_bot) {
            sleep_if(BOT_LOOP_TIME, run_unban_bot);
            for (uint i=0; i<banned.size(); i++) {
                if (difftime(time(NULL), banned[i]._time) >= ban_duration) {
                    unban(banned.begin() + i);
                }
            }
            for (auto _failed = failed.begin(); _failed != failed.end(); ) {
                if (difftime(time(NULL), _failed->second.first_fail) >= fail_interval && _failed != failed.end()) {
                    f_io.lock();
                    _failed = failed.erase(_failed);
                    f_io.unlock();
                } else {
                    ++_failed;
                }
            }
        }
        return;
    });
}

marcelb::ipban::~ipban() {
    run_unban_bot = false;
    unban_bot.get();

    /**
     * ako aplikaciju sruši napadač - želimo da ostane banovan - unbanovat će se po isteku intervala
    */

    // for (uint i=0; i<banned.size(); i++) {
    //     unban(banned.begin() + i);
    // }
}

void marcelb::ipban::load_db() {
    string line;
    ifstream mydb (db_file);
    if (mydb.is_open()) {
        while (getline(mydb, line) ) {
            struct _ban saved_ban;
            saved_ban.ip = line.substr(0, line.find('-'));
            saved_ban._time = stol(line.substr(line.find('-')+1));
            banned.push_back(saved_ban);
        }
        mydb.close();
    }
}

bool marcelb::ipban::update_db() {
    bool success = false;
    ofstream mydb (db_file);
    if (mydb.is_open()) {
        for (int i=0; i<banned.size(); i++) {
            mydb << banned[i].ip << "-" << banned[i]._time << "\n";
        }
        success = true;
        mydb.close();
    }
    else {
        success = false;
    }
    return success;
}

bool marcelb::ipban::ban(const string& ip) {
    bool status = ufw_ban(ip);
    io.lock();
    banned.push_back({ip, time(NULL)});
    status = status && update_db();
    io.unlock();
    return status;
}

bool marcelb::ipban::unban(vector<_ban>::iterator ban_itr) {
    bool status = ufw_unban(ban_itr->ip);
    io.lock();
    banned.erase(ban_itr);
    status = status && update_db();
    io.unlock();
    return status;
}

bool marcelb::ipban::ufw_ban(const string& ip) {
    string ufw_cmd = "sudo ufw deny from " + ip + " to any";
    try {
        string execute_res = exec(ufw_cmd);
        if (execute_res == "Rule added\n") {
            return true;
        }
    } catch (const string except) {
        cout << except << endl;
    }
    return false;
}


bool marcelb::ipban::ufw_unban(const string& ip) {
    string ufw_cmd = "sudo ufw delete deny from " + ip + " to any";
    try {
        string execute_res = exec(ufw_cmd);
        if (execute_res == "Rule deleted\n") {
            return true;
        }
    } catch (const string except) {
        cout << except << endl;
    }
    return false;
}


void marcelb::ipban::fail(const string& ip) {
    lock_guard<mutex> _io(f_io);
    if (failed[ip].n_fails == 0) {
        failed[ip].n_fails = 1;
        failed[ip].first_fail = time(NULL);
    } else if (++failed[ip].n_fails >= fail_limit) {
        ban(ip);
        failed.erase(ip);
    }
}


bool marcelb::ipban::unfail(const string& ip) {
    lock_guard<mutex> _io(f_io);
    return failed.erase(ip);;
}


static void marcelb::sleep_if(const uint& _time, const bool& _condition) {
    time_t start_time = time(NULL);
    do {
        sleep(BOT_SLEEP_LOOP_TIME);
    } while (difftime(time(NULL), start_time) < _time && _condition);
}
