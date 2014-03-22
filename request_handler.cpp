#include "request_handler.hpp"
#include "mime_types.hpp"
#include "reply.hpp"
#include "request.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>

namespace http {
namespace server {

request_handler::request_handler(const std::string &doc_root)
    : doc_root_(doc_root)
{
}

void request_handler::handle_request(const request &req, reply &rep)
{
    // Decode url to path.
    std::string request_path;
    if (!url_decode(req.uri, request_path))
    {
        rep = reply::stock_reply(reply::bad_request);
        return;
    }

    // Request path must be absolute and not contain "..".
    if (request_path.empty() || request_path[0] != '/'
            || request_path.find("..") != std::string::npos)
    {
        rep = reply::stock_reply(reply::bad_request);
        return;
    }

    if (req.method == "POST") {
        rep = reply::stock_reply(reply::bad_request);
        return;
    }

    // If path ends in slash (i.e. is a directory) then add "index.html".
    if (request_path[request_path.size() - 1] == '/')
    {
        request_path += "index.html";
    }

    // GET parameters after ?
    std::size_t first_question_pos = request_path.size();
    if (request_path.find("?") != std::string::npos) {
        first_question_pos = request_path.find_first_of("?");
    }

    // Determine the file extension.
    std::size_t last_slash_pos = request_path.find_last_of("/");
    std::size_t last_dot_pos = request_path.find_last_of(".");
    std::string extension;
    if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos)
    {
        extension = request_path.substr(last_dot_pos + 1, first_question_pos - last_dot_pos - 1);
    }

    // Open the file to send back.
    std::string full_path = doc_root_ + request_path.substr(0, first_question_pos);
    std::cout << full_path << std::endl;    // TODO: here
    std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
    if (!is)
    {
//        std::cout << last_slash_pos << " " << request_path.size() << std::endl;    // TODO: here
        if (request_path.find("index.html") != std::string::npos) {
            rep = reply::stock_reply(reply::forbidden);
        }
        else {
            rep = reply::stock_reply(reply::not_found);
        }
        return;
    }

    // Fill out the reply to be sent to the client.
    rep.status = reply::ok;

    char buf[512];
    while (is.read(buf, sizeof(buf)).gcount() > 0)
        rep.content.append(buf, is.gcount());

    rep.headers.resize(5);

    rep.headers[0].name = "Date";
    rep.headers[0].value = make_datetime_string();
    rep.headers[1].name = "Server";
    rep.headers[1].value = "Alexei_SERVER";
    rep.headers[2].name = "Content-Length";
    rep.headers[2].value = boost::lexical_cast<std::string>(rep.content.size());
    rep.headers[3].name = "Content-Type";
    rep.headers[3].value = mime_types::extension_to_type(extension);
    rep.headers[4].name = "Connection";
    rep.headers[4].value = "close";

    if (req.method == "HEAD") {
        rep.content.erase();
    }
}

bool request_handler::url_decode(const std::string& in, std::string& out)
{
    out.clear();
    out.reserve(in.size());
    for (std::size_t i = 0; i < in.size(); ++i)
    {
        if (in[i] == '%')
        {
            if (i + 3 <= in.size())
            {
                int value = 0;
                std::istringstream is(in.substr(i + 1, 2));
                if (is >> std::hex >> value)
                {
                    out += static_cast<char>(value);
                    i += 2;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        else if (in[i] == '+')
        {
            out += ' ';
        }
        else
        {
            out += in[i];
        }
    }
    return true;
}

std::string request_handler::make_datetime_string() {
    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
    return boost::posix_time::to_simple_string(now).c_str();
}

}
}
