//
// Created by tim on 05.02.22.
//

#include "ImageProcess.h"

void ImageProcess::setImagePointer(uint8_t *imagepointer) {
    this->image = imagepointer;
}

void ImageProcess::setImageSize(int w, int h) {
    this->width = w;
    this->height = h;
}

void ImageProcess::setColorFormat(uint8_t cf) {
    this->colorformat = cf;
    switch (cf) {
        case COLORFORMAT_RGB:
            this->off_red=0;
            this->off_green=1;
            this->off_green=2;
            break;
        case COLORFORMAT_BGR:
            this->off_red=2;
            this->off_green=1;
            this->off_blue=0;
            break;
    }
}

uint8_t ImageProcess::getRed(int x, int y) {
    return this->image[(y*width+x)*3+this->off_red];
}

uint8_t ImageProcess::getGreen(int x, int y) {
    return this->image[(y*width+x)*3+this->off_green];
}

uint8_t ImageProcess::getBlue(int x, int y) {
    return this->image[(y*width+x)*3+this->off_blue];
}

void ImageProcess::colorFinder(int fenster_x, int fenster_y,int width,int height,int farbkontrollerot,int farbkontrollegruen,int farbkontrolleblau) {
    int rd=0;
    int gd=0;
    int bd=0;
    for(int x=fenster_x;x<(fenster_x+width);x=x+1) {
        for (int y = fenster_y; y < (fenster_y+height); y = y + 1) {


            int pixel = y*this->width+x;
            rd=this->image[pixel*3+this->off_red];
            gd=this->image[pixel*3+this->off_green];
            bd=this->image[pixel*3+this->off_blue];





            if(rd > farbkontrollerot+gd&& rd>farbkontrollerot+bd ){
                //BB.setpixelcolor(x/abstand, y/abstand, 255,0,0);
                image[pixel*3+this->off_blue]=255;
                image[pixel*3+this->off_red]=0;
                image[pixel*3+this->off_green]=0;
            }else
            if(gd > farbkontrollegruen+rd&& gd>farbkontrollegruen+bd ){
                image[pixel*3+this->off_green]=255;
                image[pixel*3+this->off_red]=0;
                image[pixel*3+this->off_blue]=0;
            }else
            if(bd > farbkontrolleblau+gd&& bd>farbkontrolleblau+rd ){
                //image[pixel*3+this->off_blue]=255;
            }else {


                //int rgb = rd;
                //rgb = (rgb << 8) + gd;
                //rgb = (rgb << 8) + bd;
                //image.setRGB(x,y,rgb);
            }
            /*lu if( rd<100) {
                 image.setRGB(x, y, 16711680);
             }*/


        }
    }
}

ImageProcess::ImageProcess() {

}
