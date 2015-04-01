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
#include <QSystemTrayIcon>
#include <QTranslator>

// Forward declaration
class MainWindow;
class SettingsDialog;


/** Central application class.
 * This class implements all the application logic. */
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
   * to a WAVE file. Qt resource paths are possible. */
  typedef QVector< QPair<QString, QString>  > SoundItemList;

public:
  /** Constructor. */
  explicit Application(int &argc, char *argv[]);

  /** Retunrs a list of all known profiles. */
  QStringList profiles();
  /** Updates the list of all known profiles. */
  void setProfiles(const QStringList &profiles);

  /** Returns true if the given profile is known. */
  bool hasProfile(const QString &name);
  /** Adds a new profile. */
  void addProfile(const QString &name);
  /** Returns the currently selected profile. */
  QString profile();
  /** Sets a profile. */
  void setProfile(const QString &profile);

  /** If true, a system-tray icon is shown. */
  bool showTrayIcon();
  /** Show/hides the system-tray icon. */
  void setShowTrayIcon(bool enable);

  /** Returns the duration of the timer for the specified profile or for the current profile
   * (default). */
  int duration(const QString &prof=QString());
  /** Sets the duration of the timer for the speicifed profile or for the current profile
   * (default). */
  void setDuration(int dur, const QString &prof=QString());

  /** Returns the duration of the "last minutes" for the specified profile or for the current
   * profile (default). */
  int lastMinutes(const QString &prof=QString());
  /** Sets the duration of the "last minutes" for the specified profile or for the current
   * profile (default). */
  void setLastMinutes(int N, const QString &prof=QString());

  /** Returns the current timer state. */
  TimerState timerState() const;
  /** Sets the timer state. */
  void setTimerState(TimerState state);

  /** Retunrs the current clock visibility. */
  ClockVisibility clockVisibility() const;
  /** Sets the current clock visibility. */
  void setClockVisibility(ClockVisibility vis);

  /** Returns the current time left. */
  int timeLeft() const;
  /** Returns the current time left in 10th of a minute. */
  int ticksLeft() const;

  /** Returns whether the time left is shown. */
  bool showTimeLeft(const QString &prof=QString());
  /** Enables/Disables wheter the time left is shown. */
  void setShowTimeLeft(bool show, const QString &prof=QString());

  /** Returns @c true if the clock ticks are shown for the specified profile or the current
   * profile (default). */
  bool showTicks(const QString &prof=QString());
  /** Sets whether the clock ticks are shown for the specified profile or the current
   * profile (default). */
  void setShowTicks(bool show, const QString &prof=QString());

  /** Returns @c true if the direction of the clock is clock-wise for the specified or current
   * profile (default). */
  bool clockWise(const QString &prof=QString());
  /** Specifies whether the direction of the clock is clock-wise for the specified or current
   * profile (default). */
  void setClockWise(bool clockWise, const QString &prof=QString());

  /** Returns the clock color for the specified or current profile. */
  QColor timeColor(const QString &prof=QString());
  /** Sets the clock color for the specified or current profile. */
  void setTimeColor(const QColor &color, const QString &prof=QString());
  /** Returns the clock color for the "last minutes" for the specified or current profile. */
  QColor lastMinutesColor(const QString &prof=QString());
  /** Specifies the clock color for the "last minutes" for the specified or current profile. */
  void setLastMinutesColor(const QColor &color, const QString &prof=QString());

  /** Returns @c true if the time left is smaller that the specified "last minutes". */
  bool isInLastMinutes();

  /** Returns the list of known sound items. */
  const SoundItemList &soundItems() const;
  /** Retruns @c true if the specified sound item is known. */
  bool hasSoundItem(const QString &filename) const;
  /** Adds a sound item. */
  void addSoundItem(const QString &filename);

  /** Retunrs the selected sound item, played at the end for the specified or current profile.*/
  QString endSound(const QString &prof=QString());
  /** Sets sound item, played at the end for the specified or current profile.*/
  void setEndSound(const QString &file, const QString &prof=QString());

  /** Retunrs the selected sound item, played at the start of the "last minutes" for the specified
   * or current profile.*/
  QString lastMinutesSound(const QString &prof=QString());
  /** Sets the selected sound item, played at the start of the "last minutes" for the specified
   * or current profile.*/
  void setLastMinutesSound(const QString &file, const QString &prof=QString());

public:
  /** Constructs and returns the context menu. It will be shown as a context menu of the
   * clock-display and as the menu of the system-tray icon. */
  QMenu   *menu();

signals:
  /** Gets emitted when the clock display should be updated. */
  void updateClock();
  /** Gets emitted when the duration changed. */
  void durationChanged();

protected slots:
  /** Handle "timer start" events. */
  void onTimerStart();
  /** Handle "timer pause" events. */
  void onTimerPause();
  /** Handle system-tray icon events. */
  void onTrayIconActivated(QSystemTrayIcon::ActivationReason action);
  /** Handle "update time left" events. */
  void onUpdateTimeLeft();
  /** Handle "clock visibility" events. */
  void onClockVisibilityChanged(QAction *action);
  /** Shows the settings dialog. */
  void onShowSettings();
  /** Shows the "about" dialog. */
  void onAbout();
  /** Quits the application. */
  void onQuit();

protected:
  /** Persistent settings. */
  QSettings _settings;
  /** Current translation. */
  QTranslator _translator;

  /** The main window instance. */
  MainWindow *_mainWindow;
  /** The system-tray icon instance. */
  QSystemTrayIcon *_trayIcon;
  /** Holds the menu instance. */
  QMenu   *_menu;
  //QMenu   *_profileMenu;

  /** Holds the current timer state. */
  TimerState _timerState;
  /** Holds the time left in 10th of a minute. */
  int  _timeLeft;
  /** The timer, triggers an event every 10th of a minute. */
  QTimer _timer;

  /** Holds the current clock visibility. */
  ClockVisibility _clockVisibility;
  /** Holds the list of known sounds. */
  SoundItemList _sounds;

  /** Menu action to start/stop the timer. */
  QAction *_startStop;
  /** Menu action to pause/resume the timer. */
  QAction *_pause;
  /** An action group for clock-display modes. */
  QActionGroup *_clockDisplay;
  /** Menu action to hide the clock display. */
  QAction *_hideClock;
  /** Menu action to show the clock display in "normal" mode. */
  QAction *_showNormal;
  /** Menu action to show the clock display "always on top". */
  QAction *_showOnTop;
  /** Menu action to show the clock display in full-screen. */
  QAction *_showFullScreen;
  /** Menu action to show settings. */
  QAction *_showSettings;
  /** Menu action to show the "about" dialog. */
  QAction *_about;
  /** Menu action to quit the application. */
  QAction *_quit;

  /** The sound effect being played on the start of the "last minutes". */
  QSoundEffect *_lmSound;
  /** The sound effect being played at the end. */
  QSoundEffect *_endSound;
};

#endif // APPLICATION_HH
