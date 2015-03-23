#ifndef SETTINGSDIALOG_HH
#define SETTINGSDIALOG_HH

#include <QDialog>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include "application.hh"
#include "colorbutton.hh"

class SettingsDialog : public QDialog
{
  Q_OBJECT

public:
  explicit SettingsDialog(Application &app, QWidget *parent = 0);

  inline int duration() { return _duration->value(); }
  inline int lastMinutes() { return _lastMinutes->value(); }

  inline QString lastMinutesSound() { return _lmSound->currentData().toString(); }
  inline QString endSound() { return _endSound->currentData().toString(); }

  inline const QColor &timeColor() const { return _timeColor->color(); }
  inline const QColor &lastMinutesColor() const { return _lmColor->color(); }

  inline bool showTimeLeft() const { return _showTimeLeft->isChecked(); }
  inline bool showTicks() const { return _showTicks->isChecked(); }

protected:
  Application &_app;
  QSpinBox *_duration;
  QSpinBox *_lastMinutes;
  QComboBox *_endSound;
  QComboBox *_lmSound;
  ColorButton *_timeColor;
  ColorButton *_lmColor;
  QCheckBox   *_showTimeLeft;
  QCheckBox   *_showTicks;
};

#endif // SETTINGSDIALOG_HH
