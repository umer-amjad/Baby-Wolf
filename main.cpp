//
//  main.cpp
//  BabyWolf
//
//
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include "Parser.hpp"
#include "Tests.hpp"

void init_x(const Function f) {
    Display *dis;
    int screen;
    Window win;
    GC gc;
    /* get the colors black and white (see section for details) */
    unsigned long black,white;
    
    /* use the information from the environment variable DISPLAY
     to create the X connection:
     */
    dis=XOpenDisplay("");
    screen=DefaultScreen(dis);
    black=BlackPixel(dis,screen);    /* get color black */
    white=WhitePixel(dis, screen);  /* get color white */
    
    /* once the display is initialized, create the window.
     This window will be have be 200 pixels across and 300 down.
     It will have the foreground white and background black
     */
    
    int height = 800;
    int width = 1000;
    
    double x_zero = 500;
    double y_zero = 400;
    double x_scale = 50;
    double y_scale = 50;
    
    win=XCreateSimpleWindow(dis,DefaultRootWindow(dis),0,0,
                            width, height, 5, white, white);
    
    
    /* here is where some properties of the window can be set.
     The third and fourth items indicate the name which appears
     at the top of the window and the name of the minimized window
     respectively.
     */
    XSetStandardProperties(dis,win,"this is the graph, my dude","HI!",None,NULL,0,NULL);
    
    XSelectInput(dis, win, StructureNotifyMask);
    
    
    XMapWindow(dis, win);

    
    /* create the Graphics Context */
    gc=XCreateGC(dis, win, 0,0);
    
    /* here is another routine to set the foreground and background
     colors _currently_ in use in the window.
     */
    for(;;) {
        XEvent e;
        XNextEvent(dis, &e);
        if (e.type == MapNotify)
            break;
    }
    
    XSetForeground(dis,gc,black);
    
    //draw x axis:
    XDrawLine(dis, win, gc, 0, y_zero, width-1, y_zero);
    
    //notches:
    for(int i = 0; i < width-1; i++){
        if (((int)x_zero - i) % (int)x_scale == 0){
            XDrawLine(dis, win, gc, i, y_zero, i, y_zero+5);
            int x_val = (i-x_zero)/x_scale;
            char str[30];
            int len = sprintf(str,"%d",x_val);
            //XDrawString(dis, win, gc, i, y_zero+20, str, len);
        } else if ((((int)x_zero - i) % ((int)x_scale/10)) == 0){
            XDrawLine(dis, win, gc, i, y_zero, i, y_zero+2);
        }
    }
    
    //draw y axis:
    
    XDrawLine(dis, win, gc, x_zero, 0, x_zero, height-1);
    //notches:
    for(int i = 0; i < height-1; i++){
        if (((int)y_zero - i) % (int)y_scale == 0){
            XDrawLine(dis, win, gc, x_zero-5, i, x_zero, i);
        } else if ((((int)y_zero - i) % ((int)y_scale/10)) == 0){
            XDrawLine(dis, win, gc, x_zero-2, i, x_zero, i);
        }
    }
    
    XSetForeground(dis,gc,45568);//green

    //draw graph
    
    int pixel_separation = 1; //1 pixel per calculation
    int i = 0;
    std::vector<XPoint> points; //vector of points to draw lines between
    //you need to go out of bounds:
    for (int x_coord = 0; x_coord < width + pixel_separation; x_coord+=pixel_separation){
        double x_val = (x_coord-x_zero)/x_scale;
        //std::cout << "X val is " << x_val << std::endl;
        double y_val = f.evaluate(x_val);
        //std::cout << "Y val is " << y_val << std::endl;
        int y_coord = round(-(y_scale*y_val)+y_zero);
        points.push_back({(short)x_coord, (short)y_coord});
        i++;
    }
    XDrawLines(dis, win, gc, points.data(), i, CoordModeOrigin);

    //std::cout << "Num points " << i << std::endl;
    
    XFlush(dis);
    
//    //testing animation....
//    int test = 0;
//    while (true){
//        if (test == width){
//            test = 0;
//        }
//        XSetForeground(dis,gc,black);
//        for (int i = 0; i < 30; i++){
//            XDrawPoint(dis, win, gc, test+i, 7);
//            XDrawPoint(dis, win, gc, test+i, 8);
//        }
//        XSetForeground(dis,gc,white);
//        for (int i = 0; i < 30; i++){
//            XDrawPoint(dis, win, gc, test-i, 7);
//            XDrawPoint(dis, win, gc, test-i, 8);
//        }
//        usleep(3000);
//        XFlush(dis);
//        ++test;
//    }
};


void CloseWindow(Display* dis, Window win, GC gc){
    
    
    XFreeGC(dis, gc);
    XDestroyWindow(dis,win);
    XCloseDisplay(dis);
}

//see notes in Parser.hpp and Function.hpp

//set options:

void setOptions(){
    std::cout << "Enter a digit to change corresponding option.\n";
    std::cout << "Press d when done.\n";
    while(true){
        std::cout << std::boolalpha;
        std::cout << "0: Prefix: " << AbstractFunction::opts.prefix << '\n';
        std::cout << "1: Infix: " << AbstractFunction::opts.infix << std::endl;
        std::cout << "2: Simplify: " << AbstractFunction::opts.simplify << std::endl;
        std::cout << "3: Evaluate: " << AbstractFunction::opts.evaluate << std::endl;
        std::cout << "4: Log base: " ;
        (AbstractFunction::opts.base == M_E) ? (std::cout << "e") : (std::cout << AbstractFunction::opts.base);
        std::cout << std::endl;
        std::string displayOption;
        getline(std::cin, displayOption);
        if (displayOption == "d"){
            break;
        } else if (displayOption == "0"){
            AbstractFunction::opts.prefix = !AbstractFunction::opts.prefix;
        } else if (displayOption == "1"){
            AbstractFunction::opts.infix = !AbstractFunction::opts.infix;
        } else if (displayOption == "2"){
            AbstractFunction::opts.simplify = !AbstractFunction::opts.simplify;
        } else if (displayOption == "3"){
            AbstractFunction::opts.evaluate = !AbstractFunction::opts.evaluate;
        } else if (displayOption == "4"){
            std::cout << "Enter the base to change to." << std::endl;
            std::string newLogBase;
            getline(std::cin, newLogBase);
            try {
                if (newLogBase == "e"){
                    AbstractFunction::opts.base = M_E;
                } else {
                    double newBase = std::stod(newLogBase);
                    if (newBase == 1 || newBase < 0){
                        throw std::invalid_argument("");
                    }
                    AbstractFunction::opts.base =  newBase;
                }
            } catch (const std::exception& e){
                std::cout << "Log base entered must be a positive real number not equal to one, or you can enter \"e\". The base's value remains " << AbstractFunction::opts.base << std::endl;
            }
            AbstractFunction::opts.evaluate = !AbstractFunction::opts.evaluate;
        } else {
            std::cout << "Please enter d, or an integer between 0 and 4 inclusive\n";
        }
    }
}

//return true if no errors, false if any errors caught

int main(int argc, const char * argv[]) {
    if (testAll()){
        std::cout << "All tests passed!" << std::endl;
    }
    while (true) {
        double arg = 0;
        std::string myExpr;
        std::cout << "Enter function, q to quit, or o for options: " << std::endl;
        getline(std::cin, myExpr);
        if (myExpr == "q"){
            break;
        } else if (myExpr == "o"){
            setOptions();
            continue;
        }
        const Function f(parse(myExpr));
        if (f.isNull())
            continue;
        std::cout << "Function is: " << '\n';
        std::cout << f;
        std::cout << "Derivative is: " << '\n';
        //std::cout << f.simplify().derivative().simplify();
        //init_x(f);
        if (AbstractFunction::opts.simplify){
            std::cout << "Simplified function is: " << '\n';
            const Function simpleF = f.simplify();
            std::cout << simpleF;
        }
        if (AbstractFunction::opts.evaluate){
            std::cout << "Enter argument: " << std::endl;
            std::cin >> arg;
            std::cout << f.getName() << "(" << arg << ") = " << f.evaluate(arg) << '\n';
            std::cin.ignore();
        }
    }
}

//test cases:
//1-2-3
//2^3^4
//2x^2+19x+2^(1+x/2)
//x^2 + 19x + 7 -1/2
// (2+5) + 6*(x^2 + 3^(45+x))
//(|x+3|)(1+5)|x+(2+9|3-x|)*(x|x-2|)|
//(3 + (5 * (9 + (x + (3 * ((x ^ 2) * |(x + (5 * (13 + (|(x + 5)| * |(x - 5)| * (5 + (2 * |((x ^ 2) + 3)|))))) + (6 * (13 + (|(x + 5)| * |(x - 5)| * (5 + (2 * |((x ^ 2) + 3)|))))))|))))))
//1-2-x-3-(1+2-3-x-4-(7/5x/7/8))
//(x*1)+(x^2*2)+(x^2*1)+3x^2+4
//6+7+(8+(9-(-x)))
//x^(3+(7-5))^(x+2+3+4/2)^(-(-x))
//sinx+5
//cosx
//sin(x+5)
//acoshx
//acsc(x+3)
//asech(x^2+5x+3)



