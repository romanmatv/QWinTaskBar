#include "qwintaskbar.h"
#include <QGuiApplication>
#include <QPainter>
#include <qpa/qplatformnativeinterface.h>

// for windows progress bar
#ifndef __GNUC__
#    define CALLBACK WINAPI
#    include <shobjidl.h>
#endif

QT_BEGIN_NAMESPACE
Q_GUI_EXPORT HICON qt_pixmapToWinHICON(const QPixmap &p);
QT_END_NAMESPACE

namespace {
ITaskbarList3* pITask = nullptr;
}

static inline QWindow *windowOfWidget(const QWidget *widget)
{
    if (QWindow *window = widget->windowHandle())
        return window;
    if (QWidget *topLevel = widget->nativeParentWidget())
        return topLevel->windowHandle();
    return nullptr;
}

static inline HWND hwndOfWidget(const QWidget *w)
{
    void *result = nullptr;
    if (QWindow *window = windowOfWidget(w))
        result = QGuiApplication::platformNativeInterface()->nativeResourceForWindow("handle", window);
    return static_cast<HWND>(result);
}

QWinTaskBar::QWinTaskBar(QWidget *parent):
    QObject{}, pWidget(parent), m_state(NOPROGRESS), m_min(0), m_max(100), m_value(-1)
{
    CoInitialize(NULL);
    HRESULT hRes = CoCreateInstance(CLSID_TaskbarList,
                                    NULL,CLSCTX_INPROC_SERVER,
                                    IID_ITaskbarList3,(LPVOID*) &pITask);
    if (FAILED(hRes))
    {
        pITask = nullptr;
        CoUninitialize();
        return;
    }

    pITask->HrInit();
}

QWinTaskBar::~QWinTaskBar()
{
    if (pITask) {
        pITask->Release();
        pITask = NULL;
        CoUninitialize();
    }
}

void QWinTaskBar::setMax(int maximum)
{
    setRange(qMin(m_min, maximum), maximum);
}

void QWinTaskBar::setMin(int minimum)
{
    setRange(minimum, qMax(m_max, minimum));
}

void QWinTaskBar::setRange(int minimum, int maximum)
{
    if (m_min != minimum){
        m_min = minimum;
        emit minChanged(minimum);
    }

    if (m_max != qMax(minimum, maximum)){
        m_max = qMax(minimum, maximum);
        emit maxChanged(m_max);
    }
}

void QWinTaskBar::setValue(int value)
{
    if (m_value != value){
        if (m_value == value ||
            ((value > m_max || value < m_min) &&
             (m_max != 0 || m_min != 0)))
            return;
        m_value = value;
        emit valueChanged(value);

        if (pITask) {
            const HWND winId = hwndOfWidget(pWidget);
            pITask->SetProgressValue(winId, m_value - m_min, m_max - m_min + 1);
        }
    }
}

void QWinTaskBar::setVisible(bool visible)
{    
    if (visible){
        setState(NORMAL);
    }else{
        setState(NOPROGRESS);
    }
}

void QWinTaskBar::setState(State state){
    if (!pITask)
        return;

    if (state != m_state){
        const HWND winId = hwndOfWidget(pWidget);
        pITask->SetProgressState(winId, (TBPFLAG)state);
        m_state = state;
        emit stateChanged(state);
    }
}

void QWinTaskBar::clearOverlayIcon(){
    if (!pITask)
        return;

    const HWND winId = hwndOfWidget(pWidget);
    pITask->SetOverlayIcon(winId, NULL, NULL);
}

void QWinTaskBar::setOverlayTextIcon(QString text, QPixmap icon){
    if (!pITask)
        return;

    const HWND winId = hwndOfWidget(pWidget);
    if (text.isEmpty() && icon.isNull()) {
        pITask->SetOverlayIcon(winId, NULL, NULL);
    } else {
        QPixmap pix = icon;
        pix.setDevicePixelRatio(1); // We want device-pixel sized font depending on the pix.height
        QPainter p(&pix);
        p.setPen(Qt::white);
        QFont font = p.font();
        font.setPixelSize(pix.height() * 0.5);
        p.setFont(font);
        p.drawText(pix.rect(), Qt::AlignCenter, text);
        //const HICON icon = pix.toImage().toHICON();
        const HICON icon = qt_pixmapToWinHICON(pix);
        pITask->SetOverlayIcon(winId, icon, (wchar_t*)text.utf16());
        DestroyIcon(icon);
    }
}

HRESULT QWinTaskBar::addTab(QWidget *widget)
{
    return (pITask && widget) ?
               pITask->AddTab(hwndOfWidget(widget)) : E_NOTIMPL;
}

HRESULT QWinTaskBar::deleteTab(QWidget *widget)
{
    return (pITask && widget) ?
               pITask->DeleteTab(hwndOfWidget(widget)) : E_NOTIMPL;
}

HRESULT QWinTaskBar::activateTab(QWidget *widget)
{
    return (pITask && widget) ?
               pITask->ActivateTab(hwndOfWidget(widget)) : E_NOTIMPL;
}

HRESULT QWinTaskBar::setActiveAlt(QWidget *widget)
{
    return (pITask && widget) ?
               pITask->SetActiveAlt(hwndOfWidget(widget)) : E_NOTIMPL;
}

HRESULT QWinTaskBar::registerTab(QWidget *tab, QWidget *mainWindow)
{
    return (pITask && tab && mainWindow) ?
               pITask->RegisterTab(hwndOfWidget(tab), hwndOfWidget(mainWindow)) : E_NOTIMPL;
}

HRESULT QWinTaskBar::unregisterTab(QWidget *tab)
{
    return (pITask && tab) ?
               pITask->UnregisterTab(hwndOfWidget(tab)) : E_NOTIMPL;
}

HRESULT QWinTaskBar::setTabOrder(QWidget *tab, QWidget *insertBefore)
{
    return (pITask && tab && insertBefore) ?
               pITask->SetTabOrder(hwndOfWidget(tab), hwndOfWidget(insertBefore)) : E_NOTIMPL;
}
