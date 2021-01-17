/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QCommandLinkButton>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionAbout;
    QAction *actionAbout_Qt;
    QAction *actionConnect;
    QAction *actionToggleWindow;
    QAction *actionSettings;
    QAction *actionTunel_settings;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_5;
    QTabWidget *tabWidget;
    QWidget *tab_main;
    QGridLayout *gridLayout_2;
    QPushButton *btnAbout;
    QPushButton *pushButton_2;
    QVBoxLayout *verticalLayout_5;
    QLabel *lblCnxStatus;
    QLabel *label_10;
    QSpacerItem *verticalSpacer_4;
    QHBoxLayout *horizontalLayout_3;
    QLabel *logo;
    QSpacerItem *verticalSpacer_5;
    QPushButton *btnCountryFlag;
    QWidget *tabLogs;
    QGridLayout *gridLayout;
    QPlainTextEdit *logsview;
    QPushButton *btnSaveLogs;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnClearLogs;
    QWidget *tab_config;
    QFormLayout *formLayout;
    QLabel *label_7;
    QSpinBox *editLocalHttpPort;
    QLabel *label_8;
    QSpinBox *editLocalSocksPort;
    QGroupBox *chkUseProxy;
    QFormLayout *formLayout_2;
    QLabel *label_6;
    QLineEdit *editProxyHost;
    QLabel *label_5;
    QSpinBox *editProxyPort;
    QCheckBox *chkProxyAuth;
    QLineEdit *editProxyUser;
    QLabel *label_4;
    QLineEdit *editProxyPass;
    QPushButton *btnSaveTunnelConfig;
    QCheckBox *chkConnectOnStart;
    QSpacerItem *verticalSpacer_3;
    QCheckBox *chkSetProxyConfig;
    QComboBox *cmbRegions;
    QLabel *label;
    QWidget *tab_settings;
    QVBoxLayout *verticalLayout;
    QCheckBox *chkStratupOnLogin;
    QCheckBox *chkHideFromDockMac;
    QCheckBox *chkTrayDialog;
    QCheckBox *chkShowWindowOnStart;
    QCheckBox *chkTunnelNotifications;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_2;
    QToolButton *btnIconLogo;
    QGroupBox *chkCustomCoreExe;
    QHBoxLayout *horizontalLayout;
    QLineEdit *editCustomCoreExe;
    QToolButton *btnCustomCoreExe;
    QGroupBox *chkCustomCoreConfig;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *editCustomCoreConfig;
    QToolButton *btnCustomCoreConfig;
    QSpacerItem *verticalSpacer;
    QWidget *tabAbout;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_6;
    QLabel *logo_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_3;
    QLabel *label_9;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_13;
    QSpacerItem *verticalSpacer_2;
    QCommandLinkButton *commandLinkButton;
    QMenuBar *menubar;
    QMenu *menuHelp;
    QMenu *menuMain;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(303, 418);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionAbout_Qt = new QAction(MainWindow);
        actionAbout_Qt->setObjectName(QString::fromUtf8("actionAbout_Qt"));
        actionConnect = new QAction(MainWindow);
        actionConnect->setObjectName(QString::fromUtf8("actionConnect"));
        actionToggleWindow = new QAction(MainWindow);
        actionToggleWindow->setObjectName(QString::fromUtf8("actionToggleWindow"));
        actionSettings = new QAction(MainWindow);
        actionSettings->setObjectName(QString::fromUtf8("actionSettings"));
        actionTunel_settings = new QAction(MainWindow);
        actionTunel_settings->setObjectName(QString::fromUtf8("actionTunel_settings"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout_5 = new QHBoxLayout(centralwidget);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab_main = new QWidget();
        tab_main->setObjectName(QString::fromUtf8("tab_main"));
        tab_main->setLayoutDirection(Qt::LeftToRight);
        gridLayout_2 = new QGridLayout(tab_main);
        gridLayout_2->setSpacing(0);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(5, 5, 5, 5);
        btnAbout = new QPushButton(tab_main);
        btnAbout->setObjectName(QString::fromUtf8("btnAbout"));
        btnAbout->setMaximumSize(QSize(32, 32));
        btnAbout->setStyleSheet(QString::fromUtf8("color: rgb(36, 205, 4);\n"
"\n"
"    min-width: 5ex;\n"
"    min-height: 5ex;"));
        btnAbout->setFlat(true);

        gridLayout_2->addWidget(btnAbout, 3, 0, 1, 1);

        pushButton_2 = new QPushButton(tab_main);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setMaximumSize(QSize(32, 32));
        pushButton_2->setStyleSheet(QString::fromUtf8("color: rgb(252, 0, 6);\n"
"\n"
"    min-width: 5ex;\n"
"    min-height: 5ex;"));
        pushButton_2->setFlat(true);

        gridLayout_2->addWidget(pushButton_2, 3, 2, 1, 1);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(0);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        lblCnxStatus = new QLabel(tab_main);
        lblCnxStatus->setObjectName(QString::fromUtf8("lblCnxStatus"));
        lblCnxStatus->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(lblCnxStatus);

        label_10 = new QLabel(tab_main);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(label_10);


        gridLayout_2->addLayout(verticalLayout_5, 3, 1, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_4, 2, 1, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        logo = new QLabel(tab_main);
        logo->setObjectName(QString::fromUtf8("logo"));
        logo->setMaximumSize(QSize(250, 250));
        logo->setPixmap(QPixmap(QString::fromUtf8(":/imgs/off.png")));
        logo->setScaledContents(true);
        logo->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(logo);


        gridLayout_2->addLayout(horizontalLayout_3, 1, 0, 1, 3);

        verticalSpacer_5 = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_5, 0, 1, 1, 1);

        btnCountryFlag = new QPushButton(tab_main);
        btnCountryFlag->setObjectName(QString::fromUtf8("btnCountryFlag"));
        btnCountryFlag->setMaximumSize(QSize(32, 32));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/imgs/fastCountry.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnCountryFlag->setIcon(icon);
        btnCountryFlag->setIconSize(QSize(20, 20));
        btnCountryFlag->setFlat(true);

        gridLayout_2->addWidget(btnCountryFlag, 0, 2, 1, 1);

        tabWidget->addTab(tab_main, QString());
        tabLogs = new QWidget();
        tabLogs->setObjectName(QString::fromUtf8("tabLogs"));
        gridLayout = new QGridLayout(tabLogs);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(5);
        gridLayout->setVerticalSpacing(3);
        gridLayout->setContentsMargins(5, 5, 5, 5);
        logsview = new QPlainTextEdit(tabLogs);
        logsview->setObjectName(QString::fromUtf8("logsview"));
        logsview->setStyleSheet(QString::fromUtf8("background-color: rgb(196, 200, 200);\n"
"color: rgb(0, 0, 0);"));
        logsview->setLineWrapMode(QPlainTextEdit::NoWrap);
        logsview->setReadOnly(true);
        logsview->setTextInteractionFlags(Qt::LinksAccessibleByKeyboard|Qt::LinksAccessibleByMouse|Qt::TextBrowserInteraction|Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        gridLayout->addWidget(logsview, 0, 0, 1, 3);

        btnSaveLogs = new QPushButton(tabLogs);
        btnSaveLogs->setObjectName(QString::fromUtf8("btnSaveLogs"));

        gridLayout->addWidget(btnSaveLogs, 1, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(117, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 1, 1, 1);

        btnClearLogs = new QPushButton(tabLogs);
        btnClearLogs->setObjectName(QString::fromUtf8("btnClearLogs"));

        gridLayout->addWidget(btnClearLogs, 1, 2, 1, 1);

        tabWidget->addTab(tabLogs, QString());
        tab_config = new QWidget();
        tab_config->setObjectName(QString::fromUtf8("tab_config"));
        formLayout = new QFormLayout(tab_config);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setContentsMargins(5, 10, 5, 5);
        label_7 = new QLabel(tab_config);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_7);

        editLocalHttpPort = new QSpinBox(tab_config);
        editLocalHttpPort->setObjectName(QString::fromUtf8("editLocalHttpPort"));
        editLocalHttpPort->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        editLocalHttpPort->setAccelerated(true);
        editLocalHttpPort->setMinimum(1);
        editLocalHttpPort->setMaximum(999999999);
        editLocalHttpPort->setValue(8081);

        formLayout->setWidget(2, QFormLayout::FieldRole, editLocalHttpPort);

        label_8 = new QLabel(tab_config);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(3, QFormLayout::LabelRole, label_8);

        editLocalSocksPort = new QSpinBox(tab_config);
        editLocalSocksPort->setObjectName(QString::fromUtf8("editLocalSocksPort"));
        editLocalSocksPort->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        editLocalSocksPort->setAccelerated(true);
        editLocalSocksPort->setMinimum(1);
        editLocalSocksPort->setMaximum(999999999);
        editLocalSocksPort->setValue(1081);

        formLayout->setWidget(3, QFormLayout::FieldRole, editLocalSocksPort);

        chkUseProxy = new QGroupBox(tab_config);
        chkUseProxy->setObjectName(QString::fromUtf8("chkUseProxy"));
        chkUseProxy->setFlat(true);
        chkUseProxy->setCheckable(true);
        chkUseProxy->setChecked(false);
        formLayout_2 = new QFormLayout(chkUseProxy);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        formLayout_2->setContentsMargins(0, -1, 0, 0);
        label_6 = new QLabel(chkUseProxy);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_6);

        editProxyHost = new QLineEdit(chkUseProxy);
        editProxyHost->setObjectName(QString::fromUtf8("editProxyHost"));

        formLayout_2->setWidget(0, QFormLayout::FieldRole, editProxyHost);

        label_5 = new QLabel(chkUseProxy);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_5);

        editProxyPort = new QSpinBox(chkUseProxy);
        editProxyPort->setObjectName(QString::fromUtf8("editProxyPort"));
        editProxyPort->setMinimumSize(QSize(100, 0));
        editProxyPort->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        editProxyPort->setAccelerated(true);
        editProxyPort->setMinimum(1);
        editProxyPort->setMaximum(999999999);
        editProxyPort->setValue(80);

        formLayout_2->setWidget(1, QFormLayout::FieldRole, editProxyPort);

        chkProxyAuth = new QCheckBox(chkUseProxy);
        chkProxyAuth->setObjectName(QString::fromUtf8("chkProxyAuth"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, chkProxyAuth);

        editProxyUser = new QLineEdit(chkUseProxy);
        editProxyUser->setObjectName(QString::fromUtf8("editProxyUser"));
        editProxyUser->setEnabled(false);

        formLayout_2->setWidget(2, QFormLayout::FieldRole, editProxyUser);

        label_4 = new QLabel(chkUseProxy);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout_2->setWidget(3, QFormLayout::LabelRole, label_4);

        editProxyPass = new QLineEdit(chkUseProxy);
        editProxyPass->setObjectName(QString::fromUtf8("editProxyPass"));
        editProxyPass->setEnabled(false);
        editProxyPass->setEchoMode(QLineEdit::Password);

        formLayout_2->setWidget(3, QFormLayout::FieldRole, editProxyPass);


        formLayout->setWidget(4, QFormLayout::SpanningRole, chkUseProxy);

        btnSaveTunnelConfig = new QPushButton(tab_config);
        btnSaveTunnelConfig->setObjectName(QString::fromUtf8("btnSaveTunnelConfig"));
        btnSaveTunnelConfig->setAutoExclusive(true);

        formLayout->setWidget(6, QFormLayout::FieldRole, btnSaveTunnelConfig);

        chkConnectOnStart = new QCheckBox(tab_config);
        chkConnectOnStart->setObjectName(QString::fromUtf8("chkConnectOnStart"));

        formLayout->setWidget(0, QFormLayout::SpanningRole, chkConnectOnStart);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        formLayout->setItem(7, QFormLayout::FieldRole, verticalSpacer_3);

        chkSetProxyConfig = new QCheckBox(tab_config);
        chkSetProxyConfig->setObjectName(QString::fromUtf8("chkSetProxyConfig"));
        chkSetProxyConfig->setChecked(true);

        formLayout->setWidget(5, QFormLayout::SpanningRole, chkSetProxyConfig);

        cmbRegions = new QComboBox(tab_config);
        cmbRegions->addItem(QString());
        cmbRegions->setObjectName(QString::fromUtf8("cmbRegions"));

        formLayout->setWidget(1, QFormLayout::FieldRole, cmbRegions);

        label = new QLabel(tab_config);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label);

        tabWidget->addTab(tab_config, QString());
        tab_settings = new QWidget();
        tab_settings->setObjectName(QString::fromUtf8("tab_settings"));
        verticalLayout = new QVBoxLayout(tab_settings);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(5, 10, 5, 5);
        chkStratupOnLogin = new QCheckBox(tab_settings);
        chkStratupOnLogin->setObjectName(QString::fromUtf8("chkStratupOnLogin"));

        verticalLayout->addWidget(chkStratupOnLogin);

        chkHideFromDockMac = new QCheckBox(tab_settings);
        chkHideFromDockMac->setObjectName(QString::fromUtf8("chkHideFromDockMac"));

        verticalLayout->addWidget(chkHideFromDockMac);

        chkTrayDialog = new QCheckBox(tab_settings);
        chkTrayDialog->setObjectName(QString::fromUtf8("chkTrayDialog"));

        verticalLayout->addWidget(chkTrayDialog);

        chkShowWindowOnStart = new QCheckBox(tab_settings);
        chkShowWindowOnStart->setObjectName(QString::fromUtf8("chkShowWindowOnStart"));

        verticalLayout->addWidget(chkShowWindowOnStart);

        chkTunnelNotifications = new QCheckBox(tab_settings);
        chkTunnelNotifications->setObjectName(QString::fromUtf8("chkTunnelNotifications"));

        verticalLayout->addWidget(chkTunnelNotifications);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_2 = new QLabel(tab_settings);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_4->addWidget(label_2);

        btnIconLogo = new QToolButton(tab_settings);
        btnIconLogo->setObjectName(QString::fromUtf8("btnIconLogo"));
        btnIconLogo->setStyleSheet(QString::fromUtf8("QMenu{icon-size: 40px;}"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/imgs/logo.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnIconLogo->setIcon(icon1);
        btnIconLogo->setIconSize(QSize(32, 32));
        btnIconLogo->setCheckable(true);
        btnIconLogo->setAutoExclusive(true);
        btnIconLogo->setPopupMode(QToolButton::InstantPopup);

        horizontalLayout_4->addWidget(btnIconLogo);


        verticalLayout->addLayout(horizontalLayout_4);

        chkCustomCoreExe = new QGroupBox(tab_settings);
        chkCustomCoreExe->setObjectName(QString::fromUtf8("chkCustomCoreExe"));
        chkCustomCoreExe->setFlat(true);
        chkCustomCoreExe->setCheckable(true);
        chkCustomCoreExe->setChecked(false);
        horizontalLayout = new QHBoxLayout(chkCustomCoreExe);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 12, 0, 0);
        editCustomCoreExe = new QLineEdit(chkCustomCoreExe);
        editCustomCoreExe->setObjectName(QString::fromUtf8("editCustomCoreExe"));
        editCustomCoreExe->setReadOnly(true);

        horizontalLayout->addWidget(editCustomCoreExe);

        btnCustomCoreExe = new QToolButton(chkCustomCoreExe);
        btnCustomCoreExe->setObjectName(QString::fromUtf8("btnCustomCoreExe"));

        horizontalLayout->addWidget(btnCustomCoreExe);


        verticalLayout->addWidget(chkCustomCoreExe);

        chkCustomCoreConfig = new QGroupBox(tab_settings);
        chkCustomCoreConfig->setObjectName(QString::fromUtf8("chkCustomCoreConfig"));
        chkCustomCoreConfig->setFlat(true);
        chkCustomCoreConfig->setCheckable(true);
        chkCustomCoreConfig->setChecked(false);
        horizontalLayout_2 = new QHBoxLayout(chkCustomCoreConfig);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, -1, 0, 0);
        editCustomCoreConfig = new QLineEdit(chkCustomCoreConfig);
        editCustomCoreConfig->setObjectName(QString::fromUtf8("editCustomCoreConfig"));
        editCustomCoreConfig->setReadOnly(true);

        horizontalLayout_2->addWidget(editCustomCoreConfig);

        btnCustomCoreConfig = new QToolButton(chkCustomCoreConfig);
        btnCustomCoreConfig->setObjectName(QString::fromUtf8("btnCustomCoreConfig"));

        horizontalLayout_2->addWidget(btnCustomCoreConfig);


        verticalLayout->addWidget(chkCustomCoreConfig);

        verticalSpacer = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        tabWidget->addTab(tab_settings, QString());
        tabAbout = new QWidget();
        tabAbout->setObjectName(QString::fromUtf8("tabAbout"));
        verticalLayout_3 = new QVBoxLayout(tabAbout);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(5, 10, 5, 5);
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(5, 5, 5, 5);
        logo_2 = new QLabel(tabAbout);
        logo_2->setObjectName(QString::fromUtf8("logo_2"));
        logo_2->setMaximumSize(QSize(128, 128));
        logo_2->setPixmap(QPixmap(QString::fromUtf8(":/imgs/logo.png")));
        logo_2->setScaledContents(true);
        logo_2->setAlignment(Qt::AlignCenter);

        horizontalLayout_6->addWidget(logo_2);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_3 = new QLabel(tabAbout);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QFont font;
        font.setPointSize(24);
        label_3->setFont(font);

        verticalLayout_2->addWidget(label_3);

        label_9 = new QLabel(tabAbout);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        verticalLayout_2->addWidget(label_9);

        label_11 = new QLabel(tabAbout);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        verticalLayout_2->addWidget(label_11);

        label_12 = new QLabel(tabAbout);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setFocusPolicy(Qt::TabFocus);
        label_12->setOpenExternalLinks(true);

        verticalLayout_2->addWidget(label_12);


        horizontalLayout_6->addLayout(verticalLayout_2);


        verticalLayout_3->addLayout(horizontalLayout_6);

        label_13 = new QLabel(tabAbout);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setWordWrap(true);
        label_13->setMargin(15);

        verticalLayout_3->addWidget(label_13);

        verticalSpacer_2 = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);

        commandLinkButton = new QCommandLinkButton(tabAbout);
        commandLinkButton->setObjectName(QString::fromUtf8("commandLinkButton"));

        verticalLayout_3->addWidget(commandLinkButton);

        tabWidget->addTab(tabAbout, QString());

        horizontalLayout_5->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 303, 22));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menuMain = new QMenu(menubar);
        menuMain->setObjectName(QString::fromUtf8("menuMain"));
        MainWindow->setMenuBar(menubar);

        menubar->addAction(menuMain->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuHelp->addAction(actionAbout);
        menuHelp->addAction(actionAbout_Qt);
        menuMain->addAction(actionConnect);
        menuMain->addSeparator();
        menuMain->addAction(actionToggleWindow);
        menuMain->addSeparator();
        menuMain->addAction(actionSettings);
        menuMain->addAction(actionTunel_settings);

        retranslateUi(MainWindow);
        QObject::connect(chkProxyAuth, SIGNAL(toggled(bool)), editProxyUser, SLOT(setEnabled(bool)));
        QObject::connect(chkProxyAuth, SIGNAL(toggled(bool)), editProxyPass, SLOT(setEnabled(bool)));

        tabWidget->setCurrentIndex(2);
        btnSaveTunnelConfig->setDefault(true);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        actionAbout->setText(QApplication::translate("MainWindow", "About", nullptr));
        actionAbout_Qt->setText(QApplication::translate("MainWindow", "About Qt", nullptr));
        actionConnect->setText(QApplication::translate("MainWindow", "Connect", nullptr));
        actionToggleWindow->setText(QApplication::translate("MainWindow", "Show", nullptr));
        actionSettings->setText(QApplication::translate("MainWindow", "Settings", nullptr));
        actionTunel_settings->setText(QApplication::translate("MainWindow", "Tunel settings", nullptr));
        btnAbout->setText(QApplication::translate("MainWindow", "?", nullptr));
        pushButton_2->setText(QApplication::translate("MainWindow", "X", nullptr));
        lblCnxStatus->setText(QApplication::translate("MainWindow", "Stoped", nullptr));
        label_10->setText(QApplication::translate("MainWindow", "Tunel core", nullptr));
        logo->setText(QString());
        btnCountryFlag->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab_main), QApplication::translate("MainWindow", "Main", nullptr));
        btnSaveLogs->setText(QApplication::translate("MainWindow", "Save", nullptr));
        btnClearLogs->setText(QApplication::translate("MainWindow", "Clear", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabLogs), QApplication::translate("MainWindow", "Logs", nullptr));
        label_7->setText(QApplication::translate("MainWindow", "Local tunnel port Http", nullptr));
        label_8->setText(QApplication::translate("MainWindow", "Socks", nullptr));
        chkUseProxy->setTitle(QApplication::translate("MainWindow", "Use http proxy connection", nullptr));
        label_6->setText(QApplication::translate("MainWindow", "Server", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "Port", nullptr));
        chkProxyAuth->setText(QApplication::translate("MainWindow", "User", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "Pass", nullptr));
        btnSaveTunnelConfig->setText(QApplication::translate("MainWindow", "Apply", nullptr));
        chkConnectOnStart->setText(QApplication::translate("MainWindow", "Connect on start", nullptr));
        chkSetProxyConfig->setText(QApplication::translate("MainWindow", "Set and restore system proxy \n"
"on connect or disconnect", nullptr));
        cmbRegions->setItemText(0, QApplication::translate("MainWindow", "Fastest", nullptr));

        label->setText(QApplication::translate("MainWindow", "Country selection", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_config), QApplication::translate("MainWindow", "Tunnel", nullptr));
        chkStratupOnLogin->setText(QApplication::translate("MainWindow", "Start on Login", nullptr));
        chkHideFromDockMac->setText(QApplication::translate("MainWindow", "Hide app from dock (MacOS)", nullptr));
        chkTrayDialog->setText(QApplication::translate("MainWindow", "Window as menu in status icon (tray)", nullptr));
        chkShowWindowOnStart->setText(QApplication::translate("MainWindow", "Show window on start", nullptr));
        chkTunnelNotifications->setText(QApplication::translate("MainWindow", "Notify tunnel connection status change", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "Select icon for status notifications", nullptr));
        btnIconLogo->setText(QApplication::translate("MainWindow", "Logo", nullptr));
        chkCustomCoreExe->setTitle(QApplication::translate("MainWindow", "Custom tunnel core executable", nullptr));
        btnCustomCoreExe->setText(QApplication::translate("MainWindow", "...", nullptr));
        chkCustomCoreConfig->setTitle(QApplication::translate("MainWindow", "Custom tunnel config file", nullptr));
        btnCustomCoreConfig->setText(QApplication::translate("MainWindow", "...", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_settings), QApplication::translate("MainWindow", "Config", nullptr));
        logo_2->setText(QString());
        label_3->setText(QApplication::translate("MainWindow", "PsiphonQt", nullptr));
        label_9->setText(QApplication::translate("MainWindow", "Version 1.0", nullptr));
        label_11->setText(QApplication::translate("MainWindow", "Created 2020, 16-11", nullptr));
        label_12->setText(QApplication::translate("MainWindow", "<html><head/><body><p>by <a href=\"https://github.com/rsanjuan87/\"><span style=\" text-decoration: underline; color:#0068da;\">rsanjuan87</span></a></p></body></html>", nullptr));
        label_13->setText(QApplication::translate("MainWindow", "The program is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.", nullptr));
        commandLinkButton->setText(QApplication::translate("MainWindow", "Contribute with the project !!!", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabAbout), QApplication::translate("MainWindow", "About", nullptr));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", nullptr));
        menuMain->setTitle(QApplication::translate("MainWindow", "Main", nullptr));
        Q_UNUSED(MainWindow);
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
