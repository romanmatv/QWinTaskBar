#ifndef QWINTASKBAR_H
#define QWINTASKBAR_H

#include "QWinTaskBar_global.h"
#include <QWidget>

class QWINTASKBAR_EXPORT QWinTaskBar: public QObject
{
    Q_OBJECT

    Q_PROPERTY(int min READ min WRITE setMin NOTIFY minChanged FINAL)
    Q_PROPERTY(int max READ max WRITE setMax NOTIFY maxChanged FINAL)
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged FINAL)
    Q_PROPERTY(State state READ state WRITE setState NOTIFY stateChanged FINAL)
public:

    QWinTaskBar(QWidget *parent);
    ~QWinTaskBar();

    enum State
    {
        NOPROGRESS	= 0,
        INDETERMINATE	= 0x1,
        NORMAL	= 0x2,
        ERROR	= 0x4,
        PAUSED	= 0x8
    };

    //Progress Bar

    int value() const { return m_value; }
    int max() const { return m_max; }
    int min() const { return m_min; }

    State state() const {return m_state;}

    void setOverlayTextIcon(QString text = QString(), QPixmap icon = QPixmap());
    void clearOverlayIcon();

    //TABS

    HRESULT addTab(QWidget *widget);
    HRESULT deleteTab(QWidget *widget);
    HRESULT activateTab(QWidget *widget);
    HRESULT setActiveAlt(QWidget *widget);
    HRESULT registerTab(QWidget *tab, QWidget *mainWindow);
    HRESULT unregisterTab(QWidget *tab);
    HRESULT setTabOrder(QWidget *tab, QWidget *insertBefore);

public Q_SLOTS:
    void setMin(int min);
    void setMax(int max);
    void setRange(int min, int max);

    void setValue(int value);

    void setVisible(bool visible);

    void setState(State status);

Q_SIGNALS:
    void valueChanged(int value);
    void minChanged(int min);
    void maxChanged(int max);
    void stateChanged(State state);


protected:
    QWidget *pWidget;
    State m_state;

    int m_min, m_max, m_value;

    //TODO: Listing of tabs
    //QList<QWidget*> tabs;
};

#endif // QWINTASKBAR_H
