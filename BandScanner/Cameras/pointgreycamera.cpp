
//=============================================================================
// This class applies Point Grey's Flea3 USB 3.0 API to BandScanner software.
//
// ****************** On Qt Framework implementation ************************
// a. Point Grey's API is compatible only with specific compilers
// b. Also Qt 5.0+ is compatible with specific compilers
// c. The only possible compiler configuration to make the Pg API works is
//    with MSVC 2010 (Tools->Options->Compilers).
// d. Anything else result "linker error" problems.
//
// Sources : a. http://www.ptgrey.com/KB/10119
//           b. http://doc.qt.io/qt-5/supported-platforms.html
//***************************************************************************
//=============================================================================

#include "pointgreycamera.h"

pointgreycamera::pointgreycamera(int deviceNumber)
    :MCameraBase(deviceNumber)
{
    frame = NULL;
}

bool pointgreycamera::grabFrame(QImage *grabedFrame)
{

    QMutexLocker locker(&mCameraLock);
    Image rawImage;//The Image class is used to retrieve images from a camera and proccessing them
    //rawImage.SetColorProcessing(NEAREST_NEIGHBOR);
    error = cam.RetrieveBuffer( &rawImage );//Input a single frame image
    if (error != PGRERROR_OK)
    {
        error.PrintErrorTrace();
        return false;
    }

//    Image newImg;
//    error = rawImage.Convert( PIXEL_FORMAT_RAW8, &newImg );
//    if (error != PGRERROR_OK)
//    {
//        error.PrintErrorTrace();
//        return false;
//    }



/* The following code applies on Image format
 * to get the actual values from image array
 * with image.getData() and put them in a
 * QImage object that current aplication can
 * proccess. Its a way to "type cast" from PG's
 * API Image object to QImage object on Qt API.
 */


//    static QVector<QRgb> colorTable;
//    if ( colorTable.isEmpty() ){
//        for (int i=0; i<256; i++)
//            colorTable.push_back(qRgb(i,i,i));
//    }
    // Create QImage with same dimensions as input image
    QImage tmpImg( (uchar *)rawImage.GetData(), width, height, width*3, QImage::Format_RGB888);
    //tmpImg.setColorTable(colorTable);
    (*grabedFrame) = tmpImg.copy();

    return true;
}

/* Point Grey Camera connection sequence
 * This routine works with only one camera
*/
bool pointgreycamera :: connectToCamera()
{
    QMutexLocker locker(&mCameraLock);

    error = busMgr.GetCameraFromIndex(0, &guid);//'0' is first Camera input
    if (error != PGRERROR_OK)
    {
        error.PrintErrorTrace();
        return false;
    }

    error = cam.Connect(&guid);
    if (error != PGRERROR_OK)
    {
        error.PrintErrorTrace();
        return false;
    }

    return true;
}

/*
 * On Initialize() we set default parameters
 * for Pg camera characteristics.
*/
int pointgreycamera::Initialize()
{

    QMutexLocker locker(&mCameraLock);

    minGain = 0; maxGain = 24.082; minShutter = 0; //maxShutter = 47.574;
    maxShutter = 37; // For res. 1600x1200 and Frame Rate ~21

    if(minShutter<1) minShutter = 1;
    if(maxShutter>1000000) maxShutter = 1000000;
    bool valid;
    fmt7Info.mode = k_fmt7Mode;
    Format7PacketInfo fmt7PacketInfo;
    Format7ImageSettings fmt7ImageSettings;
    fmt7ImageSettings.mode = k_fmt7Mode;
    fmt7ImageSettings.offsetX = 0;
    fmt7ImageSettings.offsetY = 0;
//    fmt7ImageSettings.width = 4096;
//    fmt7ImageSettings.height = 2160;
//    fmt7ImageSettings.width = 1920;
//    fmt7ImageSettings.height = 1080;
    fmt7ImageSettings.width = 1600;
    fmt7ImageSettings.height = 1200;
    fmt7ImageSettings.pixelFormat = k_fmt7PixFmt;

    error = cam.ValidateFormat7Settings(&fmt7ImageSettings,&valid,&fmt7PacketInfo );
        if (error != PGRERROR_OK)
        {
            error.PrintErrorTrace();
            return false;
        }
   // fmt7PacketInfo.maxBytesPerPacket = 20280;
    error = cam.SetFormat7Configuration(&fmt7ImageSettings,fmt7PacketInfo.recommendedBytesPerPacket);
    if (error != PGRERROR_OK)
    {
        error.PrintErrorTrace();
        return false;
    }


//    GigEImageSettingsInfo imageSettingsInfo;
//    error = cam.GetGigEImageSettingsInfo( &imageSettingsInfo );
//    if (error != PGRERROR_OK)
//    {
//        error.PrintErrorTrace();
//        return false;
//    }
//    GigEImageSettings imageSettings;
//    imageSettings.offsetX = 0;
//    imageSettings.offsetY = 0;
//    imageSettings.height = 4096;
//    imageSettings.width = 2160;
//    imageSettings.pixelFormat = PIXEL_FORMAT_RGB;

//    error = cam.SetGigEImageSettings( &imageSettings );
//    if (error != PGRERROR_OK)
//    {
//        error.PrintErrorTrace();
//        return false;
//    }

    error = cam.StartCapture();
    if (error != PGRERROR_OK)
    {
        error.PrintErrorTrace();
        return false;
    }

    //setting SHUTTER @ ms | PointGrey range 0-47,574ms
    shutter.type = SHUTTER;
    shutter.onOff = true;
    shutter.autoManualMode = false;
    shutter.absControl = true;
    shutter.absValue = 40;
    error = cam.SetProperty(&shutter);
    if (error != PGRERROR_OK)
    {
        error.PrintErrorTrace();
    }

    // setting GAIN @ db | PointGrey range 0-24db
    gain.type = GAIN;
    gain.onOff = true;
    gain.autoManualMode = false;
    gain.absControl = true;
    gain.absValue = 2;
    error = cam.SetProperty(&gain);
    if (error != PGRERROR_OK)
    {
        error.PrintErrorTrace();
    }

    width = getInputSourceWidth();
    height = getInputSourceHeight();

    return 1;
}

bool pointgreycamera::disconnectCamera()
{
    QMutexLocker locker(&mCameraLock);

    error = cam.StopCapture();
    if (error != PGRERROR_OK)
    {
        error.PrintErrorTrace();
        return false;
    }
    error = cam.Disconnect();
    if (error != PGRERROR_OK)
    {
        error.PrintErrorTrace();
        return false;
    }else return true;
}

bool pointgreycamera::isCameraConnected()
{
    QMutexLocker locker(&mCameraLock);

    error = cam.Connect(&guid);
    if (error != PGRERROR_OK)
    {
        error.PrintErrorTrace();
        return false;
    }else return true;
}

int pointgreycamera::getInputSourceHeight()
{
//    return 2160;
    return 1200;
}

int pointgreycamera::getInputSourceWidth()
{
    return 1600;
//    return 4096;
}

double pointgreycamera::getShutter()
{
    QMutexLocker locker(&mCameraLock);

    Property prop;
    prop.type = SHUTTER;
    error = cam.GetProperty( &prop );
    if (error != PGRERROR_OK)
    {
        error.PrintErrorTrace();
        return false;
    }
    return (double)prop.absValue*1000;
}

void pointgreycamera::setShutter(double newshutter)
{
    QMutexLocker locker(&mCameraLock);

    Property prop;
    prop.type = SHUTTER;
    error = cam.GetProperty( &prop );
    if (error != PGRERROR_OK)
    {
        error.PrintErrorTrace();
    }
    prop.onOff = true;
    prop.autoManualMode = false;
    prop.absControl = true;
    prop.absValue = (float)newshutter/1000;//SHUTTER_DISPLAY_DIVIDER  1000 @ definitions.h
    error = cam.SetProperty(&prop);
    if (error != PGRERROR_OK)
    {
        error.PrintErrorTrace();
    }
}

double pointgreycamera::getGain()
{
    QMutexLocker locker(&mCameraLock);

    Property prop;
    prop.type = GAIN;
    error = cam.GetProperty( &prop );
    if (error != PGRERROR_OK)
    {
        error.PrintErrorTrace();
        return false;
    }
    return (double)prop.absValue;
}

void pointgreycamera::setGain(double newgain)
{
    QMutexLocker locker(&mCameraLock);

    Property prop;
    prop.type = GAIN;
    if (error != PGRERROR_OK)
    {
        error.PrintErrorTrace();
    }
    prop.onOff = true;
    prop.autoManualMode = false;
    prop.absControl = true;
    prop.absValue = (float)newgain;
    error = cam.SetProperty(&prop);
    if (error != PGRERROR_OK)
    {
        error.PrintErrorTrace();
    }
}
