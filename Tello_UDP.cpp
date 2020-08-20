#include "Tello_UDP.h"

void run_service(boost::asio::io_service& io_service ) {
    io_service.run();
}

Tello::UPD::UPD() : SDK_io_service(), SDK_socket(SDK_io_service, udp::endpoint(udp::v4(), SDK_ListenPort))
{
    // Yeet
    std::cout << "hello" << std::endl;
    //SDK_UDP_server_thread = std::thread(run_service, std::ref(SDK_io_service));
     //   SDK_recieve_new();
}



void Tello::UPD::SDK_StartServer()
{
   SDK_UDP_server_thread = std::thread(run_service, std::ref(SDK_io_service));
   SDK_recieve_new();
}

void Tello::UPD::SDK_StopServer()
{
    if(!SDK_io_service.stopped())
        SDK_io_service.stop();

    SDK_UDP_server_thread.join();
}

void Tello::UPD::SDK_SendRequest(std::string message, int timeout, std::function<void (Tello::UDP_Response)> callback)
{
    UDP_Request request;

    request.time = system_clock::now().time_since_epoch().count();
    request.timeout = timeout;
    request.callback = callback;
    request.sentMessage = message;

    SDK_socket.send_to(boost::asio::buffer(message), SDK_send_endpoint);

    SDK_requestQueue.push(request);

}

void Tello::UPD::SDK_recieve_new()
{
    SDK_socket.async_receive_from(
        boost::asio::buffer(SDK_input_buffer),
        SDK_listen_endpoint,
        boost::bind(&Tello::UPD::SDK_handle_recieve, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Tello::UPD::SDK_handle_recieve(const boost::system::error_code &error, size_t message_size)
{
    if (!error || error == boost::asio::error::message_size)
    {
        // Get the first element in the queue
        auto first = SDK_requestQueue.front();

        // Check if the element has timed out

        // If not then the recieved message is meanth for it
        SDK_requestQueue.pop();
        UDP_Response response;
        std::string message = "";
        for(int i = 0; i < message_size; i++)
            message.push_back(SDK_input_buffer[i]);


        response.message = message;
        first.callback(response);

        SDK_recieve_new();
    }
}
