#ifndef ACTION_H
#define ACTION_H
#include <cstdlib>
#include <filesystem>
#include <string>

struct Config_s {
    std::string home;
    std::string programname;
    std::string configdir;
    std::string configpath;
    std::string dotfilespath;
};

typedef struct Config_s Config_t;

Config_t get_config();
void backup(Config_t);
void restore(Config_t);
void uninstall(Config_t);
void newconf(Config_t);

#endif // !ACTION_H
