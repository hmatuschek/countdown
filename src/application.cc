#include "application.hh"
#include <QSound>

Application::Application(int argc, char *argv[]) :
  QApplication(argc, argv), _settings("com.github.hmatuschek", "Countdown"),
  _timer()
{
  _running = false;
  _timeLeft = 10*duration();

  // 10x per minute
  _timer.setInterval(6000);
  _timer.setSingleShot(false);

  _showFullScreen = new QAction(tr("Full screen"), this);
  _showFullScreen->setCheckable(true);
  _showFullScreen->setChecked(false);

  _startStop = new QAction(tr("Start"), this);
  _startStop->setCheckable(true);
  _startStop->setChecked(false);

  _showSettings = new QAction(tr("Settings"), this);
  _quit  = new QAction(tr("Quit"), this);

  QObject::connect(_startStop, SIGNAL(toggled(bool)), this, SLOT(onStartStop(bool)));
  QObject::connect(&_timer, SIGNAL(timeout()), this, SLOT(onUpdateTimeLeft()));
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
Application::isInLastMinutes() {
  return _running && (0 != _timeLeft) && (_timeLeft/10 <= lastMinutes());
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


QAction *Application::actShowFullScreen() { return _showFullScreen; }
QAction *Application::actStartStop() { return _startStop; }
QAction *Application::actShowSettings() { return _showSettings; }
QAction *Application::actQuit() { return _quit; }


void
Application::onStartStop(bool start) {
  _running = start;
  if (_running) {
    _timeLeft = 10*duration();
    _timer.start();
  } else {
    _timer.stop();
    _timeLeft = 0;
  }
  emit updateClock();
}


void
Application::onUpdateTimeLeft()
{
  _timeLeft--;

  if (0 == _timeLeft) {
    QSound::play(endSound());
  } else if (lastMinutes() == (_timeLeft/10)) {
    QSound::play(lastMinutesSound());
  }
  emit updateClock();
}
