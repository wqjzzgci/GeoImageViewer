#ifndef LayerUtilsTest_H
#define LayerUtilsTest_H

// Qt
#include <QObject>
#include <QtTest>

namespace Tests
{

//*************************************************************************

class LayerUtilsTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void test_GeoComputationMethods();
    void test_FileReadWriteMethods();
    void cleanupTestCase();

private:

};

//*************************************************************************

} 

#endif // LayerUtilsTest_H
