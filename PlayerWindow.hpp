#include <QMainWindow>
#include "ZonePlayer.hpp"

class PlayerWindow: public QMainWindow {
	Q_OBJECT
public:
	PlayerWindow(QWidget* par=0);

	ZonePlayer* player;

private:
	void makeToolbar();

public slots:
	void foundDevice(Device* dev);
	void setPlayer(ZonePlayer* pl);
};
