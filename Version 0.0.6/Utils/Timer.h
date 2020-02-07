#ifndef TIMER_H
#define TIMER_H

//Standard Libary
#include <chrono>
#include <iostream>

class Timer {
public:
	Timer() {
		startTimePoint = std::chrono::high_resolution_clock::now();
	}

	~Timer() {
		Stop();
	}

	void Stop() {
		auto endTimePoint = std::chrono::high_resolution_clock::now();
		auto start = std::chrono::time_point_cast<std::chrono::minutes>(startTimePoint).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::minutes>(endTimePoint).time_since_epoch().count();
		auto duration = end - start;
		std::cout << "------------------------------------------\n";
		std::cout << "Simulation Time: " << duration << " minutes\n";
	}
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> startTimePoint;
};

#endif //TIMER_H