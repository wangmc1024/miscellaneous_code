#include "time_wheel.hpp"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

void string_callback(const std::string& msg) {
    std::cout << "Timer triggered, message: " << msg << std::endl;
}

void int_callback(const int& value) {
    std::cout << "Timer triggered, value: " << value << std::endl;
}

int main() {
    try {
        TimeWheel<std::string> string_timer(60, 100);
        TimeWheel<int> int_timer(60, 100);

        // Start the time wheel
        string_timer.start();
        int_timer.start();

        std::cout << "Time wheel started" << std::endl;

        // Add a few timers
        auto* timer1 = string_timer.add_timer(
            500,  // Trigger after 500ms
            string_callback,
            "500ms timer triggered!"
        );

        auto* timer2 = string_timer.add_timer(
            2000,  // Trigger after 2 seconds
            string_callback,
            "2 seconds timer triggered!"
        );

        auto* timer3 = int_timer.add_timer(
            1000,  // Trigger after 1 second
            int_callback,
            100
        );

        auto* timer4 = int_timer.add_timer(
            3000,  // Trigger after 3 seconds
            int_callback,
            300
        );

        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        std::cout << "Cancel timer 2" << std::endl;
        string_timer.remove_timer(timer2);

        std::this_thread::sleep_for(std::chrono::seconds(2));

        string_timer.stop();
        int_timer.stop();
        std::cout << "Time wheel stopped" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
