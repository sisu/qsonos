#include "HttpServer.hpp"
#include "HttpSession.hpp"

HttpServer::HttpServer() {
	connect(this, SIGNAL(newConnection()),
			this, SLOT(getConnection()));
}

void HttpServer::start() {
	listen();
}

void HttpServer::getConnection() {
	while(hasPendingConnections()) {
		QTcpSocket* sock = nextPendingConnection();
		new HttpSession(sock, *this);
	}
}
