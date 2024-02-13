#ifndef IP_BAN
#define IP_BAN

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <future>
#include <mutex>
#include <fstream>
#include <algorithm>

#include <time.h>
#include <unistd.h>

#include "../../exec/lib/exec.hpp"

using namespace std;

namespace marcelb {

#define BOT_LOOP_TIME 60    // 1 minutes
#define BOT_SLEEP_LOOP_TIME 1 // 1 second

/**
 * Banned object
 * IP address and ban time
*/
struct _ban {
    string ip;
    time_t _time;
};

/**
 * Auxiliary structure - to track the number of errors
*/
struct _fail {
    time_t first_fail;
    uint n_fails = 0;
};

/**
 * Library to ban IP addresses through the UFW firewall for a certain period of time
 * Automatically removes ban after timeout
 * It has its own DB mechanism for protection against irreversible ban
*/
class ipban {
    mutex io, f_io, wl_io;
    time_t ban_duration;
    uint fail_interval;
    uint fail_limit;
    string db_file;
    vector<_ban> banned;
    map<string, struct _fail> failed;
    vector<string> white_list;
    future<void> unban_bot;
    bool run_unban_bot = true;
    
    /**
     * The method loads banned IP addresses from the database
    */
    void load_db();

    /**
     * The method updates database states with memory states
    */
    bool update_db();

    /**
     * The method removes the ban for the passed iterator of the banned vector and updates the base
    */
    bool unban(vector<_ban>::iterator ban_itr);

    /**
     * The method checks if the forwarding ip address is currently banned
    */
    bool is_banned(const string& ip);

    /**
     * Method calls exec and adds rules to UFW firewall
    */
    bool ufw_ban(const string& ip);

    /**
     * The method calls exec and removes the rule in the UFW firewall
    */
    bool ufw_unban(const string& ip);

    /**
     * Checks whether the forwarded address is in the white list
     * If it returns true, if not false
    */
    bool is_in_white_list(const string& ip);

    public:

    /**
     * Constructor, receives the default value of the duration of the ban in minutes,
     * address error tracking time, number of allowed errors
     * and the database file path
    */
    ipban(const uint& _duration, const uint& _fail_interval = 30, const uint& _fail_limit = 3, const string& db_file = "ipban.db");    // u minutama?

    /**
     * The method that bans the forwarded IP address, adds it to the banned vector, updates the database
     * Returns the status of the operation
    */
    bool ban(const string& ip);

    /**
     * Incrementally increase the number of errors for the forwarded address
     * if the number of allowed errors in the interval is exceeded - the address is banned
    */
    bool fail(const string& ip);

    /**
     * Removes errors for forwarded address
    */
    bool unfail(const string& ip);

    /**
     * Adds the forwarded address to the white list
    */
    void add_white_list(const string& ip);

    /**
     * Adds forwarded addresses to the white list
    */
    void add_white_list(const vector<string>& ips);

    /**
     * Destructor
    */
    ~ipban();

};

/**
 * Sleep function, which sleeps until timeout or logic condition condition
 * Receives time in seconds, and logical condition
*/
static void sleep_if(const uint& _time, const bool& _condition);

}

#endif