#include "action.hpp"
#include "parser.hpp"
#include "yaml-cpp/yaml.h"
#include <cstdio>
#include <cstring>

int main(int argc, char *argv[]) {
    int force = 0;
    ActionEnum action;
    if (argc <= 1 || argc >= 4) {
        fprintf(stderr, "Use : %s [--force] Action\n Action = newconf, backup, restore, uninstall ", argv[0]);
        exit(1);
    }

    if (argc == 3 && strcmp(argv[1], "force")) {
        force = 1;
        action = parser(argv[2]);
    } else if (argc == 2) {
        action = parser(argv[1]);
    } else {
        fprintf(stderr, "Use : %s [--force] Action\n Action = newconf, backup, restore, uninstall ", argv[0]);
        exit(1);
    }
    Config_t config = get_config();

    if (action == ActionEnum::Backup) {
        backup(config);
    } else if (action == ActionEnum::Restore) {
        restore(config, force);
    } else if (action == ActionEnum::Uninstall) {
        uninstall(config, force);
    } else if (action == ActionEnum::Newconf) {
        newconf(config);
    } else if (action == ActionEnum::Error) {
        fprintf(stderr, "Use : %s Action\n Action = newconf, backup, restore, uninstall ", argv[0]);
    }

    return 0;
}
