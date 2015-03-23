#include "settingsdialog.hh"
#include <QTabWidget>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QColorDialog>
#include "soundselect.hh"

SettingsDialog::SettingsDialog(Application &app, QWidget *parent)
  : QDialog(parent), _app(app)
{
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
  QVector< QPair<QString,QString> > sounds;
  sounds << QPair<QString,QString>(tr("<none>"),"")
         << QPair<QString,QString>(tr("Factory Bell"), "://sounds/bell_factory_break.wav")
         << QPair<QString,QString>(tr("Big Bell"), "://sounds/big_bell.wav")
         << QPair<QString,QString>(tr("Medieval Bell"), "://sounds/medieval_bell.wav")
         << QPair<QString,QString>(tr("Mono Bell"), "://sounds/mono_bell.wav")
         << QPair<QString,QString>(tr("School Bell"), "://sounds/school_bell.wav")
         << QPair<QString,QString>(tr("Single Chime"), "://sounds/single_chime.wav");
  QWidget *soundPage = new QWidget();
  QFormLayout *soundLayout = new QFormLayout();
  _lmSound = new SoundSelect(sounds, _app.lastMinutesSound());
  _endSound = new SoundSelect(sounds, _app.endSound());
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
  layout->addWidget(pages);
  layout->addWidget(bbox);
  this->setLayout(layout);

  QObject::connect(bbox, SIGNAL(accepted()), this, SLOT(accept()));
  QObject::connect(bbox, SIGNAL(rejected()), this, SLOT(reject()));
}
