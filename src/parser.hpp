#ifndef PARSER_H
#define PARSER_H

#include <string>

enum actionenum {
    Newconf = 1,
    Backup = 2,
    Restore = 3,
    Uninstall = 4,
    Error = 0,
};
typedef enum actionenum ActionEnum;

actionenum parser(const std::string &str);

#endif // !PARSER_H
