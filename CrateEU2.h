/*
 * Crate.h
 *
 *  Created on: 14 февр. 2020 г.
 *      Author: sergius
 */

#ifndef CRATEEU2_H_
#define CRATEEU2_H_
#include <iostream>
//#include <stddef.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "ltr/include/ltrapi.h"
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
 *
 * Возможно нужно обработать переполнение буфера сервера.
 * От переполнения буфера модуля должен спасать его
 * сброс перед началом измерений.
 */
class CrateEU2 {
private:
	const char *ltrd_addr_str;
	DWORD ltrd_addr_dword;
	WORD ltrd_port;
	const char *p_csn;
	char csn[16];
	TLTR tltr;
//	TLTR_CRATE_INFO crate_info;
	int f_ready;
	WORD modules[LTR_MODULES_PER_CRATE_MAX];

	int _resetModule(WORD);

public:
	/*CrateEU2(const char *crate_sn = NULL, const char *ltrd_addr_str =
	 "127.0.0.1", WORD ltrd_port = LTRD_PORT_DEFAULT);*/
	CrateEU2(const char *crate_sn, const char *ltrd_addr_str, WORD ltrd_port);
	CrateEU2() :
			CrateEU2(NULL, "127.0.0.1", LTRD_PORT_DEFAULT) {
	}
	int init(void);
	int resetModule(WORD);
	int resetModules(void);
	int getData(WORD module, DWORD *buf, DWORD *buf_tmark, DWORD len,
			DWORD timeout = 100); // таймаут в мс
	int getTestData(WORD module, DWORD *buf, DWORD *buf_tmark, DWORD len,
			DWORD timeout = 100); // таймаут в мс
	int isReady(void) {
		return f_ready;
	}
	int startSyncMarks(unsigned int startmark_mode, unsigned int secmark_mode);
};

#endif /* CRATEEU2_H_ */
