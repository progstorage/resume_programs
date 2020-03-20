#ifndef RULES_H
#define RULES_H

#include <QDialog>
#include "ui_rules.h"

namespace Ui {
class Rules;
}

class Rules : public QDialog
{
    Q_OBJECT

public:
    explicit Rules(QWidget *parent = nullptr):
        QDialog(parent), ui(new Ui::Rules)
    {
        ui->setupUi(this);
    }
    ~Rules()
    {
        delete ui;
    }

private:
    Ui::Rules *ui;
};

#endif // RULES_H
