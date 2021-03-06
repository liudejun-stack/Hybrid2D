#ifndef MATH_H
#define MATH_H

#include <iostream>
#include <Eigen/Core>


template<typename Scalar> using Vector2 = Eigen::Matrix<Scalar, 2, 1, Eigen::DontAlign>;
using Vec2d = Vector2<double>;

template<typename Scalar> using Vector3 = Eigen::Matrix<Scalar, 3, 1, Eigen::DontAlign>;
using Vec3d = Vector3<double>;

template<typename Scalar> using Vector9 = Eigen::Matrix<Scalar, 9, 1, Eigen::DontAlign>;
using Vec9d = Vector9<double>;

template<typename Scalar> using Vector2 = Eigen::Matrix<Scalar, 2, 1, Eigen::DontAlign>;
using Vec2i = Vector2<int>;

template<typename Scalar> using Vector3 = Eigen::Matrix<Scalar, 3, 1, Eigen::DontAlign>;
using Vec3i = Vector3<int>;


#define ASSERT(x) \
                 if (! (x)) \
                { \
                   std::cout << "ERROR!! Assert " << #x << " failed\n"; \
                   std::cout << " on line " << __LINE__  << "\n"; \
                   std::cout << " in file " << __FILE__ << "\n";  \
                   std::string pause; \
                   std::cin >> pause; \
                }

#endif //MATH_H