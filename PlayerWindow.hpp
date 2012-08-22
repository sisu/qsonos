#include <QMainWindow>
#include "ZonePlayer.hpp"

class PlaylistWidget;
class QSlider;

class PlayerWindow: public QMainWindow {
	Q_OBJECT
public:
	PlayerWindow(QWidget* par=0);

	ZonePlayer* player;
	PlaylistWidget* playlist;
	QSlider* playSlider;

private:
	void makeToolbar();

public slots:
	void foundDevice(Device* dev);
	void setPlayer(ZonePlayer* pl);
	void handleChange(ArgMap args);
	void handleResult(ArgMap args);
};
