#ifndef INTERACTION_H
#define INTERACTION_H
#include "Body.hpp"
#include <memory>

class Interaction{
    public:
    Interaction(std::weak_ptr<Body> _body1, std::weak_ptr<Body> _body2) : body1(_body1), body2(_body2){};

    //Methods:
    bool checkContact();


    //Smart pointers:
    std::weak_ptr<Body> body1;
    std::weak_ptr<Body> body2;
    


};

#endif //INTERACTION_H