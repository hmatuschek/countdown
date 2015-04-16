#include "application.hh"
#include "settingsdialog.hh"
#include "mainwindow.hh"
#include <QMenu>
#include <QFileInfo>
#include <QMessageBox>
#include <QStringList>
#include <QDebug>


Application::Application(int &argc, char *argv[])
  : QApplication(argc, argv), _settings("com.github.hmatuschek", "Countdown"),
    _translator(), _menu(0), _timer()
{
  _translator.load("://i18n/countdown.qm");
  this->installTranslator(&_translator);

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
  if (! showTrayIcon()) { _hideClock->setEnabled(false); }

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

  _showSettings = new QAction(tr("Settings ..."), this);
  _about = new QAction(tr("About Countdown"), this);
  _quit  = new QAction(tr("Quit"), this);

  // create list of known sounds
  _sounds << QPair<QString,QString>(tr("Factory Bell"), "://sounds/bell_factory_break.wav")
      << QPair<QString,QString>(tr("Big Bell"), "://sounds/big_bell.wav")
      << QPair<QString,QString>(tr("Medieval Bell"), "://sounds/medieval_bell.wav")
      << QPair<QString,QString>(tr("Mono Bell"), "://sounds/mono_bell.wav")
      << QPair<QString,QString>(tr("School Bell"), "://sounds/school_bell.wav")
      << QPair<QString,QString>(tr("Single Chime"), "://sounds/single_chime.wav")
      << QPair<QString,QString>(tr("Steam Whistle"), "://sounds/steam_whistle.wav");

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
  _mainWindow = new MainWindow(*this);
  setClockVisibility(NORMAL);

  // Create tray icon
  _trayIcon = new QSystemTrayIcon();
  _trayIcon->setIcon(QIcon("://icons/icon_stopped32.png"));
  _trayIcon->setContextMenu(menu());
  if (showTrayIcon()) {
    _trayIcon->show();
  }

  QObject::connect(_startStop, SIGNAL(triggered()), this, SLOT(onTimerStart()));
  QObject::connect(_pause, SIGNAL(triggered()), this, SLOT(onTimerPause()));
  QObject::connect(&_timer, SIGNAL(timeout()), this, SLOT(onUpdateTimeLeft()));
  QObject::connect(_showSettings, SIGNAL(triggered()), this, SLOT(onShowSettings()));
  QObject::connect(_clockDisplay, SIGNAL(triggered(QAction*)),
                   this, SLOT(onClockVisibilityChanged(QAction*)));
  QObject::connect(_about, SIGNAL(triggered()), this, SLOT(onAbout()));
  QObject::connect(_quit, SIGNAL(triggered()), this, SLOT(onQuit()));
  QObject::connect(_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                   this, SLOT(onTrayIconActivated(QSystemTrayIcon::ActivationReason)));

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
    _trayIcon->setIcon(QIcon("://icons/icon_run32.png"));
  } else if (STOPPED == _timerState) {
    _pause->setEnabled(false);
    _startStop->setText(tr("Start"));
    _pause->setText(tr("Pause"));
    _timer.stop();
    _timeLeft = 10*duration();
    _trayIcon->setIcon(QIcon("://icons/icon_stopped32.png"));
  } else if (PAUSED == _timerState) {
    _pause->setEnabled(true);
    _startStop->setText(tr("Stop"));
    _pause->setText(tr("Resume"));
    _timer.stop();
    _trayIcon->setIcon(QIcon("://icons/icon_pause32.png"));
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
  Qt::WindowFlags flags;
  switch (_clockVisibility) {
  case HIDDEN:
    _mainWindow->showNormal();
    flags = _mainWindow->windowFlags();
    _mainWindow->setWindowFlags(flags & (! Qt::WindowStaysOnTopHint));
    _mainWindow->hide();
    _hideClock->setChecked(true);
    break;
  case NORMAL:
    _mainWindow->showNormal();
    flags = _mainWindow->windowFlags();
    _mainWindow->setWindowFlags(flags & (! Qt::WindowStaysOnTopHint));
    _mainWindow->show();
    _showNormal->setChecked(true);
    break;
  case ONTOP:
    _mainWindow->showNormal();
    flags = _mainWindow->windowFlags();
    _mainWindow->setWindowFlags(flags | Qt::WindowStaysOnTopHint);
    _mainWindow->show();
    _showOnTop->setChecked(true);
    break;
  case FULLSCREEN:
    flags = _mainWindow->windowFlags();
    _mainWindow->setWindowFlags(flags & (! Qt::WindowStaysOnTopHint));
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
  return prfs;
}

void
Application::setProfiles(const QStringList &profiles) {
  _settings.beginWriteArray("profiles", profiles.size());
  for (int i=0; i<profiles.size(); i++) {
    _settings.setArrayIndex(i);
    _settings.setValue("name", profiles.at(i));
  }
  _settings.endArray();
  _settings.sync();
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
  _settings.sync();
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

bool
Application::showTrayIcon() {
  return _settings.value("showTrayIcon", true).toBool();
}

void
Application::setShowTrayIcon(bool enable) {
  _settings.setValue("showTrayIcon", enable);
  if (enable) {
    _hideClock->setEnabled(true);
    _trayIcon->show();
  } else {
    if (HIDDEN == clockVisibility()) {
      setClockVisibility(NORMAL);
    }
    _hideClock->setEnabled(false);
    _trayIcon->hide();
  }
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
  // Get profile
  QString profile = prof;
  if (profile.isNull()) { profile = this->profile(); }
  // store sound
  _settings.setValue(QString("%1/endSound").arg(profile), file);
  // update playback
  if (! endSound().isEmpty()) {
    _endSound->setSource(QUrl::fromLocalFile(endSound()));
  }
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
  if (! lastMinutesSound().isEmpty()) {
    _lmSound->setSource(QUrl::fromLocalFile(lastMinutesSound()));
  }
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
    _menu->addAction(_about);
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
Application::onTrayIconActivated(QSystemTrayIcon::ActivationReason action) {
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
Application::onAbout() {
  QMessageBox::about(
        0, tr("About Countdown"),
        tr("<h2 align=\"center\">Countdown - version 1.2.1</h2>"
           "<h4 align=\"center\"><a href=\"https://github.com/hmatuschek/countdown\">"
           "https://github.com/hmatuschek/countdown</a></h4>"
           "<p align=\"center\">(c) 2015, by Hannes Matuschek <hmatuschek@gmail.com></p>"
           "<p>This program is free software; you can redistribute it and/or modify it under the "
           "terms of the GNU General Public License as published by the Free Software Foundation; "
           "either version 2 of the License, or (at your option) any later version.</p>"
           "<p>This program is distributed in the hope that it will be useful, but WITHOUT ANY "
           "WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A "
           "PARTICULAR PURPOSE. See the GNU General Public License for more details.</p>"
           "<p>You should have received a copy of the GNU General Public License along with "
           "this program; if not, write to the<br/>"
           "Free Software Foundation, Inc.,<br/>"
           "51 Franklin Street, Fifth Floor,<br/>"
           "Boston, MA 02110-1301, USA.</p>"));
}

void
Application::onQuit() {
  if (RUNNING == timerState()) {
    if (QMessageBox::Yes != QMessageBox::question(
          0, tr("Quit?"), tr("The timer is still running. Realy quit?"),
          QMessageBox::No|QMessageBox::Yes, QMessageBox::No)) {
      return;
    }
  }
  quit();
}

void
Application::onShowSettings()
{
  // Show dialog
  SettingsDialog dialog(*this);
  if (QDialog::Accepted != dialog.exec()) { return; }

  // Get list of all profile settings
  QList<ProfileSettings> &settings = dialog.profileSettings();
  QList<QString> profileNames;

  // Apply all settings left
  for (int i=0; i<settings.size(); i++) {
    settings[i].apply();
    if (i>0) { profileNames.push_back(settings[i].profile()); }
  }

  // Update list of available profiles
  setProfiles(profileNames);

  // Get selected profile
  QString profile = dialog.profile();

  // Set the current profile
  setProfile(profile);
  // Update "show tray icon".
  setShowTrayIcon(dialog.showTrayIcon());
}
