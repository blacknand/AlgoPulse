#include <zmq.hpp>
#include <chrono>
#include <thread>
#include <sstream>
#include <iostream>

int main() {
    // Create ZeroMQ context and publisher socket.
    zmq::context_t context(1);
    zmq::socket_t publisher(context, ZMQ_PUB);

    publisher.bind("tcp://*:5555");
    std::cout << "Publisher started on tcp://*:5555" << std::endl;

    while (true) {
        auto now = std::chrono::system_clock::now().time_since_epoch();
        long long ts = std::chrono::duration_cast<std::chrono::microseconds>(now).count();

        // Format: symbol,bidPrice,askPrice,bidVolume,askVolume,timestamp
        std::ostringstream oss;
        oss << "AAPL,150.0,151.0,100,100," << ts;
        std::string msg = oss.str();

        zmq::message_t message(msg.c_str(), msg.size());
        publisher.send(message, zmq::send_flags::none);

        std::cout << "Published: " << msg << std::endl;

        // Sleep for 100 milliseconds before sending the next message.
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
