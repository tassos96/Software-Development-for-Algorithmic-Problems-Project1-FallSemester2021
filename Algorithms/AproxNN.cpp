#include "AproxNN.h"

#define CHECKED_FACTOR 50

tuple<vector<tuple<int,Image*>>, microseconds> aproxKNN(Image* queryImage,
                                                        Lsh* structure,
                                                        int numNeighbors) {
    PriorityQueue<PriorityFurther> queue;

    //start timer
    high_resolution_clock::time_point startTimer = high_resolution_clock::now();

    int numTables = structure->getNumTables();     // number of lsh tables
    int checked = 0; // stop when a lot of potential NNeighbours are checked
    for (int i = 0; i < numTables; ++i) {
        LshTable *tbl = structure->getHashTable(i);
        tuple<int, Bucket *>bucketTpl = tbl->getBucket(queryImage);
        Bucket * buckPtr = get<1>(bucketTpl);
        if(buckPtr == nullptr)
            continue;
        vector<Image *> *buckImgs = buckPtr->getImages();
        for (int j = 0; j < buckImgs->size(); ++j) {
            if(buckImgs->at(j)->isMarked())
                continue;
            buckImgs->at(j)->markImage();
            queue.tryInsert(queryImage,buckImgs->at(j),numNeighbors);
            if(++checked > (CHECKED_FACTOR*numTables))
                break;
        }
        if(checked > (CHECKED_FACTOR*numTables))
            break;
    }
    //stop timer
    high_resolution_clock::time_point stopTimer = high_resolution_clock::now();
    auto timerDuration = duration_cast<microseconds>(stopTimer - startTimer);

    //gather results
    vector<tuple<int, Image*>> result;
    queue.transferToVector(&result);
    return make_tuple(result, timerDuration);
}
