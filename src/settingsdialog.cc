#include "settingsdialog.hh"
#include <QTabWidget>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QColorDialog>
#include "soundselect.hh"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>
#include <QCloseEvent>


/* ******************************************************************************************* *
 * Implementation of ProfileSettings
 * ******************************************************************************************* */
ProfileSettings::ProfileSettings(const QString profile, Application &app)
  : _profile(profile), _application(app)
{
  _duration     = _application.duration(_profile);
  _lastMinutes  = _application.lastMinutes(_profile);
  _lmSound      = _application.lastMinutesSound(_profile);
  _endSound     = _application.endSound(_profile);
  _timeColor    = _application.timeColor(_profile);
  _lmColor      = _application.lastMinutesColor(_profile);
  _clockWise    = _application.clockWise(_profile);
  _showTimeLeft = _application.showTimeLeft(_profile);
  _showTicks    = _application.showTicks();
}

ProfileSettings::ProfileSettings(const ProfileSettings &other)
  : _profile(other._profile), _application(other._application),
    _duration(other._duration), _lastMinutes(other._lastMinutes),
    _lmSound(other._lmSound), _endSound(other._endSound),
    _timeColor(other._timeColor), _clockWise(other._clockWise),
    _showTimeLeft(other._showTimeLeft), _showTicks(other._showTicks)
{
  // pass...
}

ProfileSettings &
ProfileSettings::operator =(const ProfileSettings &other) {
  _duration     = other._duration;
  _lastMinutes  = other._lastMinutes;
  _lmSound      = other._lmSound;
  _endSound     = other._endSound;
  _timeColor    = other._timeColor;
  _lmColor      = other._lmColor;
  _clockWise    = other._clockWise;
  _showTimeLeft = other._showTimeLeft;
  _showTicks    = other._showTicks;

  return *this;
}

void
ProfileSettings::apply() {
  _application.setDuration(_duration, _profile);
  _application.setLastMinutes(_lastMinutes, _profile);
  _application.setLastMinutesSound(_lmSound, _profile);
  _application.setEndSound(_endSound, _profile);
  _application.setTimeColor(_timeColor, _profile);
  _application.setLastMinutesColor(_lmColor, _profile);
  _application.setClockWise(_clockWise, _profile);
  _application.setShowTimeLeft(_showTimeLeft, _profile);
  _application.setShowTicks(_showTicks, _profile);
}



/* ******************************************************************************************* *
 * Implementation of SettingsDialog
 * ******************************************************************************************* */
SettingsDialog::SettingsDialog(Application &app, QWidget *parent)
  : QDialog(parent), _app(app)
{
  _profiles = new QComboBox();
  _profiles->addItem(tr("Default"), "");
  _profiles->setToolTip(tr("Select a profile."));
  _profiles->setCurrentIndex(0);
  _lastProfileIndex = 0;

  /* Load all profile settings. */
  QStringList profiles = _app.profiles();
  _profileSettings.push_back(ProfileSettings("", _app));
  for (int i=0; i<profiles.size(); i++) {
    _profiles->addItem(profiles.at(i), profiles.at(i));
    if (profiles.at(i) == _app.profile()) {
      _profiles->setCurrentIndex(i+1);
      _lastProfileIndex = (i+1);
    }
    _profileSettings.push_back(ProfileSettings(profiles.at(i), _app));
  }

  QHBoxLayout *profLayout = new QHBoxLayout();
  QPushButton *addProf = new QPushButton(tr("+"));
  addProf->setToolTip(tr("Create a new profile."));
  QPushButton *remProf = new QPushButton(tr("-"));
  remProf->setToolTip(tr("Delete the selected profile."));
  profLayout->addWidget(new QLabel(tr("Profile")));
  profLayout->addWidget(_profiles, 1);
  profLayout->addWidget(addProf);
  profLayout->addWidget(remProf);

  // Profile settings
  QTabWidget *pages = new QTabWidget();

  /* Timer settings */
  QWidget *timerPage = new QWidget();
  QFormLayout *timerLayout = new QFormLayout();
  _duration = new QSpinBox();
  _duration->setMinimum(1); _duration->setMaximum(1000);
  _duration->setValue(_app.duration());
  _duration->setToolTip(tr("Set the overall duration of the timer."));
  _lastMinutes = new QSpinBox();
  _lastMinutes->setMinimum(0); _duration->setMaximum(1000);
  _lastMinutes->setValue(_app.lastMinutes());
  _lastMinutes->setToolTip(tr("Specify the last minutes of the timer.\n"
                              "The clock display will change the color "
                              "for this timespan"));
  timerLayout->addRow(tr("Duration [min]"), _duration);
  timerLayout->addRow(tr("Last minutes"), _lastMinutes);
  timerPage->setLayout(timerLayout);
  pages->addTab(timerPage, tr("Timer"));

  /* Sound settings */
  QWidget *soundPage = new QWidget();
  QFormLayout *soundLayout = new QFormLayout();
  _lmSound = new SoundSelect(_app.soundItems(), _app.lastMinutesSound());
  _lmSound->setToolTip(tr("Select the sound to be played when the last minutes starts.\n"
                          "Select <none> to disable."));
  _endSound = new SoundSelect(_app.soundItems(), _app.endSound());
  _endSound->setToolTip(tr("Select the sound to be played when the time is over.\n"
                           "Select <none> to disable."));
  soundLayout->addRow(tr("Last minutes sound"), _lmSound);
  soundLayout->addRow(tr("End sound"), _endSound);
  soundPage->setLayout(soundLayout);
  pages->addTab(soundPage, tr("Sound"));

  /* Color settings */
  QWidget *displayPage = new QWidget();
  QFormLayout *displayLayout = new QFormLayout();
  _timeColor = new ColorButton(_app.timeColor());
  _timeColor->setToolTip(tr("Select the color for the clock."));
  _lmColor   = new ColorButton(_app.lastMinutesColor());
  _lmColor->setToolTip(tr("Select the color for the clock when the last minutes starts."));
  _clockWise = new QCheckBox();
  _clockWise->setChecked(_app.clockWise());
  _clockWise->setToolTip(tr("Specifies the clock direction."));
  _showTimeLeft = new QCheckBox();
  _showTimeLeft->setChecked(_app.showTimeLeft());
  _showTimeLeft->setToolTip(tr("If selected, the remaining mintues are shown."));
  _showTicks = new QCheckBox();
  _showTicks->setChecked(_app.showTicks());
  _showTicks->setToolTip(tr("If selected, the clock ticks are displayed."));
  _showTrayIcon = new QCheckBox();
  _showTrayIcon->setChecked(_app.showTrayIcon());
  _showTrayIcon->setToolTip(tr("If selected, the tray-icon is shown."));

  displayLayout->addRow(tr("Default color"), _timeColor);
  displayLayout->addRow(tr("Last minutes color"), _lmColor);
  displayLayout->addRow(tr("Clockwise"), _clockWise);
  displayLayout->addRow(tr("Show time left"), _showTimeLeft);
  displayLayout->addRow(tr("Show time ticks"), _showTicks);
  displayLayout->addRow(tr("Show tray icon"), _showTrayIcon);

  displayPage->setLayout(displayLayout);
  pages->addTab(displayPage, tr("Display"));

  QDialogButtonBox *bbox
      = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal);
  QVBoxLayout *layout = new QVBoxLayout();
  layout->addLayout(profLayout);
  layout->addWidget(pages);
  layout->addWidget(bbox);
  this->setLayout(layout);

  QObject::connect(bbox, SIGNAL(accepted()), this, SLOT(accept()));
  QObject::connect(bbox, SIGNAL(rejected()), this, SLOT(reject()));
  QObject::connect(_profiles, SIGNAL(currentIndexChanged(int)), this, SLOT(onProfileSelected(int)));
  QObject::connect(addProf, SIGNAL(clicked()), this, SLOT(onAddProfile()));
  QObject::connect(remProf, SIGNAL(clicked()), this, SLOT(onRemProfile()));
}


void
SettingsDialog::onProfileSelected(int idx)
{
  // Save the current settings
  _profileSettings[_lastProfileIndex].setDuration(_duration->value());
  _profileSettings[_lastProfileIndex].setLastMinutes(_lastMinutes->value());
  _profileSettings[_lastProfileIndex].setLastMinutesSound(_lmSound->selectedSound());
  _profileSettings[_lastProfileIndex].setEndSound(_endSound->selectedSound());
  _profileSettings[_lastProfileIndex].setTimeColor(_timeColor->color());
  _profileSettings[_lastProfileIndex].setLastMinutesColor(_lmColor->color());
  _profileSettings[_lastProfileIndex].setClockWise(_clockWise->isChecked());
  _profileSettings[_lastProfileIndex].setShowTimeLeft(_showTimeLeft->isChecked());
  _profileSettings[_lastProfileIndex].setShowTicks(_showTicks->isChecked());

  // Update dialog for selected profile
  _duration->setValue(_profileSettings.at(idx).duration());
  _lastMinutes->setValue(_profileSettings.at(idx).lastMinutes());
  _lmSound->selectSound(_profileSettings.at(idx).lastMinutesSound());
  _endSound->selectSound(_profileSettings.at(idx).endSound());
  _timeColor->setColor(_profileSettings.at(idx).timeColor());
  _lmColor->setColor(_profileSettings.at(idx).lastMinutesColor());
  _clockWise->setChecked(_profileSettings.at(idx).clockWise());
  _showTimeLeft->setChecked(_profileSettings.at(idx).showTimeLeft());
  _showTicks->setChecked(_profileSettings.at(idx).showTicks());

  // Store currently selected profile index
  _lastProfileIndex = idx;
}

void
SettingsDialog::onAddProfile() {
  QString name = QInputDialog::getText(
        0, tr("Select a profile name"), tr("Profile name")).simplified();
  if (name.isEmpty()) { return; }
  // Check if profile exists
  for (int i=1; i<_profiles->count(); i++) {
    if (name == _profiles->itemData(i).toString()) {
      QMessageBox::critical(0, tr("Can not create profile"),
                            tr("Can not create profile '%1': Profile exists already.").arg(name));
      return;
    }
  }

  // Add profile and select it
  _profiles->addItem(name, name);
  _profileSettings.push_back(ProfileSettings("", _app));
  _profiles->setCurrentIndex(_profiles->count()-1);
}

void
SettingsDialog::onRemProfile() {
  if (0 == _lastProfileIndex) {
    QMessageBox::information(0, tr("Can not remove profile"),
                             tr("Can not remove default profile."));
    return;
  }
  int currentIdx = _lastProfileIndex;
  // Remove profile from combobox, this also updates the "removed" settings
  _profiles->removeItem(currentIdx);
  // Finally remove the associated settings
  _profileSettings.removeAt(currentIdx);
}

void
SettingsDialog::closeEvent(QCloseEvent *evt) {
  hide(); evt->ignore();
}
