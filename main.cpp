/**
 * Программа выполняет тестовое чтение данных из модулей
 * крейта LTR-EU-2(2 модуля LTR24).
 *
 * Каждый запрос на чтение инициирует подключение к модулю,
 * его конфигурирование, чтение заданного количества слов,
 * отключение.
 *
 * Программа включает 3 основных класса: CrateEU2, CrateReader
 * и интерфейсный класс IRecord.
 *
 * CrateEU2 - сейчас реализует функцию подключения-чтения крейта. Предполагается,
 * что в дальнейшем в нем должны быть реализованы функции подключения,
 * настройки/запуска синхрометок, чтение через установленное подключение и т.д.
 *
 * IRecord - интерфейс объявляет функцию record. Ей передаются принятые
 * из модуля данные. Здесь может быть их обработка и регистрация
 * В качестве примера сделаны два класса - StdoutRecorder и StdoutChannelRecorder
 * Первый выводит все данные в stdout, второй выводит только данные заданного канала.
 *
 * CrateReader - позволяет производить опрос модулей одного крейта. Рассылает
 * принятые данные обработчикам IRecord. Здесь можно реализовать обработку данных
 * до рассылки, а также сохранение данных, если рассылка не нужна.
 */
#include <iostream>
//#include "ltr/include/ltrapi.h"
#include <ltr/include/ltr24api.h>
#include "CrateEU2.h"
#include "StdoutRecorder.h"
#include "StdoutChannelRecorder.h"
#include "CrateReader.h"
#include "defines.h"
#include "functions.h"
using namespace std;

int main(int argc, char **argv) {
	// Обработка аргументов
	// Формат ввода: <progname> [ip [port]]
	const char *ip = IP_LTRD;
	DWORD port = PORT_LTRD;
	if (!(getargs(argc, argv, ip, port) == 0))
		return -1;

	// создаем объект крейта
	CrateEU2 crate("5T380577", ip, port);

	// создаем приемники-обработчики данных
	StdoutRecorder rec(0); // выводит все в stdout в виде "метка:запись"
	StdoutChannelRecorder rec1(1, 1); 	// как StdoutRecorder, но выводит только заданный канал
	StdoutChannelRecorder rec2(2, 2); 	// второй аргумент - опциональный идентификатор, нужен,
										// чтобы найти вывод этого обработчика. По-умолчанию 0

	// создаем объект, опрашивающий крейт, вызывающий обработчики
	CrateReader creader(crate);

	// регистрируем обработчики
	creader.subscribe(rec);
	creader.subscribe(rec1);
	creader.subscribe(rec2);

//	creader.readTestModule(1, 50); 	// запрашиваем 50 слов из модуля 1,
									// реального обмена не происходит,
									// возвращаются случайные значения

	creader.readModule(1); // запрашиваем 1024 слова из модуля 1
	creader.readModule(2, 20); // запрашиваем 20 слов из модуля 2
	return 0;
}
