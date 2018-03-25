//
//  Grapher.hpp
//  BabyWolf
//
//  Copyright © 2018 Umer. All rights reserved.
//

#ifndef Grapher_hpp
#define Grapher_hpp

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

struct GraphProperties{
    int height = 800;
    int width = 1000;
    
    int x_zero = 500;
    int y_zero = 400;
    
    double x_scale = 50;
    double y_scale = 50;
    
    int pixels_per_calculation = 1;
};


class Grapher {
    Display *dis;
    int screen;
    Window win;
    bool openWindow();
    bool drawAxis();
    bool drawGraph();
    bool labelAxis();
    
};

#endif /* Grapher_hpp */
