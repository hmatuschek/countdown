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

  inline int duration() const { return _duration; }
  inline void setDuration(int duration) { _duration = duration; }
  inline int lastMinutes() const { return _lastMinutes; }
  inline void setLastMinutes(int duration) { _lastMinutes = duration; }

  inline const QString &lastMinutesSound() const { return _lmSound; }
  inline void setLastMinutesSound(const QString &sound) { _lmSound = sound; }
  inline const QString &endSound() const { return _endSound; }
  inline void setEndSound(const QString &sound) { _endSound = sound; }

  inline const QColor &timeColor() const { return _timeColor; }
  inline void setTimeColor(const QColor &color) { _timeColor = color; }
  inline const QColor &lastMinutesColor() const { return _lmColor; }
  inline void setLastMinutesColor(const QColor &color) { _lmColor = color; }

  inline bool clockWise() const { return _clockWise; }
  inline void setClockWise(bool clockWise) { _clockWise = clockWise; }
  inline bool showTimeLeft() const { return _showTimeLeft; }
  inline void setShowTimeLeft(bool showTimeLeft) { _showTimeLeft = showTimeLeft; }
  inline bool showTicks() const { return _showTicks; }
  inline void setShowTicks(bool showTicks) { _showTicks = showTicks; }

protected:
  QString _profile;
  Application &_application;

  int _duration;
  int _lastMinutes;
  QString _lmSound;
  QString _endSound;
  QColor _timeColor;
  QColor _lmColor;
  bool _clockWise;
  bool _showTimeLeft;
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
  void onProfileSelected(int idx);
  void onAddProfile();
  void onRemProfile();

protected:
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
