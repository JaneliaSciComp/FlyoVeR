/********************************************************************************
** Form generated from reading UI file 'console.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONSOLE_H
#define UI_CONSOLE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QStackedWidget>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QTimeEdit>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionOpenConfig;
    QAction *actionQuit;
    QAction *actionSave;
    QAction *actionAdd;
    QAction *actionAbout;
    QAction *actionOpen_TraceFile;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_14;
    QHBoxLayout *horizontalLayout_51;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QSpinBox *numCamerasSpinBox;
    QSpacerItem *horizontalSpacer_15;
    QHBoxLayout *horizontalLayout;
    QLabel *displayCountLabel;
    QSpinBox *numDisplaysSpinBox;
    QSpacerItem *horizontalSpacer_17;
    QHBoxLayout *horizontalLayout_52;
    QLabel *label_32;
    QSpinBox *startingDisplaySpinBox;
    QSpacerItem *horizontalSpacer_16;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_2;
    QSpinBox *cameraSelectSpinBox;
    QTabWidget *tabWidget;
    QWidget *tab;
    QHBoxLayout *horizontalLayout_2;
    QFrame *frame;
    QVBoxLayout *verticalLayout_38;
    QHBoxLayout *horizontalLayout_32;
    QGroupBox *fileGroupBox;
    QVBoxLayout *fileGroupBoxLayout;
    QSpacerItem *verticalSpacer_4;
    QGroupBox *startingLocationsBox;
    QVBoxLayout *verticalLayout_22;
    QSpacerItem *verticalSpacer_5;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_10;
    QVBoxLayout *verticalLayout_2;
    QPushButton *initializeCameraButton;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_6;
    QRadioButton *blankRadioButton;
    QHBoxLayout *horizontalLayout_17;
    QRadioButton *imageRadioButton;
    QPushButton *imageCalibrationButton;
    QHBoxLayout *horizontalLayout_23;
    QRadioButton *movieRadioButton;
    QPushButton *movieCalibrationButton;
    QHBoxLayout *horizontalLayout_24;
    QRadioButton *osgRadioButton;
    QPushButton *osgDataButton;
    QGroupBox *blankingGroupBox;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_6;
    QVBoxLayout *verticalLayout_15;
    QHBoxLayout *horizontalLayout_15;
    QSlider *horizontalBlankingSlider;
    QSpinBox *horizontalBlankingSpinBox;
    QHBoxLayout *horizontalLayout_16;
    QSlider *horizontalBlankingSlider_2;
    QSpinBox *horizontalBlankingSpinBox_2;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_19;
    QCheckBox *enableDistortion;
    QCheckBox *gangedCheckBox;
    QCheckBox *xCenterDistortsBox;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_6;
    QSlider *focalLengthSlider_x;
    QSpinBox *focalLengthSpinBox_x;
    QHBoxLayout *horizontalLayout_22;
    QLabel *label_9;
    QSlider *focalLengthSlider_y;
    QSpinBox *focalLengthSpinBox_y;
    QHBoxLayout *horizontalLayout_21;
    QLabel *label_8;
    QSlider *xCenterSlider;
    QSpinBox *xCenterSpinBox;
    QHBoxLayout *horizontalLayout_20;
    QLabel *label_7;
    QSlider *yCenterSlider;
    QSpinBox *yCenterSpinBox;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_11;
    QRadioButton *TCP_radioBtn;
    QRadioButton *UDP_radioBtn;
    QRadioButton *radioButton_3;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_8;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_3;
    QLineEdit *hostnameCtrl;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_4;
    QLineEdit *portCtrl;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_5;
    QLineEdit *retriesCtrl;
    QHBoxLayout *horizontalLayout_9;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *networkConnectButton;
    QSpacerItem *horizontalSpacer_11;
    QCheckBox *gangMotionCheckBox;
    QPushButton *disableMotionButton;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *displayBlankingButton;
    QSpacerItem *horizontalSpacer_12;
    QHBoxLayout *horizontalLayout_61;
    QCheckBox *enableTimedTrialCheckBox;
    QTimeEdit *timeTrialEditor;
    QSpacerItem *horizontalSpacer_19;
    QGroupBox *groupBox_10;
    QHBoxLayout *horizontalLayout_68;
    QHBoxLayout *horizontalLayout_67;
    QToolButton *rewindButton;
    QToolButton *playBackwardsButton;
    QToolButton *playForwardButton;
    QToolButton *recordButton;
    QSpinBox *strideLength;
    QSlider *timeSlider;
    QDoubleSpinBox *timeLabel;
    QWidget *tab_5;
    QHBoxLayout *horizontalLayout_44;
    QGroupBox *displayBox;
    QHBoxLayout *horizontalLayout_45;
    QStackedWidget *displayWidget;
    QWidget *page;
    QWidget *page_2;
    QVBoxLayout *verticalLayout_16;
    QVBoxLayout *verticalLayout_13;
    QPushButton *resetButton;
    QPushButton *distributeHoriz;
    QPushButton *distributeVert;
    QPushButton *smoothButton;
    QPushButton *linearizeEdgesButton;
    QSpacerItem *verticalSpacer_3;
    QCheckBox *framePackingEnabled;
    QWidget *tab_2;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_14;
    QPushButton *diffuseLightButton;
    QLabel *diffuseLabel;
    QSpacerItem *horizontalSpacer_7;
    QLabel *label_18;
    QSpinBox *diffusePowerLevel;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_13;
    QPushButton *ambientLightButton;
    QLabel *ambientLabel;
    QWidget *layoutWidget_2;
    QHBoxLayout *horizontalLayout_18;
    QPushButton *backgroundColorButton;
    QLabel *backgroundLabel;
    QWidget *layoutWidget_4;
    QHBoxLayout *horizontalLayout_35;
    QPushButton *globalBrightnessButton;
    QLabel *brightnessLabel;
    QWidget *layoutWidget_3;
    QHBoxLayout *horizontalLayout_50;
    QLabel *label_30;
    QComboBox *shaderComboBox;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_29;
    QLabel *label_10;
    QSpacerItem *horizontalSpacer_5;
    QLabel *label_14;
    QHBoxLayout *horizontalLayout_30;
    QSpacerItem *horizontalSpacer_6;
    QCheckBox *computeAverage;
    QCheckBox *editValues;
    QHBoxLayout *horizontalLayout_25;
    QLabel *label_11;
    QPushButton *calibrateXButton;
    QLineEdit *calibrateXValue;
    QDoubleSpinBox *calibrateXGain;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_26;
    QLabel *label_12;
    QPushButton *calibrateYButton;
    QLineEdit *calibrateYValue;
    QDoubleSpinBox *calibrateYGain;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_27;
    QLabel *label_13;
    QPushButton *calibrateZButton;
    QLineEdit *calibrateZValue;
    QDoubleSpinBox *calibrateZGain;
    QHBoxLayout *horizontalLayout_31;
    QSpacerItem *horizontalSpacer_13;
    QPushButton *updateCalibrationValuesButton;
    QSpacerItem *horizontalSpacer_14;
    QWidget *tab_4;
    QGridLayout *gridLayout;
    QGroupBox *groupBox_8;
    QVBoxLayout *verticalLayout_23;
    QHBoxLayout *horizontalLayout_55;
    QLabel *label_33;
    QDoubleSpinBox *ballRadiusBox;
    QHBoxLayout *horizontalLayout_28;
    QLabel *label_15;
    QCheckBox *crossbarEnabledCheckBox;
    QLabel *label_34;
    QDoubleSpinBox *crossbarWidthSpinBox;
    QHBoxLayout *horizontalLayout_65;
    QLabel *label_35;
    QDoubleSpinBox *minimumVelocityThreshold;
    QCheckBox *restrictVerticalMotion;
    QVBoxLayout *verticalLayout_8;
    QCheckBox *rotatedCamerasCheckBox;
    QHBoxLayout *horizontalLayout_33;
    QLabel *label_19;
    QSlider *fovSlider;
    QSpinBox *spinBox;
    QHBoxLayout *horizontalLayout_34;
    QSpacerItem *horizontalSpacer_8;
    QLabel *label_20;
    QSlider *fovOffsetSlider;
    QDoubleSpinBox *fovOffsetSpinBox;
    QGroupBox *groupBox_16;
    QVBoxLayout *verticalLayout_35;
    QHBoxLayout *horizontalLayout_64;
    QCheckBox *velocitySmoothingEnabled;
    QHBoxLayout *horizontalLayout_102;
    QLabel *label_65;
    QSpinBox *velocitySmoothingInterval;
    QFrame *frame_3;
    QVBoxLayout *verticalLayout_36;
    QLabel *label_38;
    QHBoxLayout *horizontalLayout_101;
    QSpacerItem *horizontalSpacer_22;
    QCheckBox *inputSmoothingEnabled;
    QSpacerItem *horizontalSpacer_23;
    QLabel *preBlendLabel;
    QSpinBox *inputSmoothingInterval;
    QHBoxLayout *horizontalLayout_104;
    QSpacerItem *horizontalSpacer_27;
    QCheckBox *intermediarySmoothingEnabled;
    QSpacerItem *horizontalSpacer_28;
    QLabel *preBlendLabel_2;
    QSpinBox *intermediarySmoothingInterval;
    QFrame *postBlendFrame;
    QVBoxLayout *verticalLayout_37;
    QHBoxLayout *horizontalLayout_103;
    QSpacerItem *horizontalSpacer_21;
    QCheckBox *outputSmoothingEnabled;
    QSpacerItem *horizontalSpacer_26;
    QLabel *label_66;
    QSpinBox *outputSmoothingInterval;
    QGroupBox *groupBox_5;
    QVBoxLayout *verticalLayout_12;
    QHBoxLayout *horizontalLayout_60;
    QHBoxLayout *horizontalLayout_43;
    QLabel *label_25;
    QLineEdit *portNameControl;
    QHBoxLayout *horizontalLayout_54;
    QLabel *label_31;
    QSpinBox *outputRateSpinBox;
    QHBoxLayout *horizontalLayout_53;
    QCheckBox *fileExportToggle;
    QSpacerItem *horizontalSpacer_20;
    QLabel *label_17;
    QLineEdit *exportFileNameBox;
    QToolButton *exportFileNameSelector;
    QSpacerItem *horizontalSpacer_18;
    QLabel *label_16;
    QComboBox *exportFileNameStyle;
    QHBoxLayout *horizontalLayout_59;
    QCheckBox *treadmillOutputToggle;
    QSpacerItem *horizontalSpacer_24;
    QCheckBox *reducedOutputToggle;
    QSpacerItem *horizontalSpacer_25;
    QVBoxLayout *verticalLayout_9;
    QHBoxLayout *horizontalLayout_37;
    QLabel *label_21;
    QComboBox *frameModeComboBox;
    QHBoxLayout *horizontalLayout_38;
    QLabel *label_23;
    QSlider *indicatorSizeSlider;
    QSpinBox *indicatorSizeSpinBox;
    QHBoxLayout *horizontalLayout_42;
    QLabel *label_24;
    QSlider *borderSizeSlider;
    QSpinBox *borderSizeSpinBox;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_11;
    QHBoxLayout *horizontalLayout_40;
    QLabel *label_22;
    QSpinBox *screenSelectSpinBox;
    QHBoxLayout *horizontalLayout_41;
    QHBoxLayout *horizontalLayout_36;
    QSlider *indicatorHorizontalPositionSlider;
    QSpinBox *indicatorHorizontalPositionSpinBox;
    QVBoxLayout *verticalLayout_10;
    QHBoxLayout *horizontalLayout_39;
    QSpacerItem *horizontalSpacer_9;
    QSlider *indicatorVerticalPositionSlider;
    QSpacerItem *horizontalSpacer_10;
    QSpinBox *indicatorVerticalPositionSpinBox;
    QVBoxLayout *verticalLayout_28;
    QCheckBox *showDebugCheckBox;
    QGroupBox *debugPanel;
    QVBoxLayout *verticalLayout_21;
    QVBoxLayout *verticalLayout_20;
    QLabel *label_29;
    QHBoxLayout *horizontalLayout_49;
    QVBoxLayout *verticalLayout_17;
    QLabel *label_26;
    QDoubleSpinBox *xoffset;
    QVBoxLayout *verticalLayout_18;
    QLabel *label_27;
    QDoubleSpinBox *yoffset;
    QVBoxLayout *verticalLayout_19;
    QLabel *label_28;
    QDoubleSpinBox *zoffset;
    QCheckBox *debugPhysicsCheckBox;
    QCheckBox *showInvisibleObjectsCheckBox;
    QCheckBox *useCustomGLWidget;
    QWidget *tab_6;
    QHBoxLayout *horizontalLayout_58;
    QVBoxLayout *verticalLayout_29;
    QPushButton *headingStatusButton;
    QGroupBox *groupBox_15;
    QVBoxLayout *verticalLayout_25;
    QCheckBox *thresholdTurningEnabled;
    QHBoxLayout *horizontalLayout_62;
    QVBoxLayout *verticalLayout_33;
    QSpacerItem *verticalSpacer_6;
    QDoubleSpinBox *minThresholdScale;
    QVBoxLayout *verticalLayout_24;
    QHBoxLayout *horizontalLayout_46;
    QLabel *label_36;
    QDoubleSpinBox *thresholdWeight;
    QSlider *thresholdTurningSlider;
    QVBoxLayout *verticalLayout_34;
    QSpacerItem *verticalSpacer_7;
    QDoubleSpinBox *maxThresholdScale;
    QHBoxLayout *horizontalLayout_671;
    QLabel *label_37;
    QDoubleSpinBox *autoHeadingTurnRate;
    QSpacerItem *verticalSpacer_11;
    QVBoxLayout *verticalLayout_32;
    QGroupBox *groupBox_17;
    QHBoxLayout *horizontalLayout_57;
    QGroupBox *groupBox_7;
    QVBoxLayout *verticalLayout_27;
    QCheckBox *plotSymmetricCheckBox;
    QVBoxLayout *verticalLayout_26;
    QTableWidget *plotTableWidget;
    QHBoxLayout *horizontalLayout_47;
    QPushButton *addRowButton;
    QPushButton *deleteRowButton;
    QStackedWidget *ratioDisplayWidget;
    QWidget *page_5;
    QWidget *page_6;
    QGroupBox *groupBox_18;
    QHBoxLayout *horizontalLayout_56;
    QGroupBox *groupBox_9;
    QVBoxLayout *verticalLayout_30;
    QCheckBox *plotSymmetricCheckBox_2;
    QVBoxLayout *verticalLayout_31;
    QTableWidget *plotVelocityTableWidget;
    QHBoxLayout *horizontalLayout_48;
    QPushButton *addRowButton_2;
    QPushButton *deleteRowButton_2;
    QStackedWidget *velocityDisplayWidget;
    QWidget *page_7;
    QWidget *page_8;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuJovian;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1063, 966);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionOpenConfig = new QAction(MainWindow);
        actionOpenConfig->setObjectName(QString::fromUtf8("actionOpenConfig"));
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionAdd = new QAction(MainWindow);
        actionAdd->setObjectName(QString::fromUtf8("actionAdd"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionOpen_TraceFile = new QAction(MainWindow);
        actionOpen_TraceFile->setObjectName(QString::fromUtf8("actionOpen_TraceFile"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout_14 = new QVBoxLayout(centralwidget);
        verticalLayout_14->setObjectName(QString::fromUtf8("verticalLayout_14"));
        horizontalLayout_51 = new QHBoxLayout();
#ifndef Q_OS_MAC
        horizontalLayout_51->setSpacing(-1);
#endif
        horizontalLayout_51->setObjectName(QString::fromUtf8("horizontalLayout_51"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_3->addWidget(label);

        numCamerasSpinBox = new QSpinBox(centralwidget);
        numCamerasSpinBox->setObjectName(QString::fromUtf8("numCamerasSpinBox"));
        numCamerasSpinBox->setAlignment(Qt::AlignCenter);
        numCamerasSpinBox->setMinimum(1);
        numCamerasSpinBox->setMaximum(32);
        numCamerasSpinBox->setValue(3);

        horizontalLayout_3->addWidget(numCamerasSpinBox);


        horizontalLayout_51->addLayout(horizontalLayout_3);

        horizontalSpacer_15 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_51->addItem(horizontalSpacer_15);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        displayCountLabel = new QLabel(centralwidget);
        displayCountLabel->setObjectName(QString::fromUtf8("displayCountLabel"));

        horizontalLayout->addWidget(displayCountLabel);

        numDisplaysSpinBox = new QSpinBox(centralwidget);
        numDisplaysSpinBox->setObjectName(QString::fromUtf8("numDisplaysSpinBox"));
        numDisplaysSpinBox->setMinimum(1);

        horizontalLayout->addWidget(numDisplaysSpinBox);


        horizontalLayout_51->addLayout(horizontalLayout);

        horizontalSpacer_17 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_51->addItem(horizontalSpacer_17);

        horizontalLayout_52 = new QHBoxLayout();
        horizontalLayout_52->setObjectName(QString::fromUtf8("horizontalLayout_52"));
        label_32 = new QLabel(centralwidget);
        label_32->setObjectName(QString::fromUtf8("label_32"));

        horizontalLayout_52->addWidget(label_32);

        startingDisplaySpinBox = new QSpinBox(centralwidget);
        startingDisplaySpinBox->setObjectName(QString::fromUtf8("startingDisplaySpinBox"));
        startingDisplaySpinBox->setMinimum(1);

        horizontalLayout_52->addWidget(startingDisplaySpinBox);


        horizontalLayout_51->addLayout(horizontalLayout_52);

        horizontalSpacer_16 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_51->addItem(horizontalSpacer_16);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_4->addWidget(label_2);

        cameraSelectSpinBox = new QSpinBox(centralwidget);
        cameraSelectSpinBox->setObjectName(QString::fromUtf8("cameraSelectSpinBox"));
        cameraSelectSpinBox->setEnabled(true);
        cameraSelectSpinBox->setMinimum(1);
        cameraSelectSpinBox->setMaximum(3);

        horizontalLayout_4->addWidget(cameraSelectSpinBox);


        horizontalLayout_51->addLayout(horizontalLayout_4);


        verticalLayout_14->addLayout(horizontalLayout_51);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        horizontalLayout_2 = new QHBoxLayout(tab);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        frame = new QFrame(tab);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_38 = new QVBoxLayout(frame);
        verticalLayout_38->setObjectName(QString::fromUtf8("verticalLayout_38"));
        horizontalLayout_32 = new QHBoxLayout();
        horizontalLayout_32->setObjectName(QString::fromUtf8("horizontalLayout_32"));
        fileGroupBox = new QGroupBox(frame);
        fileGroupBox->setObjectName(QString::fromUtf8("fileGroupBox"));
        fileGroupBox->setFlat(false);
        fileGroupBoxLayout = new QVBoxLayout(fileGroupBox);
        fileGroupBoxLayout->setContentsMargins(0, 0, 0, 0);
        fileGroupBoxLayout->setObjectName(QString::fromUtf8("fileGroupBoxLayout"));
        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        fileGroupBoxLayout->addItem(verticalSpacer_4);


        horizontalLayout_32->addWidget(fileGroupBox);

        startingLocationsBox = new QGroupBox(frame);
        startingLocationsBox->setObjectName(QString::fromUtf8("startingLocationsBox"));
        verticalLayout_22 = new QVBoxLayout(startingLocationsBox);
        verticalLayout_22->setContentsMargins(0, 0, 0, 0);
        verticalLayout_22->setObjectName(QString::fromUtf8("verticalLayout_22"));
        verticalSpacer_5 = new QSpacerItem(20, 540, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_22->addItem(verticalSpacer_5);


        horizontalLayout_32->addWidget(startingLocationsBox);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        initializeCameraButton = new QPushButton(frame);
        initializeCameraButton->setObjectName(QString::fromUtf8("initializeCameraButton"));

        verticalLayout_2->addWidget(initializeCameraButton);

        groupBox_3 = new QGroupBox(frame);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        verticalLayout_6 = new QVBoxLayout(groupBox_3);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        blankRadioButton = new QRadioButton(groupBox_3);
        blankRadioButton->setObjectName(QString::fromUtf8("blankRadioButton"));
        blankRadioButton->setChecked(true);

        verticalLayout_6->addWidget(blankRadioButton);

        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setObjectName(QString::fromUtf8("horizontalLayout_17"));
        imageRadioButton = new QRadioButton(groupBox_3);
        imageRadioButton->setObjectName(QString::fromUtf8("imageRadioButton"));

        horizontalLayout_17->addWidget(imageRadioButton);

        imageCalibrationButton = new QPushButton(groupBox_3);
        imageCalibrationButton->setObjectName(QString::fromUtf8("imageCalibrationButton"));

        horizontalLayout_17->addWidget(imageCalibrationButton);


        verticalLayout_6->addLayout(horizontalLayout_17);

        horizontalLayout_23 = new QHBoxLayout();
        horizontalLayout_23->setObjectName(QString::fromUtf8("horizontalLayout_23"));
        movieRadioButton = new QRadioButton(groupBox_3);
        movieRadioButton->setObjectName(QString::fromUtf8("movieRadioButton"));

        horizontalLayout_23->addWidget(movieRadioButton);

        movieCalibrationButton = new QPushButton(groupBox_3);
        movieCalibrationButton->setObjectName(QString::fromUtf8("movieCalibrationButton"));

        horizontalLayout_23->addWidget(movieCalibrationButton);


        verticalLayout_6->addLayout(horizontalLayout_23);

        horizontalLayout_24 = new QHBoxLayout();
        horizontalLayout_24->setObjectName(QString::fromUtf8("horizontalLayout_24"));
        osgRadioButton = new QRadioButton(groupBox_3);
        osgRadioButton->setObjectName(QString::fromUtf8("osgRadioButton"));

        horizontalLayout_24->addWidget(osgRadioButton);

        osgDataButton = new QPushButton(groupBox_3);
        osgDataButton->setObjectName(QString::fromUtf8("osgDataButton"));

        horizontalLayout_24->addWidget(osgDataButton);


        verticalLayout_6->addLayout(horizontalLayout_24);


        verticalLayout_2->addWidget(groupBox_3);


        horizontalLayout_10->addLayout(verticalLayout_2);

        blankingGroupBox = new QGroupBox(frame);
        blankingGroupBox->setObjectName(QString::fromUtf8("blankingGroupBox"));
        blankingGroupBox->setEnabled(true);
        verticalLayout = new QVBoxLayout(blankingGroupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox_6 = new QGroupBox(blankingGroupBox);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        verticalLayout_15 = new QVBoxLayout(groupBox_6);
        verticalLayout_15->setObjectName(QString::fromUtf8("verticalLayout_15"));
        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        horizontalBlankingSlider = new QSlider(groupBox_6);
        horizontalBlankingSlider->setObjectName(QString::fromUtf8("horizontalBlankingSlider"));
        horizontalBlankingSlider->setEnabled(true);
        horizontalBlankingSlider->setValue(0);
        horizontalBlankingSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_15->addWidget(horizontalBlankingSlider);

        horizontalBlankingSpinBox = new QSpinBox(groupBox_6);
        horizontalBlankingSpinBox->setObjectName(QString::fromUtf8("horizontalBlankingSpinBox"));
        horizontalBlankingSpinBox->setEnabled(true);

        horizontalLayout_15->addWidget(horizontalBlankingSpinBox);


        verticalLayout_15->addLayout(horizontalLayout_15);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setObjectName(QString::fromUtf8("horizontalLayout_16"));
        horizontalBlankingSlider_2 = new QSlider(groupBox_6);
        horizontalBlankingSlider_2->setObjectName(QString::fromUtf8("horizontalBlankingSlider_2"));
        horizontalBlankingSlider_2->setEnabled(true);
        horizontalBlankingSlider_2->setValue(99);
        horizontalBlankingSlider_2->setOrientation(Qt::Horizontal);

        horizontalLayout_16->addWidget(horizontalBlankingSlider_2);

        horizontalBlankingSpinBox_2 = new QSpinBox(groupBox_6);
        horizontalBlankingSpinBox_2->setObjectName(QString::fromUtf8("horizontalBlankingSpinBox_2"));
        horizontalBlankingSpinBox_2->setEnabled(true);

        horizontalLayout_16->addWidget(horizontalBlankingSpinBox_2);


        verticalLayout_15->addLayout(horizontalLayout_16);


        verticalLayout->addWidget(groupBox_6);

        groupBox_4 = new QGroupBox(blankingGroupBox);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        verticalLayout_5 = new QVBoxLayout(groupBox_4);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        horizontalLayout_19 = new QHBoxLayout();
        horizontalLayout_19->setObjectName(QString::fromUtf8("horizontalLayout_19"));
        enableDistortion = new QCheckBox(groupBox_4);
        enableDistortion->setObjectName(QString::fromUtf8("enableDistortion"));
        enableDistortion->setChecked(true);

        horizontalLayout_19->addWidget(enableDistortion);

        gangedCheckBox = new QCheckBox(groupBox_4);
        gangedCheckBox->setObjectName(QString::fromUtf8("gangedCheckBox"));
        gangedCheckBox->setChecked(true);

        horizontalLayout_19->addWidget(gangedCheckBox);

        xCenterDistortsBox = new QCheckBox(groupBox_4);
        xCenterDistortsBox->setObjectName(QString::fromUtf8("xCenterDistortsBox"));

        horizontalLayout_19->addWidget(xCenterDistortsBox);


        verticalLayout_5->addLayout(horizontalLayout_19);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        label_6 = new QLabel(groupBox_4);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_12->addWidget(label_6);

        focalLengthSlider_x = new QSlider(groupBox_4);
        focalLengthSlider_x->setObjectName(QString::fromUtf8("focalLengthSlider_x"));
        focalLengthSlider_x->setMinimum(1);
        focalLengthSlider_x->setMaximum(200);
        focalLengthSlider_x->setValue(100);
        focalLengthSlider_x->setOrientation(Qt::Horizontal);

        horizontalLayout_12->addWidget(focalLengthSlider_x);

        focalLengthSpinBox_x = new QSpinBox(groupBox_4);
        focalLengthSpinBox_x->setObjectName(QString::fromUtf8("focalLengthSpinBox_x"));
        focalLengthSpinBox_x->setMinimum(1);
        focalLengthSpinBox_x->setMaximum(200);
        focalLengthSpinBox_x->setValue(100);

        horizontalLayout_12->addWidget(focalLengthSpinBox_x);


        verticalLayout_5->addLayout(horizontalLayout_12);

        horizontalLayout_22 = new QHBoxLayout();
        horizontalLayout_22->setObjectName(QString::fromUtf8("horizontalLayout_22"));
        label_9 = new QLabel(groupBox_4);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        horizontalLayout_22->addWidget(label_9);

        focalLengthSlider_y = new QSlider(groupBox_4);
        focalLengthSlider_y->setObjectName(QString::fromUtf8("focalLengthSlider_y"));
        focalLengthSlider_y->setMinimum(1);
        focalLengthSlider_y->setMaximum(200);
        focalLengthSlider_y->setValue(100);
        focalLengthSlider_y->setOrientation(Qt::Horizontal);

        horizontalLayout_22->addWidget(focalLengthSlider_y);

        focalLengthSpinBox_y = new QSpinBox(groupBox_4);
        focalLengthSpinBox_y->setObjectName(QString::fromUtf8("focalLengthSpinBox_y"));
        focalLengthSpinBox_y->setMinimum(1);
        focalLengthSpinBox_y->setMaximum(200);
        focalLengthSpinBox_y->setValue(100);

        horizontalLayout_22->addWidget(focalLengthSpinBox_y);


        verticalLayout_5->addLayout(horizontalLayout_22);

        horizontalLayout_21 = new QHBoxLayout();
        horizontalLayout_21->setObjectName(QString::fromUtf8("horizontalLayout_21"));
        label_8 = new QLabel(groupBox_4);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        horizontalLayout_21->addWidget(label_8);

        xCenterSlider = new QSlider(groupBox_4);
        xCenterSlider->setObjectName(QString::fromUtf8("xCenterSlider"));
        xCenterSlider->setMinimum(-50);
        xCenterSlider->setMaximum(150);
        xCenterSlider->setValue(50);
        xCenterSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_21->addWidget(xCenterSlider);

        xCenterSpinBox = new QSpinBox(groupBox_4);
        xCenterSpinBox->setObjectName(QString::fromUtf8("xCenterSpinBox"));
        xCenterSpinBox->setMinimum(-50);
        xCenterSpinBox->setMaximum(150);
        xCenterSpinBox->setValue(50);

        horizontalLayout_21->addWidget(xCenterSpinBox);


        verticalLayout_5->addLayout(horizontalLayout_21);

        horizontalLayout_20 = new QHBoxLayout();
        horizontalLayout_20->setObjectName(QString::fromUtf8("horizontalLayout_20"));
        label_7 = new QLabel(groupBox_4);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_20->addWidget(label_7);

        yCenterSlider = new QSlider(groupBox_4);
        yCenterSlider->setObjectName(QString::fromUtf8("yCenterSlider"));
        yCenterSlider->setMinimum(1);
        yCenterSlider->setMaximum(100);
        yCenterSlider->setValue(50);
        yCenterSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_20->addWidget(yCenterSlider);

        yCenterSpinBox = new QSpinBox(groupBox_4);
        yCenterSpinBox->setObjectName(QString::fromUtf8("yCenterSpinBox"));
        yCenterSpinBox->setMaximum(100);
        yCenterSpinBox->setValue(50);

        horizontalLayout_20->addWidget(yCenterSpinBox);


        verticalLayout_5->addLayout(horizontalLayout_20);


        verticalLayout->addWidget(groupBox_4);


        horizontalLayout_10->addWidget(blankingGroupBox);


        verticalLayout_4->addLayout(horizontalLayout_10);

        groupBox_2 = new QGroupBox(frame);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        horizontalLayout_11 = new QHBoxLayout(groupBox_2);
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        horizontalLayout_11->setSizeConstraint(QLayout::SetDefaultConstraint);
        horizontalLayout_11->setContentsMargins(-1, 0, -1, 0);
        TCP_radioBtn = new QRadioButton(groupBox_2);
        TCP_radioBtn->setObjectName(QString::fromUtf8("TCP_radioBtn"));
        TCP_radioBtn->setChecked(true);

        horizontalLayout_11->addWidget(TCP_radioBtn);

        UDP_radioBtn = new QRadioButton(groupBox_2);
        UDP_radioBtn->setObjectName(QString::fromUtf8("UDP_radioBtn"));

        horizontalLayout_11->addWidget(UDP_radioBtn);

        radioButton_3 = new QRadioButton(groupBox_2);
        radioButton_3->setObjectName(QString::fromUtf8("radioButton_3"));

        horizontalLayout_11->addWidget(radioButton_3);


        verticalLayout_4->addWidget(groupBox_2);

        frame_2 = new QFrame(frame);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(frame_2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_3 = new QLabel(frame_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_5->addWidget(label_3);

        hostnameCtrl = new QLineEdit(frame_2);
        hostnameCtrl->setObjectName(QString::fromUtf8("hostnameCtrl"));

        horizontalLayout_5->addWidget(hostnameCtrl);


        horizontalLayout_8->addLayout(horizontalLayout_5);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_4 = new QLabel(frame_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_6->addWidget(label_4);

        portCtrl = new QLineEdit(frame_2);
        portCtrl->setObjectName(QString::fromUtf8("portCtrl"));

        horizontalLayout_6->addWidget(portCtrl);


        horizontalLayout_8->addLayout(horizontalLayout_6);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_2);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_5 = new QLabel(frame_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_7->addWidget(label_5);

        retriesCtrl = new QLineEdit(frame_2);
        retriesCtrl->setObjectName(QString::fromUtf8("retriesCtrl"));

        horizontalLayout_7->addWidget(retriesCtrl);


        horizontalLayout_8->addLayout(horizontalLayout_7);


        verticalLayout_3->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_3);

        networkConnectButton = new QPushButton(frame_2);
        networkConnectButton->setObjectName(QString::fromUtf8("networkConnectButton"));
        networkConnectButton->setEnabled(false);

        horizontalLayout_9->addWidget(networkConnectButton);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_11);

        gangMotionCheckBox = new QCheckBox(frame_2);
        gangMotionCheckBox->setObjectName(QString::fromUtf8("gangMotionCheckBox"));
        gangMotionCheckBox->setEnabled(false);

        horizontalLayout_9->addWidget(gangMotionCheckBox);

        disableMotionButton = new QPushButton(frame_2);
        disableMotionButton->setObjectName(QString::fromUtf8("disableMotionButton"));
        disableMotionButton->setEnabled(false);

        horizontalLayout_9->addWidget(disableMotionButton);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_4);

        displayBlankingButton = new QPushButton(frame_2);
        displayBlankingButton->setObjectName(QString::fromUtf8("displayBlankingButton"));
        displayBlankingButton->setEnabled(false);

        horizontalLayout_9->addWidget(displayBlankingButton);

        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_12);


        verticalLayout_3->addLayout(horizontalLayout_9);

        horizontalLayout_61 = new QHBoxLayout();
        horizontalLayout_61->setObjectName(QString::fromUtf8("horizontalLayout_61"));
        enableTimedTrialCheckBox = new QCheckBox(frame_2);
        enableTimedTrialCheckBox->setObjectName(QString::fromUtf8("enableTimedTrialCheckBox"));

        horizontalLayout_61->addWidget(enableTimedTrialCheckBox);

        timeTrialEditor = new QTimeEdit(frame_2);
        timeTrialEditor->setObjectName(QString::fromUtf8("timeTrialEditor"));
        timeTrialEditor->setCurrentSection(QDateTimeEdit::HourSection);

        horizontalLayout_61->addWidget(timeTrialEditor);

        horizontalSpacer_19 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_61->addItem(horizontalSpacer_19);


        verticalLayout_3->addLayout(horizontalLayout_61);


        verticalLayout_4->addWidget(frame_2);


        horizontalLayout_32->addLayout(verticalLayout_4);


        verticalLayout_38->addLayout(horizontalLayout_32);

        groupBox_10 = new QGroupBox(frame);
        groupBox_10->setObjectName(QString::fromUtf8("groupBox_10"));
        horizontalLayout_68 = new QHBoxLayout(groupBox_10);
        horizontalLayout_68->setObjectName(QString::fromUtf8("horizontalLayout_68"));
        horizontalLayout_67 = new QHBoxLayout();
        horizontalLayout_67->setObjectName(QString::fromUtf8("horizontalLayout_67"));
        rewindButton = new QToolButton(groupBox_10);
        rewindButton->setObjectName(QString::fromUtf8("rewindButton"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/Rewind.png"), QSize(), QIcon::Normal, QIcon::Off);
        rewindButton->setIcon(icon);
        rewindButton->setIconSize(QSize(32, 32));

        horizontalLayout_67->addWidget(rewindButton);

        playBackwardsButton = new QToolButton(groupBox_10);
        playBackwardsButton->setObjectName(QString::fromUtf8("playBackwardsButton"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/Back.png"), QSize(), QIcon::Normal, QIcon::Off);
        playBackwardsButton->setIcon(icon1);
        playBackwardsButton->setIconSize(QSize(32, 32));

        horizontalLayout_67->addWidget(playBackwardsButton);

        playForwardButton = new QToolButton(groupBox_10);
        playForwardButton->setObjectName(QString::fromUtf8("playForwardButton"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/Play.png"), QSize(), QIcon::Normal, QIcon::Off);
        playForwardButton->setIcon(icon2);
        playForwardButton->setIconSize(QSize(32, 32));

        horizontalLayout_67->addWidget(playForwardButton);

        recordButton = new QToolButton(groupBox_10);
        recordButton->setObjectName(QString::fromUtf8("recordButton"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/Record.png"), QSize(), QIcon::Normal, QIcon::Off);
        recordButton->setIcon(icon3);
        recordButton->setIconSize(QSize(32, 32));

        horizontalLayout_67->addWidget(recordButton);

        strideLength = new QSpinBox(groupBox_10);
        strideLength->setObjectName(QString::fromUtf8("strideLength"));
        strideLength->setMinimum(1);

        horizontalLayout_67->addWidget(strideLength);

        timeSlider = new QSlider(groupBox_10);
        timeSlider->setObjectName(QString::fromUtf8("timeSlider"));
        timeSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_67->addWidget(timeSlider);

        timeLabel = new QDoubleSpinBox(groupBox_10);
        timeLabel->setObjectName(QString::fromUtf8("timeLabel"));
        timeLabel->setFrame(false);
        timeLabel->setReadOnly(true);
        timeLabel->setButtonSymbols(QAbstractSpinBox::NoButtons);
        timeLabel->setDecimals(4);

        horizontalLayout_67->addWidget(timeLabel);


        horizontalLayout_68->addLayout(horizontalLayout_67);


        verticalLayout_38->addWidget(groupBox_10);


        horizontalLayout_2->addWidget(frame);

        tabWidget->addTab(tab, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QString::fromUtf8("tab_5"));
        horizontalLayout_44 = new QHBoxLayout(tab_5);
        horizontalLayout_44->setSpacing(6);
        horizontalLayout_44->setObjectName(QString::fromUtf8("horizontalLayout_44"));
        displayBox = new QGroupBox(tab_5);
        displayBox->setObjectName(QString::fromUtf8("displayBox"));
        horizontalLayout_45 = new QHBoxLayout(displayBox);
        horizontalLayout_45->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_45->setObjectName(QString::fromUtf8("horizontalLayout_45"));
        displayWidget = new QStackedWidget(displayBox);
        displayWidget->setObjectName(QString::fromUtf8("displayWidget"));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        displayWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        displayWidget->addWidget(page_2);

        horizontalLayout_45->addWidget(displayWidget);


        horizontalLayout_44->addWidget(displayBox);

        verticalLayout_16 = new QVBoxLayout();
        verticalLayout_16->setObjectName(QString::fromUtf8("verticalLayout_16"));
        verticalLayout_13 = new QVBoxLayout();
        verticalLayout_13->setObjectName(QString::fromUtf8("verticalLayout_13"));
        resetButton = new QPushButton(tab_5);
        resetButton->setObjectName(QString::fromUtf8("resetButton"));

        verticalLayout_13->addWidget(resetButton);

        distributeHoriz = new QPushButton(tab_5);
        distributeHoriz->setObjectName(QString::fromUtf8("distributeHoriz"));

        verticalLayout_13->addWidget(distributeHoriz);

        distributeVert = new QPushButton(tab_5);
        distributeVert->setObjectName(QString::fromUtf8("distributeVert"));

        verticalLayout_13->addWidget(distributeVert);

        smoothButton = new QPushButton(tab_5);
        smoothButton->setObjectName(QString::fromUtf8("smoothButton"));

        verticalLayout_13->addWidget(smoothButton);

        linearizeEdgesButton = new QPushButton(tab_5);
        linearizeEdgesButton->setObjectName(QString::fromUtf8("linearizeEdgesButton"));

        verticalLayout_13->addWidget(linearizeEdgesButton);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_13->addItem(verticalSpacer_3);


        verticalLayout_16->addLayout(verticalLayout_13);

        framePackingEnabled = new QCheckBox(tab_5);
        framePackingEnabled->setObjectName(QString::fromUtf8("framePackingEnabled"));

        verticalLayout_16->addWidget(framePackingEnabled);


        horizontalLayout_44->addLayout(verticalLayout_16);

        horizontalLayout_44->setStretch(0, 10);
        tabWidget->addTab(tab_5, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        layoutWidget = new QWidget(tab_2);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 70, 390, 37));
        horizontalLayout_14 = new QHBoxLayout(layoutWidget);
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        horizontalLayout_14->setContentsMargins(0, 0, 0, 0);
        diffuseLightButton = new QPushButton(layoutWidget);
        diffuseLightButton->setObjectName(QString::fromUtf8("diffuseLightButton"));

        horizontalLayout_14->addWidget(diffuseLightButton);

        diffuseLabel = new QLabel(layoutWidget);
        diffuseLabel->setObjectName(QString::fromUtf8("diffuseLabel"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(diffuseLabel->sizePolicy().hasHeightForWidth());
        diffuseLabel->setSizePolicy(sizePolicy);
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(25, 25, 25, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        QBrush brush2(QColor(37, 37, 37, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush2);
        QBrush brush3(QColor(31, 31, 31, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        QBrush brush4(QColor(12, 12, 12, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush4);
        QBrush brush5(QColor(16, 16, 16, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        QBrush brush6(QColor(0, 0, 0, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush6);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush6);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush4);
        QBrush brush7(QColor(255, 255, 220, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush4);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush6);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush6);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush6);
        diffuseLabel->setPalette(palette);
        diffuseLabel->setFrameShape(QFrame::StyledPanel);

        horizontalLayout_14->addWidget(diffuseLabel);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_14->addItem(horizontalSpacer_7);

        label_18 = new QLabel(layoutWidget);
        label_18->setObjectName(QString::fromUtf8("label_18"));

        horizontalLayout_14->addWidget(label_18);

        diffusePowerLevel = new QSpinBox(layoutWidget);
        diffusePowerLevel->setObjectName(QString::fromUtf8("diffusePowerLevel"));
        diffusePowerLevel->setMinimum(0);
        diffusePowerLevel->setMaximum(100);
        diffusePowerLevel->setValue(10);

        horizontalLayout_14->addWidget(diffusePowerLevel);

        layoutWidget1 = new QWidget(tab_2);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(20, 20, 291, 32));
        horizontalLayout_13 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        horizontalLayout_13->setContentsMargins(0, 0, 0, 0);
        ambientLightButton = new QPushButton(layoutWidget1);
        ambientLightButton->setObjectName(QString::fromUtf8("ambientLightButton"));

        horizontalLayout_13->addWidget(ambientLightButton);

        ambientLabel = new QLabel(layoutWidget1);
        ambientLabel->setObjectName(QString::fromUtf8("ambientLabel"));
        sizePolicy.setHeightForWidth(ambientLabel->sizePolicy().hasHeightForWidth());
        ambientLabel->setSizePolicy(sizePolicy);
        ambientLabel->setFrameShape(QFrame::StyledPanel);

        horizontalLayout_13->addWidget(ambientLabel);

        layoutWidget_2 = new QWidget(tab_2);
        layoutWidget_2->setObjectName(QString::fromUtf8("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(20, 120, 291, 32));
        horizontalLayout_18 = new QHBoxLayout(layoutWidget_2);
        horizontalLayout_18->setObjectName(QString::fromUtf8("horizontalLayout_18"));
        horizontalLayout_18->setContentsMargins(0, 0, 0, 0);
        backgroundColorButton = new QPushButton(layoutWidget_2);
        backgroundColorButton->setObjectName(QString::fromUtf8("backgroundColorButton"));

        horizontalLayout_18->addWidget(backgroundColorButton);

        backgroundLabel = new QLabel(layoutWidget_2);
        backgroundLabel->setObjectName(QString::fromUtf8("backgroundLabel"));
        sizePolicy.setHeightForWidth(backgroundLabel->sizePolicy().hasHeightForWidth());
        backgroundLabel->setSizePolicy(sizePolicy);
        backgroundLabel->setAutoFillBackground(false);
        backgroundLabel->setFrameShape(QFrame::StyledPanel);

        horizontalLayout_18->addWidget(backgroundLabel);

        layoutWidget_4 = new QWidget(tab_2);
        layoutWidget_4->setObjectName(QString::fromUtf8("layoutWidget_4"));
        layoutWidget_4->setGeometry(QRect(20, 160, 291, 37));
        horizontalLayout_35 = new QHBoxLayout(layoutWidget_4);
        horizontalLayout_35->setObjectName(QString::fromUtf8("horizontalLayout_35"));
        horizontalLayout_35->setContentsMargins(0, 0, 0, 0);
        globalBrightnessButton = new QPushButton(layoutWidget_4);
        globalBrightnessButton->setObjectName(QString::fromUtf8("globalBrightnessButton"));

        horizontalLayout_35->addWidget(globalBrightnessButton);

        brightnessLabel = new QLabel(layoutWidget_4);
        brightnessLabel->setObjectName(QString::fromUtf8("brightnessLabel"));
        sizePolicy.setHeightForWidth(brightnessLabel->sizePolicy().hasHeightForWidth());
        brightnessLabel->setSizePolicy(sizePolicy);
        brightnessLabel->setFrameShape(QFrame::StyledPanel);

        horizontalLayout_35->addWidget(brightnessLabel);

        layoutWidget_3 = new QWidget(tab_2);
        layoutWidget_3->setObjectName(QString::fromUtf8("layoutWidget_3"));
        layoutWidget_3->setGeometry(QRect(20, 210, 239, 26));
        horizontalLayout_50 = new QHBoxLayout(layoutWidget_3);
        horizontalLayout_50->setObjectName(QString::fromUtf8("horizontalLayout_50"));
        horizontalLayout_50->setContentsMargins(0, 0, 0, 0);
        label_30 = new QLabel(layoutWidget_3);
        label_30->setObjectName(QString::fromUtf8("label_30"));

        horizontalLayout_50->addWidget(label_30);

        shaderComboBox = new QComboBox(layoutWidget_3);
        shaderComboBox->setObjectName(QString::fromUtf8("shaderComboBox"));

        horizontalLayout_50->addWidget(shaderComboBox);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        verticalLayout_7 = new QVBoxLayout(tab_3);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        horizontalLayout_29 = new QHBoxLayout();
        horizontalLayout_29->setSpacing(6);
        horizontalLayout_29->setObjectName(QString::fromUtf8("horizontalLayout_29"));
        label_10 = new QLabel(tab_3);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setTextFormat(Qt::RichText);
        label_10->setWordWrap(true);

        horizontalLayout_29->addWidget(label_10);

        horizontalSpacer_5 = new QSpacerItem(60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_29->addItem(horizontalSpacer_5);

        label_14 = new QLabel(tab_3);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setPixmap(QPixmap(QString::fromUtf8(":/images/axis.jpg")));

        horizontalLayout_29->addWidget(label_14);

        horizontalLayout_29->setStretch(0, 12);

        verticalLayout_7->addLayout(horizontalLayout_29);

        horizontalLayout_30 = new QHBoxLayout();
        horizontalLayout_30->setObjectName(QString::fromUtf8("horizontalLayout_30"));
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_30->addItem(horizontalSpacer_6);

        computeAverage = new QCheckBox(tab_3);
        computeAverage->setObjectName(QString::fromUtf8("computeAverage"));

        horizontalLayout_30->addWidget(computeAverage);

        editValues = new QCheckBox(tab_3);
        editValues->setObjectName(QString::fromUtf8("editValues"));

        horizontalLayout_30->addWidget(editValues);

        horizontalLayout_30->setStretch(0, 2);
        horizontalLayout_30->setStretch(1, 6);
        horizontalLayout_30->setStretch(2, 6);

        verticalLayout_7->addLayout(horizontalLayout_30);

        horizontalLayout_25 = new QHBoxLayout();
        horizontalLayout_25->setObjectName(QString::fromUtf8("horizontalLayout_25"));
        label_11 = new QLabel(tab_3);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        horizontalLayout_25->addWidget(label_11);

        calibrateXButton = new QPushButton(tab_3);
        calibrateXButton->setObjectName(QString::fromUtf8("calibrateXButton"));

        horizontalLayout_25->addWidget(calibrateXButton);

        calibrateXValue = new QLineEdit(tab_3);
        calibrateXValue->setObjectName(QString::fromUtf8("calibrateXValue"));
        calibrateXValue->setReadOnly(true);

        horizontalLayout_25->addWidget(calibrateXValue);

        calibrateXGain = new QDoubleSpinBox(tab_3);
        calibrateXGain->setObjectName(QString::fromUtf8("calibrateXGain"));
        calibrateXGain->setDecimals(4);
        calibrateXGain->setMinimum(0.0001);
        calibrateXGain->setMaximum(1000);
        calibrateXGain->setSingleStep(0.1);
        calibrateXGain->setValue(1);

        horizontalLayout_25->addWidget(calibrateXGain);


        verticalLayout_7->addLayout(horizontalLayout_25);

        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        verticalLayout_7->addItem(verticalSpacer);

        horizontalLayout_26 = new QHBoxLayout();
        horizontalLayout_26->setObjectName(QString::fromUtf8("horizontalLayout_26"));
        label_12 = new QLabel(tab_3);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        horizontalLayout_26->addWidget(label_12);

        calibrateYButton = new QPushButton(tab_3);
        calibrateYButton->setObjectName(QString::fromUtf8("calibrateYButton"));

        horizontalLayout_26->addWidget(calibrateYButton);

        calibrateYValue = new QLineEdit(tab_3);
        calibrateYValue->setObjectName(QString::fromUtf8("calibrateYValue"));
        calibrateYValue->setReadOnly(true);

        horizontalLayout_26->addWidget(calibrateYValue);

        calibrateYGain = new QDoubleSpinBox(tab_3);
        calibrateYGain->setObjectName(QString::fromUtf8("calibrateYGain"));
        calibrateYGain->setDecimals(4);
        calibrateYGain->setMinimum(0.0001);
        calibrateYGain->setMaximum(1000);
        calibrateYGain->setSingleStep(0.1);
        calibrateYGain->setValue(1);

        horizontalLayout_26->addWidget(calibrateYGain);


        verticalLayout_7->addLayout(horizontalLayout_26);

        verticalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        verticalLayout_7->addItem(verticalSpacer_2);

        horizontalLayout_27 = new QHBoxLayout();
        horizontalLayout_27->setObjectName(QString::fromUtf8("horizontalLayout_27"));
        label_13 = new QLabel(tab_3);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        horizontalLayout_27->addWidget(label_13);

        calibrateZButton = new QPushButton(tab_3);
        calibrateZButton->setObjectName(QString::fromUtf8("calibrateZButton"));

        horizontalLayout_27->addWidget(calibrateZButton);

        calibrateZValue = new QLineEdit(tab_3);
        calibrateZValue->setObjectName(QString::fromUtf8("calibrateZValue"));
        calibrateZValue->setReadOnly(true);

        horizontalLayout_27->addWidget(calibrateZValue);

        calibrateZGain = new QDoubleSpinBox(tab_3);
        calibrateZGain->setObjectName(QString::fromUtf8("calibrateZGain"));
        calibrateZGain->setDecimals(4);
        calibrateZGain->setMinimum(0.0001);
        calibrateZGain->setMaximum(1000);
        calibrateZGain->setSingleStep(0.1);
        calibrateZGain->setValue(1);

        horizontalLayout_27->addWidget(calibrateZGain);


        verticalLayout_7->addLayout(horizontalLayout_27);

        horizontalLayout_31 = new QHBoxLayout();
        horizontalLayout_31->setObjectName(QString::fromUtf8("horizontalLayout_31"));
        horizontalSpacer_13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_31->addItem(horizontalSpacer_13);

        updateCalibrationValuesButton = new QPushButton(tab_3);
        updateCalibrationValuesButton->setObjectName(QString::fromUtf8("updateCalibrationValuesButton"));

        horizontalLayout_31->addWidget(updateCalibrationValuesButton);

        horizontalSpacer_14 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_31->addItem(horizontalSpacer_14);


        verticalLayout_7->addLayout(horizontalLayout_31);

        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        gridLayout = new QGridLayout(tab_4);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        groupBox_8 = new QGroupBox(tab_4);
        groupBox_8->setObjectName(QString::fromUtf8("groupBox_8"));
        verticalLayout_23 = new QVBoxLayout(groupBox_8);
        verticalLayout_23->setObjectName(QString::fromUtf8("verticalLayout_23"));
        horizontalLayout_55 = new QHBoxLayout();
        horizontalLayout_55->setObjectName(QString::fromUtf8("horizontalLayout_55"));
        label_33 = new QLabel(groupBox_8);
        label_33->setObjectName(QString::fromUtf8("label_33"));

        horizontalLayout_55->addWidget(label_33);

        ballRadiusBox = new QDoubleSpinBox(groupBox_8);
        ballRadiusBox->setObjectName(QString::fromUtf8("ballRadiusBox"));
        ballRadiusBox->setDecimals(3);
        ballRadiusBox->setMaximum(1524);
        ballRadiusBox->setValue(6);

        horizontalLayout_55->addWidget(ballRadiusBox);


        verticalLayout_23->addLayout(horizontalLayout_55);

        horizontalLayout_28 = new QHBoxLayout();
        horizontalLayout_28->setObjectName(QString::fromUtf8("horizontalLayout_28"));
        label_15 = new QLabel(groupBox_8);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_15->sizePolicy().hasHeightForWidth());
        label_15->setSizePolicy(sizePolicy1);

        horizontalLayout_28->addWidget(label_15);

        crossbarEnabledCheckBox = new QCheckBox(groupBox_8);
        crossbarEnabledCheckBox->setObjectName(QString::fromUtf8("crossbarEnabledCheckBox"));
        QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(crossbarEnabledCheckBox->sizePolicy().hasHeightForWidth());
        crossbarEnabledCheckBox->setSizePolicy(sizePolicy2);

        horizontalLayout_28->addWidget(crossbarEnabledCheckBox);

        label_34 = new QLabel(groupBox_8);
        label_34->setObjectName(QString::fromUtf8("label_34"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(label_34->sizePolicy().hasHeightForWidth());
        label_34->setSizePolicy(sizePolicy3);

        horizontalLayout_28->addWidget(label_34);

        crossbarWidthSpinBox = new QDoubleSpinBox(groupBox_8);
        crossbarWidthSpinBox->setObjectName(QString::fromUtf8("crossbarWidthSpinBox"));
        sizePolicy2.setHeightForWidth(crossbarWidthSpinBox->sizePolicy().hasHeightForWidth());
        crossbarWidthSpinBox->setSizePolicy(sizePolicy2);
        crossbarWidthSpinBox->setDecimals(3);
        crossbarWidthSpinBox->setMaximum(1524);
        crossbarWidthSpinBox->setValue(6);

        horizontalLayout_28->addWidget(crossbarWidthSpinBox);


        verticalLayout_23->addLayout(horizontalLayout_28);

        horizontalLayout_65 = new QHBoxLayout();
        horizontalLayout_65->setObjectName(QString::fromUtf8("horizontalLayout_65"));
        label_35 = new QLabel(groupBox_8);
        label_35->setObjectName(QString::fromUtf8("label_35"));

        horizontalLayout_65->addWidget(label_35);

        minimumVelocityThreshold = new QDoubleSpinBox(groupBox_8);
        minimumVelocityThreshold->setObjectName(QString::fromUtf8("minimumVelocityThreshold"));
        minimumVelocityThreshold->setDecimals(8);
        minimumVelocityThreshold->setValue(1e-06);

        horizontalLayout_65->addWidget(minimumVelocityThreshold);


        verticalLayout_23->addLayout(horizontalLayout_65);

        restrictVerticalMotion = new QCheckBox(groupBox_8);
        restrictVerticalMotion->setObjectName(QString::fromUtf8("restrictVerticalMotion"));

        verticalLayout_23->addWidget(restrictVerticalMotion);


        gridLayout->addWidget(groupBox_8, 1, 0, 1, 1);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        rotatedCamerasCheckBox = new QCheckBox(tab_4);
        rotatedCamerasCheckBox->setObjectName(QString::fromUtf8("rotatedCamerasCheckBox"));
        rotatedCamerasCheckBox->setChecked(true);

        verticalLayout_8->addWidget(rotatedCamerasCheckBox);

        horizontalLayout_33 = new QHBoxLayout();
        horizontalLayout_33->setObjectName(QString::fromUtf8("horizontalLayout_33"));
        label_19 = new QLabel(tab_4);
        label_19->setObjectName(QString::fromUtf8("label_19"));

        horizontalLayout_33->addWidget(label_19);

        fovSlider = new QSlider(tab_4);
        fovSlider->setObjectName(QString::fromUtf8("fovSlider"));
        fovSlider->setMinimum(30);
        fovSlider->setMaximum(270);
        fovSlider->setValue(90);
        fovSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_33->addWidget(fovSlider);

        spinBox = new QSpinBox(tab_4);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setMinimum(30);
        spinBox->setMaximum(270);
        spinBox->setValue(90);

        horizontalLayout_33->addWidget(spinBox);


        verticalLayout_8->addLayout(horizontalLayout_33);

        horizontalLayout_34 = new QHBoxLayout();
        horizontalLayout_34->setObjectName(QString::fromUtf8("horizontalLayout_34"));
        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_34->addItem(horizontalSpacer_8);

        label_20 = new QLabel(tab_4);
        label_20->setObjectName(QString::fromUtf8("label_20"));

        horizontalLayout_34->addWidget(label_20);

        fovOffsetSlider = new QSlider(tab_4);
        fovOffsetSlider->setObjectName(QString::fromUtf8("fovOffsetSlider"));
        fovOffsetSlider->setMaximum(1800);
        fovOffsetSlider->setValue(564);
        fovOffsetSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_34->addWidget(fovOffsetSlider);

        fovOffsetSpinBox = new QDoubleSpinBox(tab_4);
        fovOffsetSpinBox->setObjectName(QString::fromUtf8("fovOffsetSpinBox"));
        fovOffsetSpinBox->setDecimals(1);
        fovOffsetSpinBox->setMaximum(180);
        fovOffsetSpinBox->setSingleStep(0.1);
        fovOffsetSpinBox->setValue(56.4);

        horizontalLayout_34->addWidget(fovOffsetSpinBox);


        verticalLayout_8->addLayout(horizontalLayout_34);


        gridLayout->addLayout(verticalLayout_8, 0, 1, 1, 1);

        groupBox_16 = new QGroupBox(tab_4);
        groupBox_16->setObjectName(QString::fromUtf8("groupBox_16"));
        verticalLayout_35 = new QVBoxLayout(groupBox_16);
        verticalLayout_35->setObjectName(QString::fromUtf8("verticalLayout_35"));
        horizontalLayout_64 = new QHBoxLayout();
        horizontalLayout_64->setObjectName(QString::fromUtf8("horizontalLayout_64"));
        velocitySmoothingEnabled = new QCheckBox(groupBox_16);
        velocitySmoothingEnabled->setObjectName(QString::fromUtf8("velocitySmoothingEnabled"));

        horizontalLayout_64->addWidget(velocitySmoothingEnabled);

        horizontalLayout_102 = new QHBoxLayout();
        horizontalLayout_102->setObjectName(QString::fromUtf8("horizontalLayout_102"));
        label_65 = new QLabel(groupBox_16);
        label_65->setObjectName(QString::fromUtf8("label_65"));

        horizontalLayout_102->addWidget(label_65);

        velocitySmoothingInterval = new QSpinBox(groupBox_16);
        velocitySmoothingInterval->setObjectName(QString::fromUtf8("velocitySmoothingInterval"));
        velocitySmoothingInterval->setMinimum(1);
        velocitySmoothingInterval->setMaximum(10000);
        velocitySmoothingInterval->setValue(32);

        horizontalLayout_102->addWidget(velocitySmoothingInterval);


        horizontalLayout_64->addLayout(horizontalLayout_102);


        verticalLayout_35->addLayout(horizontalLayout_64);

        frame_3 = new QFrame(groupBox_16);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        verticalLayout_36 = new QVBoxLayout(frame_3);
        verticalLayout_36->setObjectName(QString::fromUtf8("verticalLayout_36"));
        label_38 = new QLabel(frame_3);
        label_38->setObjectName(QString::fromUtf8("label_38"));

        verticalLayout_36->addWidget(label_38);

        horizontalLayout_101 = new QHBoxLayout();
        horizontalLayout_101->setObjectName(QString::fromUtf8("horizontalLayout_101"));
        horizontalSpacer_22 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_101->addItem(horizontalSpacer_22);

        inputSmoothingEnabled = new QCheckBox(frame_3);
        inputSmoothingEnabled->setObjectName(QString::fromUtf8("inputSmoothingEnabled"));

        horizontalLayout_101->addWidget(inputSmoothingEnabled);

        horizontalSpacer_23 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_101->addItem(horizontalSpacer_23);

        preBlendLabel = new QLabel(frame_3);
        preBlendLabel->setObjectName(QString::fromUtf8("preBlendLabel"));

        horizontalLayout_101->addWidget(preBlendLabel);

        inputSmoothingInterval = new QSpinBox(frame_3);
        inputSmoothingInterval->setObjectName(QString::fromUtf8("inputSmoothingInterval"));
        inputSmoothingInterval->setMinimum(1);
        inputSmoothingInterval->setMaximum(10000);
        inputSmoothingInterval->setValue(32);

        horizontalLayout_101->addWidget(inputSmoothingInterval);


        verticalLayout_36->addLayout(horizontalLayout_101);

        horizontalLayout_104 = new QHBoxLayout();
        horizontalLayout_104->setObjectName(QString::fromUtf8("horizontalLayout_104"));
        horizontalSpacer_27 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_104->addItem(horizontalSpacer_27);

        intermediarySmoothingEnabled = new QCheckBox(frame_3);
        intermediarySmoothingEnabled->setObjectName(QString::fromUtf8("intermediarySmoothingEnabled"));

        horizontalLayout_104->addWidget(intermediarySmoothingEnabled);

        horizontalSpacer_28 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_104->addItem(horizontalSpacer_28);

        preBlendLabel_2 = new QLabel(frame_3);
        preBlendLabel_2->setObjectName(QString::fromUtf8("preBlendLabel_2"));

        horizontalLayout_104->addWidget(preBlendLabel_2);

        intermediarySmoothingInterval = new QSpinBox(frame_3);
        intermediarySmoothingInterval->setObjectName(QString::fromUtf8("intermediarySmoothingInterval"));
        intermediarySmoothingInterval->setMinimum(1);
        intermediarySmoothingInterval->setMaximum(10000);
        intermediarySmoothingInterval->setValue(32);

        horizontalLayout_104->addWidget(intermediarySmoothingInterval);


        verticalLayout_36->addLayout(horizontalLayout_104);

        postBlendFrame = new QFrame(frame_3);
        postBlendFrame->setObjectName(QString::fromUtf8("postBlendFrame"));
        postBlendFrame->setEnabled(true);
        postBlendFrame->setFrameShape(QFrame::NoFrame);
        postBlendFrame->setFrameShadow(QFrame::Plain);
        verticalLayout_37 = new QVBoxLayout(postBlendFrame);
        verticalLayout_37->setContentsMargins(0, 0, 0, 0);
        verticalLayout_37->setObjectName(QString::fromUtf8("verticalLayout_37"));
        horizontalLayout_103 = new QHBoxLayout();
        horizontalLayout_103->setObjectName(QString::fromUtf8("horizontalLayout_103"));
        horizontalSpacer_21 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_103->addItem(horizontalSpacer_21);

        outputSmoothingEnabled = new QCheckBox(postBlendFrame);
        outputSmoothingEnabled->setObjectName(QString::fromUtf8("outputSmoothingEnabled"));

        horizontalLayout_103->addWidget(outputSmoothingEnabled);

        horizontalSpacer_26 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_103->addItem(horizontalSpacer_26);

        label_66 = new QLabel(postBlendFrame);
        label_66->setObjectName(QString::fromUtf8("label_66"));

        horizontalLayout_103->addWidget(label_66);

        outputSmoothingInterval = new QSpinBox(postBlendFrame);
        outputSmoothingInterval->setObjectName(QString::fromUtf8("outputSmoothingInterval"));
        outputSmoothingInterval->setMinimum(1);
        outputSmoothingInterval->setMaximum(10000);
        outputSmoothingInterval->setValue(32);

        horizontalLayout_103->addWidget(outputSmoothingInterval);


        verticalLayout_37->addLayout(horizontalLayout_103);


        verticalLayout_36->addWidget(postBlendFrame);


        verticalLayout_35->addWidget(frame_3);


        gridLayout->addWidget(groupBox_16, 0, 0, 1, 1);

        groupBox_5 = new QGroupBox(tab_4);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        verticalLayout_12 = new QVBoxLayout(groupBox_5);
        verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
        horizontalLayout_60 = new QHBoxLayout();
        horizontalLayout_60->setObjectName(QString::fromUtf8("horizontalLayout_60"));
        horizontalLayout_43 = new QHBoxLayout();
        horizontalLayout_43->setObjectName(QString::fromUtf8("horizontalLayout_43"));
        label_25 = new QLabel(groupBox_5);
        label_25->setObjectName(QString::fromUtf8("label_25"));

        horizontalLayout_43->addWidget(label_25);

        portNameControl = new QLineEdit(groupBox_5);
        portNameControl->setObjectName(QString::fromUtf8("portNameControl"));

        horizontalLayout_43->addWidget(portNameControl);


        horizontalLayout_60->addLayout(horizontalLayout_43);

        horizontalLayout_54 = new QHBoxLayout();
        horizontalLayout_54->setObjectName(QString::fromUtf8("horizontalLayout_54"));
        label_31 = new QLabel(groupBox_5);
        label_31->setObjectName(QString::fromUtf8("label_31"));

        horizontalLayout_54->addWidget(label_31);

        outputRateSpinBox = new QSpinBox(groupBox_5);
        outputRateSpinBox->setObjectName(QString::fromUtf8("outputRateSpinBox"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(outputRateSpinBox->sizePolicy().hasHeightForWidth());
        outputRateSpinBox->setSizePolicy(sizePolicy4);
        outputRateSpinBox->setMinimum(1);
        outputRateSpinBox->setMaximum(1024);
        outputRateSpinBox->setSingleStep(5);
        outputRateSpinBox->setValue(60);

        horizontalLayout_54->addWidget(outputRateSpinBox);


        horizontalLayout_60->addLayout(horizontalLayout_54);


        verticalLayout_12->addLayout(horizontalLayout_60);

        horizontalLayout_53 = new QHBoxLayout();
        horizontalLayout_53->setSpacing(0);
        horizontalLayout_53->setObjectName(QString::fromUtf8("horizontalLayout_53"));
        fileExportToggle = new QCheckBox(groupBox_5);
        fileExportToggle->setObjectName(QString::fromUtf8("fileExportToggle"));
        fileExportToggle->setLayoutDirection(Qt::RightToLeft);

        horizontalLayout_53->addWidget(fileExportToggle);

        horizontalSpacer_20 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_53->addItem(horizontalSpacer_20);

        label_17 = new QLabel(groupBox_5);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        horizontalLayout_53->addWidget(label_17);

        exportFileNameBox = new QLineEdit(groupBox_5);
        exportFileNameBox->setObjectName(QString::fromUtf8("exportFileNameBox"));

        horizontalLayout_53->addWidget(exportFileNameBox);

        exportFileNameSelector = new QToolButton(groupBox_5);
        exportFileNameSelector->setObjectName(QString::fromUtf8("exportFileNameSelector"));

        horizontalLayout_53->addWidget(exportFileNameSelector);

        horizontalSpacer_18 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_53->addItem(horizontalSpacer_18);

        label_16 = new QLabel(groupBox_5);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        horizontalLayout_53->addWidget(label_16);

        exportFileNameStyle = new QComboBox(groupBox_5);
        exportFileNameStyle->setObjectName(QString::fromUtf8("exportFileNameStyle"));

        horizontalLayout_53->addWidget(exportFileNameStyle);


        verticalLayout_12->addLayout(horizontalLayout_53);

        horizontalLayout_59 = new QHBoxLayout();
        horizontalLayout_59->setObjectName(QString::fromUtf8("horizontalLayout_59"));
        treadmillOutputToggle = new QCheckBox(groupBox_5);
        treadmillOutputToggle->setObjectName(QString::fromUtf8("treadmillOutputToggle"));
        treadmillOutputToggle->setLayoutDirection(Qt::RightToLeft);

        horizontalLayout_59->addWidget(treadmillOutputToggle);

        horizontalSpacer_24 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_59->addItem(horizontalSpacer_24);

        reducedOutputToggle = new QCheckBox(groupBox_5);
        reducedOutputToggle->setObjectName(QString::fromUtf8("reducedOutputToggle"));
        reducedOutputToggle->setLayoutDirection(Qt::RightToLeft);

        horizontalLayout_59->addWidget(reducedOutputToggle);

        horizontalSpacer_25 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_59->addItem(horizontalSpacer_25);


        verticalLayout_12->addLayout(horizontalLayout_59);


        gridLayout->addWidget(groupBox_5, 2, 0, 1, 1);

        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        horizontalLayout_37 = new QHBoxLayout();
        horizontalLayout_37->setObjectName(QString::fromUtf8("horizontalLayout_37"));
        label_21 = new QLabel(tab_4);
        label_21->setObjectName(QString::fromUtf8("label_21"));

        horizontalLayout_37->addWidget(label_21);

        frameModeComboBox = new QComboBox(tab_4);
        frameModeComboBox->setObjectName(QString::fromUtf8("frameModeComboBox"));

        horizontalLayout_37->addWidget(frameModeComboBox);


        verticalLayout_9->addLayout(horizontalLayout_37);

        horizontalLayout_38 = new QHBoxLayout();
        horizontalLayout_38->setObjectName(QString::fromUtf8("horizontalLayout_38"));
        horizontalLayout_38->setContentsMargins(-1, -1, 2, -1);
        label_23 = new QLabel(tab_4);
        label_23->setObjectName(QString::fromUtf8("label_23"));

        horizontalLayout_38->addWidget(label_23);

        indicatorSizeSlider = new QSlider(tab_4);
        indicatorSizeSlider->setObjectName(QString::fromUtf8("indicatorSizeSlider"));
        indicatorSizeSlider->setMaximum(1000);
        indicatorSizeSlider->setValue(100);
        indicatorSizeSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_38->addWidget(indicatorSizeSlider);

        indicatorSizeSpinBox = new QSpinBox(tab_4);
        indicatorSizeSpinBox->setObjectName(QString::fromUtf8("indicatorSizeSpinBox"));
        indicatorSizeSpinBox->setMaximum(1000);
        indicatorSizeSpinBox->setValue(100);

        horizontalLayout_38->addWidget(indicatorSizeSpinBox);


        verticalLayout_9->addLayout(horizontalLayout_38);

        horizontalLayout_42 = new QHBoxLayout();
        horizontalLayout_42->setSpacing(6);
        horizontalLayout_42->setObjectName(QString::fromUtf8("horizontalLayout_42"));
        horizontalLayout_42->setContentsMargins(-1, -1, 2, -1);
        label_24 = new QLabel(tab_4);
        label_24->setObjectName(QString::fromUtf8("label_24"));

        horizontalLayout_42->addWidget(label_24);

        borderSizeSlider = new QSlider(tab_4);
        borderSizeSlider->setObjectName(QString::fromUtf8("borderSizeSlider"));
        borderSizeSlider->setMaximum(200);
        borderSizeSlider->setValue(20);
        borderSizeSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_42->addWidget(borderSizeSlider);

        borderSizeSpinBox = new QSpinBox(tab_4);
        borderSizeSpinBox->setObjectName(QString::fromUtf8("borderSizeSpinBox"));
        borderSizeSpinBox->setMaximum(200);
        borderSizeSpinBox->setValue(20);

        horizontalLayout_42->addWidget(borderSizeSpinBox);


        verticalLayout_9->addLayout(horizontalLayout_42);

        groupBox = new QGroupBox(tab_4);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_11 = new QVBoxLayout(groupBox);
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        horizontalLayout_40 = new QHBoxLayout();
        horizontalLayout_40->setObjectName(QString::fromUtf8("horizontalLayout_40"));
        label_22 = new QLabel(groupBox);
        label_22->setObjectName(QString::fromUtf8("label_22"));

        horizontalLayout_40->addWidget(label_22);

        screenSelectSpinBox = new QSpinBox(groupBox);
        screenSelectSpinBox->setObjectName(QString::fromUtf8("screenSelectSpinBox"));
        screenSelectSpinBox->setEnabled(true);
        screenSelectSpinBox->setMinimum(1);
        screenSelectSpinBox->setMaximum(3);

        horizontalLayout_40->addWidget(screenSelectSpinBox);


        verticalLayout_11->addLayout(horizontalLayout_40);

        horizontalLayout_41 = new QHBoxLayout();
        horizontalLayout_41->setObjectName(QString::fromUtf8("horizontalLayout_41"));
        horizontalLayout_36 = new QHBoxLayout();
        horizontalLayout_36->setObjectName(QString::fromUtf8("horizontalLayout_36"));
        indicatorHorizontalPositionSlider = new QSlider(groupBox);
        indicatorHorizontalPositionSlider->setObjectName(QString::fromUtf8("indicatorHorizontalPositionSlider"));
        indicatorHorizontalPositionSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_36->addWidget(indicatorHorizontalPositionSlider);

        indicatorHorizontalPositionSpinBox = new QSpinBox(groupBox);
        indicatorHorizontalPositionSpinBox->setObjectName(QString::fromUtf8("indicatorHorizontalPositionSpinBox"));

        horizontalLayout_36->addWidget(indicatorHorizontalPositionSpinBox);


        horizontalLayout_41->addLayout(horizontalLayout_36);

        verticalLayout_10 = new QVBoxLayout();
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        horizontalLayout_39 = new QHBoxLayout();
        horizontalLayout_39->setSpacing(0);
        horizontalLayout_39->setObjectName(QString::fromUtf8("horizontalLayout_39"));
        horizontalSpacer_9 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout_39->addItem(horizontalSpacer_9);

        indicatorVerticalPositionSlider = new QSlider(groupBox);
        indicatorVerticalPositionSlider->setObjectName(QString::fromUtf8("indicatorVerticalPositionSlider"));
        indicatorVerticalPositionSlider->setOrientation(Qt::Vertical);

        horizontalLayout_39->addWidget(indicatorVerticalPositionSlider);

        horizontalSpacer_10 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout_39->addItem(horizontalSpacer_10);


        verticalLayout_10->addLayout(horizontalLayout_39);

        indicatorVerticalPositionSpinBox = new QSpinBox(groupBox);
        indicatorVerticalPositionSpinBox->setObjectName(QString::fromUtf8("indicatorVerticalPositionSpinBox"));

        verticalLayout_10->addWidget(indicatorVerticalPositionSpinBox);


        horizontalLayout_41->addLayout(verticalLayout_10);


        verticalLayout_11->addLayout(horizontalLayout_41);


        verticalLayout_9->addWidget(groupBox);


        gridLayout->addLayout(verticalLayout_9, 1, 1, 4, 1);

        verticalLayout_28 = new QVBoxLayout();
        verticalLayout_28->setObjectName(QString::fromUtf8("verticalLayout_28"));
        showDebugCheckBox = new QCheckBox(tab_4);
        showDebugCheckBox->setObjectName(QString::fromUtf8("showDebugCheckBox"));

        verticalLayout_28->addWidget(showDebugCheckBox);

        debugPanel = new QGroupBox(tab_4);
        debugPanel->setObjectName(QString::fromUtf8("debugPanel"));
        debugPanel->setEnabled(true);
        verticalLayout_21 = new QVBoxLayout(debugPanel);
        verticalLayout_21->setObjectName(QString::fromUtf8("verticalLayout_21"));
        verticalLayout_20 = new QVBoxLayout();
        verticalLayout_20->setObjectName(QString::fromUtf8("verticalLayout_20"));
        label_29 = new QLabel(debugPanel);
        label_29->setObjectName(QString::fromUtf8("label_29"));

        verticalLayout_20->addWidget(label_29);

        horizontalLayout_49 = new QHBoxLayout();
        horizontalLayout_49->setObjectName(QString::fromUtf8("horizontalLayout_49"));
        verticalLayout_17 = new QVBoxLayout();
        verticalLayout_17->setObjectName(QString::fromUtf8("verticalLayout_17"));
        label_26 = new QLabel(debugPanel);
        label_26->setObjectName(QString::fromUtf8("label_26"));

        verticalLayout_17->addWidget(label_26);

        xoffset = new QDoubleSpinBox(debugPanel);
        xoffset->setObjectName(QString::fromUtf8("xoffset"));
        xoffset->setDecimals(3);
        xoffset->setMinimum(-1);
        xoffset->setMaximum(1);
        xoffset->setSingleStep(0.01);
        xoffset->setValue(0.07);

        verticalLayout_17->addWidget(xoffset);


        horizontalLayout_49->addLayout(verticalLayout_17);

        verticalLayout_18 = new QVBoxLayout();
        verticalLayout_18->setObjectName(QString::fromUtf8("verticalLayout_18"));
        label_27 = new QLabel(debugPanel);
        label_27->setObjectName(QString::fromUtf8("label_27"));

        verticalLayout_18->addWidget(label_27);

        yoffset = new QDoubleSpinBox(debugPanel);
        yoffset->setObjectName(QString::fromUtf8("yoffset"));
        yoffset->setDecimals(3);
        yoffset->setMinimum(-1);
        yoffset->setMaximum(1);
        yoffset->setSingleStep(0.01);
        yoffset->setValue(-0.95);

        verticalLayout_18->addWidget(yoffset);


        horizontalLayout_49->addLayout(verticalLayout_18);

        verticalLayout_19 = new QVBoxLayout();
        verticalLayout_19->setObjectName(QString::fromUtf8("verticalLayout_19"));
        label_28 = new QLabel(debugPanel);
        label_28->setObjectName(QString::fromUtf8("label_28"));

        verticalLayout_19->addWidget(label_28);

        zoffset = new QDoubleSpinBox(debugPanel);
        zoffset->setObjectName(QString::fromUtf8("zoffset"));
        zoffset->setDecimals(3);
        zoffset->setMinimum(-1);
        zoffset->setMaximum(1);
        zoffset->setSingleStep(0.01);
        zoffset->setValue(-0.08);

        verticalLayout_19->addWidget(zoffset);


        horizontalLayout_49->addLayout(verticalLayout_19);


        verticalLayout_20->addLayout(horizontalLayout_49);


        verticalLayout_21->addLayout(verticalLayout_20);

        debugPhysicsCheckBox = new QCheckBox(debugPanel);
        debugPhysicsCheckBox->setObjectName(QString::fromUtf8("debugPhysicsCheckBox"));
        QFont font;
        font.setPointSize(10);
        debugPhysicsCheckBox->setFont(font);

        verticalLayout_21->addWidget(debugPhysicsCheckBox);

        showInvisibleObjectsCheckBox = new QCheckBox(debugPanel);
        showInvisibleObjectsCheckBox->setObjectName(QString::fromUtf8("showInvisibleObjectsCheckBox"));
        showInvisibleObjectsCheckBox->setFont(font);

        verticalLayout_21->addWidget(showInvisibleObjectsCheckBox);

        useCustomGLWidget = new QCheckBox(debugPanel);
        useCustomGLWidget->setObjectName(QString::fromUtf8("useCustomGLWidget"));
        QFont font1;
        font1.setPointSize(10);
        font1.setBold(false);
        font1.setWeight(50);
        useCustomGLWidget->setFont(font1);

        verticalLayout_21->addWidget(useCustomGLWidget);


        verticalLayout_28->addWidget(debugPanel);


        gridLayout->addLayout(verticalLayout_28, 5, 0, 1, 1);

        tabWidget->addTab(tab_4, QString());
        tab_6 = new QWidget();
        tab_6->setObjectName(QString::fromUtf8("tab_6"));
        horizontalLayout_58 = new QHBoxLayout(tab_6);
        horizontalLayout_58->setObjectName(QString::fromUtf8("horizontalLayout_58"));
        verticalLayout_29 = new QVBoxLayout();
        verticalLayout_29->setObjectName(QString::fromUtf8("verticalLayout_29"));
        headingStatusButton = new QPushButton(tab_6);
        headingStatusButton->setObjectName(QString::fromUtf8("headingStatusButton"));

        verticalLayout_29->addWidget(headingStatusButton);

        groupBox_15 = new QGroupBox(tab_6);
        groupBox_15->setObjectName(QString::fromUtf8("groupBox_15"));
        groupBox_15->setEnabled(true);
        verticalLayout_25 = new QVBoxLayout(groupBox_15);
        verticalLayout_25->setObjectName(QString::fromUtf8("verticalLayout_25"));
        thresholdTurningEnabled = new QCheckBox(groupBox_15);
        thresholdTurningEnabled->setObjectName(QString::fromUtf8("thresholdTurningEnabled"));

        verticalLayout_25->addWidget(thresholdTurningEnabled);

        horizontalLayout_62 = new QHBoxLayout();
        horizontalLayout_62->setObjectName(QString::fromUtf8("horizontalLayout_62"));
        verticalLayout_33 = new QVBoxLayout();
        verticalLayout_33->setObjectName(QString::fromUtf8("verticalLayout_33"));
        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Minimum);

        verticalLayout_33->addItem(verticalSpacer_6);

        minThresholdScale = new QDoubleSpinBox(groupBox_15);
        minThresholdScale->setObjectName(QString::fromUtf8("minThresholdScale"));
        minThresholdScale->setEnabled(false);
        minThresholdScale->setMaximum(1);
        minThresholdScale->setSingleStep(0.01);
        minThresholdScale->setValue(0.9);

        verticalLayout_33->addWidget(minThresholdScale);


        horizontalLayout_62->addLayout(verticalLayout_33);

        verticalLayout_24 = new QVBoxLayout();
        verticalLayout_24->setObjectName(QString::fromUtf8("verticalLayout_24"));
        horizontalLayout_46 = new QHBoxLayout();
        horizontalLayout_46->setObjectName(QString::fromUtf8("horizontalLayout_46"));
        label_36 = new QLabel(groupBox_15);
        label_36->setObjectName(QString::fromUtf8("label_36"));

        horizontalLayout_46->addWidget(label_36);

        thresholdWeight = new QDoubleSpinBox(groupBox_15);
        thresholdWeight->setObjectName(QString::fromUtf8("thresholdWeight"));
        thresholdWeight->setEnabled(false);
        thresholdWeight->setAlignment(Qt::AlignCenter);
        thresholdWeight->setButtonSymbols(QAbstractSpinBox::NoButtons);
        thresholdWeight->setDecimals(4);
        thresholdWeight->setMaximum(1);
        thresholdWeight->setSingleStep(0.01);
        thresholdWeight->setValue(0.9);

        horizontalLayout_46->addWidget(thresholdWeight);


        verticalLayout_24->addLayout(horizontalLayout_46);

        thresholdTurningSlider = new QSlider(groupBox_15);
        thresholdTurningSlider->setObjectName(QString::fromUtf8("thresholdTurningSlider"));
        thresholdTurningSlider->setEnabled(false);
        thresholdTurningSlider->setMaximum(100);
        thresholdTurningSlider->setValue(0);
        thresholdTurningSlider->setSliderPosition(0);
        thresholdTurningSlider->setOrientation(Qt::Horizontal);
        thresholdTurningSlider->setTickPosition(QSlider::TicksBelow);

        verticalLayout_24->addWidget(thresholdTurningSlider);


        horizontalLayout_62->addLayout(verticalLayout_24);

        verticalLayout_34 = new QVBoxLayout();
        verticalLayout_34->setObjectName(QString::fromUtf8("verticalLayout_34"));
        verticalSpacer_7 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Minimum);

        verticalLayout_34->addItem(verticalSpacer_7);

        maxThresholdScale = new QDoubleSpinBox(groupBox_15);
        maxThresholdScale->setObjectName(QString::fromUtf8("maxThresholdScale"));
        maxThresholdScale->setEnabled(false);
        maxThresholdScale->setMaximum(1);
        maxThresholdScale->setSingleStep(0.01);
        maxThresholdScale->setValue(1);

        verticalLayout_34->addWidget(maxThresholdScale);


        horizontalLayout_62->addLayout(verticalLayout_34);


        verticalLayout_25->addLayout(horizontalLayout_62);

        horizontalLayout_671 = new QHBoxLayout();
        horizontalLayout_671->setObjectName(QString::fromUtf8("horizontalLayout_671"));
        label_37 = new QLabel(groupBox_15);
        label_37->setObjectName(QString::fromUtf8("label_37"));

        horizontalLayout_671->addWidget(label_37);

        autoHeadingTurnRate = new QDoubleSpinBox(groupBox_15);
        autoHeadingTurnRate->setObjectName(QString::fromUtf8("autoHeadingTurnRate"));
        autoHeadingTurnRate->setEnabled(false);
        autoHeadingTurnRate->setAlignment(Qt::AlignCenter);
        autoHeadingTurnRate->setButtonSymbols(QAbstractSpinBox::NoButtons);
        autoHeadingTurnRate->setDecimals(1);
        autoHeadingTurnRate->setMinimum(0.1);
        autoHeadingTurnRate->setMaximum(1000);
        autoHeadingTurnRate->setSingleStep(1);
        autoHeadingTurnRate->setValue(90);

        horizontalLayout_671->addWidget(autoHeadingTurnRate);


        verticalLayout_25->addLayout(horizontalLayout_671);


        verticalLayout_29->addWidget(groupBox_15);

        verticalSpacer_11 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_29->addItem(verticalSpacer_11);


        horizontalLayout_58->addLayout(verticalLayout_29);

        verticalLayout_32 = new QVBoxLayout();
        verticalLayout_32->setObjectName(QString::fromUtf8("verticalLayout_32"));
        groupBox_17 = new QGroupBox(tab_6);
        groupBox_17->setObjectName(QString::fromUtf8("groupBox_17"));
        horizontalLayout_57 = new QHBoxLayout(groupBox_17);
        horizontalLayout_57->setObjectName(QString::fromUtf8("horizontalLayout_57"));
        groupBox_7 = new QGroupBox(groupBox_17);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        verticalLayout_27 = new QVBoxLayout(groupBox_7);
        verticalLayout_27->setObjectName(QString::fromUtf8("verticalLayout_27"));
        plotSymmetricCheckBox = new QCheckBox(groupBox_7);
        plotSymmetricCheckBox->setObjectName(QString::fromUtf8("plotSymmetricCheckBox"));
        plotSymmetricCheckBox->setChecked(true);

        verticalLayout_27->addWidget(plotSymmetricCheckBox);

        verticalLayout_26 = new QVBoxLayout();
        verticalLayout_26->setObjectName(QString::fromUtf8("verticalLayout_26"));
        plotTableWidget = new QTableWidget(groupBox_7);
        if (plotTableWidget->columnCount() < 2)
            plotTableWidget->setColumnCount(2);
        if (plotTableWidget->rowCount() < 2)
            plotTableWidget->setRowCount(2);
        plotTableWidget->setObjectName(QString::fromUtf8("plotTableWidget"));
        plotTableWidget->setRowCount(2);
        plotTableWidget->setColumnCount(2);
        plotTableWidget->horizontalHeader()->setVisible(false);
        plotTableWidget->horizontalHeader()->setCascadingSectionResizes(false);
        plotTableWidget->horizontalHeader()->setDefaultSectionSize(88);
        plotTableWidget->horizontalHeader()->setStretchLastSection(false);
        plotTableWidget->verticalHeader()->setVisible(false);
        plotTableWidget->verticalHeader()->setDefaultSectionSize(25);
        plotTableWidget->verticalHeader()->setStretchLastSection(false);

        verticalLayout_26->addWidget(plotTableWidget);

        horizontalLayout_47 = new QHBoxLayout();
        horizontalLayout_47->setObjectName(QString::fromUtf8("horizontalLayout_47"));
        addRowButton = new QPushButton(groupBox_7);
        addRowButton->setObjectName(QString::fromUtf8("addRowButton"));

        horizontalLayout_47->addWidget(addRowButton);

        deleteRowButton = new QPushButton(groupBox_7);
        deleteRowButton->setObjectName(QString::fromUtf8("deleteRowButton"));

        horizontalLayout_47->addWidget(deleteRowButton);


        verticalLayout_26->addLayout(horizontalLayout_47);


        verticalLayout_27->addLayout(verticalLayout_26);


        horizontalLayout_57->addWidget(groupBox_7);

        ratioDisplayWidget = new QStackedWidget(groupBox_17);
        ratioDisplayWidget->setObjectName(QString::fromUtf8("ratioDisplayWidget"));
        page_5 = new QWidget();
        page_5->setObjectName(QString::fromUtf8("page_5"));
        ratioDisplayWidget->addWidget(page_5);
        page_6 = new QWidget();
        page_6->setObjectName(QString::fromUtf8("page_6"));
        ratioDisplayWidget->addWidget(page_6);

        horizontalLayout_57->addWidget(ratioDisplayWidget);

        horizontalLayout_57->setStretch(1, 10);

        verticalLayout_32->addWidget(groupBox_17);

        groupBox_18 = new QGroupBox(tab_6);
        groupBox_18->setObjectName(QString::fromUtf8("groupBox_18"));
        horizontalLayout_56 = new QHBoxLayout(groupBox_18);
        horizontalLayout_56->setObjectName(QString::fromUtf8("horizontalLayout_56"));
        groupBox_9 = new QGroupBox(groupBox_18);
        groupBox_9->setObjectName(QString::fromUtf8("groupBox_9"));
        verticalLayout_30 = new QVBoxLayout(groupBox_9);
        verticalLayout_30->setObjectName(QString::fromUtf8("verticalLayout_30"));
        plotSymmetricCheckBox_2 = new QCheckBox(groupBox_9);
        plotSymmetricCheckBox_2->setObjectName(QString::fromUtf8("plotSymmetricCheckBox_2"));
        plotSymmetricCheckBox_2->setChecked(true);

        verticalLayout_30->addWidget(plotSymmetricCheckBox_2);

        verticalLayout_31 = new QVBoxLayout();
        verticalLayout_31->setObjectName(QString::fromUtf8("verticalLayout_31"));
        plotVelocityTableWidget = new QTableWidget(groupBox_9);
        if (plotVelocityTableWidget->columnCount() < 2)
            plotVelocityTableWidget->setColumnCount(2);
        if (plotVelocityTableWidget->rowCount() < 2)
            plotVelocityTableWidget->setRowCount(2);
        plotVelocityTableWidget->setObjectName(QString::fromUtf8("plotVelocityTableWidget"));
        plotVelocityTableWidget->setRowCount(2);
        plotVelocityTableWidget->setColumnCount(2);
        plotVelocityTableWidget->horizontalHeader()->setVisible(false);
        plotVelocityTableWidget->horizontalHeader()->setCascadingSectionResizes(false);
        plotVelocityTableWidget->horizontalHeader()->setDefaultSectionSize(88);
        plotVelocityTableWidget->horizontalHeader()->setStretchLastSection(false);
        plotVelocityTableWidget->verticalHeader()->setVisible(false);
        plotVelocityTableWidget->verticalHeader()->setDefaultSectionSize(25);
        plotVelocityTableWidget->verticalHeader()->setStretchLastSection(false);

        verticalLayout_31->addWidget(plotVelocityTableWidget);

        horizontalLayout_48 = new QHBoxLayout();
        horizontalLayout_48->setObjectName(QString::fromUtf8("horizontalLayout_48"));
        addRowButton_2 = new QPushButton(groupBox_9);
        addRowButton_2->setObjectName(QString::fromUtf8("addRowButton_2"));

        horizontalLayout_48->addWidget(addRowButton_2);

        deleteRowButton_2 = new QPushButton(groupBox_9);
        deleteRowButton_2->setObjectName(QString::fromUtf8("deleteRowButton_2"));

        horizontalLayout_48->addWidget(deleteRowButton_2);


        verticalLayout_31->addLayout(horizontalLayout_48);


        verticalLayout_30->addLayout(verticalLayout_31);


        horizontalLayout_56->addWidget(groupBox_9);

        velocityDisplayWidget = new QStackedWidget(groupBox_18);
        velocityDisplayWidget->setObjectName(QString::fromUtf8("velocityDisplayWidget"));
        page_7 = new QWidget();
        page_7->setObjectName(QString::fromUtf8("page_7"));
        velocityDisplayWidget->addWidget(page_7);
        page_8 = new QWidget();
        page_8->setObjectName(QString::fromUtf8("page_8"));
        velocityDisplayWidget->addWidget(page_8);

        horizontalLayout_56->addWidget(velocityDisplayWidget);

        horizontalLayout_56->setStretch(1, 10);

        verticalLayout_32->addWidget(groupBox_18);


        horizontalLayout_58->addLayout(verticalLayout_32);

        horizontalLayout_58->setStretch(1, 5);
        tabWidget->addTab(tab_6, QString());

        verticalLayout_14->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1063, 22));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuJovian = new QMenu(menubar);
        menuJovian->setObjectName(QString::fromUtf8("menuJovian"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuJovian->menuAction());
        menuFile->addAction(actionAdd);
        menuFile->addAction(actionOpen);
        menuFile->addSeparator();
        menuFile->addAction(actionOpenConfig);
        menuFile->addAction(actionSave);
        menuFile->addSeparator();
        menuFile->addAction(actionOpen_TraceFile);
        menuFile->addSeparator();
        menuFile->addAction(actionQuit);
        menuJovian->addAction(actionAbout);

        retranslateUi(MainWindow);
        QObject::connect(horizontalBlankingSlider, SIGNAL(valueChanged(int)), horizontalBlankingSpinBox, SLOT(setValue(int)));
        QObject::connect(horizontalBlankingSlider_2, SIGNAL(valueChanged(int)), horizontalBlankingSpinBox_2, SLOT(setValue(int)));
        QObject::connect(horizontalBlankingSpinBox_2, SIGNAL(valueChanged(int)), horizontalBlankingSlider_2, SLOT(setValue(int)));
        QObject::connect(focalLengthSlider_x, SIGNAL(valueChanged(int)), focalLengthSpinBox_x, SLOT(setValue(int)));
        QObject::connect(horizontalBlankingSpinBox, SIGNAL(valueChanged(int)), horizontalBlankingSlider, SLOT(setValue(int)));
        QObject::connect(focalLengthSpinBox_x, SIGNAL(valueChanged(int)), focalLengthSlider_x, SLOT(setValue(int)));
        QObject::connect(xCenterSlider, SIGNAL(valueChanged(int)), xCenterSpinBox, SLOT(setValue(int)));
        QObject::connect(yCenterSpinBox, SIGNAL(valueChanged(int)), yCenterSlider, SLOT(setValue(int)));
        QObject::connect(yCenterSlider, SIGNAL(valueChanged(int)), yCenterSpinBox, SLOT(setValue(int)));
        QObject::connect(focalLengthSlider_y, SIGNAL(valueChanged(int)), focalLengthSpinBox_y, SLOT(setValue(int)));
        QObject::connect(xCenterSpinBox, SIGNAL(valueChanged(int)), xCenterSlider, SLOT(setValue(int)));
        QObject::connect(rotatedCamerasCheckBox, SIGNAL(toggled(bool)), fovOffsetSlider, SLOT(setEnabled(bool)));
        QObject::connect(focalLengthSpinBox_y, SIGNAL(valueChanged(int)), focalLengthSlider_y, SLOT(setValue(int)));
        QObject::connect(rotatedCamerasCheckBox, SIGNAL(toggled(bool)), fovOffsetSpinBox, SLOT(setEnabled(bool)));
        QObject::connect(fovSlider, SIGNAL(valueChanged(int)), spinBox, SLOT(setValue(int)));
        QObject::connect(spinBox, SIGNAL(valueChanged(int)), fovSlider, SLOT(setValue(int)));
        QObject::connect(indicatorHorizontalPositionSlider, SIGNAL(valueChanged(int)), indicatorHorizontalPositionSpinBox, SLOT(setValue(int)));
        QObject::connect(indicatorHorizontalPositionSpinBox, SIGNAL(valueChanged(int)), indicatorHorizontalPositionSlider, SLOT(setValue(int)));
        QObject::connect(indicatorVerticalPositionSlider, SIGNAL(valueChanged(int)), indicatorVerticalPositionSpinBox, SLOT(setValue(int)));
        QObject::connect(indicatorVerticalPositionSpinBox, SIGNAL(valueChanged(int)), indicatorVerticalPositionSlider, SLOT(setValue(int)));
        QObject::connect(indicatorSizeSlider, SIGNAL(valueChanged(int)), indicatorSizeSpinBox, SLOT(setValue(int)));
        QObject::connect(indicatorSizeSpinBox, SIGNAL(valueChanged(int)), indicatorSizeSlider, SLOT(setValue(int)));
        QObject::connect(borderSizeSlider, SIGNAL(valueChanged(int)), borderSizeSpinBox, SLOT(setValue(int)));
        QObject::connect(cameraSelectSpinBox, SIGNAL(valueChanged(int)), displayWidget, SLOT(setCurrentIndex(int)));
        QObject::connect(borderSizeSpinBox, SIGNAL(valueChanged(int)), borderSizeSlider, SLOT(setValue(int)));
        QObject::connect(showDebugCheckBox, SIGNAL(toggled(bool)), debugPanel, SLOT(setVisible(bool)));
        QObject::connect(headingStatusButton, SIGNAL(toggled(bool)), groupBox_15, SLOT(setEnabled(bool)));
        QObject::connect(thresholdTurningEnabled, SIGNAL(toggled(bool)), thresholdTurningSlider, SLOT(setEnabled(bool)));
        QObject::connect(thresholdTurningEnabled, SIGNAL(toggled(bool)), minThresholdScale, SLOT(setEnabled(bool)));
        QObject::connect(thresholdTurningEnabled, SIGNAL(toggled(bool)), maxThresholdScale, SLOT(setEnabled(bool)));
        QObject::connect(thresholdTurningEnabled, SIGNAL(toggled(bool)), label_37, SLOT(setEnabled(bool)));
        QObject::connect(thresholdTurningEnabled, SIGNAL(toggled(bool)), autoHeadingTurnRate, SLOT(setEnabled(bool)));

        tabWidget->setCurrentIndex(0);
        frameModeComboBox->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", 0, QApplication::UnicodeUTF8));
        actionOpen->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionOpenConfig->setText(QApplication::translate("MainWindow", "Open Config", 0, QApplication::UnicodeUTF8));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("MainWindow", "Save Config", 0, QApplication::UnicodeUTF8));
        actionSave->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionAdd->setText(QApplication::translate("MainWindow", "Add", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("MainWindow", "About", 0, QApplication::UnicodeUTF8));
        actionOpen_TraceFile->setText(QApplication::translate("MainWindow", "Open Trace File", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Number of Cameras:", 0, QApplication::UnicodeUTF8));
        displayCountLabel->setText(QApplication::translate("MainWindow", "Number of Displays (1)", 0, QApplication::UnicodeUTF8));
        label_32->setText(QApplication::translate("MainWindow", "Starting Display", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Camera:", 0, QApplication::UnicodeUTF8));
        fileGroupBox->setTitle(QApplication::translate("MainWindow", "Scene Files", 0, QApplication::UnicodeUTF8));
        startingLocationsBox->setTitle(QApplication::translate("MainWindow", "Start Locations", 0, QApplication::UnicodeUTF8));
        initializeCameraButton->setText(QApplication::translate("MainWindow", "Initialize", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Start Display With:", 0, QApplication::UnicodeUTF8));
        blankRadioButton->setText(QApplication::translate("MainWindow", "Nothing", 0, QApplication::UnicodeUTF8));
        imageRadioButton->setText(QApplication::translate("MainWindow", "Image", 0, QApplication::UnicodeUTF8));
        imageCalibrationButton->setText(QApplication::translate("MainWindow", "Set ...", 0, QApplication::UnicodeUTF8));
        movieRadioButton->setText(QApplication::translate("MainWindow", "Movie", 0, QApplication::UnicodeUTF8));
        movieCalibrationButton->setText(QApplication::translate("MainWindow", "Set ...", 0, QApplication::UnicodeUTF8));
        osgRadioButton->setText(QApplication::translate("MainWindow", "OSG Model", 0, QApplication::UnicodeUTF8));
        osgDataButton->setText(QApplication::translate("MainWindow", "Cow ...", 0, QApplication::UnicodeUTF8));
        blankingGroupBox->setTitle(QApplication::translate("MainWindow", "Distortion", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        groupBox_6->setToolTip(QApplication::translate("MainWindow", "Controls size of viewport. Any control with an * in its name is per camera.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        groupBox_6->setTitle(QApplication::translate("MainWindow", "Horizontal Blanking*", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        groupBox_4->setToolTip(QApplication::translate("MainWindow", "Controls vertical and horizontal warping. Any control with an * in its name is per camera.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        groupBox_4->setTitle(QApplication::translate("MainWindow", "Warping*", 0, QApplication::UnicodeUTF8));
        enableDistortion->setText(QApplication::translate("MainWindow", "Enable", 0, QApplication::UnicodeUTF8));
        gangedCheckBox->setText(QApplication::translate("MainWindow", "Gang Cameras?", 0, QApplication::UnicodeUTF8));
        xCenterDistortsBox->setText(QApplication::translate("MainWindow", "X Center Distorts in Y", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "x Focal Length", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("MainWindow", "y Focal Length", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("MainWindow", "X Center ", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainWindow", "Y Center ", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Connection Type", 0, QApplication::UnicodeUTF8));
        TCP_radioBtn->setText(QApplication::translate("MainWindow", "TCP", 0, QApplication::UnicodeUTF8));
        UDP_radioBtn->setText(QApplication::translate("MainWindow", "UDP", 0, QApplication::UnicodeUTF8));
        radioButton_3->setText(QApplication::translate("MainWindow", "Shared Memory", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Hostname:", 0, QApplication::UnicodeUTF8));
        hostnameCtrl->setText(QApplication::translate("MainWindow", "localhost", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "Port:", 0, QApplication::UnicodeUTF8));
        portCtrl->setText(QApplication::translate("MainWindow", "22222", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "Retries", 0, QApplication::UnicodeUTF8));
        retriesCtrl->setText(QApplication::translate("MainWindow", "3", 0, QApplication::UnicodeUTF8));
        networkConnectButton->setText(QApplication::translate("MainWindow", "Connect", 0, QApplication::UnicodeUTF8));
        gangMotionCheckBox->setText(QApplication::translate("MainWindow", "Gang Motion and Blanking", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        disableMotionButton->setToolTip(QApplication::translate("MainWindow", "When Connected, disables input from ball, but keeps system running", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        disableMotionButton->setText(QApplication::translate("MainWindow", "Disable Motion", 0, QApplication::UnicodeUTF8));
        displayBlankingButton->setText(QApplication::translate("MainWindow", "Enable Display Blanking", 0, QApplication::UnicodeUTF8));
        enableTimedTrialCheckBox->setText(QApplication::translate("MainWindow", "Enable Timed Trial", 0, QApplication::UnicodeUTF8));
        timeTrialEditor->setDisplayFormat(QApplication::translate("MainWindow", "h:mm:ss", 0, QApplication::UnicodeUTF8));
        groupBox_10->setTitle(QApplication::translate("MainWindow", "Playback Controls", 0, QApplication::UnicodeUTF8));
        rewindButton->setText(QApplication::translate("MainWindow", "...", 0, QApplication::UnicodeUTF8));
        playBackwardsButton->setText(QApplication::translate("MainWindow", "...", 0, QApplication::UnicodeUTF8));
        playForwardButton->setText(QApplication::translate("MainWindow", "...", 0, QApplication::UnicodeUTF8));
        recordButton->setText(QApplication::translate("MainWindow", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        strideLength->setToolTip(QApplication::translate("MainWindow", "Stride By ...", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Setup", 0, QApplication::UnicodeUTF8));
        displayBox->setTitle(QApplication::translate("MainWindow", "Display Configuration*", 0, QApplication::UnicodeUTF8));
        resetButton->setText(QApplication::translate("MainWindow", "Reset", 0, QApplication::UnicodeUTF8));
        distributeHoriz->setText(QApplication::translate("MainWindow", "Distribute Horizontally", 0, QApplication::UnicodeUTF8));
        distributeVert->setText(QApplication::translate("MainWindow", "Distribute Vertically", 0, QApplication::UnicodeUTF8));
        smoothButton->setText(QApplication::translate("MainWindow", "Smooth", 0, QApplication::UnicodeUTF8));
        linearizeEdgesButton->setText(QApplication::translate("MainWindow", "Linearize Edges", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        framePackingEnabled->setToolTip(QApplication::translate("MainWindow", "If enabled, packs 3 frames into the R, G, and B color channels", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        framePackingEnabled->setText(QApplication::translate("MainWindow", "Enable Frame Packing", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_5), QApplication::translate("MainWindow", "Display", 0, QApplication::UnicodeUTF8));
        diffuseLightButton->setText(QApplication::translate("MainWindow", "Diffuse Light Color", 0, QApplication::UnicodeUTF8));
        diffuseLabel->setText(QApplication::translate("MainWindow", "#191919", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("MainWindow", "Intensity", 0, QApplication::UnicodeUTF8));
        ambientLightButton->setText(QApplication::translate("MainWindow", "Ambient Light Color", 0, QApplication::UnicodeUTF8));
        ambientLabel->setText(QApplication::translate("MainWindow", "#191919", 0, QApplication::UnicodeUTF8));
        backgroundColorButton->setText(QApplication::translate("MainWindow", "Background Color", 0, QApplication::UnicodeUTF8));
        backgroundLabel->setText(QString());
#ifndef QT_NO_TOOLTIP
        globalBrightnessButton->setToolTip(QApplication::translate("MainWindow", "Controls the brighness per screen. Any control with an * in its name is per camera.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        globalBrightnessButton->setText(QApplication::translate("MainWindow", "Global Brightness*", 0, QApplication::UnicodeUTF8));
        brightnessLabel->setText(QApplication::translate("MainWindow", "#ffffff", 0, QApplication::UnicodeUTF8));
        label_30->setText(QApplication::translate("MainWindow", "Shading Method", 0, QApplication::UnicodeUTF8));
        shaderComboBox->clear();
        shaderComboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "OSG Default", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Per Vertex", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Per Pixel", 0, QApplication::UnicodeUTF8)
        );
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Lighting Controls", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("MainWindow", "For each of the following calibrations, rotate the ball 180 degrees around the designated axis. The system will compensate for accidental movement about the other axes.", 0, QApplication::UnicodeUTF8));
        label_14->setText(QString());
        computeAverage->setText(QApplication::translate("MainWindow", "Average multiple runs", 0, QApplication::UnicodeUTF8));
        editValues->setText(QApplication::translate("MainWindow", "Edit Values", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("MainWindow", "Rotate around X", 0, QApplication::UnicodeUTF8));
        calibrateXButton->setText(QApplication::translate("MainWindow", "Start Calibration...", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("MainWindow", "Rotate around Y", 0, QApplication::UnicodeUTF8));
        calibrateYButton->setText(QApplication::translate("MainWindow", "Start Calibration...", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("MainWindow", "Rotate around Z", 0, QApplication::UnicodeUTF8));
        calibrateZButton->setText(QApplication::translate("MainWindow", "Start Calibration...", 0, QApplication::UnicodeUTF8));
        updateCalibrationValuesButton->setText(QApplication::translate("MainWindow", "Update Values", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MainWindow", "Calibration", 0, QApplication::UnicodeUTF8));
        groupBox_8->setTitle(QApplication::translate("MainWindow", "Miscellaneous", 0, QApplication::UnicodeUTF8));
        label_33->setText(QApplication::translate("MainWindow", "Ball Radius:", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("MainWindow", "Crossbar", 0, QApplication::UnicodeUTF8));
        crossbarEnabledCheckBox->setText(QApplication::translate("MainWindow", "Enabled", 0, QApplication::UnicodeUTF8));
        label_34->setText(QApplication::translate("MainWindow", "Width", 0, QApplication::UnicodeUTF8));
        label_35->setText(QApplication::translate("MainWindow", "Minimum Velocity  Threshold", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        minimumVelocityThreshold->setToolTip(QApplication::translate("MainWindow", "Velocity below this value is treated as 0, i.e., no movement. Use to filter noisy input", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        restrictVerticalMotion->setText(QApplication::translate("MainWindow", "Restrict Vertical Motion", 0, QApplication::UnicodeUTF8));
        rotatedCamerasCheckBox->setText(QApplication::translate("MainWindow", "Use Rotated Cameras", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("MainWindow", "Field of View", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("MainWindow", "Offset", 0, QApplication::UnicodeUTF8));
        groupBox_16->setTitle(QApplication::translate("MainWindow", "Smoothing ", 0, QApplication::UnicodeUTF8));
        velocitySmoothingEnabled->setText(QApplication::translate("MainWindow", "Velocity", 0, QApplication::UnicodeUTF8));
        label_65->setText(QApplication::translate("MainWindow", "Time Interval (ms):", 0, QApplication::UnicodeUTF8));
        label_38->setText(QApplication::translate("MainWindow", "Heading", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        inputSmoothingEnabled->setToolTip(QApplication::translate("MainWindow", "Smooths the input from the treadmill", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        inputSmoothingEnabled->setText(QApplication::translate("MainWindow", "Input", 0, QApplication::UnicodeUTF8));
        preBlendLabel->setText(QApplication::translate("MainWindow", "Time Interval (ms):", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        intermediarySmoothingEnabled->setToolTip(QApplication::translate("MainWindow", "Smooths the angle computed from the input", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        intermediarySmoothingEnabled->setText(QApplication::translate("MainWindow", "Intermediary", 0, QApplication::UnicodeUTF8));
        preBlendLabel_2->setText(QApplication::translate("MainWindow", "Time Interval (ms):", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        outputSmoothingEnabled->setToolTip(QApplication::translate("MainWindow", "Smooths the final angle after blending with auto heading", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        outputSmoothingEnabled->setText(QApplication::translate("MainWindow", "Output", 0, QApplication::UnicodeUTF8));
        label_66->setText(QApplication::translate("MainWindow", "Time Interval (ms):", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("MainWindow", "Output", 0, QApplication::UnicodeUTF8));
        label_25->setText(QApplication::translate("MainWindow", "Port", 0, QApplication::UnicodeUTF8));
        label_31->setText(QApplication::translate("MainWindow", "Output Rate:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        fileExportToggle->setToolTip(QApplication::translate("MainWindow", "Export full output data to file?", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        fileExportToggle->setText(QApplication::translate("MainWindow", "File Export?", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("MainWindow", "File Name", 0, QApplication::UnicodeUTF8));
        exportFileNameSelector->setText(QApplication::translate("MainWindow", "...", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("MainWindow", "Style:", 0, QApplication::UnicodeUTF8));
        exportFileNameStyle->clear();
        exportFileNameStyle->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "None", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Counter", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Time Stamp", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        exportFileNameStyle->setToolTip(QApplication::translate("MainWindow", "Style for writing file:\n"
"None: one file overwritten on each connect\n"
"Counter: file name appended with counter, incremented on each connect/disconnect\n"
"TimeStamp: file name appended with time stamp on each connect/disconnect", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        treadmillOutputToggle->setToolTip(QApplication::translate("MainWindow", "Include raw treadmill data in output stream?", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        treadmillOutputToggle->setText(QApplication::translate("MainWindow", "Treadmill Output?", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        reducedOutputToggle->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>Send reduced data to output stream (time, position, events). Does not affect data written to file if enabled.</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        reducedOutputToggle->setText(QApplication::translate("MainWindow", "Reduced Output?", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("MainWindow", "Frame Indicator", 0, QApplication::UnicodeUTF8));
        frameModeComboBox->clear();
        frameModeComboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Off", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "On, Blink on Connect", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "On", 0, QApplication::UnicodeUTF8)
        );
        label_23->setText(QApplication::translate("MainWindow", "Size", 0, QApplication::UnicodeUTF8));
        label_24->setText(QApplication::translate("MainWindow", "Border", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("MainWindow", "Position", 0, QApplication::UnicodeUTF8));
        label_22->setText(QApplication::translate("MainWindow", "Screen:", 0, QApplication::UnicodeUTF8));
        showDebugCheckBox->setText(QApplication::translate("MainWindow", "Show Debug Panel", 0, QApplication::UnicodeUTF8));
        debugPanel->setTitle(QApplication::translate("MainWindow", "Debugging", 0, QApplication::UnicodeUTF8));
        label_29->setText(QApplication::translate("MainWindow", "Alignment offset for tracker", 0, QApplication::UnicodeUTF8));
        label_26->setText(QApplication::translate("MainWindow", "xoffset", 0, QApplication::UnicodeUTF8));
        label_27->setText(QApplication::translate("MainWindow", "yoffset", 0, QApplication::UnicodeUTF8));
        label_28->setText(QApplication::translate("MainWindow", "zoffset", 0, QApplication::UnicodeUTF8));
        debugPhysicsCheckBox->setText(QApplication::translate("MainWindow", "Debug Physics (only effective before scene is read)", 0, QApplication::UnicodeUTF8));
        showInvisibleObjectsCheckBox->setText(QApplication::translate("MainWindow", "Show Invisible Items", 0, QApplication::UnicodeUTF8));
        useCustomGLWidget->setText(QApplication::translate("MainWindow", "Use custom GL widget", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("MainWindow", "Configuration", 0, QApplication::UnicodeUTF8));
        headingStatusButton->setText(QApplication::translate("MainWindow", "Auto Heading Enabled", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        groupBox_15->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        groupBox_15->setTitle(QApplication::translate("MainWindow", "Turning Control", 0, QApplication::UnicodeUTF8));
        thresholdTurningEnabled->setText(QApplication::translate("MainWindow", "Enable", 0, QApplication::UnicodeUTF8));
        label_36->setText(QApplication::translate("MainWindow", "%Threshold", 0, QApplication::UnicodeUTF8));
        label_37->setText(QApplication::translate("MainWindow", "Auto Heading Turn Rate", 0, QApplication::UnicodeUTF8));
        groupBox_17->setTitle(QApplication::translate("MainWindow", "Pitch/Roll vs Degrees/Sec", 0, QApplication::UnicodeUTF8));
        groupBox_7->setTitle(QApplication::translate("MainWindow", "Plot Points", 0, QApplication::UnicodeUTF8));
        plotSymmetricCheckBox->setText(QApplication::translate("MainWindow", "Symmetric in Y", 0, QApplication::UnicodeUTF8));
        addRowButton->setText(QApplication::translate("MainWindow", "Add", 0, QApplication::UnicodeUTF8));
        deleteRowButton->setText(QApplication::translate("MainWindow", "Delete", 0, QApplication::UnicodeUTF8));
        groupBox_18->setTitle(QApplication::translate("MainWindow", "Velocity vs Gain", 0, QApplication::UnicodeUTF8));
        groupBox_9->setTitle(QApplication::translate("MainWindow", "Plot Points", 0, QApplication::UnicodeUTF8));
        plotSymmetricCheckBox_2->setText(QApplication::translate("MainWindow", "Symmetric in Y", 0, QApplication::UnicodeUTF8));
        addRowButton_2->setText(QApplication::translate("MainWindow", "Add", 0, QApplication::UnicodeUTF8));
        deleteRowButton_2->setText(QApplication::translate("MainWindow", "Delete", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_6), QApplication::translate("MainWindow", "Turning", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuJovian->setTitle(QApplication::translate("MainWindow", "Jovian", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONSOLE_H
