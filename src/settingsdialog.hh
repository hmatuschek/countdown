#ifndef SETTINGSDIALOG_HH
#define SETTINGSDIALOG_HH

#include <QDialog>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QColor>
#include <QHash>

#include "application.hh"
#include "colorbutton.hh"
#include "soundselect.hh"


/** Combines all profile-specific settings. */
class ProfileSettings
{
public:
  /** Constructor, reads the current settings for the specified profile. */
  ProfileSettings(const QString profile, Application &app);
  /** Copy constructor. */
  ProfileSettings(const ProfileSettings &other);

  /** Assignment. */
  ProfileSettings &operator=(const ProfileSettings &other);

  /** Applies the settings to the application. */
  void apply();

  /** Retunrs the profile name. */
  inline QString profile() const { return _profile; }
  /** Returns the duration of the timer. */
  inline int duration() const { return _duration; }
  /** Sets the duration of the timer. */
  inline void setDuration(int duration) { _duration = duration; }
  /** Returns the duration of the "last minutes". */
  inline int lastMinutes() const { return _lastMinutes; }
  /** Sets the duration of the "last minutes". */
  inline void setLastMinutes(int duration) { _lastMinutes = duration; }
  /** Returns the sound being played as the "last minutes" start. */
  inline const QString &lastMinutesSound() const { return _lmSound; }
  /** Sets the sound being played as the "last minutes" start. */
  inline void setLastMinutesSound(const QString &sound) { _lmSound = sound; }
  /** Returns the sound being played at the end. */
  inline const QString &endSound() const { return _endSound; }
  /** Sets the sound being played at the end. */
  inline void setEndSound(const QString &sound) { _endSound = sound; }
  /** Returns the clock color. */
  inline const QColor &timeColor() const { return _timeColor; }
  /** Sets the clock color. */
  inline void setTimeColor(const QColor &color) { _timeColor = color; }
  /** Returns the clock color for the last minutes. */
  inline const QColor &lastMinutesColor() const { return _lmColor; }
  /** Sets the clock color for the last minutes. */
  inline void setLastMinutesColor(const QColor &color) { _lmColor = color; }
  /** Returns @c true if the clock direction is clock-wise. */
  inline bool clockWise() const { return _clockWise; }
  /** Sets, whether the clock direction is clock-wise. */
  inline void setClockWise(bool clockWise) { _clockWise = clockWise; }
  /** Returns  @c if the time left is shown. */
  inline bool showTimeLeft() const { return _showTimeLeft; }
  /** Sets, whether the time left is shown. */
  inline void setShowTimeLeft(bool showTimeLeft) { _showTimeLeft = showTimeLeft; }
  /** Returns @c true if the clock ticks are shown. */
  inline bool showTicks() const { return _showTicks; }
  /** Sets, whether the clock ticks are shown. */
  inline void setShowTicks(bool showTicks) { _showTicks = showTicks; }

protected:
  /** The current profile name. */
  QString _profile;
  /** Holds a weak reference to the application instance. */
  Application &_application;
  /** Holds the timer duration. */
  int _duration;
  /** Holds the "last minutes". */
  int _lastMinutes;
  /** The "last minutes" sound. */
  QString _lmSound;
  /** The end sound. */
  QString _endSound;
  /** The clock color. */
  QColor _timeColor;
  /** The "last minutes" clock color. */
  QColor _lmColor;
  /** Clock direction. */
  bool _clockWise;
  /** Show time left. */
  bool _showTimeLeft;
  /** Show clock ticks. */
  bool _showTicks;
};


/** The settings dialog. */
class SettingsDialog : public QDialog
{
  Q_OBJECT

public:
  /** Constructor, loads the current settings from the @c app instance. */
  explicit SettingsDialog(Application &app, QWidget *parent = 0);

  /** Returns the selected profile. */
  inline QString profile() { return _profiles->currentData().toString(); }
  /** Retruns true if the system-tray icond should be shown. */
  inline bool showTrayIcon() const { return _showTrayIcon->isChecked(); }

  /** Returns the list of all profile settings. */
  inline QList<ProfileSettings> &profileSettings() {
    return _profileSettings;
  }

protected slots:
  /** Gets called if dialog is accepted. */
  virtual void accept();
  /** Gets called on profile selection. */
  void onProfileSelected(int idx);
  /** Adds a new profile. */
  void onAddProfile();
  /** Removes a profile. */
  void onRemProfile();

protected:
  /** Gets called on close. */
  void closeEvent(QCloseEvent *evt);

protected:
  Application &_app;
  QList<ProfileSettings> _profileSettings;

  QComboBox *_profiles;
  int _lastProfileIndex;

  QSpinBox *_duration;
  QSpinBox *_lastMinutes;
  SoundSelect *_endSound;
  SoundSelect *_lmSound;
  ColorButton *_timeColor;
  ColorButton *_lmColor;
  QCheckBox   *_clockWise;
  QCheckBox   *_showTimeLeft;
  QCheckBox   *_showTicks;
  QCheckBox   *_showTrayIcon;
};

#endif // SETTINGSDIALOG_HH
