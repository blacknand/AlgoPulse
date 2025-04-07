#include "DataIngestion.h"
#include "HFTBot.h"
#include <sstream>
#include <iostream>

DataIngestion::DataIngestion(const std::string& endpoint, DataCallback callback)
    : running(false), context(1), socket(context, ZMQ_SUB), endpoint(endpoint), callback(callback) {
    socket.connect(endpoint);
    socket.set(zmq::sockopt::subscribe, "");
}

void DataIngestion::start() {
    running = true;
    while (running) {
        zmq::message_t message;
        // Block until a message is received.
        auto result = socket.recv(message, zmq::recv_flags::none);
        if (result) {
            std::string msgStr(static_cast<char*>(message.data()), message.size());

            // Parse the message.
            // Expected format: symbol,bidPrice,askPrice,bidVolume,askVolume,timestamp
            std::istringstream iss(msgStr);
            std::string token;
            MarketData data;
            
            if (std::getline(iss, token, ',')) data.symbol = token;
            if (std::getline(iss, token, ',')) data.bidPrice = std::stod(token);
            if (std::getline(iss, token, ',')) data.askPrice = std::stod(token);
            if (std::getline(iss, token, ',')) data.bidVolume = std::stoi(token);
            if (std::getline(iss, token, ',')) data.askVolume = std::stoi(token);
            if (std::getline(iss, token, ',')) {
                long long ts = std::stoll(token);
                data.timestamp = std::chrono::microseconds(ts);
            }

            // Pass the parsed data to the callback.
            callback(data);
        }
    }
}

void DataIngestion::stop() {
    running = false;
}
