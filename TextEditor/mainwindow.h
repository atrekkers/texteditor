#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionExit_triggered();

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_textEdit_textChanged();

    void on_actionSelect_triggered();

    void on_actionScale_triggered();

    void on_actionScale_2_triggered();

    void on_actionFont_Color_triggered();

    void on_actionBackground_Color_triggered();

    void on_actionHighlight_Color_triggered();

    void on_actionWin_1251_triggered();

    void on_actionUTF8_triggered();

    void on_actionUTF16_triggered();

    void on_actionUTF32_triggered();

    void on_actionKOI8_U_triggered();

private:
    Ui::MainWindow *ui;
    QString file_name;
    bool TextChanged;
    void Save();
    void CheckSave();
    void ScaleFont(int scale);
    void Encode(QString encode_to);
};

#endif // MAINWINDOW_H
