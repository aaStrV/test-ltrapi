/*
 * Crate.cpp
 *
 *  Created on: 14 февр. 2020 г.
 *      Author: sergius
 */

#include "CrateEU2.h"

CrateEU2::CrateEU2(const char *_crate_sn, const char *_ltrd_addr_str,
		WORD _ltrd_port) :
		p_csn(_crate_sn), ltrd_addr_str(_ltrd_addr_str), ltrd_port(_ltrd_port), f_ready(
				0) {

	ltrd_addr_dword = htonl(inet_addr(ltrd_addr_str));

	// Выведем информацию по созданному крейту
	if (p_csn)
		cout << "CrateEU2::CrateEU2(): serial = " << p_csn << endl;
	else
		cout << "CrateEU2::CrateEU2(): serial = NULL" << endl;
	cout << "CrateEU2::CrateEU2(): ltrd_addr_str = " << ltrd_addr_str << endl;
	cout << "CrateEU2::CrateEU2(): ltrd_port = " << ltrd_port << endl;
	cout << "CrateEU2::CrateEU2(): ltrd_addr_dword = " << hex
			<< this->ltrd_addr_dword << dec << endl;
}

int CrateEU2::init() {
	int err;
	int ready_modules = 0;

	cout << "CrateEU2::init(): starting" << endl;

	err = LTR_Init(&tltr);
	if (err) {
		cerr << "CrateEU2::CrateEU2(), LTR_Init: error " << err << endl;
		f_ready = 0;
		return err;
	}

	// открываем соединение с крейтом. Нужно получить оттуда серийник
	// и список модулей
	err = LTR_OpenCrate(&tltr, this->ltrd_addr_dword, this->ltrd_port,
			LTR_CRATE_IFACE_UNKNOWN, this->p_csn);
	if (err < 0) {
		cerr << "CrateEU2::init(), LTR_OpenCrate: error " << err << endl;
		f_ready = 0;
		return err;
	}
	strcpy(csn, tltr.csn);
	p_csn = csn; 	// Теперь мы знаем серийный номер крейта,
					// даже если подключались "вслепую"(csn)

	// Получаем список модулей
	err = LTR_GetCrateModules(&tltr, modules);
	if (err < 0) {
		cerr << "CrateEU2::init(), LTR_GetCrateModules: error " << err << endl;
		f_ready = 0;
		LTR_Close(&tltr);
		return err;
	}
	// Выведем подключенные
	for (int i = 0; i < LTR_MODULES_PER_CRATE_MAX; i++) {
		if (modules[i] != LTR_MID_EMPTY) {
			cout << "CrateEU2::init(): module #" << i << " type: " << modules[i]
					<< endl;
			if ((modules[i] != LTR_MID_IDENTIFYING))
				ready_modules++;
		}
	}

	// Отключаем синхронизацию. Ее нужно включить перед
	// началом измерений
	err = LTR_MakeStartMark(&tltr, LTR_MARK_OFF);
	if (err < 0) {
		cerr << "CrateEU2::init(), LTR_MakeStartMark: error " << err << endl;
		f_ready = 0;
		return err;
	}
	err = LTR_StopSecondMark(&tltr);
	if (err < 0) {
		cerr << "CrateEU2::init(), LTR_StopSecondMark: error " << err << endl;
		f_ready = 0;
		return err;
	}

	// Соединение больше не нужно, обязательно закрыть
	err = LTR_Close(&tltr);
	if (err < 0) {
		cerr << "CrateEU2::init(), LTR_Close: error " << err << endl;
		f_ready = 0;
		return err;
	}

	if (ready_modules) {
		cout << "CrateEU2::init(): found " << ready_modules << " ready modules"
				<< endl;
		f_ready = 1;
	} else
		cout << "CrateEU2::init(): ready modules not fount, try later" << endl;

	cout << "CrateEU2::init(): done" << endl;
	return 0;
}

/**
 * Запуск синхронизации. Режимы описаны в ltrapi
 */
int CrateEU2::startSyncMarks(unsigned int startmark_mode,
		unsigned int secmark_mode) {
	// Если флаг не установлен - инициализация незавершена,
	// соединяться нельзя, выходим
	if (!f_ready)
		return LTR_ERROR_UNKNOWN;

	// Если синхранизация не смогла запуститься, логично
	// считать систему не готовой к измерениям
	f_ready = 0;

	// Создаем описатель соединения на основе крейтового. Нужно
	// заполнить поля saddr, sport, csn, cc
	TLTR tltr_crate;
	int err = 0;
	err = LTR_Init(&tltr_crate);
	if (err < 0) {
		cerr << "CrateEU2::startSyncMarks(), LTR_Init: error " << err << endl;
		return err;
	}

	// Открываем соединение. Оно должно быть обязательно закрыто
	err = LTR_OpenCrate(&tltr_crate, ltrd_addr_dword, ltrd_port,
			LTR_CRATE_IFACE_UNKNOWN, csn);
	if (err < 0) {
		cerr << "CrateEU2::startSyncMarks(), LTR_OpenCrate: error " << err
				<< endl;
		return err;
	}

	// Включаем синхронизацию
	err = LTR_MakeStartMark(&tltr_crate, startmark_mode);
	if (err < 0) {
		cerr << "CrateEU2::startSyncMarks(), LTR_MakeStartMark: error " << err
				<< endl;
		return err;
	}
	err = LTR_StartSecondMark(&tltr_crate, secmark_mode);
	if (err < 0) {
		cerr << "CrateEU2::startSyncMarks(), LTR_StartSecondMark: error " << err
				<< endl;
		f_ready = 0;
		return err;
	}

	// Соединение больше не нужно, обязательно закрыть
	err = LTR_Close(&tltr_crate);
	if (err < 0) {
		cerr << "CrateEU2::startSyncMarks(), LTR_Close: error " << err << endl;
		return err;
	}

	cout << "CrateEU2::startSyncMarks(), finished" << endl;
	f_ready = 1;
	return 0;
}

int CrateEU2::getData(WORD module, DWORD *buf, DWORD *buf_tmark, DWORD len,
		DWORD timeout) {
	int err = 0;
	int receaved = 0;

	// Если флаг не установлен - инициализация незавершена,
	// соединяться нельзя, выходим
	if (!f_ready)
		return LTR_ERROR_UNKNOWN;

	if (module < 0 || module > LTR_MODULES_PER_CRATE_MAX
			|| modules[module] == LTR_MID_IDENTIFYING
			|| modules[module] == LTR_MID_EMPTY)
		return LTR_ERROR_EMPTY_SLOT;

	// Создаем описатель соединения на основе крейтового. Нужно
	// заполнить поля saddr, sport, csn, cc
	TLTR tltr_module;
	err = LTR_Init(&tltr_module);
	if (err < 0) {
		cerr << "CrateEU2::getData(), LTR_Init: error " << err << endl;
		return err;
	}
	tltr_module.saddr = ltrd_addr_dword;
	tltr_module.sport = ltrd_port;
	strcpy(tltr_module.csn, csn);
	tltr_module.cc = module;

	// Открываем соединение. Оно должно быть обязательно закрыто
	err = LTR_Open(&tltr_module);
	if (err < 0) {
		cerr << "CrateEU2::getData(), LTR_Open: error " << err << endl;
		return err;
	}

	// Читаем блок данных длиной len, в течении timeout.
	// В переменной receaved принятое количество записей
	err = receaved = LTR_Recv(&tltr_module, buf, buf_tmark, len, timeout);
	if (err < 0) {
		cerr << "CrateEU2::getData(), LTR_Recv: error " << err << endl;
		LTR_Close(&tltr);
		return err;
	}

	// Закрываем соединение
	err = LTR_Close(&tltr_module);
	if (err < 0) {
		cerr << "CrateEU2::getData(), LTR_Close: error " << err << endl;
		return err;
	}
	return receaved;
}

int CrateEU2::resetModule(WORD module) {
	int err = 0;

	// Если флаг не установлен - инициализация незавершена,
	// соединяться нельзя, выходим
	if (!f_ready)
		return LTR_ERROR_UNKNOWN;

	if (module < 0 || module > LTR_MODULES_PER_CRATE_MAX
			|| modules[module] == LTR_MID_EMPTY)
		return LTR_ERROR_EMPTY_SLOT;

	// Открываем управляющее соединение с ltrd. Обязательно закрыть
	err = LTR_OpenSvcControl(&tltr, ltrd_addr_dword, ltrd_port);
	if (err < 0) {
		cerr << "CrateEU2::resetModule(), LTR_OpenSvcControl: error " << err
				<< endl;
		return err;
	}

	// Сбрасываем модуль module
	err = _resetModule(module);
	if (err < 0) {
		cerr << "CrateEU2::resetModule(), _resetModule: error " << err << endl;
		LTR_Close(&tltr);
		return err;
	}

	// Закрываем управляющее соединение с ltrd
	err = LTR_Close(&tltr);
	if (err < 0) {
		cerr << "CrateEU2::resetModule(), LTR_Close: error " << err << endl;
		return err;
	}
	return 0;
}

int CrateEU2::resetModules() {
	int err = 0;

	// Если флаг не установлен - инициализация незавершена,
	// соединяться нельзя, выходим
	if (!f_ready)
		return LTR_ERROR_UNKNOWN;

	// Открываем управляющее соединение с ltrd. Обязательно закрыть
	err = LTR_OpenSvcControl(&tltr, ltrd_addr_dword, ltrd_port);
	if (err < 0) {
		cerr << "CrateEU2::resetModule(), LTR_OpenSvcControl: error " << err
				<< endl;
		return err;
	}

	// Сбрасываем непустые модули
	for (int i = 0; i < LTR_MODULES_PER_CRATE_MAX; i++) {
		if (modules[i] == LTR_MID_EMPTY)
			continue;
		_resetModule(i);
	}

	// Закрываем управляющее соединение с ltrd
	err = LTR_Close(&tltr);
	if (err < 0) {
		cerr << "CrateEU2::resetModule(), LTR_Close: error " << err << endl;
		return err;
	}

	return 0;
}

/**
 * Сбрасывает модуль. Управляющее соединение должно быть установлено
 */
int CrateEU2::_resetModule(WORD module) {
	int err = 0;

	// Сбрасываем модуль module
	err = LTR_ResetModule(&tltr, LTR_CRATE_IFACE_UNKNOWN, csn, module, 0);
	if (err < 0) {
		cerr << "CrateEU2::_resetModule(), LTR_ResetModule: error " << err
				<< endl;
	}
	return err;
}

int CrateEU2::getTestData(WORD module, DWORD *buf, DWORD *buf_tmark, DWORD len,
		DWORD timeout) {
//	cout << "CrateEU2::getTestData(), started:" << endl;
	buf[0] = 0;
	buf_tmark[0] = 0;
	for (int i = 1; i < len; i++) {
		buf[i] = (DWORD) rand();
		buf_tmark[i] = (DWORD) rand();
	}
	/*cout << "CrateEU2::getTestData(), finished, buf_tmark[0]:buf[0] = " << hex
	 << buf_tmark[0] << ":" << buf[0] << dec << endl;*/
	return len;
}
