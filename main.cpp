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

Display *dis;
int screen;
Window win;
GC gc;

void init_x() {
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
    
    int height = 500;
    int width = 500;
    
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
    
    double x_zero = 250;
    double y_zero = 250;
    double x_scale = 50;
    double y_scale = 50;
    
    //draw x axis:
    XDrawLine(dis, win, gc, 0, y_zero, width-1, y_zero);
    
    //notches:
    for(int i = 0; i < width-1; i++){
        if (((int)x_zero - i) % (int)x_scale == 0){
            XDrawLine(dis, win, gc, i, y_zero, i, y_zero+5);
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
    double y_val_1 = 0;
    int y_coord_1 = 0;
    
    int pixel_seperation = 1; //1 pixel per calculation
    for (int pix_x = 0; pix_x < width-1; pix_x+=pixel_seperation){
        double x_val_1 = (pix_x-x_zero)/x_scale;
        double x_val_2 = ((pix_x+pixel_seperation)-x_zero)/x_scale;
        std::cout << "X val 1 is " << x_val_1 << std::endl;
        if (pix_x == 0) {
            y_val_1 = cos(x_val_1);
        }
        double y_val_2 = cos(x_val_2);
        if (pix_x == 0){
            y_coord_1 = round(-(y_scale*y_val_1)+y_zero);
        }
        int y_coord_2 = round(-(y_scale*y_val_2)+y_zero);
        XDrawLine(dis, win, gc, pix_x, y_coord_1, pix_x+pixel_seperation, y_coord_2);

        y_val_1 = y_val_2;
        y_coord_1 = y_coord_2;

    }


    XFlush(dis);
};


//see notes in Parser.hpp and Function.hpp

//set options:

void setOptions(){
    std::cout << "Enter a digit to toggle corresponding option.\n"
    << "True indicates that that option will be used. Press d when done.\n";
    while(true){
        std::cout << std::boolalpha;
        std::cout << "0: Prefix: " << Function::opts.prefix << '\n';
        std::cout << "1: Infix: " << Function::opts.infix << std::endl;
        std::cout << "2: Simplify: " << Function::opts.simplify << std::endl;
        std::cout << "3: Evaluate: " << Function::opts.evaluate << std::endl;
        std::string displayOption;
        getline(std::cin, displayOption);
        if (displayOption == "d"){
            break;
        } else if (displayOption == "0"){
            Function::opts.prefix = !Function::opts.prefix;
        } else if (displayOption == "1"){
            Function::opts.infix = !Function::opts.infix;
        } else if (displayOption == "2"){
            Function::opts.simplify = !Function::opts.simplify;
        } else if (displayOption == "3"){
            Function::opts.evaluate = !Function::opts.evaluate;
        } else {
            std::cout << "Please enter d, 0, or 1\n";
        }
    }
}

//return true if no errors, false if any errors caught

int main(int argc, const char * argv[]) {
    init_x();
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
        const Function* f(parse(myExpr));
        if (f == nullptr)
            continue;
        std::cout << "Function is: " << '\n';
        std::cout << *f;
        if (Function::opts.simplify){
            const Function* simpleF(f->simplify());
            std::cout << "Simplified function is: " << '\n';
            std::cout << *simpleF;
        }
        if (Function::opts.evaluate){
            std::cout << "Enter argument: " << std::endl;
            std::cin >> arg;
            std::cout << f->getName() << "(" << arg << ") = " << f->eval(arg) << '\n';
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



