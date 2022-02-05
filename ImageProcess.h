//
// Created by tim on 05.02.22.
//

#ifndef OBJECTTRACKING_IMAGEPROCESS_H
#define OBJECTTRACKING_IMAGEPROCESS_H

#define COLORFORMAT_RGB 0
#define COLORFORMAT_BGR 1
#define COLORFORMAT_RGBA 2

#include <unistd.h>
#include <cstdint>



class ImageProcess {
public:
    ImageProcess();
    void setImagePointer(uint8_t *image);
    void setImageSize(int width, int height);
    void setColorFormat(uint8_t cf);
    void colorFinder(int fenster_x, int fenster_y,int width,int height,int farbkontrollerot,int farbkontrollegruen,int farbkontrolleblau);
    uint8_t getRed(int x, int y);
    uint8_t getGreen(int x, int y);
    uint8_t getBlue(int x, int y);
    uint8_t *image;
    int width;
    int height;
    uint8_t off_red=2;
    uint8_t off_green=1;
    uint8_t off_blue=0;

private:




    uint8_t colorformat=COLORFORMAT_RGB;
};


#endif //OBJECTTRACKING_IMAGEPROCESS_H
