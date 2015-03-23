#ifndef APPLICATION_HH
#define APPLICATION_HH

#include <QApplication>
#include <QAction>
#include <QSettings>
#include <QTimer>


class Application : public QApplication
{
  Q_OBJECT

public:
  explicit Application(int argc, char *argv[]);

  int duration();
  void setDuration(int dur);

  int lastMinutes();
  void setLastMinutes(int N);

  bool running() const;
  int timeLeft() const;
  int ticksLeft() const;

  bool showTimeLeft();
  void setShowTimeLeft(bool show);

  bool isInLastMinutes();

public:
  QAction *actShowFullScreen();
  QAction *actStartStop();
  QAction *actShowSettings();
  QAction *actQuit();

signals:
  void updateClock();
  void durationChanged();

protected slots:
  void onStartStop(bool start);
  void onUpdateTimeLeft();

protected:
  QSettings _settings;
  bool _running;
  int  _timeLeft;
  QTimer _timer;

  QAction *_showFullScreen;
  QAction *_startStop;
  QAction *_showSettings;
  QAction *_quit;
};

#endif // APPLICATION_HH
