#include <iostream>
#include <vector>
#include <iomanip>

long double truncated(long double d) {
	const long double powerOfTen = 1000.0;
	return std::trunc(d * powerOfTen) / powerOfTen;
}
//����� ��� ������
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
//����� ��� ����� �����
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
	void setRate(int trend) { //������ ���� �� 5% � ����������� �� ������
		if (trend == 0) this->rate = truncated(this->rate * 1.05);
		else this->rate = truncated(this->rate * 0.95);
	}
};

void showUser(std::vector <long double> &user, std::vector <Currency>& arrCurr) {
	std::cout << "��������� ������ �����:\n";
	for (int i = 0; i < user.size(); i++) {
		std::cout << user[i] << " " << arrCurr[i].getName() << "\n";
	}
	std::cout << "\n";
}

//����� ������ �����
void showExc(std::vector <Currency> & arrCurr, std::vector <Exchange>& arrExc) {
	std::cout << "����� �����\n";
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
	//������������� ������ ������ �����.
	std::cout.setf(std::ios::fixed);
	std::cout.precision(3);
	showUser(user, arrCurr);
	showExc(arrCurr, arrExc);
	std::string stdOut;

	while (true) {

		//����� ������ ������, ������� �� ����� ��������
		std::cout << "�������� ������, ������� ������� ��������, � ������� ��������������� �� �����:\n";
		for (int i = 0; i < arrCurr.size(); i++) {
			std::cout << arrCurr[i].getIndex() << " - " << arrCurr[i].getName() << ";\n";
		}
		std::cout << arrCurr.size() << " - �����\n";
		int indUserCurrency;
		std::cin >> indUserCurrency;
		//��������: ���� �������� - �� int
		if (std::cin.fail() || std::cin.peek() != '\n') {
			std::cout << "������������ ��������. �������� ������ ������.\n\n";
			std::cin.clear();
			std::cin.ignore(32767, '\n');
			continue;
		}
		//��������: ���� �������� - int, �� ���������
		if (indUserCurrency == arrCurr.size()) break;
		if (indUserCurrency > arrCurr.size() || indUserCurrency < 0) {
			std::cout << "������������ ��������. �������� ������ ������.\n";
			continue;
		}
		//���� ������ ����� ��� ������
		std::cout << "\n������� ����� ��� ������ (" << arrCurr[indUserCurrency].getName() << "): ";
		long double userGivenMoney;
		std::cin >> userGivenMoney;
		//��������: ���� �������� - �� long double
		if (std::cin.fail() || std::cin.peek() != '\n') {
			std::cout << "������������ ��������. �������� ������ � ����� ��� ������ ������.\n";
			std::cin.clear();
			std::cin.ignore(32767, '\n');
			continue;
		}
		// ��������: ���� �������� - long double, �� ����� ������, ��� ���� ����� � �����
		if (userGivenMoney > user[indUserCurrency]) {
			std::cout << "�� ����� ����� �� ������� �������. �������� ������ � ����� ��� ������ ������.\n";
			continue;
		}
		// ��������: ���� �������� - long double, �� ����� �������������
		else if (userGivenMoney < 0) {
			std::cout << "������������ ��������. �������� ������ � ����� ��� ������ ������.\n";
			continue;
		}
		//������ �����, ������� ����� ��������
		std::vector <int> arrIndCurr = {}; //������� �����
		std::vector <long double> arrRate = {}; //��������������� �� ����
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
		//����� ������ ������, ������� �� ����� ��������
		int indArrIndCurr;
		while (true) {
			std::cout << "\n�������� ������, ������� ������� ��������, � ������� ��������������� �� �����:\n";
			for (int i = 0; i < arrIndCurr.size(); i++) {
				std::cout << i << " - " << arrCurr[arrIndCurr[i]].getName() << ";\n";
			}
			std::cin >> indArrIndCurr;
			//��������: ���� �������� - �� int
			if (std::cin.fail() || std::cin.peek() != '\n') {
				std::cout << "������������ ��������. �������� ������ ������.\n\n";
				std::cin.clear();
				std::cin.ignore(32767, '\n');
				continue;
			}
			//��������: ���� �������� - int, �� ������������
			else if (indArrIndCurr >= arrIndCurr.size() || indArrIndCurr < 0) {
				std::cout << "������������ ��������. �������� ������ ������.\n\n";
				continue;
			}
			else break;
		}
		//������� ����� ��� ������ � ������, ������� ������ �������� ����
		long double atmGivenMoney = truncated(userGivenMoney * arrRate[indArrIndCurr]);
		//��������: ���� � ��������� ��� ����� �����
		if (atmGivenMoney > atm[arrIndCurr[indArrIndCurr]]) {
			std::cout << "\n����� � ������: " << atmGivenMoney << " " << arrCurr[arrIndCurr[indArrIndCurr]].getName();
			std::cout << "\n�������� �� ����� ������ ������ �����. ��������� ������ ������� ������.\n\n";
			continue;
		}
		//������������� ������ �������� �� �����
		std::cout << "\n����� � ������: " << atmGivenMoney << " " << arrCurr[arrIndCurr[indArrIndCurr]].getName() << ".\n";
		std::cout << "���� ������� ����� - ������� 0.\n";
		std::cout << "���� ������� ������ ����� ������ - ������� 1.\n";
		std::cout << "���� �������� �� ����������� - ������� ����� ������ ��������.\n";;
		std::cin >> stdOut;
		if (stdOut == "0") break;
		if (stdOut == "1") continue;
		//�����
		user[indUserCurrency] -= userGivenMoney;
		user[arrIndCurr[indArrIndCurr]] += atmGivenMoney;
		atm[arrIndCurr[indArrIndCurr]] -= atmGivenMoney;
		atm[indUserCurrency] += userGivenMoney;
		//����� ����� ����� ������
		std::cout << "\n";
		showUser(user, arrCurr);
		//����� ����� ������ ����, ������� ����� ������ �������� �������� �� 5% � ����� �������
		for (int i = 0; i < arrExc.size(); i++) {
			arrExc[i].setRate(rand() & 1);
		}
		//����� ����� ������ �����
		showExc(arrCurr, arrExc);
		//���� ��������, ����� �� �� ��������� ����� ����� ��� ����
		std::cout << "���� ������� ����� - ������� 0.\n";
		std::cout << "���� ������ ��� ��� ��������� ����� - ������� ����� ������ ��������.\n";
		std::cin >> stdOut;
		if (stdOut == "0") break;
		std::cout << "\n";
	}
	return 0;
}