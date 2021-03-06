#ifndef IMAGEDATAPROVIDER_H
#define IMAGEDATAPROVIDER_H


// Qt
#include <QObject>
#include <QRect>
#include <QRectF>
#include <QPolygonF>
#include <QImage>
#include <QStringList>

// OpenCV
#include <opencv2/core/core.hpp>

// Project
#include "Global.h"
#include "LibExport.h"

class GDALDataset;
class QMutex;

namespace Core
{

//******************************************************************************

class GIV_DLL_EXPORT ImageDataProvider : public QObject
{
    Q_OBJECT

    PROPERTY_ACCESSORS(QString, imageName, getImageName, setImageName)
    PROPERTY_GETACCESSOR(QString, location, getLocation)

    // INPUT Image Data Info :
    PROPERTY_GETACCESSOR(int, inputNbBands, getInputNbBands)
    PROPERTY_GETACCESSOR(bool, inputIsComplex, inputIsComplex)
    PROPERTY_GETACCESSOR(int, inputWidth, getInputWidth)
    PROPERTY_GETACCESSOR(int, inputHeight, getInputHeight)
    PROPERTY_GETACCESSOR(int, inputDepth, getInputDepthInBytes)

    // OUTPUT Image Data Info :

    // Layer info (e.g nbBands = 4*Original Nb Bands for Complex images, depth is always 32F )
    PROPERTY_GETACCESSOR(int, nbBands, getNbBands)
    PROPERTY_GETACCESSOR(bool, isComplex, isComplex)
    PROPERTY_GETACCESSOR(int, width, getWidth)
    PROPERTY_GETACCESSOR(int, height, getHeight)
    PROPERTY_GETACCESSOR(int, depth, getDepthInBytes)

    // Pixel extent defines a rect=(0,0,width,height)
    PROPERTY_GETACCESSOR(QRect, pixelExtent, getPixelExtent)

    PROPERTY_ACCESSORS(QVector<double>, minValues, getMinValues, setMinValues)
    PROPERTY_ACCESSORS(QVector<double>, maxValues, getMaxValues, setMaxValues)
    PROPERTY_ACCESSORS(QVector<QVector<double> >, bandHistograms, getBandHistograms, setBandHistograms)
    PROPERTY_GETACCESSOR(QStringList, bandNames, getBandNames)

    // Option
    PROPERTY_ACCESSORS(bool, cutNoDataBRBoundary, getCutNoDataBRBoundary, setCutNoDataBRBoundary)
    PROPERTY_ACCESSORS(bool, editable, isEditable, setEditable)

public:
    static const float NoDataValue;

    static cv::Mat computeMask(const cv::Mat & data, float noDataValue=NoDataValue);

    explicit ImageDataProvider(QObject *parent = 0);
    virtual cv::Mat getImageData(const QRect & srcPixelExtent=QRect(), int dstPixelWidth=0, int dstPixelHeight=0) const = 0 ;
    QVector<double> getPixelValue(const QPoint & pixelCoords, bool * isComplex = 0) const;

    virtual QString fetchProjectionRef() const { return QString("Unknown"); }
    virtual QPolygonF fetchGeoExtent(const QRect & pixelExtent) const
    {
        QVector<QPoint> points;
        if (!pixelExtent.isEmpty())
        {
            points << pixelExtent.topLeft() << pixelExtent.topRight()
                << pixelExtent.bottomRight() << pixelExtent.bottomLeft();
        }
        return fetchGeoExtent(points);
    }
    virtual QPolygonF fetchGeoExtent(const QVector<QPoint> & points=QVector<QPoint>()) const
    { Q_UNUSED(points); return QPolygonF(); }
    virtual QVector<double> fetchGeoTransform() const { return  QVector<double>(); }

    virtual bool isValid() const
    { return false; }

protected:
    static void setupDataInfo(const cv::Mat & src, ImageDataProvider * dst);


};

//******************************************************************************

class GIV_DLL_EXPORT GDALDataProvider : public ImageDataProvider
{
    Q_OBJECT
    PROPERTY_GETACCESSOR(QString, filePath, getFilePath)
    PTR_PROPERTY_GETACCESSOR(GDALDataset, dataset, getDataset)

public:
    explicit GDALDataProvider(QObject *parent = 0);
    virtual ~GDALDataProvider();

    virtual cv::Mat getImageData(const QRect & srcPixelExtent=QRect(), int dstPixelWidth=0, int dstPixelHeight=0) const;
    bool setup(const QString & filepath);

    virtual QString fetchProjectionRef() const;
//    virtual QPolygonF fetchGeoExtent(const QRect & pixelExtent=QRect()) const;
    virtual QPolygonF fetchGeoExtent(const QVector<QPoint> & points=QVector<QPoint>()) const;
    virtual QVector<double> fetchGeoTransform() const;

    virtual bool isValid() const
    { return _dataset != 0; }


protected:
    QMutex * _mutex;

};

//******************************************************************************

}

#endif // IMAGEDATAPROVIDER_H
