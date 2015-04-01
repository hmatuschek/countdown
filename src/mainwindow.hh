#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include "application.hh"
#include "countdown.hh"


/** Countdown clock window. */
class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  /** Constructor. */
  explicit MainWindow(Application &app, QWidget *parent=0);
  /** Gets called on closing the window. */
  void closeEvent(QCloseEvent *evt);

protected:
  /** Holds a weak reference to the application instance. */
  Application &_app;
  /** Holds the countdown clock widget. */
  Countdown *_countdown;
};

#endif // MAINWINDOW_HH
