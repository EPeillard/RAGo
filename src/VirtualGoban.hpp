#ifndef VIRTUALGOBAN_HPP
#define VIRTUALGOBAN_HPP

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
    void drawClock();
    void drawClockBorders(int);
    void drawBorders();
    void removeBorders();
};

}

#endif // VIRTUALGOBAN_HPP
