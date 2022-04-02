#include "hm/vfs.h"

namespace
{
    std::string& base_path_ref()
    {
        static std::string bp;
        return bp;
    }
}

const std::string& get_base_path()
{
    return base_path_ref();
}

void set_base_path(const std::string& p)
{
    base_path_ref() = p;
}

