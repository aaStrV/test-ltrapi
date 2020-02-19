#ifndef CRATEEU2_H_
#define CRATEEU2_H_
#include <iostream>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
//#include "ltr/include/ltrapi.h"
#include <ltr/include/ltr24api.h>
using namespace std;

/**
 * Класс описывает крейт типа LTR-EU-2
 *
 * Жизненный цикл:
 *   - создание
 *   - инициализация init().
 *   - [сброс, для закрытия (возможно)существующих соединений]
 *   - [настройка/запуск синхронизации]
 *   - получение блока данных от модуля
 *     по запросу клиента getData().
 *
 * Функция getData() должна быть потокобезопасной,
 * поэтому создает свои подключения и использует
 * буфер клиента(клиент может быть в другом потоке).
 */
class CrateEU2 {
private:
	const char *ltrd_addr_str;
	DWORD ltrd_addr_dword;
	WORD ltrd_port;
	const char *p_csn;
	char csn[16];
	WORD modules[LTR_MODULES_PER_CRATE_MAX];

public:
	/*CrateEU2(const char *crate_sn = NULL, const char *ltrd_addr_str =
	 "127.0.0.1", WORD ltrd_port = LTRD_PORT_DEFAULT);*/
	CrateEU2(const char *crate_sn, const char *ltrd_addr_str, WORD ltrd_port);
	CrateEU2() :
			CrateEU2(NULL, "127.0.0.1", LTRD_PORT_DEFAULT) {
	}
	int getData(WORD module, DWORD *buf, DWORD *buf_tmark, DWORD len,
			DWORD timeout = 100); // таймаут в мс
	int getTestData(WORD module, DWORD *buf, DWORD *buf_tmark, DWORD len,
			DWORD timeout = 100); // таймаут в мс
};

#endif /* CRATEEU2_H_ */
