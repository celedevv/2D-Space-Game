#include "network.h"
//#include "widget.h"
#include <QMessageBox>

Network::Network(QObject *parent)
    : QObject(parent),
    socket(new QTcpSocket(this))
{
    connect(socket, &QTcpSocket::connected, this, &Network::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &Network::onDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &Network::onReadyRead);
}

void Network::connectToServer()
{
    socket->connectToHost("10.55.33.18", 9999);

    if (!socket->waitForConnected(1000)) {
        QMessageBox::critical(nullptr, "Fehler", "Verbindung zum Server fehlgeschlagen!");
    }
}

void Network::onConnected()
{
    QMessageBox::information(nullptr, "TCP Client", "Connected to server!");
    setRole("Pilot");
}

void Network::onDisconnected()
{
    QMessageBox::information(nullptr, "TCP Client", "Disconnected from server!");
}

void Network::onReadyRead()
{
    QByteArray data = socket->readAll();
    processRawData(data);
}

void Network::sendCommand(const QString &command)
{
    if (!command.isEmpty()) {
        qDebug() << command;
        socket->write(command.toUtf8() + "\r\n");
    } else {
        QMessageBox::warning(nullptr, "Warnung", "Das Befehlsfeld ist leer!");
    }
}

void Network::processRawData(const QByteArray& data) {
    buffer.append(data);

    while (buffer.contains("\r\n")) {
        int endPos = buffer.indexOf("\r\n");
        QByteArray commandData = buffer.left(endPos);
        buffer = buffer.mid(endPos + 2);

        QString command = QString::fromUtf8(commandData);
        qDebug() << command;
        processCommand(command);
    }
}

void Network::processCommand(const QString& rawCommand) {
    if (!rawCommand.startsWith("#")) {
        emit invalidCommandReceived(rawCommand);
        return;
    }

    if (rawCommand.startsWith("#newEnemy")) {
        emit spawnEnemy();
    } else if (rawCommand.startsWith("#enemyKilled")) {
        emit despawnEnemy();
    }

    QStringList parts = rawCommand.mid(1).split(":");

    if (parts.isEmpty()) {
        emit invalidCommandReceived(rawCommand);
        return;
    }

    QString keyword = parts[0];

    if (keyword == "game") {
        if (parts.size() < 2) {
            emit invalidCommandReceived(rawCommand);
            return;
        }

        QString action = parts[1];

        if (action == "start" && parts.size() == 2) {
            emit doGameStarted();
        }
        else if (action == "over" && parts.size() >= 3) {
            QString reason = parts.mid(2).join(":");
            emit gameOver(reason);
        }
        else if (action == "won" && parts.size() == 2) {
            emit gameWon();
        }
    }
    else {
        emit invalidCommandReceived(rawCommand);
    }
        if (keyword == "drive" && parts.size() == 3) {
            bool okCurrent, okMax;
            float current = parts[1].toFloat(&okCurrent);
            float max = parts[2].toFloat(&okMax);

            if (okCurrent && okMax) {
                emit shipSpeedChanged(current, max);
            }
        }
        else if (keyword == "meteorAmount" && parts.size() == 2) {
            bool ok;
            int amount = parts[1].toInt(&ok);
            if (ok && amount >= 0 && amount <= 100) {
                emit meteorAmountChanged(amount);
            }
        }
        else {
            emit invalidCommandReceived(rawCommand);
        }

}

void Network::setRole(const QString& role)
{
    QString command = QString("#role:%1").arg(role);
    sendCommand(command);
    emit roleAssigned(role);
}
