#include "mime_types.hpp"

namespace http {
namespace server {
namespace mime_types {

struct mapping
{
    const char* extension;
    const char* mime_type;
} mappings[] =
{
{ "gif", "image/gif" },
{ "htm", "text/html" },
{ "html", "text/html" },
{ "jpg", "image/jpeg" },
{ "png", "image/png" },

{ "css", "text/css" },
{ "js", "application/javascript" },
{ "jpeg", "image/jpeg" },
{ "swf", "application/x-shockwave-flash" },

{ "bin", "application/octet-stream" },
{ "exe", "application/octet-stream" },
{ "dll", "application/octet-stream" },
{ "deb", "application/octet-stream" },
{ "dmg", "application/octet-stream" },
{ "iso", "application/octet-stream" },
{ "img", "application/octet-stream" },
{ "msi", "application/octet-stream" },
{ "msp", "application/octet-stream" },
{ "msm", "application/octet-stream" },
//{ "", "" },

{ 0, 0 } // Marks end of list.
};

std::string extension_to_type(const std::string& extension)
{
    for (mapping* m = mappings; m->extension; ++m)
    {
        if (m->extension == extension)
        {
            return m->mime_type;
        }
    }

    return "text/plain";
}

} // namespace mime_types
} // namespace server
} // namespace http
