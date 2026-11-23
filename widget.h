#ifndef WIDGET_H
#define WIDGET_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QAction>
#include <QLabel>
#include <QScrollArea>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QImage>
#include <QPixmap>
#include <QTransform>
#include <QStyle>

class Widget : public QMainWindow
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void openImage();
    void saveImage();
    void resetImage();
    void rotateImage();
    void grayscaleImage();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void updateImageDisplay();

    QScrollArea *scrollArea;
    QLabel      *imageLabel;
    QImage      originalImage;
    QImage      currentImage;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *imageMenu;

    QToolBar *mainToolBar;

    QAction *openAction;
    QAction *saveAction;
    QAction *exitAction;
    QAction *resetAction;
    QAction *rotateAction;
    QAction *grayscaleAction;
};

#endif