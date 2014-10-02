#ifndef MYGLOBAL_H
#define MYGLOBAL_H
#include <QString>
#include <QHostAddress>

//--------------------------------------------------------------
const char BOM[]={0xef, 0xBB, 0xBF, 0};
// команды

// регистрация
const QString cmdElogon  ="logon";
const QString cmdElogoff ="logof";
const QString cmdEregistr="register";
const QString cmdErestore="restore";
const QString cmdEchange ="change";
const QString cmdEdynam  ="dynam";

//  контакты
const QString cmdEabonbook="abonbook";
const QString cmdEabonlist="abonlist";
const QString cmdEabonwait="abonwait";
const QString cmdEabonadd="abonadd";
const QString cmdEabondel="abondel";

// профили
const QString cmdEsetprofile="setprofile";
const QString cmdEgetprofile="getprofile";
const QString cmdEsettorrent="settorrent";

// коннекты
const QString cmdEsms   ="sms";
const QString cmdEaudio ="audio";
const QString cmdEvideo ="video";

// раздача
const QString cmdEdistrib="distrib";

// торрент
const QString cmdEtorrent="torrent";

// вспомогательные команды
const QString cmdEconnect   ="connect";
const QString cmdEdisconnect="disconnect";
const QString cmdEsend      ="send";
const QString cmdEload      ="load";
const QString cmdEsave      ="save";
const QString cmdEadd       ="add";
const QString cmdEdel       ="del";
const QString cmdEcamera    ="camera";
const QString cmdEColor     ="color";
const QString cmdEQuality   ="quality";
const QString cmdEfps       ="fps";

//--------------------------------------------------------------
// события

// регистрация
#define eventElogon   cmdElogon
#define eventElogoff  cmdElogoff
#define eventEregistr cmdEregistr
#define eventErestore cmdErestore
#define eventEchange  cmdEchange
#define eventEdynam   cmdEdynam
const QString eventEnat="nat";

//  контакты
const QString eventEabonbook="abonbook";
const QString eventEabonlist="abonlist";
const QString eventEabonwait="abonwait";
const QString eventEabonadd ="abonadd";
const QString eventEabondel ="abondel";

// коннекты
#define eventEsms   cmdEsms
#define eventEaudio cmdEaudio
#define eventEvideo cmdEvideo

// торрент и раздача
#define eventEdistrib cmdEdistrib
#define eventEtorrent cmdEtorrent
//--------------------------------------------------------------
// переменные
const QString varEhomedir    ="homedir";
const QString varEdownloaddir="downloaddir";
const QString varEservername ="servername";
const QString varEserverip   ="serverip";
const QString varEstunip     ="stunip";
const QString varEserverkey  ="serverkey";
const QString varEclientkey  ="clientkey";
const QString varElanip      ="lanip";
const QString varEwlanip     ="wlanip";
const QString varEwanip      ="wanip";
#define varEconnect cmdEconnect
#define varElogon   cmdElogon
const QString varElogin      ="login";
const QString varEpassword   ="password";
const QString varEuserid     ="userid";
const QString varEsessionid  ="sessionid";
const QString varEtypenat    ="typenat";
const QString varEtypenatport="typenatport";

const QString varEabonbook="abonbook";
const QString varEabonlist="abonlist";
const QString varEabonwait="abonwait";
const QString varEusername="username";
const QString varEaccept  ="accept";

#define varEsms cmdEsms
const QString varEsmslist  ="smslist";
const QString varEaudiolist="audiolist";
const QString varEvideolist="videolist";

const QString varEditem="ditem";
const QString varElitem="litem";
#define varEfilelist cmdElist

const QString varEfilepath  ="filepath";
const QString varEfilename  ="filename";
const QString varEfileid    ="fileid";
const QString varEfilelength="length";
const QString varEfileext   ="ext";
const QString varEfilenote  ="note";
const QString varEfiletag   ="tag";
const QString varEfilehash  ="hash";
const QString varEfilestatus="status";

const QString varEavailablecameras ="cameras";
#define varEcamera cmdEcamera
#define varEcolor cmdEColor
#define varEQuality cmdEQuality
#define varEfps cmdEfps
const QString varEvideokadr ="videokadr";
//---------------------------------------------------------------------------
// описание структур данных
//---------------------------------------------------------------------------
// IPv4 адрес
typedef struct
{
   quint32 host;
   quint16 port;
} MyIPv4Address_;

//---------------------------------------------------------------------------
// IPv6 адрес
typedef struct
{
   Q_IPV6ADDR host;
   quint16 port;
} MyIPv6Address;

//---------------------------------------------------------------------------
// список команд сервера
#define cmdMappingPort 	0x0000  // мапинг портов
#define cmdRegistr		0x0010	// регистрация нового клиента
#define cmdLogon		0x0011	// регистрация старого клиента
#define cmdChangePswd 	0x0012	// изменение пароля
#define cmdRestorePswd	0x0013	// восстановление пароля
#define cmdPing     	0x0014	// маркер абонента
#define cmdLogoff   	0x0015	// выход клиента
#define cmdUpdateDynam  0x0016	// update динамических параметров
#define cmdGetPrivate	0x0017	// запрос приватного канала
#define cmdConnect		0x0019  // установка прямого соединения
#define cmdConfirm		0x001a  // завершение установки прямого соединения
#define cmdGetProxy     0x0020  // запрос проки-соединения

#define cmdSetRazdacha  0x0100	// передача раздачи на сервер
#define cmdGetFileDesc	0x0101  // запрос описания файла для загрузки
#define cmdGetFileList  0x0102  // запрос списка файлов для загрузки по маске
#define cmdGetAbonBook  0x0103  // запрос абонентской книги
#define cmdGetAbonDesc  0x0104  // запрос описателя клиента
#define cmdGetAbonWait	0x0105  // запрос списка клиентов ждущих акцепт
#define cmdGetAbonList	0x0106  // запрос списка клиентов по маске
#define cmdAddAbonBook  0x0107	// добавление абонента в список контактов
#define cmdDelAbonBook  0x0108	// удаление абонентов из списка контактов
#define cmdSetProfile   0x0109  // запись профиля на сервер
#define cmdGetProfile   0x010a  // чтение профиля с сервера

#define cmdSetFileName	0x0200  // задание файла
#define cmdGetData		0x0201  // запрос данных из файла
#define cmdSetVideoMode	0x0202  // задание передачи видео
#define cmdSetAudioMode	0x0203  // задание передачи видео

#define cmdSendMarker   0x0210  // посылка маркера коннекта
#define cmdSendMessage  0x0211  // посылка сообщения
#define cmdSendAudio	0x0212  // посылка аудиоданных
#define cmdSendVideo	0x0213  // посылка видеоданных
#define cmdResumeVideo	0x8213  // подтверждение доставки видеоданных
#define cmdSendData     0x0214  // посылка данных

#define cmdMailRu		0x8000	// подтверждение доставки пакета
#define cmdAnswer		0x4000	// ответ на отсроченный запрос
//---------------------------------------------------------------------------

#endif // MYGLOBAL_H
