#include "soundselect.hh"
#include <QHBoxLayout>
#include <QVector>
#include <QPair>
#include <QUrl>
#include <QFileDialog>
#include <QFileInfo>


SoundSelect::SoundSelect(const Application::SoundItemList &sounds,
                         const QString &selectedSound, QWidget *parent) :
  QWidget(parent)
{
  _player = new QSoundEffect(this);

  _sounds = new QComboBox();
  _sounds->addItem(tr("Select file..."));
  _sounds->addItem(tr("<none>"));
  _sounds->setCurrentIndex(1);
  for (int i=0; i<sounds.size(); i++) {
    _sounds->addItem(sounds[i].first, sounds[i].second);
    if (selectedSound == sounds[i].second) {
      _sounds->setCurrentIndex(i+2);
    }
  }

  _play = new QPushButton(tr("play"));
  _play->setCheckable(true);
  _play->setChecked(false);

  QHBoxLayout *layout = new QHBoxLayout();
  layout->setContentsMargins(0,0,0,0);
  layout->addWidget(_sounds);
  layout->addWidget(_play);
  setLayout(layout);

  QObject::connect(_sounds, SIGNAL(currentIndexChanged(int)), this, SLOT(onSoundSelected(int)));
  QObject::connect(_play, SIGNAL(toggled(bool)), this, SLOT(onPlayToggled(bool)));
  QObject::connect(_player, SIGNAL(playingChanged()), this, SLOT(onPlaybackChanged()));
}


void
SoundSelect::onPlayToggled(bool play) {
  if (play) {
    _player->setSource(QUrl::fromLocalFile(selectedSound()));
    _player->play();
  } else {
    _player->stop();
  }
}

void
SoundSelect::onPlaybackChanged() {
  if (_player->isPlaying()) {
    _play->setText(tr("stop"));
    _play->setChecked(true);
  } else {
    _play->setText(tr("play"));
    _play->setChecked(false);
  }
}

void
SoundSelect::onSoundSelected(int idx) {
  if (0 == idx) {
    QString filename = QFileDialog::getOpenFileName(0, tr("Select a sound file"),
                                                    "", tr("Sound Files (*.wav)"));
    if (filename.isEmpty()) { _sounds->setCurrentIndex(1); }
    QFileInfo info(filename);
    _sounds->addItem(info.baseName(), info.canonicalFilePath());
    _sounds->setCurrentIndex(_sounds->count()-1);
  }
}
