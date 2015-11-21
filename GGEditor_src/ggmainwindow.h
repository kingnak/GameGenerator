#ifndef GGMAINWINDOW_H
#define GGMAINWINDOW_H

#include <QMainWindow>

class GGUIController;

class GGMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    GGMainWindow();

private:
    GGUIController *m_ctrl;
};

#endif // GGMAINWINDOW_H
