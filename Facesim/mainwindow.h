#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QImage>
#ifndef QT_NO_PRINTER
#include <QPrinter>
#endif


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QImage image;
    //QLabel *imageLabel;

    bool loadFile(const QString &fileName);
    void setImage(const QImage &newImage);

private slots:
    void on_load_button_clicked();
    void on_search_button_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
