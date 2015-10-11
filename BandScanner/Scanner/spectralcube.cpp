#include "spectralcube.h"
#include <QFileDialog>
#include <QDebug>
#include <QtCore>
#include <QtXml>

SpectralCube::SpectralCube()
{
    clearCubeData();
}

SpectralCube::~SpectralCube()
{
    clearCubeData();
}

bool SpectralCube::loadCube()
{
    //Ask for input directory
    QString inPath = QFileDialog::getExistingDirectory(0,"Please select cube folder", QDir::currentPath());

    if(inPath.isEmpty()) return false;
    /*
     * set index at invalid possition untill cube is loaded
     */
    mCubeImageCurrentIndex = -1;

    clearCubeData();
    /*
     * Scan directory for all images
     */

    //Regular expression looks for 3 numbers
    QRegularExpression re("\\d\\d\\d");

    //Iterate though all files in the selected path
    QDirIterator dirIt(inPath, QDirIterator::NoIteratorFlags);
    while (dirIt.hasNext()) {
        dirIt.next();
        QString filepath = dirIt.filePath();
        if (!QFileInfo(filepath).isFile()) continue;
        if (!(QFileInfo(filepath).suffix() == "png" || QFileInfo(filepath).suffix() == "bmp") ) continue;

        QString filename = dirIt.fileName();
        QRegularExpressionMatch match = re.match(filename);
        if (match.hasMatch()){
            qDebug("Loading %s", filepath.toLocal8Bit().data());
            int band = match.captured(0).toInt();
            QImage im(filepath);
            mSpectralImages.append(spectralImage(band, im));
        }
    }
    qSort(mSpectralImages);
    return true;
}

bool SpectralCube::saveCube()
{
    QFileDialog dialog(NULL);
    QString path = dialog.getExistingDirectory(NULL,
                                            "Please select save folder");
//                        QDir::homePath());


    if(path.isEmpty()) return false;

    qSort(mSpectralImages);

    QDateTime local(QDateTime::currentDateTime());
    QDomDocument document;
    QDomElement root = document.createElement("Spectral Cube " + local.toString());
    document.appendChild(root);
    QDomElement myCube;

    if(photometry() == -1) mCubeType = STEREOSCOPY;
    else mCubeType = PHOTOMETRY;

    if(mCubeType == PHOTOMETRY)
    {
        myCube = document.createElement("Photometry");
        myCube.setAttribute("Status ","YES");
        root.appendChild(myCube);
    }

    if(mCubeType == STEREOSCOPY)
    {
        myCube = document.createElement("Photometry");
        myCube.setAttribute("Status ","NO");
        root.appendChild(myCube);
    }

    int temp=0;
//    for(int band=mBandStart; band<mBandEnd; band += step)
//    {
//        temp++;
//        QString savePathName = QString("%1/image%2.png").arg(path).arg(band);
//        bool saved = spectralImages().at(temp).image.save(savePathName);
//        if (!saved) return false;
//        QDomElement img = document.createElement("Spectra Image");
//        img.setAttribute("No", QString::number(temp));
//        img.setAttribute("Band", QString::number(band) + "nm");
//        myCube.appendChild(img);
//    }
    foreach(spectralImage val, mSpectralImages)
        {
        temp ++;
            QString savePathName = QString("%1/image%2.png").arg(path).arg(val.band);
            bool saved = val.image.save(savePathName);
            if (!saved) return false;
            QDomElement img = document.createElement("Spectra Image");
            img.setAttribute("No", QString::number(temp));
            img.setAttribute("Band", QString::number(val.band) + "nm");
            myCube.appendChild(img);
        }

    QFile file(path + "cubeXML.xml");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug("Failed to open .xml file at path %s", qPrintable(path));
    } else {
        QTextStream stream(&file);
        stream << document.toString();
        file.close();
        qDebug("Success .xml writing  at %s ...", qPrintable(path));
    }

    //HD:Why return false?
    return false;
}

/**
 * @brief SpectralCube::clearCubeData Clears the cube from images and resets parameters
 */
void SpectralCube::clearCubeData()
{
//    for(int i = 0; i < spectralImages().size(); i++)
//    {
//        delete spectralImages()[i];
//    }

    spectralImages().clear();
    mCubeType = UNDEFINED;
    mBandStart = -1;
    mBandEnd = -1;
}

/**
 * @brief SpectralCube::addSpectralImage adds a new spectral image to the cube given the image and the band.
 * Also keeps track of the min and max bands
 * @param image
 * @param band
 */
void SpectralCube::addSpectralImage(QImage image, int band)
{
    if(mBandStart > band || mBandStart == -1){
        mBandStart = band;
    }
    if(mBandEnd < band || mBandEnd == -1){
        mBandEnd = band;
    }
    mSpectralImages.append( spectralImage(band, image) );
}


int SpectralCube::photometry() const
{
    return mPhotometry;
}

void SpectralCube::setPhotometry(int photometry)
{
    mPhotometry = photometry;
}
CubeType SpectralCube::cubeType() const
{
    return mCubeType;
}

void SpectralCube::setCubeType(CubeType cubeType)
{
    mCubeType = cubeType;
}
QList<spectralImage> SpectralCube::spectralImages() const
{
    return mSpectralImages;
}
