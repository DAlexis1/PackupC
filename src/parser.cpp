#include "parser.hpp"

ActionEnum parser(const std::string &str) {
    if (str == "backup") {
        return ActionEnum::Backup;
    } else if (str == "newconf") {
        return ActionEnum::Newconf;
    } else if (str == "restore") {
        return ActionEnum::Restore;
    } else if (str == "uninstall") {
        return ActionEnum::Uninstall;
    } else {
        return ActionEnum::Error;
    }
}
