#include "mainwindow.hh"
#include <QCloseEvent>


MainWindow::MainWindow(Application &app, QWidget *parent)
  : QMainWindow(parent), _app(app)
{
  setWindowTitle(tr("Countdown"));
  setWindowIcon(QIcon("://icons/icon32.png"));

  _countdown = new Countdown(_app);
  setCentralWidget(_countdown);
  setMinimumSize(200,200);
}


void
MainWindow::closeEvent(QCloseEvent *evt) {
  hide(); evt->ignore();
  _app.setClockVisibility(Application::HIDDEN);
}
