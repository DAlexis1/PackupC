#ifndef ACTION_H
#define ACTION_H
#include <cstdlib>
#include <fstream>
#include <string>
#include <yaml-cpp/yaml.h>

struct Config_s {
    std::string home;
    std::string programname;
    std::string configdir;
    std::string configpath;
    std::string dotfilespath;
};

typedef struct Config_s Config_t;

void CorrectNode(YAML::Node &backupmap, Config_t config);
Config_t get_config();
void backup(Config_t);
void restore(Config_t config, int force);
void uninstall(Config_t config, int force);
void newconf(Config_t);

#endif // !ACTION_H
