#ifndef VIRTUALGOBAN_HPP
#define VIRTUALGOBAN_HPP

#include <iostream>
#include "Projector.hpp"

namespace rago{

class VirtualGoban{

private:

    Mat matDraw;
    Projector* proj;

public:

    VirtualGoban(Projector*);
    ~VirtualGoban();

    void draw();
    void addStone(int, int, int);
    void setClock1(int);
    void setClock2(int);
    void setText(std::string);
    void handDetected();
    void drawBorders();
};

}

#endif // VIRTUALGOBAN_HPP
