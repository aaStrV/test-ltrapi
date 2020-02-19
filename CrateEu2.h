#ifndef CRATEEU2_H_
#define CRATEEU2_H_

#include <iostream>
#include <string.h>
#include <ltr24api.h>
#include "functions.h"
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

class CrateEu2
{
private:
	const char *ltrd_addr_str;
	DWORD ltrd_addr_dword;
	WORD ltrd_port;
	const char *p_csn;
	char csn[16];
	WORD modules[LTR_MODULES_PER_CRATE_MAX];
public:
	CrateEu2(const char *crate_sn, const char *ltrd_addr_str, WORD ltrd_port);
	CrateEu2(void) : p_csn(NULL), ltrd_addr_str("127.0.0.1"), ltrd_port(LTRD_PORT_DEFAULT) {
	}
	int getData(INT module, DWORD *buf, DWORD *buf_tmark, DWORD len,
			DWORD timeout); // таймаут в мс
	int getTestData(INT module, DWORD *buf, DWORD *buf_tmark, DWORD len,
			DWORD timeout); // таймаут в мс
	~CrateEu2(void);
};

#endif /* CRATEEU2_H_ */
