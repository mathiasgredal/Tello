#include "Tello_Terminal.h"

Tello_Terminal::Tello_Terminal()
{
    add_command_({ "clear", "clears the terminal screen", Tello_Terminal::clear, Tello_Terminal::no_completion });
    add_command_({ "echo", "prints to terminal screen", Tello_Terminal::echo, Tello_Terminal::no_completion });
    add_command_({ "quit", "quits the program", Tello_Terminal::exit, Tello_Terminal::no_completion });
    add_command_({ "udp", "send any string over udp", Tello_Terminal::udp, Tello_Terminal::no_completion });
    add_command_({ "sdk?", "gets the sdk version for drone", Tello_Terminal::sdk, Tello_Terminal::no_completion });
}

void Tello_Terminal::clear(argument_type& arg)
{
    arg.term.clear();
}

void Tello_Terminal::echo(argument_type& arg)
{
    if (arg.command_line.size() < 2) {
        arg.term.add_text("");
        return;
    }

    std::string str {};

    auto it = std::next(arg.command_line.begin(), 1);
    while (it != arg.command_line.end() && it->empty()) {
        ++it;
    }

    if (it != arg.command_line.end()) {
        str = *it;
        for (++it; it != arg.command_line.end(); ++it) {
            if (it->empty()) {
                continue;
            }
            str.reserve(str.size() + it->size() + 1);
            str += ' ';
            str += *it;
        }
    }

    arg.term.add_text(str);
}

void Tello_Terminal::exit(argument_type& arg)
{
    arg.val.ShouldQuit = true;
}

void Tello_Terminal::udp(argument_type& arg)
{
    if (arg.command_line.size() < 2) {
        arg.term.add_text("");
        return;
    }

    std::string str {};

    auto it = std::next(arg.command_line.begin(), 1);
    while (it != arg.command_line.end() && it->empty()) {
        ++it;
    }

    if (it != arg.command_line.end()) {
        str = *it;
        for (++it; it != arg.command_line.end(); ++it) {
            if (it->empty()) {
                continue;
            }
            str.reserve(str.size() + it->size() + 1);
            str += ' ';
            str += *it;
        }
    }

    if (arg.val.udp_server != nullptr)
        arg.val.udp_server->SDK_SendRequest(str, 2000, [arg](Tello::UDP_Response response) {
            arg.term.add_text("\t - "+response.message);
        });
    else
        arg.term.add_text("UDP server not connected");
}

void Tello_Terminal::sdk(argument_type& arg)
{
    if (arg.val.udp_server != nullptr)
        arg.val.udp_server->SDK_SendRequest("sdk?", 2000, [arg](Tello::UDP_Response response) {
            arg.term.add_text(response.message);
        });
    else
        arg.term.add_text("UDP server not connected");
}
