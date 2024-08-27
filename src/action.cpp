#include "action.hpp"
#include "yaml-cpp/yaml.h"
#include <filesystem>
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
    if (config.home.empty()) {
        printf("Please check you environment variable for $HOME (format should be /home/username on linux) and $XDG_CONFIG_HOME (format should be $HOME/.config for example)");
        exit(1);
    };
    if (config.configdir.empty()) {
        config.configdir = config.home;
        config.configdir.append("/.config");
    };
    config.dotfilespath = config.home;
    config.dotfilespath.append("/.dotfiles");
    config.configpath = config.configdir + "/" + config.programname + "/" + config.programname + ".yaml";
    return config;
};
void CorrectNode(YAML::Node &backupmap, Config_t config) {
    int endstr;
    for (YAML::const_iterator prog = backupmap.begin(); prog != backupmap.end(); ++prog) {
        std::string node_name = backupmap[prog->first.as<std::string>()].as<std::string>();
        if (node_name.at(0) == '~') {
            node_name = config.home + node_name.substr(1);
        }
        if (node_name.back() == '/') {
            endstr = node_name.length();
            node_name = node_name.substr(0, endstr - 1);
        };
        backupmap[prog->first.as<std::string>()] = node_name;
    };
}

void backup(Config_t config) {
    std::string progname, prognamepath;
    if (!std::filesystem::exists(config.dotfilespath)) {
        std::filesystem::create_directory(config.dotfilespath);
    }
    YAML::Node config_yaml = YAML::LoadFile(config.configpath);
    YAML::Node backup_map = config_yaml["Backup"];
    CorrectNode(backup_map, config);

    for (YAML::const_iterator prog = backup_map.begin(); prog != backup_map.end(); ++prog) {
        progname = prog->first.as<std::string>();
        prognamepath = prog->second.as<std::string>();

        if (std::filesystem::exists(config.dotfilespath + "/" + progname.c_str())) {
            printf("%s is already backup\n", progname.c_str());
        } else {
            std::filesystem::create_directory(config.dotfilespath + "/" + progname.c_str());

            std::filesystem::rename(prognamepath, config.dotfilespath + "/" + progname.c_str() + "/" + prognamepath.substr(prognamepath.find_last_of("/")));

            std::filesystem::create_symlink(config.dotfilespath + "/" + progname.c_str() + "/" + prognamepath.substr(prognamepath.find_last_of("/")), prognamepath);
        }
    }

    printf("All files have been backup");
}

void restore(Config_t config, int force) {
    std::string progname, prognamepath;
    YAML::Node config_yaml = YAML::LoadFile(config.dotfilespath + "/" + config.programname + "/" + config.programname + "/" + config.programname + ".yaml");
    YAML::Node backup_map = config_yaml["Backup"];
    CorrectNode(backup_map, config);

    for (YAML::const_iterator prog = backup_map.begin(); prog != backup_map.end(); ++prog) {
        progname = prog->first.as<std::string>();
        prognamepath = prog->second.as<std::string>();

        if (std::filesystem::exists(config.dotfilespath + "/" + progname.c_str() + "/" + prognamepath.substr(prognamepath.find_last_of("/")))) {
            if (std::filesystem::exists(prognamepath) && force == 1) {
                std::filesystem::remove(prognamepath);
            }
            if (!std::filesystem::exists(prognamepath)) {
                std::filesystem::create_symlink(config.dotfilespath + "/" + progname.c_str() + "/" + prognamepath.substr(prognamepath.find_last_of("/")), prognamepath);
            } else {
                printf("A config already exist for %s, please save it in another place\n", progname.c_str());
            }
        } else {
            printf("Can't restore any config for %s", progname.c_str());
        }
    }
    printf("All files have been backup\n");
};
void uninstall(Config_t config, int force) {

    std::string progname, prognamepath;
    YAML::Node config_yaml = YAML::LoadFile(config.dotfilespath + "/" + config.programname + "/" + config.programname + "/" + config.programname + ".yaml");
    YAML::Node backup_map = config_yaml["Backup"];
    CorrectNode(backup_map, config);

    for (YAML::const_iterator prog = backup_map.begin(); prog != backup_map.end(); ++prog) {
        progname = prog->first.as<std::string>();
        prognamepath = prog->second.as<std::string>();

        if (std::filesystem::exists(config.dotfilespath + "/" + progname.c_str() + "/" + prognamepath.substr(prognamepath.find_last_of("/")))) {
            if (std::filesystem::exists(prognamepath) && force == 1) {
                std::filesystem::remove(prognamepath);
            }
            if (!std::filesystem::exists(prognamepath) || std::filesystem::is_symlink(prognamepath)) {
                if (std::filesystem::is_symlink(prognamepath)) {
                    std::filesystem::remove(prognamepath);
                }
                std::filesystem::rename(config.dotfilespath + "/" + progname.c_str() + "/" + prognamepath.substr(prognamepath.find_last_of("/")), prognamepath);
                std::filesystem::remove(config.dotfilespath + "/" + progname);
            } else {
                printf("A config already exist for %s, please save it in another place\n", progname.c_str());
            }
        } else {
            printf("Can't restore any config for %s", progname.c_str());
        }
    }
    printf("Dotfiles copied back to their initial directory");
};
void newconf(Config_t config) {
    YAML::Emitter newconfE;
    std::string config_packup_path;
    config_packup_path = "/.config/" + config.programname + "/" + config.programname + ".yaml";
    newconfE << YAML::BeginMap << YAML::Key << "Backup" << YAML::Value << YAML::BeginMap << YAML::Key << config.programname << YAML::Value << "~" + config_packup_path << YAML::EndMap << YAML::EndMap;
    printf("%s", newconfE.c_str());
    std::ofstream newconfout((config.home + config_packup_path).c_str());
    newconfout << newconfE.c_str();
};
