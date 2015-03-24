#include "application.hh"
#include "settingsdialog.hh"
#include "mainwindow.hh"
#include <QMenu>
#include <QFileInfo>

Application::Application(int &argc, char *argv[]) :
  QApplication(argc, argv),
  _settings("com.github.hmatuschek", "Countdown"), _timer(), _menu(0)
{
  setWindowIcon(QIcon("://icons/icon32.png"));

  _timerState = STOPPED;
  _timeLeft = 10*duration();

  // 10x per minute
  _timer.setInterval(6000);
  _timer.setSingleShot(false);


  // Assemble actions
  _startStop = new QAction(tr("Start"), this);
  _pause     = new QAction(tr("Pause"), this);
  _pause->setEnabled(false);

  _hideClock = new QAction(tr("Hide"), this);
  _hideClock->setCheckable(true);
  _hideClock->setChecked(false);
  _showNormal = new QAction(tr("Normal"), this);
  _showNormal->setCheckable(true);
  _showNormal->setChecked(true);
  _showOnTop = new QAction(tr("On top"), this);
  _showOnTop->setCheckable(true);
  _showOnTop->setChecked(false);
  _showFullScreen = new QAction(tr("Full screen"), this);
  _showFullScreen->setCheckable(true);
  _showFullScreen->setChecked(false);
  _clockDisplay = new QActionGroup(this);
  _clockDisplay->setExclusive(true);
  _clockDisplay->addAction(_hideClock);
  _clockDisplay->addAction(_showNormal);
  _clockDisplay->addAction(_showOnTop);
  _clockDisplay->addAction(_showFullScreen);

  _showSettings = new QAction(tr("Settings..."), this);
  _quit  = new QAction(tr("Quit"), this);

  // create list of known sounds
  _sounds << QPair<QString,QString>(tr("Factory Bell"), "://sounds/bell_factory_break.wav")
      << QPair<QString,QString>(tr("Big Bell"), "://sounds/big_bell.wav")
      << QPair<QString,QString>(tr("Medieval Bell"), "://sounds/medieval_bell.wav")
      << QPair<QString,QString>(tr("Mono Bell"), "://sounds/mono_bell.wav")
      << QPair<QString,QString>(tr("School Bell"), "://sounds/school_bell.wav")
      << QPair<QString,QString>(tr("Single Chime"), "://sounds/single_chime.wav");

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

  // Create main window
  QWidget *dummy = new QWidget();
  _mainWindow = new MainWindow(*this, dummy);
  setClockVisibility(NORMAL);

  QObject::connect(_startStop, SIGNAL(triggered()), this, SLOT(onTimerStart()));
  QObject::connect(_pause, SIGNAL(triggered()), this, SLOT(onTimerPause()));
  QObject::connect(&_timer, SIGNAL(timeout()), this, SLOT(onUpdateTimeLeft()));
  QObject::connect(_showSettings, SIGNAL(triggered()), this, SLOT(onShowSettings()));
  QObject::connect(_clockDisplay, SIGNAL(triggered(QAction*)),
                   this, SLOT(onClockVisibilityChanged(QAction*)));
  QObject::connect(_quit, SIGNAL(triggered()), this, SLOT(onQuit()));

}


Application::TimerState
Application::timerState() const {
  return _timerState;
}

void
Application::setTimerState(TimerState state) {
  _timerState = state;
  if (RUNNING == _timerState) {
    _pause->setEnabled(true);
    _startStop->setText(tr("Stop"));
    _pause->setText(tr("Pause"));
    _timer.start();
  } else if (STOPPED == _timerState) {
    _pause->setEnabled(false);
    _startStop->setText(tr("Start"));
    _pause->setText(tr("Pause"));
    _timer.stop();
    _timeLeft = 10*duration();
  } else if (PAUSED == _timerState) {
    _pause->setEnabled(true);
    _startStop->setText(tr("Stop"));
    _pause->setText(tr("Resume"));
    _timer.stop();
  }
  emit updateClock();
}

int
Application::timeLeft() const {
  return _timeLeft/10 + ((_timeLeft%10) ? 1 : 0);
}

int
Application::ticksLeft() const {
  return _timeLeft;
}

Application::ClockVisibility
Application::clockVisibility() const {
  return _clockVisibility;
}

void
Application::setClockVisibility(ClockVisibility vis) {
  _clockVisibility = vis;
  Qt::WindowFlags flags = _mainWindow->windowFlags();
  switch (_clockVisibility) {
  case HIDDEN:
    _mainWindow->setWindowFlags(flags ^ (Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint));
    _mainWindow->showNormal();
    _mainWindow->hide();
    _hideClock->setChecked(true);
    break;
  case NORMAL:
    _mainWindow->setWindowFlags(flags ^ (Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint));
    _mainWindow->showNormal();
    _mainWindow->activateWindow();
    _showNormal->setChecked(true);
    break;
  case ONTOP:
    _mainWindow->setWindowFlags(flags | (Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint));
    _mainWindow->showNormal();
    _mainWindow->activateWindow();
    _showOnTop->setChecked(true);
    break;
  case FULLSCREEN:
    _mainWindow->setWindowFlags(flags ^ (Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint));
    _mainWindow->showFullScreen();
    _showFullScreen->setChecked(true);
    break;
  }
}

QStringList
Application::profiles() {
  QStringList prfs;
  int N = _settings.beginReadArray("profiles");
  for (int i=0; i<N; i++) {
    _settings.setArrayIndex(i);
    prfs.append(_settings.value("name").toString());
  }
  _settings.endArray();
  //qDebug() << "Profiles:" << prfs;
  return prfs;
}

bool
Application::hasProfile(const QString &name) {
  if (name.isEmpty()) { return true; }
  int N = _settings.beginReadArray("profiles");
  for (int i=0; i<N; i++) {
    _settings.setArrayIndex(i);
    if (_settings.value("name").toString() == name) {
      _settings.endArray();
      //qDebug() << "Found profile:" << name;
      return true;
    }
  }
  _settings.endArray();
  //qDebug() << "Profile" << name << "not found.";
  return false;
}

void
Application::addProfile(const QString &name) {
  //qDebug() << "Add profile" << name;
  QStringList prfs;
  int N = _settings.beginReadArray("profiles");
  for (int i=0; i<N; i++) {
    _settings.setArrayIndex(i);
    prfs.append(_settings.value("name").toString());
  }
  _settings.endArray();
  prfs.append(name);
  _settings.beginWriteArray("profiles");
  for (int i=0; i<(N+1); i++) {
    _settings.setArrayIndex(i);
    _settings.setValue("name", prfs.at(i));
  }
  _settings.endArray();
}

QString
Application::profile() {
  //qDebug() << "Current profile" << _settings.value("profile").toString();
  return _settings.value("profile").toString();
}

void
Application::setProfile(const QString &profile) {
  if (! hasProfile(profile)) { addProfile(profile); }
  _settings.setValue("profile", profile);
}

int
Application::duration(const QString &prof) {
  QString profile = prof;
  if (profile.isNull()) { profile = this->profile(); }
  return _settings.value(QString("%1/duration").arg(profile), 45).toInt();
}

void
Application::setDuration(int dur, const QString &prof) {
  dur = std::max(0, dur);
  QString profile = prof;
  if (profile.isNull()) { profile = this->profile(); }
  _settings.setValue(QString("%1/duration").arg(profile), dur);

  _timeLeft = std::min(10*dur, _timeLeft);
  emit durationChanged();
  emit updateClock();
}

int
Application::lastMinutes(const QString &prof) {
  QString profile = prof;
  if (profile.isNull()) { profile = this->profile(); }
  return _settings.value(QString("%1/lastMinutes").arg(profile), 5).toInt();
}

void
Application::setLastMinutes(int N, const QString &prof) {
  N = std::max(0, N);
  QString profile = prof;
  if (profile.isNull()) { profile = this->profile(); }
  _settings.setValue(QString("%1/lastMinutes").arg(profile), N);
}

bool
Application::showTimeLeft(const QString &prof) {
  QString profile = prof;
  if (profile.isNull()) { profile = this->profile(); }
  return _settings.value(QString("%1/showTimeLeft").arg(profile), true).toBool();
}

void
Application::setShowTimeLeft(bool show, const QString &prof) {
  QString profile = prof;
  if (profile.isNull()) { profile = this->profile(); }
  return _settings.setValue(QString("%1/showTimeLeft").arg(profile), show);
  emit updateClock();
}

bool
Application::showTicks(const QString &prof) {
  QString profile = prof;
  if (profile.isNull()) { profile = this->profile(); }
  return _settings.value(QString("%1/showTicks").arg(profile), true).toBool();
}

void
Application::setShowTicks(bool show, const QString &prof) {
  QString profile = prof;
  if (profile.isNull()) { profile = this->profile(); }
  _settings.setValue(QString("%1/showTicks").arg(profile), show);
}

bool
Application::clockWise(const QString &prof) {
  QString profile = prof;
  if (profile.isNull()) { profile = this->profile(); }
  return _settings.value(QString("%1/clockWise").arg(profile), true).toBool();
}

void
Application::setClockWise(bool clockWise, const QString &prof) {
  QString profile = prof;
  if (profile.isNull()) { profile = this->profile(); }
  _settings.setValue(QString("%1/clockWise").arg(profile), clockWise);
}

QColor
Application::timeColor(const QString &prof) {
  QString profile = prof;
  if (profile.isNull()) { profile = this->profile(); }
  return _settings.value(QString("%1/timeColor").arg(profile),
                         QColor("blue")).value<QColor>();
}

void
Application::setTimeColor(const QColor &color, const QString &prof) {
  QString profile = prof;
  if (profile.isNull()) { profile = this->profile(); }
  _settings.setValue(QString("%1/timeColor").arg(profile), color);
}

QColor
Application::lastMinutesColor(const QString &prof) {
  QString profile = prof;
  if (profile.isNull()) { profile = this->profile(); }
  return _settings.value(QString("%1/lastMinutesColor").arg(profile),
                         QColor("red")).value<QColor>();
}

void
Application::setLastMinutesColor(const QColor &color, const QString &prof) {
  QString profile = prof;
  if (profile.isNull()) { profile = this->profile(); }
  return _settings.setValue(QString("%1/lastMinutesColor").arg(profile), color);
}

bool
Application::isInLastMinutes() {
  return (0 < _timeLeft) && (_timeLeft <= 10*lastMinutes());
}

const Application::SoundItemList &
Application::soundItems() const {
  return _sounds;
}

bool
Application::hasSoundItem(const QString &filename) const {
  QFileInfo info(filename);
  QPair<QString, QString> pair;
  foreach (pair, _sounds) {
    if (pair.second == info.canonicalFilePath()) { return true; }
  }
  return false;
}

void
Application::addSoundItem(const QString &filename) {
  QFileInfo info(filename);
  if (! hasSoundItem(filename)) {
    _sounds << QPair<QString,QString>(info.baseName(), info.canonicalFilePath());
  }
}

QString
Application::endSound(const QString &prof) {
  QString profile = prof;
  if (profile.isNull()) { profile = this->profile(); }
  return _settings.value(QString("%1/endSound").arg(profile)).toString();
}

void
Application::setEndSound(const QString &file, const QString &prof) {
  QString profile = prof;
  if (profile.isNull()) { profile = this->profile(); }
  _settings.setValue(QString("%1/endSound").arg(profile), file);
}

QString
Application::lastMinutesSound(const QString &prof) {
  QString profile = prof;
  if (profile.isNull()) { profile = this->profile(); }
  return _settings.value(QString("%1/lastMinutesSound").arg(profile)).toString();
}

void
Application::setLastMinutesSound(const QString &file, const QString &prof) {
  QString profile = prof;
  if (profile.isNull()) { profile = this->profile(); }
  _settings.setValue(QString("%1/lastMinutesSound").arg(profile), file);
}


QMenu
*Application::menu() {
  if (0 == _menu) {
    // Assemble Menu
    _menu = new QMenu();
    _menu->addAction(_startStop);
    _menu->addAction(_pause);
    _menu->addSeparator();
    QMenu *viewMenu = _menu->addMenu(tr("View"));
    viewMenu->addAction(_hideClock);
    viewMenu->addAction(_showNormal);
    viewMenu->addAction(_showOnTop);
    viewMenu->addAction(_showFullScreen);
    _menu->addSeparator();
    _menu->addAction(_showSettings);
    _menu->addSeparator();
    _menu->addAction(_quit);
  }
  return _menu;
}


void
Application::onTimerStart() {
  if (STOPPED == timerState()) { setTimerState(RUNNING); }
  else { setTimerState(STOPPED); }
}

void
Application::onTimerPause() {
  if (RUNNING == timerState()) { setTimerState(PAUSED); }
  else { setTimerState(RUNNING); }
}

void
Application::onUpdateTimeLeft()
{
  _timeLeft--;

  if (0 == _timeLeft) {
    setTimerState(STOPPED);
    _endSound->play();
  } else if (10*lastMinutes() == (_timeLeft)) {
    _lmSound->play();
  }
  emit updateClock();
}


void
Application::onClockVisibilityChanged(QAction *action) {
  if (_hideClock == action) {
    setClockVisibility(HIDDEN);
  } else if (_showNormal == action) {
    setClockVisibility(NORMAL);
  } else if (_showOnTop == action) {
    setClockVisibility(ONTOP);
  } else if (_showFullScreen == action) {
    setClockVisibility(FULLSCREEN);
  }
}

void
Application::onQuit() {
  quit();
}

void
Application::onShowSettings() {
  SettingsDialog dialog(*this);
  if (QDialog::Accepted != dialog.exec()) {
    return;
  }

  setProfile(dialog.profile());

  setEndSound(dialog.endSound());
  setLastMinutesSound(dialog.lastMinutesSound());

  setTimeColor(dialog.timeColor());
  setLastMinutesColor(dialog.lastMinutesColor());

  setShowTicks(dialog.showTicks());
  setShowTimeLeft(dialog.showTimeLeft());

  setClockWise(dialog.clockWise());
  setDuration(dialog.duration());
  setLastMinutes(dialog.lastMinutes());
}
