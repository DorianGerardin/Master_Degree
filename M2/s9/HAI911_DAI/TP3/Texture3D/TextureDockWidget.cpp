#include "TextureDockWidget.h"
#include <QFileDialog>
#include <QComboBox>

using namespace std;
TextureDockWidget::TextureDockWidget(TextureViewer * _viewer, QWidget * parent ):QDockWidget(parent)
{
    viewer = _viewer;

    QWidget * contents = new QWidget();

    QVBoxLayout * contentLayout = new QVBoxLayout(contents);

    QGroupBox * groupBox = new QGroupBox("Cutting plane", parent);
    groupBox->setMaximumSize(QSize(16777215, 200));

    contentLayout->addWidget ( groupBox) ;

    QGridLayout * cuttingPlaneGridLayout = new QGridLayout(groupBox);
    xHSlider = new QSlider(groupBox);
    xHSlider->setOrientation(Qt::Horizontal);

    cuttingPlaneGridLayout->addWidget(xHSlider, 1, 0, 1, 1);

    yHSlider = new QSlider(groupBox);
    yHSlider->setOrientation(Qt::Horizontal);

    cuttingPlaneGridLayout->addWidget(yHSlider, 3, 0, 1, 1);

    zHSlider = new QSlider(groupBox);
    zHSlider->setMaximum(1);
    zHSlider->setOrientation(Qt::Horizontal);

    cuttingPlaneGridLayout->addWidget(zHSlider, 5, 0, 1, 1);

    QPushButton * invertXPushButton = new QPushButton("invert", groupBox);
    cuttingPlaneGridLayout->addWidget(invertXPushButton, 1, 1, 1, 1);

    QPushButton * invertYPushButton = new QPushButton("invert", groupBox);
    cuttingPlaneGridLayout->addWidget(invertYPushButton, 3, 1, 1, 1);

    QPushButton * invertZPushButton = new QPushButton("invert", groupBox);
    cuttingPlaneGridLayout->addWidget(invertZPushButton, 5, 1, 1, 1);

    QLabel * labelCutX = new QLabel("x cut position", groupBox);
    cuttingPlaneGridLayout->addWidget(labelCutX, 0, 0, 1, 1);

    QLabel * labelCutY = new QLabel("y cut position", groupBox);
    cuttingPlaneGridLayout->addWidget(labelCutY, 2, 0, 1, 1);

    QLabel * labelCutZ = new QLabel("z cut position", groupBox);
    cuttingPlaneGridLayout->addWidget(labelCutZ, 4, 0, 1, 1);

    QCheckBox * displayXCut = new QCheckBox("display", groupBox);
    cuttingPlaneGridLayout->addWidget(displayXCut, 0, 1, 1, 1);

    QCheckBox * displayYCut = new QCheckBox("display", groupBox);
    cuttingPlaneGridLayout->addWidget(displayYCut, 2, 1, 1, 1);

    QCheckBox * displayZCut = new QCheckBox("display", groupBox);
    cuttingPlaneGridLayout->addWidget(displayZCut, 4, 1, 1, 1);


   //TODO connected les sliders

    connect(viewer, SIGNAL(setMaxCutPlanes(int,int,int)), this, SLOT(setMaxCutPlanes(int,int,int)));

    connect(xHSlider, SIGNAL(valueChanged(int)), viewer, SLOT(setXCut(int)));
    connect(yHSlider, SIGNAL(valueChanged(int)), viewer, SLOT(setYCut(int)));
    connect(zHSlider, SIGNAL(valueChanged(int)), viewer, SLOT(setZCut(int)));

    connect(displayXCut, SIGNAL(clicked(bool)), viewer, SLOT(setXCutDisplay(bool)));
    connect(displayYCut, SIGNAL(clicked(bool)), viewer, SLOT(setYCutDisplay(bool)));
    connect(displayZCut, SIGNAL(clicked(bool)), viewer, SLOT(setZCutDisplay(bool)));

    displayImageGroupBox = new QGroupBox("Label display", this);

    QVBoxLayout * segIVLayout = new QVBoxLayout(displayImageGroupBox);

    QFrame * segIFrame = new QFrame();

    segIGridLayout = new QGridLayout(segIFrame);

    QScrollArea * displayIScrollArea = new QScrollArea(displayImageGroupBox);
    displayIScrollArea->setWidget(segIFrame);

    displayIScrollArea->setWidgetResizable(true);
    displayIScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    segIVLayout->addWidget(displayIScrollArea);
    signalIMapper = new QSignalMapper(displayImageGroupBox);

    connect(signalIMapper, SIGNAL(mapped(const int &)), this, SLOT(setIVisibility(const int &)));

    QGridLayout * segIGridKayout = new QGridLayout();

    QPushButton *selectIPushButton = new QPushButton("Select All", displayImageGroupBox);
    segIGridKayout->addWidget(selectIPushButton, 0, 0, 1, 1);

    QPushButton *discardIPushButton = new QPushButton("Discard All", displayImageGroupBox);
    segIGridKayout->addWidget(discardIPushButton, 0, 1, 1, 1);

    segIVLayout->addLayout(segIGridKayout);

    segIVLayout->addStretch(0);
    connect(discardIPushButton, SIGNAL(clicked()), this, SLOT(discardIAll()));
    connect(selectIPushButton, SIGNAL(clicked()), this, SLOT(selectIAll()));

    connect(viewer, SIGNAL(setImageLabels()), this, SLOT(setImageLabels()));
    contentLayout->addWidget(displayImageGroupBox);

    contentLayout->addStretch(0);
    this->setWidget(contents);
}

void TextureDockWidget::setImageLabels(){

    const std::map<unsigned char, QColor> & colorMap = viewer->getIColorMap();

    addImageLabels(colorMap);
    selectIAll();

}

void TextureDockWidget::addImageLabels(const std::map<unsigned char, QColor> & colors){

    indexIToLabel.clear();
    std::vector<unsigned char> subdomain_indices;
    viewer->getImageSubdomainIndices(subdomain_indices);
    int i = 0;
    for (std::map<unsigned char, QColor>::const_iterator it = colors.begin(); it != colors.end() ; ++it , i++){
        indexIToLabel.push_back(it->first);
        QColor color = it->second;

        if(labelICheckButtons.size() > i ){
            imageLabels[i]->setPalette(color);
            imageLabels[i]->setVisible(true);
            labelICheckButtons[i]->setVisible(true);
        } else {

            QLabel * labelText  = new QLabel(QString::number(subdomain_indices[it->first]));
            segIGridLayout->addWidget(labelText, i, 0, 1, 1);

            QCheckBox * labelICheckBox = new QCheckBox(displayImageGroupBox);
            segIGridLayout->addWidget(labelICheckBox, i, 2, 1, 1);

            QLabel * labelColor = new QLabel(displayImageGroupBox);
            labelColor->setPalette(color);
            labelColor->setAutoFillBackground(true);

            segIGridLayout->addWidget(labelColor, i, 1, 1, 1);

            imageLabels.push_back(labelColor);
            labelICheckButtons.push_back(labelICheckBox);

            signalIMapper->setMapping(labelICheckBox, i);
            connect(labelICheckBox, SIGNAL(clicked()), signalIMapper, SLOT (map()));
        }
    }

    if( (unsigned int)labelICheckButtons.size() > colors.size() ){
        for( int b = colors.size() ; b < labelICheckButtons.size() ; b++ ){
            labelICheckButtons[b]->setVisible(false);
            imageLabels[b]->setVisible(false);
        }
    }

    displayImageGroupBox->adjustSize();

}

void TextureDockWidget::setIVisibility(int i){
    if(i < (int)indexIToLabel.size())
        viewer->setIVisibility(indexIToLabel.at(i), labelICheckButtons[i]->isChecked());
}



void TextureDockWidget::selectIAll(){
    for (int i = 0;i<labelICheckButtons.size();i++){
        if(!labelICheckButtons[i]->isChecked()){
            labelICheckButtons[i]->setChecked(true);
        }
        setIVisibility(i);
    }
}

void TextureDockWidget::discardIAll(){
    for (int i = 0;i<labelICheckButtons.size();i++){
        if(labelICheckButtons[i]->isChecked()){
            labelICheckButtons[i]->setChecked(false);
        }
    }
    viewer->discardIAll();
}

void TextureDockWidget::setMaxCutPlanes( int x, int y , int z ){
        xHSlider->setRange(0,x);
        yHSlider->setRange(0,y);
        zHSlider->setRange(0,z);
}
