#include <QMainWindow>
#include "common.hpp"

class ZonePlayer;
class Device;
class PlaylistWidget;
class QSlider;
class QLabel;
class QAction;
class QCloseEvent;
class QTimerEvent;

class PlayerWindow: public QMainWindow {
	Q_OBJECT
public:
	PlayerWindow(QWidget* par=0);

	ZonePlayer* player;

	PlaylistWidget* playlist;
	QSlider* playSlider;
	QLabel* timeLabel;

	virtual void closeEvent(QCloseEvent* event);
	virtual void timerEvent(QTimerEvent* event);

private:
	void readSettings();
	void makeToolbar();
	void connectToolbar();
	QAction* buttonActions[5];

public slots:
	void foundDevice(Device* dev);
	void setPlayer(ZonePlayer* pl);
	void handleChange(ArgMap args);
	void handleResult(ArgMap args);
};
