#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFont>
#include <QFontDialog>
#include <QColor>
#include <QColorDialog>
#include <QTextCodec>
#include <QTextCursor>

bool FileManager::Save(QString filename, QString text)
{
    QFile file(filename);
    if (file.open(QFile::WriteOnly | QFile::Text))
    {
        file.write(text.toStdString().c_str());
        file.close();
        return true;
    }
    else
    {
        return false;
    }
}

bool FileManager::Open(QString filename, QString &text)
{
    QFile file(filename);
    if (file.exists())
    {
        if (file.open(QFile::ReadOnly | QFile::Text))
        {
            text = QString::fromUtf8(file.readAll());
            file.close();

            return true;
        }
    }

    return false;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    TextChanged = false;
    ui->setupUi(this);
    connectActions();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectActions()
{
    connect(ui->actionNew, SIGNAL(triggered()), SLOT(actionNew()));
    connect(ui->actionOpen, SIGNAL(triggered()), SLOT(actionOpen()));
    connect(ui->actionExit, SIGNAL(triggered()), SLOT(actionExit()));
    connect(ui->actionSave, SIGNAL(triggered()), SLOT(actionSave()));
    connect(ui->actionSaveAs, SIGNAL(triggered()), SLOT(actionSaveAs()));

    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(textEditTextChanged()));
    connect(ui->actionSelect, SIGNAL(triggered()), SLOT(actionSelect()));
    connect(ui->actionScale, SIGNAL(triggered()), SLOT(actionScale()));
    connect(ui->actionScale2, SIGNAL(triggered()), SLOT(actionScale2()));

    connect(ui->actionFontColor, SIGNAL(triggered()), SLOT(actionFontColor()));
    connect(ui->actionBackgroundColor, SIGNAL(triggered()), SLOT(actionBackgroundColor()));
    connect(ui->actionHighlightColor, SIGNAL(triggered()), SLOT(actionHighlightColor()));

    connect(ui->actionWin1251, SIGNAL(triggered()), SLOT(actionWin1251()));
    connect(ui->actionUTF8, SIGNAL(triggered()), SLOT(actionUTF8()));
    connect(ui->actionUTF16, SIGNAL(triggered()), SLOT(actionUTF16()));
    connect(ui->actionUTF32, SIGNAL(triggered()), SLOT(actionUTF32()));
    connect(ui->actionKOI8U, SIGNAL(triggered()), SLOT(actionKOI8U()));
}

void MainWindow::showEvent(QShowEvent *event)
{
    ui->actionSave->setEnabled(false);
    QMainWindow::showEvent(event);
    font_size = ui->textEdit->font().pointSize();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    actionExit();
}

void MainWindow::actionNew()
{
    CheckSave();
    file_name = "";
    ui->textEdit->clear();
    TextChanged = false;
    ui->actionSave->setEnabled(false);
    this->setWindowTitle("TextEditor");
}

void MainWindow::actionOpen()
{
    file_name = QFileDialog::getOpenFileName();
    if (file_name != "")
    {
        QString text;

        if (file_manager.Open(file_name, text))
        {
            ui->textEdit->setPlainText(text);
            this->setWindowTitle("TextEditor - " + file_name);
            TextChanged = false;
            ui->actionSave->setEnabled(false);
        }
        else
        {
            QMessageBox::information(this, "Title", "Can't open file");
        }

    }

}

void MainWindow::actionExit()
{
    CheckSave();
    QApplication::quit();
}

void MainWindow::actionSave()
{
    if ((file_name != "") && TextChanged)
    {
        Save();
    }

}

void MainWindow::actionSaveAs()
{
    QString temp_file_name = QFileDialog::getSaveFileName();
    if (temp_file_name != "")
    {
        file_name = temp_file_name;
        Save();
        this->setWindowTitle("TextEditor - " + file_name);
    }
}

void MainWindow::textEditTextChanged()
{
    if (!TextChanged)
    {
        TextChanged = true;
        ui->actionSave->setEnabled(true);
    }
}

void MainWindow::Save()
{
    if (file_manager.Save(file_name, ui->textEdit->toPlainText()))
    {
        TextChanged = false;
        ui->actionSave->setEnabled(false);
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
                actionSaveAs();
            }
            else
            {
                actionSave();
            }
        }
    }
}

void MainWindow::actionSelect()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, this);
    if (ok)
    {
        QTextCursor cursor(ui->textEdit->textCursor());
        QString select_text = cursor.selectedText();
        if (select_text.isEmpty())
        {
            ui->textEdit->selectAll();
            ui->textEdit->setCurrentFont(font);
        }
        else
        {
            ui->textEdit->selectionChanged();
            ui->textEdit->setCurrentFont(font);
        }
    }
}

void MainWindow::actionScale()
{
    ScaleFont(5);
}

void MainWindow::actionScale2()
{
    ScaleFont(-5);
}

void MainWindow::ScaleFont(int scale)
{
    if (font_size < 0)
    {
        return;
    }

    font_size += scale;

    QTextCursor cursor(ui->textEdit->textCursor());
    QString select_text = cursor.selectedText();
    if (select_text.isEmpty())
    {
        ui->textEdit->selectAll();
        ui->textEdit->setFontPointSize(font_size);
    }
    else
    {
        ui->textEdit->selectionChanged();
        ui->textEdit->setFontPointSize(font_size);
    }
}

void MainWindow::actionFontColor()
{
    QColor color = QColorDialog::getColor(Qt::white, this);

    if (color.isValid())
    {
        ui->textEdit->setTextColor(color);
    }
}

void MainWindow::actionBackgroundColor()
{
    QColor color = QColorDialog::getColor(Qt::white, this);

    QPalette p = ui->textEdit->palette();
    p.setColor(QPalette::Base, color);
    ui->textEdit->setPalette(p);
}

void MainWindow::actionHighlightColor()
{
    QColor color = QColorDialog::getColor(Qt::white, this);

    if (color.isValid())
    {
        ui->textEdit->setTextBackgroundColor(color);
    }
}

// кодировки не работают до конца, надо потестировать больше кодировок

void MainWindow::actionWin1251()
{
    Encode("Windows-1251");
}

void MainWindow::actionUTF8()
{
    Encode("UTF-8");
}

void MainWindow::actionUTF16()
{
    Encode("UTF-16");
}

void MainWindow::actionUTF32()
{
    Encode("UTF-32");
}

void MainWindow::actionKOI8U()
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
