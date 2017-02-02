#include "dialog.h"
#include "ui_dialog.h"


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->setFixedSize(788,443);
    QMessageBox::information(this,"Information","<font color='red'>Please do not use next symbols: / | \\ \" \'. Maybe in the next version this bug will be fix!");

    ui->outerText->setReadOnly(true);
    ui->outerText->setTextColor(QColor(0,0,0,0));
    QPalette pal = ui->outerText->palette();
    pal.setColor(QPalette::Active, QPalette::Base, qApp->palette().color(QPalette::Disabled, QPalette::Window));
    ui->outerText->setPalette(pal);
   
    connect(ui->checkBox,SIGNAL(clicked()),this,SLOT(isClicked()));
    connect(ui->EncryptButton,SIGNAL(clicked()),this,SLOT(encrypt()));
    connect(ui->enterText,SIGNAL(textChanged()),this,SLOT(regexp()));
    connect(ui->key,SIGNAL(textChanged(QString)),this,SLOT(set_key()));
    connect(ui->DecryptButton,SIGNAL(clicked()),this,SLOT(on_DecryptButton_clicked()));

    QPixmap pic("table1.png");
    ui->picture->setPixmap(pic);

    timer = new QTimer(this);
    ui->lblClock->setText(QTime::currentTime().toString()); //Выводим значение времени в начале работы для устранения отсутствия времени до первого срабатывания таймера
    timer->start(1000);                                     //Задаем время срабатывания таймера (в мс)
    connect(timer,SIGNAL(timeout()),this,SLOT(time_update()));

}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::encrypt(){

    ui->outerText->setTextColor(Qt::black);
    QByteArray arr = ui->outerText->toPlainText().toUpper().toLocal8Bit();
    char* tmp = arr.data();
    std::reverse(tmp,tmp + strlen(tmp)); //переворот
    QByteArray arr1 = ui->key->text().toUpper().toLocal8Bit();
    char* tmp1 = arr1.data();


    if((arr.size() >= arr1.size()) && (ui->checkBox->isChecked())) {
    // char* tmp = ui->textEdit->toPlainText().toLocal8Bit().data(); Не работает!
        for(int i = 0; i < strlen(tmp);){
            for(int j = 0; j < strlen(tmp1) && i < strlen(tmp); j++){
                if(tmp[i] == ' '){
                    //или ui->enterText->toPlainText().at(i)
                    tmp[i] = (char)(rand() % 2 + 48); // при конвертации в чар число 0 или 1 по таблице аски соответсвуют смайлам. 48 в таблице соответсвует 0 в чар
                    j--;
                } else {
                    int shift = ((int)(tmp1[j]))-65; //определили сдвиг
                    tmp[i]+=(shift);
                    if(int(tmp[i]) >= 91)
                        tmp[i] -= 26;
                }
                i++;
            }

        }
        QString str(tmp);
        ui->outerText->setText(str);


    }
        else
            QMessageBox::information( this, "Invalid Data Entered", "You have entered invalid data or you forget check\n");

}



void Dialog::on_DecryptButton_clicked()
{

    QByteArray arr = ui->enterText->toPlainText().toUpper().toLocal8Bit();
    char* tmp = arr.data();

    QByteArray arr1 = ui->key->text().toUpper().toLocal8Bit();
    char* tmp1 = arr1.data();

    for(int i(0); i < strlen(tmp);){
        for(int j(0); j < strlen(tmp1) && i < strlen(tmp); j++){
        if(tmp[i] == '0' || tmp[i] == '1'){
            tmp[i] = ' ';
            j--;
        }
        else {
            int shift = ((int)(tmp1[j]))-65;
            tmp[i]-=shift;
            if(int(tmp[i]) <= 64)
                tmp[i] += 26;
        }
        i++;
        }

    }
    std::reverse(tmp,tmp + strlen(tmp));
    QString str(tmp);
    ui->outerText->setText(str);
}

void Dialog::regexp()
{
    QString tmp3 = ui->enterText->toPlainText();
    QScriptValue text3 = engine.newVariant(tmp3);
    engine.globalObject().setProperty("text3",text3.toString());


    QString tmp1 = "";
    QScriptValue text = engine.newVariant(tmp1);
    engine.globalObject().setProperty("text",text.toString());

    QScriptValue res = engine.evaluate("var reg=/[0-9№`~!@#$%^&*().,=+_{};:'?-]/g; if(reg.test(text3)){text3 = text3.replace(reg,text);}text3;" ); //в res записывается результат работы скрипта, выполненный при помощи функции evaluate
    ui->outerText->setText(res.toString());
}




void Dialog::time_update(){    //Следующий метод (слот) вызывется при каждом переполнении таймера

    ui->lblClock->setText(QTime::currentTime().toString());  //Обновляем значение времени на форме

}

void Dialog::on_ExitButton_clicked()
{
    close();
}


void Dialog::on_SaveButton_clicked()
{
    QFile file(".//crypt.txt");
    if ( !file.open(QFile::WriteOnly | QFile::Text) ){
        QMessageBox::warning(this,"Error","Could not write to file");
        return;
    }
    QTextStream fout(&file);
    fout << ui->outerText->toPlainText();
    file.close();
    QMessageBox::information(this,"","Save OK");
}

void Dialog::isClicked()
{
    if(ui->checkBox->isChecked())
        ui->key->setEnabled(false);
    else
        ui->key->setEnabled(true);
}

void Dialog::set_key()
{
    QRegExp exp("[a-zA-z]{3}");
    ui->key->setValidator(new QRegExpValidator(exp,this));
}


