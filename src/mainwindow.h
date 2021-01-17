#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "proxysetter.h"

#include <QMainWindow>
#include <QProcess>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QWidgetAction>
#include <QtWidgets>
#include <QFileInfo>
#include <QDir>
#include <QFileDialog>
#include <QSysInfo>
#include <QOperatingSystemVersion>
#include <QMessageBox>
#include <QPainter>
#include <QTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDesktopServices>
#include <QScreen>

class MouseButtonSignaler : public QObject {
  Q_OBJECT
  bool eventFilter(QObject * obj, QEvent * ev) Q_DECL_OVERRIDE {
    if (obj->isWidgetType())
      emit mouseButtonEvent(static_cast<QWidget*>(obj),
                            static_cast<QMouseEvent*>(ev));
    return false;
  }
public:
  Q_SIGNAL void mouseButtonEvent(QWidget *, QMouseEvent *);
  MouseButtonSignaler(QObject * parent = 0) : QObject(parent) {}
  void installOn(QWidget * widget) {
    widget->installEventFilter(this);
  }
};


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QProcess *process, QSystemTrayIcon *tray, QMenu *menu, QWidget *parent = nullptr);
    ~MainWindow();
    enum Position{ TOP, BOTTOM, LEFT, RIGHT};

public slots:
    void toggleWindow();
    void setPage(int index);

    bool close();
    void show();
    void parceReceivedMessage( quint32 instanceId, QByteArray message );
    void setVisible(bool visible);
private slots:
    void startStopTunnel();
    void readout();
    void setTunelStoped(int i=0);
    void on_pushButton_2_clicked();
    void loadconfig();
    void readTunelConfig();
    void writeTunelConfig();
    void on_btnSaveTunnelConfig_clicked();
    void writeAppConfig();
    void readAppConfig();
    void on_chkCustomCoreExe_clicked(bool checked);
    void on_btnCustomCoreExe_clicked();
    void on_chkCustomCoreConfig_clicked(bool checked);
    void on_btnCustomCoreConfig_clicked();
    void askForReset();
    void on_chkHideFromDockMac_clicked();
    void setTunnelConnected();
    QImage gray(QImage original);
    void setTunnelConnecting();
    void timerTick();
    QImage setOpacity(QImage image, qreal opacity);
    void setIcon(QString icon);

    void on_btnAbout_clicked();

    void on_actionAbout_triggered();

    void on_actionTunel_settings_triggered();

    void on_actionSettings_triggered();

    void on_actionAbout_Qt_triggered();

    void on_commandLinkButton_clicked();

    void on_chkConnectOnStart_clicked();

    void on_chkShowWindowOnStart_clicked();

    void on_chkStratupOnLogin_toggled(bool checked);

    void on_chkTrayDialog_clicked();

    void setTrayMenu(bool showPage = false);
    void popup(QSystemTrayIcon::ActivationReason reason);
    void on_chkTunnelNotifications_clicked();

    void onGeneralClick(QWidget *widget, QMouseEvent *event);
    void createIconsMenu();
    QPair<Position, int> getWinTaskbarData();
    void on_btnClearLogs_clicked();

    void on_btnSaveLogs_clicked();

    void on_chkSetProxyConfig_clicked();

    void startTunnel();
    void stopTunnel();
    void detectSetWindowsTheme();
    QImage getImageTrayIcon();
    void updateRegions();
    void on_btnCountryFlag_clicked();

private:
    Ui::MainWindow *ui;

    QProcess *process;
    QSystemTrayIcon *tray;
    QMenu *trayMenu;
    QMenu *dockMenu;
    QString trayIcon;
    QMenuBar *menuBar;

    QString tunelCorePath;
    QString tunelCoreDir;
    QList<QString> regionsList;
    QString configPath;
    QTimer timerConnecting;
    QWidgetAction *widPopup;
    QMenu *winPopup;
    MouseButtonSignaler signaler;
    QMenu *iconMenu;

    QWidgetAction *wid1;
    QWidgetAction *wid2;
    QWidgetAction *wid3;
    QWidgetAction *wid4;
    QWidgetAction *wid5;
    QToolButton *btn1;
    QToolButton *btn2;
    QToolButton *btn3;
    QToolButton *btn4;
    QToolButton *btn5;
    bool wasConnected = false;
    bool noShowScreenCapPermission = false;
    int askPermission =  QMessageBox::Open;

    ProxySetter *systemProxy;
};


#endif // MAINWINDOW_H
