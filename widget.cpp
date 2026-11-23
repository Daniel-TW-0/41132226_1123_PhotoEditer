#include "widget.h"

Widget::Widget(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("PhotoEditer"));
    resize(900, 650);

    imageLabel = new QLabel(tr("請點擊 File -> Open Image 或工具列圖示開啟圖片"));
    imageLabel->setAlignment(Qt::AlignCenter);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidget(imageLabel);
    scrollArea->setAlignment(Qt::AlignCenter);
    scrollArea->setWidgetResizable(true);
    setCentralWidget(scrollArea);

    createActions();
    createMenus();
    createToolBars();

    statusBar()->showMessage(tr("就緒"));
}

Widget::~Widget()
{
}

void Widget::createActions()
{
    openAction = new QAction(style()->standardIcon(QStyle::SP_DialogOpenButton),
                             tr("Open Image"), this);
    openAction->setShortcut(QKeySequence(tr("Ctrl+O")));
    openAction->setStatusTip(tr("開啟本機影像檔案"));
    connect(openAction, &QAction::triggered, this, &Widget::openImage);

    saveAction = new QAction(style()->standardIcon(QStyle::SP_DialogSaveButton),
                             tr("Save"), this);
    saveAction->setShortcut(QKeySequence(tr("Ctrl+S")));
    saveAction->setStatusTip(tr("儲存目前編輯的影像"));
    connect(saveAction, &QAction::triggered, this, &Widget::saveImage);

    exitAction = new QAction(style()->standardIcon(QStyle::SP_DialogCloseButton),
                             tr("Exit"), this);
    exitAction->setShortcut(QKeySequence(tr("Ctrl+Q")));
    exitAction->setStatusTip(tr("結束應用程式"));
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    resetAction = new QAction(style()->standardIcon(QStyle::SP_BrowserReload),
                              tr("Reset Image"), this);
    resetAction->setShortcut(QKeySequence(tr("Ctrl+Z")));
    resetAction->setStatusTip(tr("還原為原始圖片"));
    connect(resetAction, &QAction::triggered, this, &Widget::resetImage);

    rotateAction = new QAction(style()->standardIcon(QStyle::SP_ArrowRight),
                               tr("Rotate 90°"), this);
    rotateAction->setShortcut(QKeySequence(tr("Ctrl+R")));
    rotateAction->setStatusTip(tr("順時針旋轉 90 度"));
    connect(rotateAction, &QAction::triggered, this, &Widget::rotateImage);

    grayscaleAction = new QAction(style()->standardIcon(QStyle::SP_FileDialogDetailedView),
                                  tr("Grayscale"), this);
    grayscaleAction->setShortcut(QKeySequence(tr("Ctrl+G")));
    grayscaleAction->setStatusTip(tr("轉換為灰階影像"));
    connect(grayscaleAction, &QAction::triggered, this, &Widget::grayscaleImage);
}

void Widget::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu(tr("Edit"));
    editMenu->addAction(resetAction);

    imageMenu = menuBar()->addMenu(tr("Image"));
    imageMenu->addAction(rotateAction);
    imageMenu->addAction(grayscaleAction);
}

void Widget::createToolBars()
{
    mainToolBar = addToolBar(tr("Main Toolbar"));
    mainToolBar->addAction(openAction);
    mainToolBar->addAction(saveAction);
    mainToolBar->addSeparator();
    mainToolBar->addAction(rotateAction);
    mainToolBar->addAction(grayscaleAction);
    mainToolBar->addAction(resetAction);
    mainToolBar->addSeparator();
    mainToolBar->addAction(exitAction);
}

void Widget::openImage()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open Image"),
        "",
        tr("Image Files (*.png *.jpg *.jpeg *.bmp)")
        );

    if (fileName.isEmpty())
        return;

    QImage loadedImage;
    if (!loadedImage.load(fileName)) {
        QMessageBox::warning(this, tr("錯誤"), tr("無法載入選取的圖片檔案。"));
        return;
    }

    originalImage = loadedImage;
    currentImage = loadedImage;
    updateImageDisplay();
    statusBar()->showMessage(tr("已開啟：%1 (%2 x %3)")
                                 .arg(fileName)
                                 .arg(currentImage.width())
                                 .arg(currentImage.height()));
}

void Widget::saveImage()
{
    if (currentImage.isNull()) {
        QMessageBox::information(this, tr("提示"), tr("目前沒有可儲存的圖片。"));
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(
        this,
        tr("Save Image"),
        "edited_image.png",
        tr("PNG Image (*.png);;JPEG Image (*.jpg *.jpeg);;BMP Image (*.bmp)")
        );

    if (fileName.isEmpty())
        return;

    if (currentImage.save(fileName)) {
        statusBar()->showMessage(tr("已儲存至：%1").arg(fileName));
    } else {
        QMessageBox::warning(this, tr("錯誤"), tr("圖片儲存失敗。"));
    }
}

void Widget::resetImage()
{
    if (originalImage.isNull())
        return;

    currentImage = originalImage;
    updateImageDisplay();
    statusBar()->showMessage(tr("已還原為原始圖片"));
}

void Widget::rotateImage()
{
    if (currentImage.isNull())
        return;

    QTransform transform;
    transform.rotate(90);
    currentImage = currentImage.transformed(transform);
    updateImageDisplay();
    statusBar()->showMessage(tr("已順時針旋轉 90 度"));
}

void Widget::grayscaleImage()
{
    if (currentImage.isNull())
        return;

    currentImage = currentImage.convertToFormat(QImage::Format_Grayscale8);
    updateImageDisplay();
    statusBar()->showMessage(tr("已套用灰階效果"));
}

void Widget::updateImageDisplay()
{
    scrollArea->setWidgetResizable(false);
    imageLabel->setPixmap(QPixmap::fromImage(currentImage));
    imageLabel->adjustSize();
}