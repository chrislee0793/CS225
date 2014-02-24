/**
 * @file filler.cpp
 * Implementation of functions in the filler namespace. Heavily based on
 * old MP4 by CS225 Staff, Fall 2010.
 *
 * @author Chase Geigle
 * @date Fall 2012
 */
#include "filler.h"
#include <algorithm>
 
animation filler::dfs::fillSolid( PNG & img, int x, int y,
        RGBAPixel fillColor, int tolerance, int frameFreq ) {
        solidColorPicker c(fillColor);
    return filler::dfs::fill(img, x, y, c, tolerance, frameFreq);
}
 
animation filler::dfs::fillGrid( PNG & img, int x, int y,
        RGBAPixel gridColor, int gridSpacing, int tolerance, int frameFreq ) {
        gridColorPicker c(gridColor, gridSpacing);
    return filler::dfs::fill(img, x, y, c, tolerance, frameFreq);
}
 
animation filler::dfs::fillGradient( PNG & img, int x, int y,
        RGBAPixel fadeColor1, RGBAPixel fadeColor2, int radius,
        int tolerance, int frameFreq ) {
        gradientColorPicker c(fadeColor1, fadeColor2, radius, x, y);
    return filler::dfs::fill(img, x, y, c, tolerance, frameFreq);
}
 
animation filler::dfs::fill( PNG & img, int x, int y,
        colorPicker & fillColor, int tolerance, int frameFreq ) {
 
    return filler::fill<Stack>(img, x, y, fillColor, tolerance, frameFreq);
}
 
animation filler::bfs::fillSolid( PNG & img, int x, int y,
        RGBAPixel fillColor, int tolerance, int frameFreq ) {
        solidColorPicker c(fillColor);
    return filler::bfs::fill(img, x, y, c, tolerance, frameFreq);
}
 
animation filler::bfs::fillGrid( PNG & img, int x, int y,
        RGBAPixel gridColor, int gridSpacing, int tolerance, int frameFreq ) {
        gridColorPicker c(gridColor, gridSpacing);
    return filler::bfs::fill(img, x, y, c, tolerance, frameFreq);
}
 
animation filler::bfs::fillGradient( PNG & img, int x, int y,
        RGBAPixel fadeColor1, RGBAPixel fadeColor2, int radius,
        int tolerance, int frameFreq ) {
        gradientColorPicker c(fadeColor1, fadeColor2, radius, x, y);
    return filler::bfs::fill(img, x, y, c, tolerance, frameFreq);
}
 
animation filler::bfs::fill( PNG & img, int x, int y,
        colorPicker & fillColor, int tolerance, int frameFreq ) {
    return filler::fill<Queue>(img, x, y, fillColor, tolerance, frameFreq);
}
 
template <template <class T> class OrderingStructure>
animation filler::fill( PNG & img, int x, int y,
        colorPicker & fillColor, int tolerance, int frameFreq ) {
        cout << "Initializing" << endl;
        animation out;
        //out.addFrame(img);
    vector<RGBAPixel*> processed;
        vector<RGBAPixel*>::iterator it;
        it = processed.begin();
        OrderingStructure<RGBAPixel*> working;
        OrderingStructure<int> lx;
        OrderingStructure<int> ly;
        RGBAPixel* starting = img(x, y);
        working.add(img(x, y));
        lx.add(x);
        ly.add(y);
        int time = 0;
        bool skip = true;
 
        int sr = starting->red;
        int sg = starting->green;
        int sb = starting->blue;

        while(!working.isEmpty()) {
 
                RGBAPixel* top = working.remove();
                int dr = top->red;
                int dg = top->green;
                int db = top->blue;
 
                int cx = lx.remove();
                int cy = ly.remove();
                int tm = pow(dr - sr,2) + pow(dg - sg, 2) + pow(db - sb, 2);
                if(tm <= tolerance && std::find(processed.begin(), processed.end(), top) == processed.end()) {
                        RGBAPixel color = fillColor(cx, cy);
                        top->red = color.red;
                        top->green = color.green;
                        top->blue = color.blue;
 
                        it = processed.insert(it, top);
                        if(cx + 1 < img.width()) {
                                working.add(img(cx + 1, cy));
                                lx.add(cx + 1);
                                ly.add(cy);
                        }
                        if(cy + 1 < img.height()) {
                                working.add(img(cx, cy + 1));
                                lx.add(cx);
                                ly.add(cy + 1);
                        }
                        if(cx - 1 >= 0) {
                                working.add(img(cx - 1, cy));
                                lx.add(cx - 1);
                                ly.add(cy);
                        }
                        if(cy - 1 >= 0) {
                                working.add(img(cx, cy - 1));
                                lx.add(cx);
                                ly.add(cy - 1);
                        }
                        time++;
                        if(time % frameFreq == 0) {
                                out.addFrame(img);
 
                        }
                }
        }
 
    return out;
}