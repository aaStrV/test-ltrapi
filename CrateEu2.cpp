#include "CrateEu2.h"

CrateEu2::~CrateEu2(void)
{
}

CrateEu2::CrateEu2(const char *_crate_sn, const char *_ltrd_addr_str,
		WORD _ltrd_port) :
		p_csn(_crate_sn), ltrd_addr_str(_ltrd_addr_str), ltrd_port(_ltrd_port) {

	ltrd_addr_dword = getIp(ltrd_addr_str);

	// Выведем информацию по созданному крейту
	if (p_csn)
		cout << "CrateEu2::CrateEu2(): serial = " << p_csn << endl;
	else
		cout << "CrateEu2::CrateEu2(): serial = NULL" << endl;
	cout << "CrateEu2::CrateEu2(): ltrd_addr_str = " << ltrd_addr_str << endl;
	cout << "CrateEu2::CrateEu2(): ltrd_port = " << ltrd_port << endl;
	cout << "CrateEu2::CrateEu2(): ltrd_addr_dword = " << hex
			<< this->ltrd_addr_dword << dec << endl;
}

/**
 * Функция CrateEU2::getData() читает запрошенное количество слов данных из заданного модуля.
 * // TODO: В 24-битном формате функция recv позвращает количество
 * // двойных слов(по 64 бит), в 20-битном одинарных(32 бит).
 * // Это надо отладить отдельно.
 *
 * Однократный цикл чтения происходит в следующей последовательности:
 *   1) Создание/инициализация структуры TLTR24(информация о модуле);
 *   2) открытие соединения;
 *   3) чтение конфигурации. Настройки сохраняются в структуре TLDR;
 *   4) изменение настроек конфигурации АЦП в соответствии с пользовательскими задачами;
 *   5) загрузка конфигурации АЦП в модуль;
 *   6) запуск АЦП;
 *   7) запрос заданного количества слов данных от модуля. Возможно есть смысл
 *     запрашивать количество данных кратное количеству включенных каналов каналов,
 *     для 24-битного формата - еще раз умножать на 2. Иначе при обработке данных
 *     могут быть пропуски(в случае если цикл чтения многократный);
 *   8) [опционально] обработка данных с АЦП функцией ProcessData
 *     для получения значений в вольтах;
 *   9) останов АЦП;
 *  10) закрытие соединения;
 *
 *  7-8 могут повторяться в цикле. Зависит от логики пользователя.
 */
int CrateEu2::getData(INT module, DWORD *buf, DWORD *buf_tmark, DWORD len,
		DWORD timeout) {
	cout << "CrateEU2::getData(): reading module " << module << ", crate "
			<< p_csn << endl;
	TLTR24 tltr_module;
	int err = 0;

	// 1)
	err = LTR24_Init(&tltr_module);
	if (err < 0) {
		cerr << "CrateEU2::getData(), LTR_Init: error " << err << endl;
		return err;
	}

	// 2)
	err = LTR24_Open(&tltr_module, ltrd_addr_dword, ltrd_port, csn, module);
	if (err < 0) {
		cerr << "CrateEU2::getData(), LTR24_Open: error " << err << endl;
		return err;
	}

	// 3)
	err = LTR24_GetConfig(&tltr_module);
	if (err < 0) {
		cerr << "CrateEU2::getData(), LTR24_GetConfig: error " << err << endl;
		LTR24_Close(&tltr_module);
		return err;
	}

	// 4)
	// 20-битный или 24-битный формат записи
	tltr_module.DataFmt = LTR24_FORMAT_20;

	// Устанавливаем частоту с помощью одной из констант
	// (Для 24-битного режима макс. частота только при
	// 2-х каналах, все 4 - только пр 58)
	tltr_module.ADCFreqCode = LTR24_FREQ_610;

	// Вкл./откл. тестовых режимов (измерение нуля/ICP-тест)
	tltr_module.TestMode = FALSE;

	// Настройка режимов каналов
	tltr_module.ChannelMode[0].Enable = TRUE;
	tltr_module.ChannelMode[0].Range = LTR24_RANGE_10;
	tltr_module.ChannelMode[0].AC = FALSE;
	tltr_module.ChannelMode[0].ICPMode = TRUE;

	tltr_module.ChannelMode[1].Enable = TRUE;
	tltr_module.ChannelMode[1].Range = LTR24_RANGE_2;
	tltr_module.ChannelMode[1].AC = TRUE;
	tltr_module.ChannelMode[1].ICPMode = TRUE;

	tltr_module.ChannelMode[2].Enable = FALSE;

	tltr_module.ChannelMode[3].Enable = FALSE;

	// 5)
	err = LTR24_SetADC(&tltr_module);
	if (err < 0) {
		cerr << "CrateEU2::getData(), LTR24_SetADC: error " << err << endl;
		LTR24_Close(&tltr_module);
		return err;
	}

	// 6)
	err = LTR24_Start(&tltr_module);
	if (err < 0) {
		cerr << "CrateEU2::getData(), LTR24_Start: error " << err << endl;
		LTR24_Close(&tltr_module);
		return err;
	}

	// 7)
	// Читаем блок данных длиной len, в течении timeout.
	// В переменной receaved принятое количество записей
	int receaved = 0;
	err = receaved = LTR24_Recv(&tltr_module, buf, buf_tmark, len, timeout);
	if (err < 0) {
		cerr << "CrateEU2::getData(), LTR24_Recv: error " << err << endl;
		LTR24_Close(&tltr_module);
		return err;
	}
	cout << "Receaved " << err << " words, module " << module << ", crate "
			<< p_csn << endl;

//	//test
//	for (int i = 0; i < len; ++i)
//		cout << buf[i] << endl;

	// 9)
	err = LTR24_Stop(&tltr_module);
	if (err < 0) {
		cerr << "CrateEU2::getData(), LTR24_Stop: error " << err << endl;
		LTR24_Close(&tltr_module);
		return err;
	}

	// 10)
	// Закрываем соединение
	err = LTR24_Close(&tltr_module);
	if (err < 0) {
		cerr << "CrateEU2::getData(), LTR_Close: error " << err << endl;
		return err;
	}
	return receaved;
}
int CrateEu2::getTestData(INT module, DWORD *buf, DWORD *buf_tmark, DWORD len,
		DWORD timeout) {
//	cout << "CrateEU2::getTestData(), started:" << endl;
	buf[0] = 0;
	buf_tmark[0] = 0;
	for (DWORD i = 1; i < len; i++) {
		buf[i] = (DWORD) rand();
		buf_tmark[i] = (DWORD) rand();
	}
	/*cout << "CrateEU2::getTestData(), finished, buf_tmark[0]:buf[0] = " << hex
	 << buf_tmark[0] << ":" << buf[0] << dec << endl;*/
	return len;
}
