#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class FileManager
{

public:
    bool Save(QString filename, QString text);
    bool Open(QString filename, QString &text);

};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    FileManager file_manager;
    explicit MainWindow(QWidget *parent = nullptr);
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);
    ~MainWindow();

private slots:
    void actionNew();
    void actionOpen();
    void actionExit();
    void actionSave();
    void actionSaveAs();

    void textEditTextChanged();
    void actionSelect();
    void actionScale();
    void actionScale2();

    void actionFontColor();
    void actionBackgroundColor();
    void actionHighlightColor();

    void actionWin1251();
    void actionUTF8();
    void actionUTF16();
    void actionUTF32();
    void actionKOI8U();

private:
    Ui::MainWindow *ui;
    QString file_name;
    bool TextChanged;
    int font_size;
    void connectActions();
    void Save();
    void CheckSave();
    void ScaleFont(int scale);
    void Encode(QString encode_to);
};

#endif // MAINWINDOW_H
