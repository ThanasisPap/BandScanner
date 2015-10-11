#include "spectralcube.h"
#include <QFileDialog>
#include <QDebug>
#include <QtCore>
#include <QtXml>
#include "LogDefs.h"

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
    QDomDocument doc;
    QString inPath = QFileDialog::getOpenFileName(0,"Open Spectral Cube's .xml file", QDir::currentPath(), "XML files (*.xml)");

    if(inPath.isEmpty()) return false;

    QFile file(inPath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        LOG_E("Failed to open file");
        return false;
    }
    else
    {
        if(!doc.setContent(&file))
        {
            LOG_E("Failed to copy content");
            return false;
        }
        file.close();
    }

    QDomElement root = doc.documentElement();
    QDomNodeList photItems = root.elementsByTagName("Photometry");
    QDomNodeList imgItems = root.elementsByTagName("Image");

    QDomNode photnode = photItems.at(0);
    QDomElement photAtt = photnode.toElement();
    QString condition = photAtt.attribute("Condition");
    QString value = photAtt.attribute("Value");

    if(condition.contains("NO")) mCubeType = WITHOUT_PHOTOMETRY;
    if(condition.contains("YES")) mCubeType = PHOTOMETRY;

    mPhotometry = value.toInt();
    mSpectralImages.clear();

    for(int i=0; i < imgItems.count(); i++)
    {
        QDomNode imgnode = imgItems.at(i);

        if(imgnode.isElement())
        {
            QDomElement imgAtt = imgnode.toElement();
            QString imgpath = imgAtt.attribute("SavePath");
            QString imgband = imgAtt.attribute("Band");
            QImage img(imgpath);
            mSpectralImages.append( spectralImage(imgband.toInt(), img));
        }
    }

    //Iterate though all files in the selected path
//    QDirIterator dirIt(inPath, QDirIterator::NoIteratorFlags);
//    while (dirIt.hasNext()) {
//        dirIt.next();
//        QString filepath = dirIt.filePath();
//        if (!QFileInfo(filepath).isFile()) continue;
//        if (!(QFileInfo(filepath).suffix() == "png" || QFileInfo(filepath).suffix() == "bmp") ) continue;

//        QString filename = dirIt.fileName();
//        QRegularExpressionMatch match = re.match(filename);
//        if (match.hasMatch()){
//            qDebug("Loading %s", filepath.toLocal8Bit().data());
//            int band = match.captured(0).toInt();
//            QImage im(filepath);
//            //Display images

//            mSpectralImages.append(spectralImage(band, im));

//            QEventLoop loop;
//            QTimer::singleShot(10, &loop, SLOT(quit()));
//            loop.exec();
//        }
//    }
    qSort(mSpectralImages);
    return true;
}

bool SpectralCube::saveCube()
{
    qSort(mSpectralImages);

    QFileDialog dialog(NULL);
    QString path = dialog.getSaveFileName(NULL,
                                            "Save Spectral Cube",
                                            "XML files (*.xml)");

    if(path.isEmpty()) return false;

    QDir().mkdir(path);
    QString filename = QDir(path).dirName();
    QString cubePath = path + "/Spectral_Cube";
    QString dataPath = path + "/Data";
    QString imagesPath = path + "/Images";
    QDir().mkdir(cubePath);
    QDir().mkdir(dataPath);
    QDir().mkdir(imagesPath);

    QDateTime local(QDateTime::currentDateTime());
    QDomDocument document;
    QDomProcessingInstruction instr = document.createProcessingInstruction("xml", "version='1.0' encoding='UTF-8'");
    document.appendChild(instr);
    QDomElement root = document.createElement("Spectral_Cube");
    root.setAttribute("Name", filename);
    root.setAttribute("Capture-Time", local.toString());
    document.appendChild(root);
    QDomElement myCube;


    if(mCubeType == PHOTOMETRY)
    {
        myCube = document.createElement("Photometry");
        myCube.setAttribute("Value ", QString::number(mPhotometry));
        myCube.setAttribute("Condition", "YES");
        root.appendChild(myCube);
    }

    if(mCubeType == WITHOUT_PHOTOMETRY)
    {
        myCube = document.createElement("Photometry");
        myCube.setAttribute("Value ", QString::number(mPhotometry));
        myCube.setAttribute("Condition", "NO");
        root.appendChild(myCube);
    }

    int temp = 0;
    foreach(spectralImage val, mSpectralImages)
    {
        temp++;
        QString savePathName = QString("%1/image%2.png").arg(cubePath).arg(val.band);
        bool saved = val.image.save(savePathName);
        if (!saved) return false;
        QFile file(savePathName);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            LOG_E("Failed to open image File");
            return false;
        }
        file.setPermissions(QFile::ReadUser);
        QDomElement img = document.createElement("Image");
        img.setAttribute("Band", val.band);
        img.setAttribute("SavePath", savePathName);
        myCube.appendChild(img);
    }

    QFile file(path + "/" + filename + ".xml");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        LOG_E("Failed to open .xml file at path %s", path.toLocal8Bit().data());
    } else {
        QTextStream stream(&file);
        stream << document.toString();
        file.close();
        LOG_D("Success .xml writing");
    }
    file.setPermissions(QFile::ReadUser);
    return true;
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
