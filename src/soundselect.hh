#ifndef SOUNDSELECT_HH
#define SOUNDSELECT_HH

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QSoundEffect>

class SoundSelect : public QWidget
{
  Q_OBJECT
public:
  typedef QVector< QPair<QString, QString>  > SoundItemList;

public:
  explicit SoundSelect(const SoundItemList &sounds, const QString &selectedSound,
                       QWidget *parent = 0);

  inline QString selectedSound() { return _sounds->currentData().toString(); }

protected slots:
  void onPlayToggled(bool play);
  void onPlayerStopped();

protected:
  QComboBox *_sounds;
  QPushButton *_play;
  QSoundEffect *_player;
};

#endif // SOUNDSELECT_HH
