
#ifndef COLORS_H
#define COLORS_H

#include <ostream>

namespace Colors
{
enum Id
{
    RESET = 0,
    FG_RED = 31,
    FG_GREEN = 32,
    FG_YELLOW = 33,
    FG_BLUE = 34,
    FG_MAGENTA = 35,
    FG_CYAN = 36,
    FG_WHITE = 37,
    FG_DEFAULT = 39,
    BG_RED = 41,
    BG_GREEN = 42,
    BG_YELLOW = 43,
    BG_BLUE = 44,
    BG_MAGENTA = 45,
    BG_CYAN = 46,
    BG_WHITE = 47,
    BG_DEFAULT = 49,
};

class Define
{
private:
    Id cid;
    const std::string estart = "\x1B[";
    const std::string estop = "m";

public:
    Define(Id id) : cid(id) {}
    std::string to_str(void)
    {
        return estart + std::to_string(cid) + estop;
    }
    friend std::ostream &operator<<(std::ostream &os, const Define &def)
    {
        return os << def.estart << def.cid << def.estop;
    }
};

} // namespace Colors

#endif
