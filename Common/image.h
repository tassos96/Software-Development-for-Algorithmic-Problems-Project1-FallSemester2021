#ifndef PROJECT_TASK1_IMAGE_H
#define PROJECT_TASK1_IMAGE_H

#include <iostream>
#include <vector>


using namespace std;

class Image {
private:
    int id;
    bool marked;
    //bool markedRange;
    vector<unsigned int> *g_results; //keep hash results
    vector<unsigned char> *pixels;

public:
    Image(int);
    ~Image();
    int getId();
    void setPixel(unsigned char);

    vector<unsigned char> *getPixels() const;
    void keepHashResult(unsigned int);

    void markImage();
    //void markImageRange();
    bool isMarked();
    //bool isMarkedRange();
    void unmarkImage();
};


#endif //PROJECT_TASK1_IMAGE_H