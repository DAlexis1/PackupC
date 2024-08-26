#include "action.hpp"
#include "yaml-cpp/yaml.h"
#include <string>

Config_t get_config() {
    Config_t config;
    const char *env_xdg = std::getenv("XDG_CONFIG_HOME");
    if (env_xdg == NULL) {
        config.configdir = std::string("");
    } else {
        config.configdir = std::string(env_xdg);
    };
    const char *env_home = std::getenv("HOME");
    if (env_home == NULL) {
        config.home = std::string("");
    } else {
        config.home = std::string(env_home);
    };
    config.programname = std::string("Packup");
    if (config.home == std::string("")) {
        printf("Please check you environment variable for $HOME (format should be /home/username on linux) and $XDG_CONFIG_HOME (format should be $HOME/.config for example)");
        exit(1);
    };
    if (config.configdir == std::string("")) {
        config.configdir = config.home;
        config.configdir.append("/.config");
    };
    config.dotfilespath = config.home;
    config.dotfilespath.append("/.dotfiles");
    config.configpath = config.configdir + "/" + config.programname + "/" + config.programname + ".yaml";
    return config;
};

YAML::Node CorrectNode(YAML::Node backupmap, Config_t config) {
    int endstr;
    for (YAML::const_iterator prog = backupmap.begin(); prog != backupmap.end(); ++prog) {
        if (backupmap[prog->first.as<std::string>()].as<std::string>().at(0) == '~') {
            backupmap[prog->first.as<std::string>()] = config.home + backupmap[prog->first.as<std::string>()].as<std::string>().substr(1);
        }
        if (backupmap[prog->first.as<std::string>()].as<std::string>().back() == '/') {
            endstr = backupmap[prog->first.as<std::string>()].as<std::string>().length();
            backupmap[prog->first.as<std::string>()] = backupmap[prog->first.as<std::string>()].as<std::string>().substr(0, endstr - 1);
        };
    };
    return backupmap;
}

void backup(Config_t config) {
    std::string progname, prognamepath;
    if (!std::filesystem::exists(config.dotfilespath)) {
        std::filesystem::create_directory(config.dotfilespath);
    }
    YAML::Node config_yaml = YAML::LoadFile(config.configpath);
    YAML::Node backup_map = config_yaml["Backup"];
    backup_map = CorrectNode(backup_map, config);
    for (YAML::const_iterator prog = backup_map.begin(); prog != backup_map.end(); ++prog) {
        progname = prog->first.as<std::string>();
        prognamepath = prog->second.as<std::string>();
        if (std::filesystem::exists(config.dotfilespath + "/" + progname.c_str())) {
            printf("%s is already backup", progname.c_str());
        } else {
            std::filesystem::create_directory(config.dotfilespath + "/" + progname.c_str());

            std::filesystem::rename(prognamepath, config.dotfilespath + "/" + progname.c_str() + "/" + prognamepath.substr(prognamepath.find_last_of("/")));

            std::filesystem::create_symlink(config.dotfilespath + "/" + progname.c_str() + "/" + prognamepath.substr(prognamepath.find_last_of("/")), prognamepath);
        }
    }
}

void restore(Config_t config) { return; };
void uninstall(Config_t config) { return; };
void newconf(Config_t config) { return; };
