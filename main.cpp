
#include <unistd.h>
#include "libuvc/libuvc.h"

#include <opencv2/highgui/highgui_c.h>

#include "ImageProcess.h"
#include "Racer.h"

ImageProcess *imageProcess=new ImageProcess();


Racer *racer=new Racer();



static void parseIMG(uint8_t *pVoid, uint32_t width, uint32_t height);

void cb(uvc_frame_t *frame, void *ptr) {
    uvc_frame_t *bgr;
    uvc_error_t ret;
    /* We'll convert the image from YUV/JPEG to BGR, so allocate space */
    bgr = uvc_allocate_frame(frame->width * frame->height * 3);
    if (!bgr) {
        printf("unable to allocate bgr frame!");
        return;
    }
    /* Do the BGR conversion */
    ret = uvc_any2bgr(frame, bgr);
    if (ret) {
        uvc_perror(ret, "uvc_any2bgr");
        uvc_free_frame(bgr);
        return;
    }

    parseIMG((uint8_t*)bgr->data,bgr->width, bgr->height);

    /* Call a user function:
     *
     * my_type *my_obj = (*my_type) ptr;
     * my_user_function(ptr, bgr);
     * my_other_function(ptr, bgr->data, bgr->width, bgr->height);
     */
    /* Call a C++ method:
     *
     * my_type *my_obj = (*my_type) ptr;
     * my_obj->my_func(bgr);
     */
    /* Use opencv.highgui to display the image:
     *
     * cvImg = cvCreateImageHeader(
     *     cvSize(bgr->width, bgr->height),
     *     IPL_DEPTH_8U,
     *     3);
     *
     * cvSetData(cvImg, bgr->data, bgr->width * 3);
     *
     * cvNamedWindow("Test", CV_WINDOW_AUTOSIZE);
     * cvShowImage("Test", cvImg);
     * cvWaitKey(10);
     *
     * cvReleaseImageHeader(&cvImg);
     */
    using namespace cv;
      auto cvImg = cvCreateImageHeader(
              cvSize(bgr->width, bgr->height),
              IPL_DEPTH_8U,
              3);

      cvSetData(cvImg, bgr->data, bgr->width * 3);

      cvNamedWindow("Test", CV_WINDOW_AUTOSIZE);
      cvShowImage("Test", cvImg);
      cvWaitKey(10);

      cvReleaseImageHeader(&cvImg);




    uvc_free_frame(bgr);
}

static void parseIMG(uint8_t *pVoid, uint32_t width, uint32_t height) {
    imageProcess->setImagePointer(pVoid);
    imageProcess->colorFinder(181, 66, 305, 322, 45,20,100);

    int averagex=0;
    int averagey=0;
    int counter=0;
    bool nothing=true;
    for(int x=181;x<181+305;x++){
        for(int y=(int)66;y<66+322;y++){
            int pixel = y*imageProcess->width+x;

            if(imageProcess->image[pixel*3+imageProcess->off_blue]==255 && imageProcess->image[pixel*3+imageProcess->off_green]==0 && imageProcess->image[pixel*3+imageProcess->off_red]==0){

                averagex+=x;
                averagey+=y;
                counter++;
                nothing=false;
            }


        }
    }
    if(nothing){
 //       racer->setSpeedForMilliseconds(-40, 5000);
//        racer->setControllForMilliseconds(30, 5000);

    }else{
        racer->setSpeed(40);
    }


}



int main(int argc, char *argv[]) {

    //config ImageProcess
    imageProcess->setImageSize(640,480);
    imageProcess->setColorFormat(COLORFORMAT_BGR);

    uvc_context_t *ctx;
    uvc_device_t *dev;
    uvc_device_handle_t *devh;
    uvc_stream_ctrl_t ctrl;
    uvc_error_t res;
    /* Initialize a UVC service context. Libuvc will set up its own libusb
     * context. Replace NULL with a libusb_context pointer to run libuvc
     * from an existing libusb context. */
    res = uvc_init(&ctx, NULL);
    if (res < 0) {
        uvc_perror(res, "uvc_init");
        return res;
    }
    puts("UVC initialized");
    /* Locates the first attached UVC device, stores in dev */
    res = uvc_find_device(
            ctx, &dev,
            0, 0, NULL); /* filter devices: vendor_id, product_id, "serial_num" */
    if (res < 0) {
        uvc_perror(res, "uvc_find_device"); /* no devices found */
    } else {
        puts("Device found");
        /* Try to open the device: requires exclusive access */
        res = uvc_open(dev, &devh);
        if (res < 0) {
            uvc_perror(res, "uvc_open"); /* unable to open device */
        } else {
            puts("Device opened");
            /* Print out a message containing all the information that libuvc
             * knows about the device */
            uvc_print_diag(devh, stderr);
            /* Try to negotiate a 640x480 30 fps YUYV stream profile */
            res = uvc_get_stream_ctrl_format_size(
                    devh, &ctrl, /* result stored in ctrl */
                    UVC_FRAME_FORMAT_YUYV, /* YUV 422, aka YUV 4:2:2. try _COMPRESSED */
                    640, 480, 30 /* width, height, fps */
            );
            /* Print out the result */
            uvc_print_stream_ctrl(&ctrl, stderr);
            if (res < 0) {
                uvc_perror(res, "get_mode"); /* device doesn't provide a matching stream */
            } else {
                /* Start the video stream. The library will call user function cb:
                 *   cb(frame, (void*) 12345)
                 */
                res = uvc_start_streaming(devh, &ctrl, cb, (void*) 12345, 0);
                if (res < 0) {
                    uvc_perror(res, "start_streaming"); /* unable to start stream */
                } else {
                    puts("Streaming...");
                    uvc_set_ae_mode(devh, 1); /* e.g., turn on auto exposure */



                    sleep(999); /* stream for 10 seconds */
                    /* End the stream. Blocks until last callback is serviced */
                    uvc_stop_streaming(devh);
                    puts("Done streaming.");
                }
            }
            /* Release our handle on the device */
            uvc_close(devh);
            puts("Device closed");
        }
        /* Release the device descriptor */
        uvc_unref_device(dev);
    }
    /* Close the UVC context. This closes and cleans up any existing device handles,
     * and it closes the libusb context if one was not provided. */
    uvc_exit(ctx);
    puts("UVC exited");
    return 0;


}
