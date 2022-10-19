#include "Window.h"
#include <QFileDialog>
Window::Window()
{
    if (this->objectName().isEmpty())
        this->setObjectName("mainWindow");
    this->resize(929, 891);

    viewer = new TextureViewer(this);

    QWidget * tectureWidget = new QWidget(this);
    QGridLayout * gridLayout = new QGridLayout(tectureWidget);

    gridLayout->addWidget(viewer, 0, 1, 1, 1);

    QAction * actionLoad3Dimage = new QAction("Load 3D image", this);

    QMenu * menuFile = new QMenu("File", this);

    menuFile->addAction(actionLoad3Dimage);

    connect(actionLoad3Dimage, SIGNAL(triggered()), this, SLOT(open3DImage()));

    QGroupBox * viewerGroupBox = new QGroupBox ("Texture viewer", this);
    QHBoxLayout * viewerLayout = new QHBoxLayout (viewerGroupBox);
    viewerLayout->addWidget (tectureWidget);

    madDockWidget = new TextureDockWidget(viewer,this);

    this->addDockWidget(Qt::RightDockWidgetArea, madDockWidget);

    this->setCentralWidget(viewerGroupBox);

    QMenuBar * menubar = new QMenuBar(this);

    menubar->addAction(menuFile->menuAction());

    this->setMenuBar(menubar);

    statusbar = new QStatusBar(this);

    this->setStatusBar(statusbar);

    this->setWindowTitle("Texture Viewer");
}

void Window::open3DImage(){

    QString selectedFilter, openFileNameLabel;
    QString fileFilter = "Known Filetypes (*.dim *.nii);;IMA (*.dim);;NIFTI (*.nii)";

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Select an input 3D image"),
                                                    openFileNameLabel,
                                                    fileFilter,
                                                    &selectedFilter);

    // In case of Cancel
    if ( fileName.isEmpty() ) {
        return;
    }

    statusBar()->showMessage("Opening 3D image...");
    if(fileName.endsWith(".dim") || fileName.endsWith(".nii") ){
        viewer->open3DImage(fileName);
        statusBar()->showMessage("3D image opened");

    }


}

