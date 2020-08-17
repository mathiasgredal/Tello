#ifndef TELLO_TERMINAL_H
#define TELLO_TERMINAL_H

#include <array>
#include <string>
#include <vector>

#include "imterm/terminal.hpp"
#include "imterm/terminal_helpers.hpp"

#include "Tello_UDP.h"

struct Terminal_State {
    bool ShouldQuit = false;
    Tello::UPD* udp_server;
};

class Tello_Terminal : public ImTerm::basic_terminal_helper<Tello_Terminal, Terminal_State>
{
public:
    Tello_Terminal();

    static std::vector<std::string> no_completion(argument_type&) { return {}; }
    static std::vector<std::string> configure_term_autocomplete(argument_type&);

    // Commands
    static void clear(argument_type&);
    static void echo(argument_type&);
    static void exit(argument_type&);
    static void udp(argument_type&);

    // API
    static void sdk(argument_type&);


};

#endif // TELLO_TERMINAL_H
