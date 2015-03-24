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



SettingsDialog::SettingsDialog(Application &app, QWidget *parent)
  : QDialog(parent), _app(app)
{
  _profiles = new QComboBox();
  _profiles->addItem(tr("Default"), "");
  _profiles->setCurrentIndex(0);
  QStringList profiles = _app.profiles();
  for (int i=0; i<profiles.size(); i++) {
    _profiles->addItem(profiles.at(i), profiles.at(i));
    if (profiles.at(i) == _app.profile()) { _profiles->setCurrentIndex(i+1); }
  }
  QHBoxLayout *profLayout = new QHBoxLayout();
  QPushButton *addProf = new QPushButton(tr("+"));
  QPushButton *remProf = new QPushButton(tr("-"));
  profLayout->addWidget(new QLabel(tr("Profile")), 1);
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
  _lastMinutes = new QSpinBox();
  _lastMinutes->setMinimum(0); _duration->setMaximum(1000);
  _lastMinutes->setValue(_app.lastMinutes());
  timerLayout->addRow(tr("Duration [min]"), _duration);
  timerLayout->addRow(tr("Last minutes"), _lastMinutes);
  timerPage->setLayout(timerLayout);
  pages->addTab(timerPage, tr("Timer"));

  /* Sound settings */
  QWidget *soundPage = new QWidget();
  QFormLayout *soundLayout = new QFormLayout();
  _lmSound = new SoundSelect(_app.soundItems(), _app.lastMinutesSound());
  _endSound = new SoundSelect(_app.soundItems(), _app.endSound());
  soundLayout->addRow(tr("Last minutes sound"), _lmSound);
  soundLayout->addRow(tr("End sound"), _endSound);
  soundPage->setLayout(soundLayout);
  pages->addTab(soundPage, tr("Sound"));

  /* Color settings */
  QWidget *displayPage = new QWidget();
  QFormLayout *displayLayout = new QFormLayout();
  _timeColor = new ColorButton(_app.timeColor());
  _lmColor   = new ColorButton(_app.lastMinutesColor());
  _showTimeLeft = new QCheckBox();
  _showTimeLeft->setChecked(_app.showTimeLeft());
  _showTicks = new QCheckBox();
  _showTicks->setChecked(_app.showTicks());

  displayLayout->addRow(tr("Default color"), _timeColor);
  displayLayout->addRow(tr("Last minutes color"), _lmColor);
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
  QString profile = _profiles->currentData().toString();
  // Update dialog
  _duration->setValue(_app.duration(profile));
  _lastMinutes->setValue(_app.lastMinutes(profile));
  _lmSound->selectSound(_app.lastMinutesSound(profile));
  _endSound->selectSound(_app.endSound(profile));
  _timeColor->setColor(_app.timeColor(profile));
  _lmColor->setColor(_app.lastMinutesColor(profile));
  _showTimeLeft->setCheckState(_app.showTimeLeft(profile) ? Qt::Checked : Qt::Unchecked);
  _showTicks->setCheckState(_app.showTicks(profile) ? Qt::Checked : Qt::Unchecked);
}

void
SettingsDialog::onAddProfile() {
  QString name = QInputDialog::getText(0, tr("Select a profile name"), tr("Profile name"));
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