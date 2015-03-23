#include "mainwindow.hh"

MainWindow::MainWindow(Application &app)
  : QMainWindow(), _app(app)
{
  setWindowTitle(tr("Countdown"));

  _countdown = new Countdown(_app);
  setCentralWidget(_countdown);
  setMinimumSize(200,200);
}
