#ifndef SOUNDSELECT_HH
#define SOUNDSELECT_HH

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QSoundEffect>


/** A small widget to select and test selected sounds. */
class SoundSelect : public QWidget
{
  Q_OBJECT

public:
  /** Container type for known sounds. The first element is the name and the second a file path
   * to the WAVE file. Qt resource paths are possible. */
  typedef QVector< QPair<QString, QString>  > SoundItemList;

public:
  explicit SoundSelect(const SoundItemList &sounds, const QString &selectedSound,
                       QWidget *parent = 0);

  /** Returns the path to the selected sound. */
  inline QString selectedSound() { return _sounds->currentData().toString(); }

protected slots:
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
