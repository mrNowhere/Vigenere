#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QString>
#include <QBitArray>
#include <QTimer>
#include <QTime>
#include <QRegExp>
#include "algorithm"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QPixmap>
#include <QValidator>
#include <QScriptEngine>
#include <QPalette>
//#include <QToolTip>
#include <QDebug>


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
private slots:
    void encrypt();
    void time_update();
    void on_ExitButton_clicked();
    void on_SaveButton_clicked();
    void on_DecryptButton_clicked();
    void regexp();
    void isClicked();
    void set_key();
private:
    QTimer* timer;
    QScriptEngine engine;
    Ui::Dialog *ui;
    QColor clr;
};

#endif // DIALOG_H
