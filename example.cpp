#include <iostream>
#include <string>
#include <thread>
#include <sstream>
#include "messageQueue.h"

/////////////////////////////////////////////////////////////////////////////
// Name:        messageQueue.h
// Purpose:     messageQueue: Simple Message Queue for intra-thread communication
// Author:      Julian Smart
// Modified by:
// Created:     13 July 2020
// Copyright:   (c) MR Ibrahim
// Licence:     GNU GENERAL PUBLIC LICENSE
/////////////////////////////////////////////////////////////////////////////
void TestQueue() {
    MessageQueue<std::string> fromMain;
    MessageQueue<std::string> fromThread;

    std::thread t([&]() {
        std::cout << "In thread...." << std::endl;
        while (true) {
            std::string msg;
            if (!fromMain.Receive(msg)) {
                std::cout << "Thread: nothing to receive" << std::endl;
                break;
            }
            if (msg == "stop") {
                break;
            }
            std::cout << ">>" << msg << std::endl;
            fromThread.Send(msg + " processed.");
        }
        fromThread.Send("stop");
    });

    std::thread m([&]() {
        for (int i = 0; i < 20; i++) {
            std::stringstream ss;
            ss << "Message No " << i;
            fromMain.Send(ss.str());
        }
        fromMain.Send("stop");
        std::string msg;
        while (true) {
            if (!fromThread.Receive(msg)) continue;
            if (msg == "stop") break;
            std::cout << "- " << msg << std::endl;
        }
    });
    m.join();
    t.join();
}

int main() {
    TestQueue();
    return 0;
}
