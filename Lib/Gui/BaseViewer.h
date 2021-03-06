#ifndef BASEVIEWER_H
#define BASEVIEWER_H


// Qt
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QCursor>
#include <QUrl>
#include <QMenu>

// Project
#include "Core/LibExport.h"
#include "ui_BaseViewer.h"

class QAction;
class QProgressDialog;
class QKeyEvent;
class QShowEvent;
class QLabel;


namespace Gui
{

//******************************************************************************

class GIV_DLL_EXPORT BaseViewer : public QWidget
{
    Q_OBJECT
public:

    struct Settings
    {
        bool enableSceneDragAndDropEvent;
        bool enableKeyNavigation;
        bool enableScroll;
        bool enableZoom;

        Settings() :
            enableSceneDragAndDropEvent(true),
            enableKeyNavigation(true),
            enableScroll(true),
            enableZoom(true)
        {
        }


    };

    explicit BaseViewer(const QString & initialText = QString(), QWidget *parent = 0);
    virtual ~BaseViewer();
    virtual void clear();

    void setSettings(const Settings & settings)
    { _settings = settings; }

    void showPointInfo(bool v);

signals:
    void viewportChanged(int zoomLevel, const QRectF & visibleSceneRect);

protected slots:
    void onZoomActionTriggered();
    virtual void onProgressValueChanged(int);
    virtual void onProgressCanceled() {}

protected:
    virtual void showEvent(QShowEvent *);
    virtual void resizeEvent(QResizeEvent *);
    virtual bool eventFilter(QObject *, QEvent *);

    virtual bool onSceneDragAndDrop(const QList<QUrl> & urls)
    { return true; }

    virtual void centerOnAtZoom(int zoomLevel, const QPointF & scenePoint = QPointF());

    void setCurrentCursor(const QCursor & c);

    QRectF getVisibleSceneRect();
    void viewportInfo();

    virtual QVector<double> getPixelValues(const QPoint &, bool *isComplex=0) const
    { return QVector<double>(); }

    QGraphicsScene _scene;

    QGraphicsSimpleTextItem * _initialTextItem;

    QString _initialText;
    QProgressDialog * _progressDialog;

    QCursor _cursor;

    int _zoomLevel;
    int _zoomMaxLevel;
    int _zoomMinLevel;
    QPointF _viewpoint;
    Settings _settings;

    QMenu _menu;

    void setZoomEnabled(bool enabled);
    Ui_BaseViewer * _ui;

protected slots:
    void onContextMenuRequested(QPoint p);

private:
    bool dragAndDropEventOnScene(QEvent * e);
    bool navigationOnKeys(QEvent * e);
    bool zoomOnWheelEvent(QEvent * e);
    void setupViewContextMenu();

    QMouseEvent _lastMouseEvent;
    bool _handScrolling;
    bool scrollOnMouse(QEvent * e);

    QAction * _zoomIn;
    QAction * _zoomOut;



};

//******************************************************************************

}

#endif // BASEVIEWER_H
