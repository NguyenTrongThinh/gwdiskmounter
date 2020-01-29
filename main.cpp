#include <QCoreApplication>
#include <QDebug>
#include <QStringList>
#include <QDBusMetaType>
#include <udisks2-qt5/ddiskmanager.h>
#include <udisks2-qt5/dblockdevice.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//    qRegisterMetaType<QList<QPair<QString,QVariantMap>>>("QList<QPair<QString,QVariantMap>>");
//    qDBusRegisterMetaType<QList<QPair<QString,QVariantMap>>>();
//    qDBusRegisterMetaType<QPair<QString,QVariantMap>>();
    QVariantMap mountOptions;
    //mountOptions.insert("", "");
    DDiskManager *mDiskManager = new DDiskManager();
    QStringList blockDevices = mDiskManager->blockDevices();
    foreach (QString block, blockDevices) {
        DBlockDevice *mBlockDevice = mDiskManager->createBlockDevice(block);
        if (!mBlockDevice->isLoopDevice() && QString::compare(mBlockDevice->idType(), "") && mBlockDevice->isValid()) {
            qDebug() << "Device Drives: " << mBlockDevice->drive();
            qDebug() << "Device Path: " << mBlockDevice->path();
            qDebug() << "Device Name: " << QString::fromLatin1(mBlockDevice->device());
            qDebug() << mBlockDevice->mount(mountOptions);
        }
        delete mBlockDevice;
    }
    a.connect(mDiskManager, &DDiskManager::blockDeviceAdded, [] (QString path) {
        qDebug() << "Block Device Added: " << path;
    });

    a.connect(mDiskManager, &DDiskManager::blockDeviceRemoved, [] (QString path) {
        qDebug() << "Block Device Removed: " << path;
    });
    return a.exec();
}
