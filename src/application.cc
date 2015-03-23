#include "application.hh"
#include "settingsdialog.hh"
#include "mainwindow.hh"
#include <QMenu>


Application::Application(int &argc, char *argv[]) :
  QApplication(argc, argv),
  _settings("com.github.hmatuschek", "Countdown"), _timer(), _menu(0)
{
  setWindowIcon(QIcon("://icons/icon32.png"));

  _running = false;
  _timeLeft = 10*duration();

  // 10x per minute
  _timer.setInterval(6000);
  _timer.setSingleShot(false);

  // Assemble actions
  _startStop = new QAction(tr("Start"), this);
  _startStop->setCheckable(true);
  _startStop->setChecked(false);

  _hideClock = new QAction(tr("Hide"), this);
  _hideClock->setCheckable(true);
  _hideClock->setChecked(isClockHidden());
  _showNormal = new QAction(tr("Normal"), this);
  _showNormal->setCheckable(true);
  _showNormal->setChecked(! isClockHidden());
  _showFullScreen = new QAction(tr("Full screen"), this);
  _showFullScreen->setCheckable(true);
  _showFullScreen->setChecked(false);
  _display = new QActionGroup(this);
  _display->setExclusive(true);
  _display->addAction(_hideClock);
  _display->addAction(_showNormal);
  _display->addAction(_showFullScreen);

  _showSettings = new QAction(tr("Settings..."), this);
  _quit  = new QAction(tr("Quit"), this);

  // Load sound effects
  _lmSound = new QSoundEffect();
  _endSound = new QSoundEffect();
  if (! lastMinutesSound().isEmpty()) {
    _lmSound->setSource(QUrl::fromLocalFile(lastMinutesSound()));
  }
  _lmSound->setLoopCount(1);
  if (! endSound().isEmpty()) {
    _endSound->setSource(QUrl::fromLocalFile(endSound()));
  }
  _endSound->setLoopCount(1);

  QObject::connect(_startStop, SIGNAL(toggled(bool)), this, SLOT(startTimer(bool)));
  QObject::connect(&_timer, SIGNAL(timeout()), this, SLOT(onUpdateTimeLeft()));
  QObject::connect(_showSettings, SIGNAL(triggered()), this, SLOT(onShowSettings()));
  QObject::connect(_display, SIGNAL(triggered(QAction*)),
                   this, SLOT(onClockVisibilityChanged(QAction*)));
  QObject::connect(_quit, SIGNAL(triggered()), this, SLOT(onQuit()));

  _mainWindow = new MainWindow(*this);
  if (isClockHidden()) {
    _mainWindow->setVisible(false);
  } else {
    _mainWindow->show();
  }
}

int
Application::duration() {
  return _settings.value("duration", 45).toInt();
}

void
Application::setDuration(int dur) {
  dur = std::max(0, dur);
  _settings.setValue("duration", std::max(0, dur));

  _timeLeft = std::min(10*dur, _timeLeft);
  emit durationChanged();
  emit updateClock();
}

int
Application::lastMinutes() {
  return _settings.value("lastMinutes", 5).toInt();
}

void
Application::setLastMinutes(int N) {
  N = std::max(0, N);
  _settings.setValue("lastMinutes", N);
}

bool
Application::running() const {
  return _running;
}

int
Application::timeLeft() const {
  return _timeLeft/10 + ((_timeLeft%10) ? 1 : 0);
}

int
Application::ticksLeft() const {
  return _timeLeft;
}

bool
Application::showTimeLeft() {
  return _settings.value("showTimeLeft", true).toBool();
}

void
Application::setShowTimeLeft(bool show) {
  return _settings.setValue("showTimeLeft", show);
  emit updateClock();
}

bool
Application::showTicks() {
  return _settings.value("showTicks", true).toBool();
}

void
Application::setShowTicks(bool show) {
  _settings.setValue("showTicks", show);
}

QColor
Application::timeColor() {
  return _settings.value("timeColor", QColor("blue")).value<QColor>();
}

void
Application::setTimeColor(const QColor &color) {
  _settings.setValue("timeColor", color);
}

QColor
Application::lastMinutesColor() {
  return _settings.value("lastMinutesColor", QColor("red")).value<QColor>();
}

void
Application::setLastMinutesColor(const QColor &color) {
  return _settings.setValue("lastMinutesColor", color);
}

bool
Application::isInLastMinutes() {
  return _running && (0 != _timeLeft) && (_timeLeft <= 10*lastMinutes());
}

QString
Application::endSound() {
  return _settings.value("endSound").toString();
}

void
Application::setEndSound(const QString &file) {
  return _settings.setValue("endSound", file);
}

QString
Application::lastMinutesSound() {
  return _settings.value("lastMinutesSound").toString();
}

void
Application::setLastMinutesSound(const QString &file) {
  return _settings.setValue("lastMinutesSound", file);
}

bool
Application::isClockHidden() {
  return _settings.value("clockHidden", true).toBool();
}

void
Application::setClockHidden(bool hidden) {
  _settings.setValue("clockHidden", hidden);
}

QMenu   *Application::menu() {
  if (0 == _menu) {
    // Assemble Menu
    _menu = new QMenu();
    _menu->addAction(_startStop);
    _menu->addAction(_showSettings);
    _menu->addSeparator();
    _menu->addSection(tr("Visibility"));
    _menu->addAction(_hideClock);
    _menu->addAction(_showNormal);
    _menu->addAction(_showFullScreen);
    _menu->addSeparator();
    _menu->addAction(_quit);
  }
  return _menu;
}


void
Application::startTimer(bool start) {
  _running = start;
  if (_running) {
    _startStop->setChecked(true);
    _timeLeft = 10*duration();
    _timer.start();
  } else {
    _startStop->setChecked(false);
    _timer.stop();
    _timeLeft = 10*duration();
  }
  emit updateClock();
}


void
Application::onUpdateTimeLeft()
{
  _timeLeft--;

  if (0 == _timeLeft) {
    startTimer(false);
    _endSound->play();
  } else if (10*lastMinutes() == (_timeLeft)) {
    _lmSound->play();
  }
  emit updateClock();
}


void
Application::onClockVisibilityChanged(QAction *action) {
  if (_hideClock == action) {
    _mainWindow->showNormal();
    _mainWindow->hide();
    setClockHidden(true);
  } else if (_showNormal == action) {
    _mainWindow->showNormal();
    _mainWindow->activateWindow();
    setClockHidden(false);
  } else if (_showFullScreen == action) {
    _mainWindow->showFullScreen();
    setClockHidden(false);
  }
}

void
Application::onQuit() {
  _mainWindow->close();
}

void
Application::onShowSettings() {
  SettingsDialog dialog(*this);
  if (QDialog::Accepted != dialog.exec()) { return; }

  setEndSound(dialog.endSound());
  setLastMinutesSound(dialog.lastMinutesSound());

  setTimeColor(dialog.timeColor());
  setLastMinutesColor(dialog.lastMinutesColor());

  setShowTicks(dialog.showTicks());
  setShowTimeLeft(dialog.showTimeLeft());

  setDuration(dialog.duration());
  setLastMinutes(dialog.lastMinutes());
}
