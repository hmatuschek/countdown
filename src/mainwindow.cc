#include "mainwindow.hh"

MainWindow::MainWindow(Application &app)
  : QMainWindow(), _app(app)
{
  _countdown = new Countdown(_app);
  setCentralWidget(_countdown);
  setMinimumSize(200,200);

  QObject::connect(_app.actShowFullScreen(), SIGNAL(toggled(bool)),
                   this, SLOT(onToggleFullScreen(bool)));

  QObject::connect(_app.actQuit(), SIGNAL(triggered()), this, SLOT(close()));
}

void
MainWindow::onToggleFullScreen(bool full) {
  if (full) { showFullScreen(); }
  else { showNormal(); }
}
