#include <iostream>
#include <vector>
#include <iomanip>

long double truncated(long double d) {
	const long double powerOfTen = 1000.0;
	return std::trunc(d * powerOfTen) / powerOfTen;
}
//класс для валюты
class Currency {
	int ind = 0;
	std::string name = "";
public:
	Currency(int ind, std::string name) {
		this->ind = ind;
		this->name = name;
	}
	unsigned int getIndex() {
		return this->ind;
	}
	std::string getName() {
		return this->name;
	}
};
//класс для курса валют
class Exchange {
	int indCurr1;
	int indCurr2;
	long double rate;
public:
	Exchange(int indCurr1, int indCurr2, long double rate) {
		this->indCurr1 = indCurr1;
		this->indCurr2 = indCurr2;
		this->rate = rate;
	}
	int getIndCurr1() {
		return this->indCurr1;
	}
	int getIndCurr2() {
		return this->indCurr2;
	}
	long double getRate() {
		return this->rate;
	}
	void setRate(int trend) { //меняет курс на 5% в зависимости от тренда
		if (trend == 0) this->rate = truncated(this->rate * 1.05);
		else this->rate = truncated(this->rate * 0.95);
	}
};

void showUser(std::vector <long double> &user, std::vector <Currency>& arrCurr) {
	std::cout << "Состояние вашего счета:\n";
	for (int i = 0; i < user.size(); i++) {
		std::cout << user[i] << " " << arrCurr[i].getName() << "\n";
	}
	std::cout << "\n";
}

//Показ курсов валют
void showExc(std::vector <Currency> & arrCurr, std::vector <Exchange>& arrExc) {
	std::cout << "Курсы валют\n";
	for (int i = 0; i < arrExc.size(); i++) {
		std::cout << arrCurr[arrExc[i].getIndCurr1()].getName() << " : " <<
			arrCurr[arrExc[i].getIndCurr2()].getName() << " = " << arrExc[i].getRate() << " : 1\n";
	}
	std::cout << "\n";
}

int main() {
	setlocale(LC_ALL, "Russian");
	srand(time(0));

	std::vector <Currency> arrCurr = { Currency(0, "RUB"),
		Currency(1, "USD"),
		Currency(2, "EUR"),
		Currency(3, "USDT"),
		Currency(4, "BTC") };
	std::vector <Exchange> arrExc = { Exchange(0, 1, 90.0),
	Exchange(0, 2, 100.0),
	Exchange(1, 2, 1.11),
	Exchange(1, 3, 1),
	Exchange(1, 4, 55200) };
	std::vector <long double> user = {1000000, 0, 0, 0, 0};
	std::vector <long double> atm = {10000, 1000, 1000, 1000, 1.5};
	//Устанавливаем формат вывода чисел.
	std::cout.setf(std::ios::fixed);
	std::cout.precision(3);
	showUser(user, arrCurr);
	showExc(arrCurr, arrExc);
	std::string stdOut;

	while (true) {

		//Выбор юзером валюты, которую он хочет обменять
		std::cout << "Выберите валюту, которую желаете обменять, и введите соответствующее ей число:\n";
		for (int i = 0; i < arrCurr.size(); i++) {
			std::cout << arrCurr[i].getIndex() << " - " << arrCurr[i].getName() << ";\n";
		}
		std::cout << arrCurr.size() << " - выход\n";
		int indUserCurrency;
		std::cin >> indUserCurrency;
		//Проверка: если значение - не int
		if (std::cin.fail() || std::cin.peek() != '\n') {
			std::cout << "Некорректное значение. Выберите валюту заново.\n\n";
			std::cin.clear();
			std::cin.ignore(32767, '\n');
			continue;
		}
		//Проверка: если значение - int, но ошибочное
		if (indUserCurrency == arrCurr.size()) break;
		if (indUserCurrency > arrCurr.size() || indUserCurrency < 0) {
			std::cout << "Некорректное значение. Выберите валюту заново.\n";
			continue;
		}
		//Ввод юзером суммы для обмена
		std::cout << "\nВведите сумму для обмена (" << arrCurr[indUserCurrency].getName() << "): ";
		long double userGivenMoney;
		std::cin >> userGivenMoney;
		//Проверка: если значение - не long double
		if (std::cin.fail() || std::cin.peek() != '\n') {
			std::cout << "Некорректное значение. Выберите валюту и сумму для обмена заново.\n";
			std::cin.clear();
			std::cin.ignore(32767, '\n');
			continue;
		}
		// Проверка: если значение - long double, но сумма больше, чем есть денег у юзера
		if (userGivenMoney > user[indUserCurrency]) {
			std::cout << "На вашем счету не хватает средств. Выберите валюту и сумму для обмена заново.\n";
			continue;
		}
		// Проверка: если значение - long double, но сумма отрицательная
		else if (userGivenMoney < 0) {
			std::cout << "Некорректное значение. Выберите валюту и сумму для обмена заново.\n";
			continue;
		}
		//Подбор валют, которые можно получить
		std::vector <int> arrIndCurr = {}; //Индексы валют
		std::vector <long double> arrRate = {}; //Соответствующий им курс
		for (int i = 0; i < arrExc.size(); i++) {
			if (arrExc[i].getIndCurr1() == indUserCurrency) {
				arrIndCurr.push_back(arrExc[i].getIndCurr2());
				arrRate.push_back(truncated(1.0 / arrExc[i].getRate()));
			}
			else if (arrExc[i].getIndCurr2() == indUserCurrency) {
				arrIndCurr.push_back(arrExc[i].getIndCurr1());
				arrRate.push_back(arrExc[i].getRate());
			}
		}
		//Выбор юзером валюты, которую он хочет получить
		int indArrIndCurr;
		while (true) {
			std::cout << "\nВыберите валюту, которую желаете получить, и введите соответствующее ей число:\n";
			for (int i = 0; i < arrIndCurr.size(); i++) {
				std::cout << i << " - " << arrCurr[arrIndCurr[i]].getName() << ";\n";
			}
			std::cin >> indArrIndCurr;
			//Проверка: если значение - не int
			if (std::cin.fail() || std::cin.peek() != '\n') {
				std::cout << "Некорректное значение. Выберите валюту заново.\n\n";
				std::cin.clear();
				std::cin.ignore(32767, '\n');
				continue;
			}
			//Проверка: если значение - int, но некорректное
			else if (indArrIndCurr >= arrIndCurr.size() || indArrIndCurr < 0) {
				std::cout << "Некорректное значение. Выберите валюту заново.\n\n";
				continue;
			}
			else break;
		}
		//подсчет суммы для обмена в валюте, которую желает получить юзер
		long double atmGivenMoney = truncated(userGivenMoney * arrRate[indArrIndCurr]);
		//Проверка: если в банкомате нет такой суммы
		if (atmGivenMoney > atm[arrIndCurr[indArrIndCurr]]) {
			std::cout << "\nСумма к выдаче: " << atmGivenMoney << " " << arrCurr[arrIndCurr[indArrIndCurr]].getName();
			std::cout << "\nБанкомат не может выдать данную сумму. Процедура обмена начнётся заново.\n\n";
			continue;
		}
		//Подтверждение юзером согласия на обмен
		std::cout << "\nСумма к выдаче: " << atmGivenMoney << " " << arrCurr[arrIndCurr[indArrIndCurr]].getName() << ".\n";
		std::cout << "Если желаете выйти - введите 0.\n";
		std::cout << "Если желаете начать обмен заново - введите 1.\n";
		std::cout << "Если согласны на конвертацию - введите любое другое значение.\n";;
		std::cin >> stdOut;
		if (stdOut == "0") break;
		if (stdOut == "1") continue;
		//Обмен
		user[indUserCurrency] -= userGivenMoney;
		user[arrIndCurr[indArrIndCurr]] += atmGivenMoney;
		atm[arrIndCurr[indArrIndCurr]] -= atmGivenMoney;
		atm[indUserCurrency] += userGivenMoney;
		//Показ счета юзера обмена
		std::cout << "\n";
		showUser(user, arrCurr);
		//Показ курса каждой пары, который после обмена случайно меняется до 5% в любую сторону
		for (int i = 0; i < arrExc.size(); i++) {
			arrExc[i].setRate(rand() & 1);
		}
		//Показ новых курсов валют
		showExc(arrCurr, arrExc);
		//Юзер выбирает, хочет ли он совершить обмен снова или уйти
		std::cout << "Если желаете выйти - введите 0.\n";
		std::cout << "Если хотите еще раз совершить обмен - введите любое другое значение.\n";
		std::cin >> stdOut;
		if (stdOut == "0") break;
		std::cout << "\n";
	}
	return 0;
}