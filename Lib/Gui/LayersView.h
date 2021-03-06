#ifndef LAYERSVIEW_H
#define LAYERSVIEW_H


// Qt
#include <QWidget>
#include <QHash>
#include <QMenu>
#include <QAction>
#include <QListWidget>

// Project
#include "Core/LibExport.h"

namespace Ui {
class LayersView;
}

namespace Core {
class BaseLayer;
}

class QListWidgetItem;
class QDropEvent;
class QEvent;

namespace Gui
{

//******************************************************************************


class GIV_DLL_EXPORT LayersView : public QWidget
{
    Q_OBJECT

public:
    explicit LayersView(QWidget *parent = 0);
    ~LayersView();

    void addLayer(Core::BaseLayer * layer);
    void setLayers(const QList<Core::BaseLayer*> & layers);

    Core::BaseLayer * getCurrentLayer();

signals:
    void layerSelected(Core::BaseLayer*);
    void saveLayer(Core::BaseLayer*);
    void createNewLayer();


protected slots:
    void onItemClicked(QListWidgetItem*);
    void onItemChanged(QListWidgetItem*);
    void onItemOrderChanged();
    void onRemoveLayer();
    void onSaveLayer();
    void onContextMenuRequested(QPoint);

protected:

    void updateZValues();

private:

    void setupMenuOnItem();
    void setupMenuNoItem();

    Ui::LayersView *ui;

    QHash<QListWidgetItem*,Core::BaseLayer*> _itemLayerMap;

    QMenu _menu;
    QAction _removeLayer;
    QAction _saveLayer;
    QAction _createNewLayer;

};

//******************************************************************************

class LayersListWidget : public QListWidget
{
    Q_OBJECT
public :
    explicit LayersListWidget(QWidget *parent = 0) :
        QListWidget(parent)
    {}
signals:
    void itemOrderChanged();
protected :
    void dropEvent(QDropEvent * e);
};

//******************************************************************************

}

#endif // LAYERSVIEW_H
