#ifndef LINE_ITERATOR_H_
#define LINE_ITERATOR_H_

#include <utility>

class MLineIterator {
public:

    MLineIterator(const double x1_, const double y1_, const double x2_, const double y2_,const int next_step =1);

    bool getNext(std::pair<int, int> &pixel);

private:

    const bool steep;
    double x1, y1, x2, y2;

    double dx, dy, error;

    int maxX, ystep;
    int y, x;
};


#endif // LINE_ITERATOR_H_
