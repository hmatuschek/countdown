#include "mainwindow.hh"
#include <QCloseEvent>


MainWindow::MainWindow(Application &app)
  : QMainWindow(), _app(app)
{
  setWindowTitle(tr("Countdown"));

  _countdown = new Countdown(_app);
  setCentralWidget(_countdown);
  setMinimumSize(200,200);
}


void
MainWindow::closeEvent(QCloseEvent *evt) {
  hide(); evt->ignore();
  _app.setClockVisibility(Application::HIDDEN);
}
