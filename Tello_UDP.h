#ifndef TELLO_UPD_H
#define TELLO_UPD_H

// This is needed because MSVC is weird and requires you to explicitly link windows libraries
#pragma comment(lib, "bcrypt")

#include <iostream>
#include <queue>
#include <thread>
#include <array>
#include <functional>
#include <atomic>
#include <chrono>

#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;
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

class UPD
{
public:

    UPD();

    // SDK UDP Server
    void SDK_StartServer();
    void SDK_StopServer();

    void SDK_SendRequest(std::string message, int timeout, std::function<void(UDP_Response)> callback);

    std::queue<UDP_Request> SDK_requestQueue;

    const u_short SDK_SendPort = 8889;
    const u_short SDK_ListenPort = 8890;

    boost::asio::io_service SDK_io_service;
    udp::socket SDK_socket;
    boost::asio::ip::address SDK_Tello_IpAddress = boost::asio::ip::make_address("192.168.1.242");
    udp::endpoint SDK_listen_endpoint;
    udp::endpoint SDK_send_endpoint = udp::endpoint(SDK_Tello_IpAddress, SDK_SendPort);

    std::array<char, 1000> SDK_input_buffer;
    std::thread SDK_UDP_server_thread;

    void SDK_recieve_new();
    void SDK_handle_recieve(const boost::system::error_code& error, std::size_t message_size);

    // Video UDP Server



};

}

#endif // TELLO_UPD_H
