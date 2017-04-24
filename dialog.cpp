#include "dialog.h"
#include "ui_dialog.h"


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->setFixedSize(693,542);


    ui->outerText->setReadOnly(true);
    QPalette pal = ui->outerText->palette();
    pal.setColor(QPalette::Active, QPalette::Base, qApp->palette().color(QPalette::Disabled, QPalette::Window));
    ui->outerText->setPalette(pal);
   
    connect(ui->checkBox,SIGNAL(clicked()),this,SLOT(isClicked()));
    connect(ui->EncryptButton,SIGNAL(clicked()),this,SLOT(encrypt()));
    connect(ui->enterText,SIGNAL(textChanged()),this,SLOT(regexp()));
    connect(ui->key,SIGNAL(textChanged(QString)),this,SLOT(set_key()));
    connect(ui->DecryptButton,SIGNAL(clicked()),this,SLOT(on_DecryptButton_clicked()));
    connect(ui->clear,SIGNAL(clicked()),this,SLOT(clearText()));

    QPixmap pic("table1.png");
    ui->picture->setPixmap(pic);

    /*timer = new QTimer(this);
    ui->lblClock->setText(QTime::currentTime().toString()); //Выводим значение времени в начале работы для устранения отсутствия времени до первого срабатывания таймера
    timer->start(1000);                                     //Задаем время срабатывания таймера (в мс)
    connect(timer,SIGNAL(timeout()),this,SLOT(time_update()));
    */
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::encrypt(){

    QByteArray arr = textInput.toUpper().toLocal8Bit();
    char* tmp = arr.data();
    std::reverse(tmp,tmp + strlen(tmp)); //переворот
    QByteArray arr1 = ui->key->text().toUpper().toLocal8Bit();
    char* tmp1 = arr1.data();

    if((arr.size() >= arr1.size() && arr1.size() != 0) && (ui->checkBox->isChecked())) {
        //char* tmp = ui->textEdit->toPlainText().toLocal8Bit().data(); Не работает!
        for(unsigned int i = 0; i < strlen(tmp);){
            for(unsigned int j = 0; j < strlen(tmp1) && i < strlen(tmp); j++){
                int shift = ((int)(tmp1[j]))-65; //определили сдвиг
                tmp[i]+=(shift);
                if(int(tmp[i]) >= 91)
                    tmp[i] -= 26;
                i++;
            }

        }
        QString str(tmp);
        ui->outerText->setText(str);
    } else
        QMessageBox::information( this, "Invalid Data Entered", "You have entered invalid data or you forget check\n");
}



void Dialog::on_DecryptButton_clicked()
{
    QByteArray arr = textInput.toUpper().toLocal8Bit();
    char* tmp = arr.data();
    QByteArray arr1 = ui->key->text().toUpper().toLocal8Bit();
    char* tmp1 = arr1.data();

    if((arr.size() >= arr1.size() && arr1.size() != 0) && (ui->checkBox->isChecked())) {
        for(unsigned int i(0); i < strlen(tmp);){
            for(unsigned int j(0); j < strlen(tmp1) && i < strlen(tmp); j++){
                int shift = ((int)(tmp1[j]))-65;
                tmp[i]-=shift;
                if(int(tmp[i]) <= 64)
                    tmp[i] += 26;
                i++;
            }

        }
        std::reverse(tmp,tmp + strlen(tmp));
        QString str(tmp);
        ui->outerText->setText(str);
    } else
        QMessageBox::information( this, "Invalid Data Entered", "You have entered invalid data or you forget check\n");
}

void Dialog::regexp()
{

    QString tmp3 = ui->enterText->toPlainText();
    QScriptValue text3 = engine.newVariant(tmp3);
    engine.globalObject().setProperty("text3",text3.toString());


    QString tmp1 = "";
    QScriptValue text = engine.newVariant(tmp1);
    engine.globalObject().setProperty("text",text.toString());

    QScriptValue res = engine.evaluate("var reg=/[0-9№’`~!@#$%^&*().,=+_{};:'?-\\s\/\|\'\")]/g; if(reg.test(text3)){text3 = text3.replace(reg,text);}text3;" ); //в res записывается результат работы скрипта, выполненный при помощи функции evaluate
    textInput = res.toString();
    //qDebug() << textInput << endl;

}




/*void Dialog::time_update(){    //Следующий метод (слот) вызывется при каждом переполнении таймера

    ui->lblClock->setText(QTime::currentTime().toString());  //Обновляем значение времени на форме

}*/

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
    QMessageBox::information(this,"Chiffre de Vigenère","Save OK");
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
    QRegExp exp("[a-z]{10}");
    ui->key->setValidator(new QRegExpValidator(exp,this));
}

void Dialog::clearText()
{
    ui->outerText->clear();
}
