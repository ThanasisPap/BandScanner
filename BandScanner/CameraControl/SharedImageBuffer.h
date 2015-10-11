
#ifndef SHAREDIMAGEBUFFER_H
#define SHAREDIMAGEBUFFER_H

// Qt
#include <QHash>
#include <QSet>
#include <QWaitCondition>
#include <QMutex>
#include <QImage>

// Local
#include <Buffer.h>

class SharedImageBuffer
{
    public:
        SharedImageBuffer();
        void add(int deviceNumber, Buffer<QImage> *imageBuffer, bool sync=false);
        Buffer<QImage>* getByDeviceNumber(int deviceNumber);
        void removeByDeviceNumber(int deviceNumber);
        void sync(int deviceNumber);
        void wakeAll();
        void setSyncEnabled(bool enable);
        bool isSyncEnabledForDeviceNumber(int deviceNumber);
        bool getSyncEnabled();
        bool containsImageBufferForDeviceNumber(int deviceNumber);

    private:
        QHash<int, Buffer<QImage>*> imageBufferMap;
        QSet<int> syncSet;
        QWaitCondition wc;
        QMutex mutex;
        int nArrived;
        bool doSync;
};

#endif // SHAREDIMAGEBUFFER_H
