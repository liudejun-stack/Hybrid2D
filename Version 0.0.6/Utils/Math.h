#ifndef MATH_H
#define MATH_H

//Standard Libary
#include <iostream>
#include <string>
#include <Eigen/Core>

using real = double;

//Templates:
template<typename Scalar> using Vector2 = Eigen::Matrix<Scalar, 2, 1, Eigen::DontAlign>;
using Vector2r = Vector2<real>;

template<typename Scalar> using Vector3 = Eigen::Matrix<Scalar, 3, 1>;
using Vector3r = Vector3<real>;


//Defines:
#define ASSERT_FLAG 1		
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