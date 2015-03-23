#ifndef APPLICATION_HH
#define APPLICATION_HH

#include <QApplication>
#include <QAction>
#include <QSettings>
#include <QTimer>
#include <QString>
#include <QSoundEffect>


class Application : public QApplication
{
  Q_OBJECT

public:
  explicit Application(int &argc, char *argv[]);

  int duration();
  void setDuration(int dur);

  int lastMinutes();
  void setLastMinutes(int N);

  bool running() const;
  int timeLeft() const;
  int ticksLeft() const;

  bool showTimeLeft();
  void setShowTimeLeft(bool show);

  bool showTicks();
  void setShowTicks(bool show);

  QColor timeColor();
  void setTimeColor(const QColor &color);
  QColor lastMinutesColor();
  void setLastMinutesColor(const QColor &color);

  bool isInLastMinutes();

  QString endSound();
  void setEndSound(const QString &file);

  QString lastMinutesSound();
  void setLastMinutesSound(const QString &file);

public:
  QAction *actShowFullScreen();
  QAction *actQuit();
  QMenu   *menu();

public slots:
  void startTimer(bool start);

signals:
  void updateClock();
  void durationChanged();

protected slots:
  void onUpdateTimeLeft();
  void onShowSettings();

protected:
  QSettings _settings;
  bool _running;
  int  _timeLeft;
  QTimer _timer;

  QMenu   *_menu;
  QAction *_startStop;
  QAction *_showFullScreen;
  QAction *_showSettings;
  QAction *_quit;

  QSoundEffect *_lmSound;
  QSoundEffect *_endSound;
};

#endif // APPLICATION_HH
