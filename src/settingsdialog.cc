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



SettingsDialog::SettingsDialog(Application &app, QWidget *parent)
  : QDialog(parent), _app(app)
{
  _profiles = new QComboBox();
  _profiles->addItem(tr("Default"), "");
  _profiles->setCurrentIndex(0);
  _profiles->setToolTip(tr("Select a profile."));
  QStringList profiles = _app.profiles();
  for (int i=0; i<profiles.size(); i++) {
    _profiles->addItem(profiles.at(i), profiles.at(i));
    if (profiles.at(i) == _app.profile()) { _profiles->setCurrentIndex(i+1); }
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

  displayLayout->addRow(tr("Default color"), _timeColor);
  displayLayout->addRow(tr("Last minutes color"), _lmColor);
  displayLayout->addRow(tr("Clockwise"), _clockWise);
  displayLayout->addRow(tr("Show time left"), _showTimeLeft);
  displayLayout->addRow(tr("Show time ticks"), _showTicks);
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
SettingsDialog::onProfileSelected(int idx) {
  QString profile = _profiles->itemData(idx).toString();
  // Update dialog for selected profile
  _duration->setValue(_app.duration(profile));
  _lastMinutes->setValue(_app.lastMinutes(profile));
  _lmSound->selectSound(_app.lastMinutesSound(profile));
  _endSound->selectSound(_app.endSound(profile));
  _timeColor->setColor(_app.timeColor(profile));
  _lmColor->setColor(_app.lastMinutesColor(profile));
  _clockWise->setChecked(_app.clockWise(profile));
  _showTimeLeft->setChecked(_app.showTimeLeft(profile));
  _showTicks->setChecked(_app.showTicks(profile));
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
  _profiles->setCurrentIndex(_profiles->count()-1);
}

void
SettingsDialog::onRemProfile() {
  if (0 == _profiles->currentIndex()) {
    QMessageBox::information(0, tr("Can not remove profile"),
                             tr("Can not remove default profile."));
    return;
  }
  _removedProfiles << _profiles->currentData().toString();
  _profiles->removeItem(_profiles->currentIndex());
}

void
SettingsDialog::closeEvent(QCloseEvent *evt) {
  hide(); evt->ignore();
}
