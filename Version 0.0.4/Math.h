#ifndef MATH_H
#define MATH_H

#include <iostream>
#include <Eigen/Core>
#include <chrono>

//Templates:
template<typename Scalar> using Vector2 = Eigen::Matrix<Scalar, 2, 1, Eigen::DontAlign>;
using Vec2d = Vector2<double>;

template<typename Scalar> using Vector3 = Eigen::Matrix<Scalar, 3, 1, Eigen::DontAlign>;
using Vec3d = Vector3<double>;

template<typename Type> void print(Type value) { std::cout << value << "\n"; }


//Classes
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
		auto start        = std::chrono::time_point_cast<std::chrono::minutes>(startTimePoint).time_since_epoch().count();
		auto end          = std::chrono::time_point_cast<std::chrono::minutes>(endTimePoint).time_since_epoch().count();
		auto duration     = end - start;

		std::cout << duration << " minutes\n";
	}
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> startTimePoint;
};


//Defines:
#define ASSERT_FLAG 1		//PROBABLY CHANGE THIS TO BE TURNED OFF IN RELEASE MODE
#if ASSERT_FLAG == 1
#define ASSERT(x) \
                 if (! (x)) \
                { \
                   std::cout << "ERROR!! Assert " << #x << " failed\n"; \
                   std::cout << " on line " << __LINE__  << "\n"; \
                   std::cout << " in file " << __FILE__ << "\n";  \
                   std::string pause; \
                   std::cin >> pause; \
                }
#else
#define ASSERT(x)
#endif

#endif //MATH_H