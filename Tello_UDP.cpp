#include "Tello_UDP.h"

void run_service(std::shared_ptr<uvw::Loop>& loop, std::atomic_bool& is_running)
{
    is_running = true;
    loop->run();
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
        std::cout << "SERVER RECIEVED: Got UDP packet from" << data.sender.ip << ":" << data.sender.port << ", of size " << data.length << std::endl;
        //SDK_handle_recieve(data, handle);
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
    std::cout << "RECIEVED: " << data.data.get() << std::endl;

    // This is used to stop the event loop from inside the thread, since libuv is not thread safe
    if (std::string(data.data.get()) == "STOP SERVER" && !is_running) {
        SDK_loop->stop();

        // Taken from here: https://stackoverflow.com/a/47270400
        int result = uv_loop_close(handle.raw()->loop);

        if (result == UV_EBUSY)
            uv_walk(
                handle.raw()->loop, [](uv_handle_t* handle, void* arg) {
                    uv_close(handle, [](uv_handle_t* handle) {
                        if (handle != nullptr) {
                            delete handle;
                        }
                    });
                },
                nullptr);
        return;
    }

    UDP_Response response;

    if (SDK_requestQueue.size() > 0) {
        auto first = SDK_requestQueue.front();
        SDK_requestQueue.pop();
        response.message = std::string(data.data.get());
        first.callback(response);
    } else {
        std::cout << "RECIEVED: " << data.data.get() << std::endl;
    }
}
