#include "Tello_UDP.h"

void run_service(std::shared_ptr<uvw::Loop>& loop, std::atomic_bool& is_running)
{
    is_running = true;
    loop->run();
}

std::queue<Tello::UDP_Request> Tello::UDP::getSDK_requestQueue() const
{
    return SDK_requestQueue;
}

Tello::UDP::UDP(std::string ip_address, u_short send_port, u_short listen_port)
{
    SDK_Address = ip_address;
    SDK_SendPort = send_port;
    SDK_ListenPort = listen_port;

    SDK_loop = uvw::Loop::getDefault();

    SDK_server = SDK_loop->resource<uvw::UDPHandle>();

    // Handle server error
    SDK_server->on<uvw::ErrorEvent>([](const uvw::ErrorEvent& error, uvw::UDPHandle& handle) {
        std::cout << "SERVER ERROR(" << error.name() << "," << error.code() << "): " << error.what() << std::endl;
        handle.close();
    });

    // Handle server udp packet recieved. I used a lambda function because referencing the other function directly didn't work
    SDK_server->on<uvw::UDPDataEvent>([this](const uvw::UDPDataEvent& data, uvw::UDPHandle& handle) {
        //std::cout << "SERVER RECIEVED: Got UDP packet from" << data.sender.ip << ":" << data.sender.port << ", of size " << data.length << std::endl;
        SDK_handle_recieve(data, handle);
    });

    SDK_server->bind("0.0.0.0", listen_port);
    SDK_server->recv();

    async_server = SDK_loop->resource<uvw::AsyncHandle>();

    async_server->on<uvw::AsyncEvent>([this](const auto&, auto&) {
        if (!is_running) {
            async_server->close();
            SDK_server->close();
            SDK_server->stop();
            SDK_loop->stop();
        }
    });

    SDK_thread = std::thread(run_service, std::ref(SDK_loop), std::ref(is_running));
}

Tello::UDP::~UDP()
{
    is_running = false;
    async_server->send();
    SDK_thread.join();
    std::cout << "UDP destroyed" << std::endl;
}

void Tello::UDP::SDK_SendRequest(std::string message, int timeout, std::function<void(Tello::UDP_Response)> callback)
{
    UDP_Request request;

    request.time = system_clock::now().time_since_epoch().count();
    request.timeout = timeout;
    request.callback = callback;
    request.sentMessage = message;

    SDK_server->trySend(SDK_Address, SDK_SendPort, message.data(), message.length());

    SDK_requestQueue.push(request);
}

void Tello::UDP::SDK_handle_recieve(const uvw::UDPDataEvent& data, uvw::UDPHandle& handle)
{
    // This is used to stop the event loop from inside the thread, since libuv is not thread safe
    UDP_Response response;    
    std::string message = std::string(data.data.get(), data.length);

    if (SDK_requestQueue.size() > 0) {
        auto first = SDK_requestQueue.front();
        SDK_requestQueue.pop();
        response.message = message;
        first.callback(response);
    }
    // else if(is telemetry response) {

    //}
    else {
        if(WriteToTerminal == nullptr)
            std::cout << "RECIEVED: " << message << std::endl;
        else
            WriteToTerminal(message);
    }
}
