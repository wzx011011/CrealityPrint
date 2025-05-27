#include "qtuserqml/main/main.h"
#include <QtQml/QQmlContext>
#include <QtQml/QQmlEngine>
#include <QtCore/QCoreApplication>
#include <QtSerialPort/QSerialPort>

#include "scence3dapp.h"

int main(int argc, char *argv[])
{
	return qtuser_qml::qmlAppMain(argc, argv, scene3dapp_startEngine);
}
