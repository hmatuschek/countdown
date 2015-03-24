#ifndef APPLICATION_HH
#define APPLICATION_HH

#include <QApplication>
#include <QAction>
#include <QActionGroup>
#include <QSettings>
#include <QTimer>
#include <QString>
#include <QSoundEffect>
#include <QPair>

// Forward declaration
class MainWindow;
class SettingsDialog;

class Application : public QApplication
{
  Q_OBJECT

public:
  /** Possible states of the timer. Initially: STOPPED. */
  typedef enum {
    STOPPED, PAUSED, RUNNING
  } TimerState;

  /** Possible clock-window visibility settings. */
  typedef enum {
    HIDDEN, NORMAL, ONTOP, FULLSCREEN
  } ClockVisibility;

  /** Container type for known sounds. The first element is the name and the second a file path
   * to the WAVE file. Qt resource paths are possible. */
  typedef QVector< QPair<QString, QString>  > SoundItemList;

public:
  explicit Application(int &argc, char *argv[]);

  QStringList profiles();
  bool hasProfile(const QString &name);
  void addProfile(const QString &name);

  QString profile();
  void setProfile(const QString &profile);

  int duration(const QString &prof=QString());
  void setDuration(int dur, const QString &prof=QString());

  int lastMinutes(const QString &prof=QString());
  void setLastMinutes(int N, const QString &prof=QString());

  TimerState timerState() const;
  void setTimerState(TimerState state);

  ClockVisibility clockVisibility() const;
  void setClockVisibility(ClockVisibility vis);

  int timeLeft() const;
  int ticksLeft() const;

  bool showTimeLeft(const QString &prof=QString());
  void setShowTimeLeft(bool show, const QString &prof=QString());

  bool showTicks(const QString &prof=QString());
  void setShowTicks(bool show, const QString &prof=QString());

  QColor timeColor(const QString &prof=QString());
  void setTimeColor(const QColor &color, const QString &prof=QString());
  QColor lastMinutesColor(const QString &prof=QString());
  void setLastMinutesColor(const QColor &color, const QString &prof=QString());

  bool isInLastMinutes();

  const SoundItemList &soundItems() const;
  bool hasSoundItem(const QString &filename) const;
  void addSoundItem(const QString &filename);

  QString endSound(const QString &prof=QString());
  void setEndSound(const QString &file, const QString &prof=QString());

  QString lastMinutesSound(const QString &prof=QString());
  void setLastMinutesSound(const QString &file, const QString &prof=QString());

public:
  QMenu   *menu();

signals:
  void updateClock();
  void durationChanged();

protected slots:
  void onTimerStart();
  void onTimerPause();
  void onUpdateTimeLeft();
  void onClockVisibilityChanged(QAction *action);
  void onShowSettings();
  void onQuit();

protected:
  QSettings _settings;
  MainWindow *_mainWindow;

  TimerState _timerState;
  int  _timeLeft;
  QTimer _timer;

  ClockVisibility _clockVisibility;
  SoundItemList _sounds;

  QMenu   *_menu;
  QAction *_startStop;
  QAction *_pause;
  QActionGroup *_clockDisplay;
  QAction *_hideClock;
  QAction *_showNormal;
  QAction *_showOnTop;
  QAction *_showFullScreen;
  QAction *_showSettings;
  QAction *_quit;

  QSoundEffect *_lmSound;
  QSoundEffect *_endSound;
};

#endif // APPLICATION_HH
