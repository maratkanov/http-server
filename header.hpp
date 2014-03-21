#ifndef HEADER_HPP
#define HEADER_HPP

#include <string>

namespace http {
namespace server {

struct header
{
    std::string name;
    std::string value;
};

}
}

#endif // HEADER_HPP
