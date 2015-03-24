#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include "application.hh"
#include "countdown.hh"


class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(Application &app, QWidget *parent=0);

  void closeEvent(QCloseEvent *evt);

protected:
  Application &_app;
  Countdown *_countdown;
};

#endif // MAINWINDOW_HH
