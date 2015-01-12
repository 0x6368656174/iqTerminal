#ifndef MYCONFIG_H
#define MYCONFIG_H

#include <QObject>
#include <QString>

#include "../MyGlobal.h"

class MyConfig : public QObject
{
   Q_OBJECT

private:
   // статические параметры
   QString m_HomeDir;
   QString m_DownloadDir;
   QString m_ServerName;

   MyIPv6Address m_ServerAddr,
                 m_ProxyAddr,
                 m_StunAddr;

   QByteArray m_serverRSApublickey;
   QByteArray m_clientRSApublickey;
   QByteArray m_clientRSAprivatekey;

   // динамические параметры
   bool m_flagLAN;
   MyIPv6Address m_LanAddr,
                 m_WLanAddr,
                 m_WanAddr;

   // флаги состояние
   bool m_onConnect;
   bool m_onLogon;
   bool m_NATdetect;

   // Переменные сеанса
   QString m_login;
   QString m_password;
   qint32  m_userID;
   qint32  m_sessionID;

   qint32  m_typeNAT;
   qint32  m_typePort;

   MyIPv6Address IPv6fromString(QString str, quint16 defaultport);

   void defaultConfig();
   void parser(QString name, QString value);
   void loadConfig();
   void UpdateDynam();
   QString NatTypeString(qint32 type);

public:
   explicit MyConfig(QObject *parent=0);

   // запуск конфигурирования
   void execute();

   // гетеры параметров
   QString getHomeDir();
   QString getDownloadDir();
   QString getServerName();

   QString getServerAddressAsString();
   QString getSTUNAddressAsString();
   QString getLANAddressAsString();
   QString getWLANAddressAsString();
   QString getWANAddressAsString();

   MyIPv6Address getServerAddress();
   MyIPv6Address getProxyAddress();
   MyIPv6Address getSTUNAddress();
   MyIPv6Address getLANAddress();
   MyIPv6Address getWLANAddress();
   MyIPv6Address getWANAddress();
   bool          getFlagLAN();

   QString getLogin();
   QString getPassword();
   qint32  getUserID();
   qint32  getSessionID();
   bool    getConnectStatus();
   bool    getLogonStatus();

   QByteArray getServerRSA();
   QByteArray getClientRSApublic();
   QByteArray getClientRSAprivate();

   qint32  getNatType();
   QString getNatTypeAsString();
   qint32  getNatPortType();
   QString getNatPortTypeAsString();

signals:
   void message(QString);
   void varChanged(QString);
   void commandExecute(QString, QString);
   void LANdetected();

public slots:
   void setIPv6Address(MyIPv6Address WAN, MyIPv6Address LAN);          // from server
   void setLogin(QString Login, QString Password, qint32 sessionID);   // from dialog
   void setUserID(qint32 userID);                                      // from server
   void Connection(bool status);                                       // from server
   void NATdetected(qint32 typeNAT, qint32 typePort, qint32 exitCode); // from NATdetect
   void setConfigParam(QString param, QVariant value);
};

#endif // MYCONFIG_H
