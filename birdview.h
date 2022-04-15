#ifndef BIRDVIEW_H
#define BIRDVIEW_H


class BirdView
{
public:
    BirdView();
    static void CallBackFunc(int event, int x, int y, int flags, void* leftCoordinate);
    void SetView();

private:
};

#endif // BIRDVIEW_H
