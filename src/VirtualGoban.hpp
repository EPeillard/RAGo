#ifndef VIRTUALGOBAN_HPP
#define VIRTUALGOBAN_HPP

#include "Projector.hpp"
#include "define.hpp"

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
    void removeClockBorders();
    void drawBorders();
    void removeBorders();
void remove(int x, int y);
};

}

#endif // VIRTUALGOBAN_HPP
