#ifndef SOUNDSELECT_HH
#define SOUNDSELECT_HH

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QSoundEffect>
#include "application.hh"

/** A small widget to select and test selected sounds. */
class SoundSelect : public QWidget
{
  Q_OBJECT

public:
  /** Constructor. */
  explicit SoundSelect(const Application::SoundItemList &sounds, const QString &selectedSound,
                       QWidget *parent = 0);

  /** Returns the path to the selected sound. */
  inline QString selectedSound() { return _sounds->currentData().toString(); }
  /** Selects the specified sound. */
  void selectSound(const QString &sound);

protected slots:
  void onSoundSelected(int idx);
  /** Start/stops playback. */
  void onPlayToggled(bool play);
  /** Gets called once the playback started or stopped. */
  void onPlaybackChanged();

protected:
  QComboBox *_sounds;
  QPushButton *_play;
  QSoundEffect *_player;
};

#endif // SOUNDSELECT_HH
