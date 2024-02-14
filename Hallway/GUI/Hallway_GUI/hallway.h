#ifndef HALLWAY_H
#define HALLWAY_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class hallway;
}
QT_END_NAMESPACE

class hallway : public QMainWindow
{
    Q_OBJECT

public:
    hallway(QWidget *parent = nullptr);
    ~hallway();

private:
    Ui::hallway *ui;
};
#endif // HALLWAY_H
