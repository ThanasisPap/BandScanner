#ifndef SPECTRALCUBE_H
#define SPECTRALCUBE_H

#include "definitions.h"

class SpectralCube
{
public:
    SpectralCube();
    ~SpectralCube();

    bool loadCube();
    bool saveCube();
    void clearCubeData();
    void addSpectralImage(QImage image, int band);
    int photometry() const;
    void setPhotometry(int photometry);

    CubeType cubeType() const;
    void setCubeType(CubeType cubeType);
    QList<spectralImage> spectralImages() const;

private:
    QList<spectralImage> mSpectralImages;
    int mBandStart, mBandEnd;
    int mPhotometry, mCubeImageCurrentIndex;
    CubeType mCubeType;
private slots:
    void acquireImage();
};

#endif // SPECTRALCUBE_H
