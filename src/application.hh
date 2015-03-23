#ifndef APPLICATION_HH
#define APPLICATION_HH

#include <QApplication>
#include <QAction>
#include <QActionGroup>
#include <QSettings>
#include <QTimer>
#include <QString>
#include <QSoundEffect>


// Forward declaration
class MainWindow;


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

  bool isClockHidden();
  void setClockHidden(bool hidden);

public:
  QMenu   *menu();

public slots:
  void startTimer(bool start);

signals:
  void updateClock();
  void durationChanged();

protected slots:
  void onUpdateTimeLeft();
  void onClockVisibilityChanged(QAction *action);
  void onShowSettings();
  void onQuit();

protected:
  QSettings _settings;

  MainWindow *_mainWindow;

  bool _running;
  int  _timeLeft;
  QTimer _timer;

  QMenu   *_menu;
  QAction *_startStop;
  QActionGroup *_display;
  QAction *_hideClock;
  QAction *_showNormal;
  QAction *_showFullScreen;
  QAction *_showSettings;
  QAction *_quit;

  QSoundEffect *_lmSound;
  QSoundEffect *_endSound;
};

#endif // APPLICATION_HH
