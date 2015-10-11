#include "ximeacamera.h"

XIMEACamera::XIMEACamera(int deviceNumber)
    :MCameraBase(deviceNumber)
{
    frame = NULL;
}

bool XIMEACamera::grabFrame(QImage *grabedFrame)
{
    //LOGFUNC("XIMEACamera::grabFrame")
    QMutexLocker locker(&mCameraLock);

    XI_RETURN	mvret;
    memset(&img, 0, sizeof(XI_IMG));

    img.size = sizeof(XI_IMG);
    //img.bp_size = 0;

    /*
     * Set timeout as twice the shutter time - convert in msec
     */
//    int image_timeout_ms = 2*this->shutter/1000;
    mvret = xiGetImage( hdev, 2000, &img );
    if(mvret != XI_OK)
        return false;

    if( img.frm == XI_MONO8){
        // Set the color table (used to translate colour indexes to qRgb values)

        // only create our color table once
        static QVector<QRgb> colorTable;
        if ( colorTable.isEmpty() ){
            LOG_D("XI_MONO8 camera format creating color table");
            for (int i=0; i<256; i++)
                colorTable.push_back(qRgb(i,i,i));
        }
        // Create QImage with same dimensions as input image
        QImage tmpImg( (uchar *)img.bp, width, height, width, QImage::Format_Indexed8);
        tmpImg.setColorTable(colorTable);
        (*grabedFrame) = tmpImg.copy();
    }
    else if( img.frm == XI_MONO16){
        LOG_C("Unsuported XI_MONO16 camera format");
        return false;
    }
    else if( img.frm == XI_RGB24){
        LOG_D("XI_RGB24 camera format");
        QImage tmpImg( (uchar *)img.bp, width, height, width*3, QImage::Format_RGB888);
        (*grabedFrame) = tmpImg.copy();
    }
    else if( img.frm == XI_RGB32){
        LOG_C("Unsuported XI_RGB32 camera format");
        return false;
    } else{
        LOG_C("Unknown camera format");
        return false;
    }

/*    if(frame == NULL)
    {
    switch( img.frm)
    {
    case XI_MONO8  : frame = cvCreateImage(cvSize( img.width, img.height), IPL_DEPTH_8U, 1); break;
    case XI_MONO16 : frame = cvCreateImage(cvSize( img.width, img.height), IPL_DEPTH_16U, 1); break;
    case XI_RGB24  : frame = cvCreateImage(cvSize( img.width, img.height), IPL_DEPTH_8U, 3); break;
    case XI_RGB32  : frame = cvCreateImage(cvSize( img.width, img.height), IPL_DEPTH_8U, 4); break;
    default :
        LOG_C("Unknown camera format");
        return false;
    }
    }

    // copy pixel data
    switch( img.frm )
    {
    case XI_MONO8  : memcpy( frame->imageData, img.bp, img.width*img.height); break;
    case XI_MONO16 : memcpy( frame->imageData, img.bp, img.width*img.height*sizeof(WORD)); break;
    case XI_RGB24  : memcpy( frame->imageData, img.bp, img.width*img.height*3); break;
    case XI_RGB32  : memcpy( frame->imageData, img.bp, img.width*img.height*sizeof(DWORD)); break;
    default: break;
    }
*/
    //memcpy( frame->imageData, img.bp, img.width*img.height*3);
//    frame->nSize=112;
//    frame->ID=0;
//    frame->nChannels=3;
//    frame->alphaChannel=0;
//    frame->depth=8;
//    frame->dataOrder=0;
//    frame->origin=0;
//    frame->align=4;
//    frame->width=img.width;
//    frame->height=img.height;
//    frame->roi=NULL;
//    frame->maskROI=NULL;
//    frame->imageId=NULL;
//    frame->tileInfo=NULL;
//    frame->imageSize=3*img.width*img.height;
//    frame->imageData=(char*)img.bp;  //the pointer to imagaData
//    frame->widthStep=3*img.width;
//    frame->imageDataOrigin=(char*)img.bp; //and again

//    *grabbedFrame = cv::cvarrToMat(frame, false);

    return true;
}

bool XIMEACamera::connectToCamera()
{
    LOGFUNC("XIMEACamera::connectToCamera");
    QMutexLocker locker(&mCameraLock);

    // Open camera
    if(xiOpenDevice( deviceNumber, &hdev ) != XI_OK )
    {
        LOG_C("Error opening ximea camera");
        return false;
    }

    //Configure camera and start aquisition
    XI_RETURN stat = XI_OK;

    char serial_number1[100]="";
    xiGetParamString(hdev, XI_PRM_DEVICE_SN, serial_number1, sizeof(serial_number1));

    stat = xiStartAcquisition(hdev);

    int isColor = 0;
    stat = xiGetParamInt(hdev, XI_PRM_IMAGE_IS_COLOR, &isColor);
    if(isColor) // for color cameras
    {
        // default image format RGB24
        LOG_D("Ximea color camera found");
        stat = xiSetParamInt( hdev, XI_PRM_IMAGE_DATA_FORMAT, XI_RGB24);
        stat = xiSetParamInt( hdev, XI_PRM_AUTO_WB, 1);
    }
    else{
        LOG_D("Ximea grey camera");
    }

    return stat == XI_OK;
}

int XIMEACamera::Initialize()
{
    QMutexLocker locker(&mCameraLock);

    float ming, maxg, mine, maxe;
    xiGetParamFloat( hdev, XI_PRM_GAIN XI_PRM_INFO_MIN, &ming);
    xiGetParamFloat( hdev, XI_PRM_GAIN XI_PRM_INFO_MAX, &maxg);

    xiGetParamFloat( hdev, XI_PRM_AE_MAX_LIMIT XI_PRM_INFO_MIN, &mine);
    xiGetParamFloat( hdev, XI_PRM_AE_MAX_LIMIT XI_PRM_INFO_MAX, &maxe);

    minGain = ming; maxGain = maxg; minShutter = mine; maxShutter = maxe;

    if(minShutter<1) minShutter = 1;
    if(maxShutter>1000000) maxShutter = 1000000;

    // set default parameters
    xiSetParamInt( hdev, XI_PRM_EXPOSURE, shutter);
    xiSetParamFloat(hdev, XI_PRM_GAIN, (float) gain );

    this->width = getInputSourceWidth();
    this->height = getInputSourceHeight();

    return XI_OK;
}

bool XIMEACamera::disconnectCamera()
{
    LOGFUNC("XIMEACamera::disconnectCamera")
    QMutexLocker locker(&mCameraLock);
    // Camera is connected
    if( hdev != INVALID_HANDLE_VALUE )
    {
        // Disconnect camera
        xiStopAcquisition( hdev );
        xiCloseDevice( hdev );
        return true;
    }
    // Camera is NOT connected
    else{
        LOG_C("Error disconecting ximea camera");
        return false;
    }
}

bool XIMEACamera::isCameraConnected()
{
    QMutexLocker locker(&mCameraLock);
    return hdev != INVALID_HANDLE_VALUE;
}

int XIMEACamera::getInputSourceWidth()
{
    int width;
    xiGetParamInt( hdev, XI_PRM_WIDTH, &width);
    return width;
}

int XIMEACamera::getInputSourceHeight()
{
    int height;
    xiGetParamInt( hdev, XI_PRM_HEIGHT, &height);
    return height;
}

double XIMEACamera::getShutter()
{
    return shutter;
}

void XIMEACamera::setShutter(double newShutter)
{
    QMutexLocker locker(&mCameraLock);
    if(shutter != newShutter){
        int expoTime = newShutter;
        int ret = xiSetParam( hdev, XI_PRM_EXPOSURE, &expoTime, sizeof(expoTime), xiTypeInteger);
        if(ret == XI_OK){
            shutter = newShutter;
        }
        else{
            LOG_C("Error setting Ximea shutter %f", newShutter);
        }
    }
}

double XIMEACamera::getGain()
{
    return gain;
}

void XIMEACamera::setGain(double newGain)
{
    QMutexLocker locker(&mCameraLock);
    if(gain != newGain){
        int ret = xiSetParamFloat(hdev, XI_PRM_GAIN, (float) newGain );
        if(ret == XI_OK){
            gain = newGain;
        }
        else{
            LOG_C("Error setting Ximea gain %f", newGain);
        }
    }
}
