#ifndef QWMAINWIND_H
#define QWMAINWIND_H

#include <QMainWindow>

namespace Ui {
class QWMainWind;
}

class QWMainWind : public QMainWindow
{
    Q_OBJECT

public:
    explicit QWMainWind(QWidget *parent = nullptr);
    ~QWMainWind();

private:
    Ui::QWMainWind *ui;
};

#endif // QWMAINWIND_H
