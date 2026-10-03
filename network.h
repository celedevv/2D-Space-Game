#ifndef NETWORK_H
#define NETWORK_H

#include <QTcpSocket>
#include <QWidget>
#include <QObject>

class Network : public QObject
{
    Q_OBJECT
public:
    explicit Network(QObject *parent = nullptr);
    void sendCommand(const QString &command);
    void connectToServer();
    void setRole(const QString& role);

signals:
    void dataReceived(const QString& data);
    void doGameStarted();
    void gameOver(const QString& reason);
    void gameWon();
    void invalidCommandReceived(const QString& rawData);
    void roleAssigned(const QString& role);
    void meteorAmountChanged(int amount);
    void shipSpeedChanged(float current, float max);
    void spawnEnemy();
    void despawnEnemy();

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void processRawData(const QByteArray& data);

private:
    QTcpSocket *socket;
    void processCommand(const QString& command);
    QByteArray buffer;
};

#endif // NETWORK_H
