#include <string>
#include "server.hpp"

int main() {
    try {
        // Initialise the server.
        http::server::server s("0.0.0.0", "8080", "/home/flexo/my_doc_root");

        // Run the server until stopped.
        s.run();
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
