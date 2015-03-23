#include "soundselect.hh"
#include <QHBoxLayout>
#include <QVector>
#include <QPair>
#include <QUrl>


SoundSelect::SoundSelect(const SoundItemList &sounds, const QString &selectedSound, QWidget *parent) :
  QWidget(parent)
{
  _player = new QSoundEffect(this);

  _sounds = new QComboBox();
  for (int i=0; i<sounds.size(); i++) {
    _sounds->addItem(sounds[i].first, sounds[i].second);
    if (selectedSound == sounds[i].second) { _sounds->setCurrentIndex(i); }
  }
  _play = new QPushButton(tr("play"));
  _play->setCheckable(true);
  _play->setChecked(false);

  QHBoxLayout *layout = new QHBoxLayout();
  layout->setContentsMargins(0,0,0,0);
  layout->addWidget(_sounds);
  layout->addWidget(_play);
  setLayout(layout);

  QObject::connect(_play, SIGNAL(toggled(bool)), this, SLOT(onPlayToggled(bool)));
  QObject::connect(_player, SIGNAL(playingChanged()), this, SLOT(onPlayerStopped()));
}


void
SoundSelect::onPlayToggled(bool play) {
  if (play) {
    _player->setSource(QUrl::fromLocalFile(selectedSound()));
    _player->play();
    _play->setText(tr("stop"));
  } else {
    _player->stop();
    _play->setText(tr("play"));
  }
}

void
SoundSelect::onPlayerStopped() {
  if (_player->isPlaying()) {
    _play->setText(tr("stop"));
    _play->setChecked(true);
  } else {
    _play->setText(tr("play"));
    _play->setChecked(false);
  }
}
