#include "RangeSearch.h"

#define CHECKED_FACTOR 50

vector<tuple<int,Image*>> aproxRangeSrch(Image* queryImage,
                                        Lsh* structure,
                                        double radius) {
    PriorityQueue<PriorityCloser> queue;

    // number of lsh tables
    int numTables = structure->getNumTables();

    int checked = 0;
    for (int i = 0; i < numTables; ++i) {
        LshTable *tbl = structure->getHashTable(i);
        tuple<int, Bucket *> bucketTpl = tbl->getBucket(queryImage);
        Bucket *buckPtr = get<1>(bucketTpl);
        if (buckPtr == nullptr)
            continue;
        vector<Image *> *buckImgs = buckPtr->getImages();
        for (int j = 0; j < buckImgs->size(); ++j) {
            if(buckImgs->at(j)->isMarked())
                continue;
            buckImgs->at(j)->markImage();

            int newDist = manhattanDistance(queryImage->getPixels(), buckImgs->at(j)->getPixels());
            if(newDist <= radius)
                queue.insert(buckImgs->at(j), newDist);

            if(++checked > (2*CHECKED_FACTOR*numTables))
                break;
        }
        if(checked > (2*CHECKED_FACTOR*numTables))
            break;
    }

    //gather results
    vector<tuple<int, Image*>> result;
    queue.transferToVector(&result);
    return result;
}

vector<tuple<int, Image*>> aproxRangeSrch(Image* queryImage,
                                          HyperCube* structure,
                                          int checkThrshld,
                                          int probes,
                                          double radius) {
    PriorityQueue<PriorityCloser> queue;

    vector<string> verticesToCheck;
    string queryVrtx = structure->getVertexIdx(queryImage);
    getVerticesToCheck(verticesToCheck, queryVrtx, queryVrtx.length());

    int checked = 0;    // stop when a lot of potential NNeighbours are checked
    for(int i = 0; i < probes; ++i) {   //How many of the possible vertices to
        string curVrtx = verticesToCheck.at(i);
        Bucket * bucketPtr = structure->getVertices().at(curVrtx);
        if(bucketPtr == nullptr)
            continue;
        vector<Image *> *buckImgs = bucketPtr->getImages();
        for(int j = 0; j < buckImgs->size(); ++j) {
            if (buckImgs->at(j)->isMarked())
                continue;
            buckImgs->at(j)->markImage();
            int newDist = manhattanDistance(queryImage->getPixels(), buckImgs->at(j)->getPixels());
            if(newDist <= radius)
                queue.insert(buckImgs->at(j), newDist);

            if(++checked > checkThrshld)
                break;
        }
        if(checked > checkThrshld)
            break;
    }

    //gather results
    vector<tuple<int, Image*>> result;
    queue.transferToVector(&result);
    return result;
}
