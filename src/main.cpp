#include "action.hpp"
#include "parser.hpp"
#include "yaml-cpp/yaml.h"
#include <cstdio>

int main(int argc, char *argv[]) {
    if (argc <= 1 || argc >= 3) {
        fprintf(stderr, "Use : %s Action\n Action = newconf, backup, restore, uninstall ", argv[0]);
        exit(1);
    }

    ActionEnum action = parser(argv[1]);
    Config_t config = get_config();

    if (action == ActionEnum::Backup) {
        backup(config);
    } else if (action == ActionEnum::Restore) {
        restore(config);
    } else if (action == ActionEnum::Uninstall) {
        uninstall(config);
    } else if (action == ActionEnum::Newconf) {
        newconf(config);
    } else if (action == ActionEnum::Error) {
        fprintf(stderr, "Use : %s Action\n Action = newconf, backup, restore, uninstall ", argv[0]);
    }

    return 0;
}
