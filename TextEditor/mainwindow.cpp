#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFont>
#include <QFontDialog>
#include <QColor>
#include <QColorDialog>
#include <QTextCodec>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    TextChanged = false;
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_triggered()
{
    CheckSave();
    file_name = "";
    ui->textEdit->clear();
    TextChanged = false;
    this->setWindowTitle("TextEditor");
}

void MainWindow::on_actionOpen_triggered()
{
    file_name = QFileDialog::getOpenFileName();
    if (file_name != "")
    {
        QFile file(file_name);
        if (file.exists())
        {
            if (file.open(QFile::ReadOnly | QFile::Text))
            {
                ui->textEdit->setPlainText(QString::fromUtf8(file.readAll()));
                this->setWindowTitle("TextEditor - " + file_name);
                file.close();
                TextChanged = false;
            }
            else
            {
                QMessageBox::information(this, "Title", "Can't open file");
            }

        }
        else
        {
            QMessageBox::information(this, "Title", "File doesn't exist");
        }

    }

}

void MainWindow::on_actionExit_triggered()
{
    CheckSave();
    QApplication::quit();
}

void MainWindow::on_actionSave_triggered()
{
    if ((file_name != "") && TextChanged)
    {
        Save();
    }

}

void MainWindow::on_actionSave_As_triggered()
{
    QString temp_file_name = QFileDialog::getSaveFileName();
    if (temp_file_name != "")
    {
        file_name = temp_file_name;
        Save();
        this->setWindowTitle("TextEditor - " + file_name);
    }
}

void MainWindow::on_textEdit_textChanged()
{
    if (!TextChanged)
    {
        TextChanged = true;
    }
}

void MainWindow::Save()
{
    QFile file(file_name);
    if (file.open(QFile::WriteOnly | QFile::Text))
    {
        file.write(ui->textEdit->toPlainText().toStdString().c_str());
        file.close();
        TextChanged = false;
    }
    else
    {
        QMessageBox::information(this, "Title", "Can't open file");
    }
}

void MainWindow::CheckSave()
{
    if (TextChanged)
    {
        QMessageBox msgBox;

        msgBox.setIcon(QMessageBox::Question);
        msgBox.setInformativeText("Save changes?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);

        if (msgBox.exec() == QMessageBox::Yes)
        {
            if (file_name == "")
            {
                on_actionSave_As_triggered();
            }
            else
            {
                on_actionSave_triggered();
            }
        }
    }
}

void MainWindow::on_actionSelect_triggered()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, this);
    if (ok)
    {
        ui->textEdit->setFont(font);
    }
}

void MainWindow::on_actionScale_triggered()
{
    ScaleFont(5);
}

void MainWindow::on_actionScale_2_triggered()
{
    ScaleFont(-5);
}

void MainWindow::ScaleFont(int scale)
{
    int font_size = ui->textEdit->font().pointSize();
    font_size += scale;
    if (font_size < 0)
    {
        return;
    }
    QFont font = ui->textEdit->font();
    font.setPointSize(font_size);
    ui->textEdit->setFont(font);
}

void MainWindow::on_actionFont_Color_triggered()
{
    QColor color = QColorDialog::getColor(Qt::white, this);

    if (color.isValid())
    {
        ui->textEdit->setTextColor(color);
    }
}

void MainWindow::on_actionBackground_Color_triggered()
{
    QColor color = QColorDialog::getColor(Qt::white, this);

    QPalette p = ui->textEdit->palette();
    p.setColor(QPalette::Base, color);
    ui->textEdit->setPalette(p);
}

void MainWindow::on_actionHighlight_Color_triggered()
{
    QColor color = QColorDialog::getColor(Qt::white, this);

    if (color.isValid())
    {
        ui->textEdit->setTextBackgroundColor(color);
    }
}

// кодировки не работают до конца, надо потестировать больше кодировок

void MainWindow::on_actionWin_1251_triggered()
{
    Encode("Windows-1251");
}

void MainWindow::on_actionUTF8_triggered()
{
    Encode("UTF-8");
}

void MainWindow::on_actionUTF16_triggered()
{
    Encode("UTF-16");
}

void MainWindow::on_actionUTF32_triggered()
{
    Encode("UTF-32");
}

void MainWindow::on_actionKOI8_U_triggered()
{
    Encode("KOI8_U");
}


void MainWindow::Encode(QString encode_to)
{
    QString string = ui->textEdit->toPlainText().toUtf8();
    QTextCodec *codec = QTextCodec::codecForName(encode_to.toStdString().c_str());
    QByteArray encodedString = codec->fromUnicode(string);
    ui->textEdit->setText(encodedString);
}

