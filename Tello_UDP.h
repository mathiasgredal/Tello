#ifndef TELLO_UPD_H
#define TELLO_UPD_H

// This is needed because MSVC is weird and requires you to explicitly link windows libraries
#pragma comment(lib, "bcrypt")

#include <array>
#include <atomic>
#include <chrono>
#include <functional>
#include <iostream>
#include <queue>
#include <thread>

#include <uvw.hpp>

using namespace std::chrono;

namespace Tello {

enum class UDP_Error : int {
    Success = 0,
    NetworkError = 1,
    Timeout = 2,
    Other = 3
};

struct UDP_Response {
    std::string message;
    UDP_Error error;
};

struct UDP_Request {
    std::string sentMessage;
    uint64_t time;
    int timeout;
    std::function<void(UDP_Response)> callback;
};

class UDP {
private:
    // Event loop
    std::thread SDK_thread;
    std::shared_ptr<uvw::Loop> SDK_loop;

    // UDP Networking
    std::shared_ptr<uvw::UDPHandle> SDK_server;

    u_short SDK_ListenPort;

    std::queue<UDP_Request> SDK_requestQueue;

    // Thread managment and communications
    std::shared_ptr<uvw::AsyncHandle> async_server;
    std::atomic_bool is_running = false;

public:
    UDP(std::string ip_address, u_short send_port, u_short listen_port);
    ~UDP();

    // SDK UDP Server
    void SDK_SendRequest(std::string message, int timeout, std::function<void(UDP_Response)> callback);

    void SDK_handle_recieve(const uvw::UDPDataEvent& data, uvw::UDPHandle& handle);
    void SDK_handle_error(const uvw::ErrorEvent& error, uvw::UDPHandle& handle);

    std::string SDK_Address;
    u_short SDK_SendPort;

    // Thread COMS, used to call functions on the SDK_thread.
    //Mainly used for shutting down the threads, as it can only be done from inside the event thread as libuv isn't thread safe
};

}

#endif // TELLO_UPD_H
