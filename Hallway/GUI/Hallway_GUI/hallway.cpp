#include "hallway.h"
#include "./ui_hallway.h"

hallway::hallway(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::hallway)
{
    ui->setupUi(this);
}

hallway::~hallway()
{
    delete ui;
}
