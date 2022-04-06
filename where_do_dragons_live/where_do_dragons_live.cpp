#include <iostream>
#include <string>
#include <cstdlib> // для rand() и srand()
#include <ctime> // для time()
#include <windows.h>
#include "getRandomNumber.h"


class Creature
{
protected:
	std::string m_name;
	char m_symbol;
	int m_healthPoint;
	int m_attackDamege;
	int m_gold;
public:
	Creature(std::string name, char symbol, int healthPoint, int attackDamage, int gold) :
		m_name(name), m_symbol(symbol), m_healthPoint(healthPoint), m_attackDamege(attackDamage), m_gold(gold)
	{

	}

	std::string getName() { return m_name; }
	char getSymbol() { return m_symbol; }
	int getHealthPoint() { return m_healthPoint; }
	int getAttackDamage() { return m_attackDamege; }
	int getGold() { return m_gold; }

	void reduceHealth(int damage)
	{
		m_healthPoint -= damage;
	}

	bool isDead()
	{
		return m_healthPoint <= 0;
	}

	void addGold(int gold)
	{
		m_gold += gold;
	}
};


class Player : public Creature
{
private:
	int m_level = 1;
public:
	Player(std::string name) :
		Creature(name, '@', 10, 1, 0) { };

	int getLevel() { return m_level; }

	void levelUp()
	{
		++m_level;
		++m_attackDamege;
	}

	bool hasWon()
	{
		if (m_level >= 20)
		{
			return true;
		}
		return false;
	}

};

class Monster : public Creature
{
public:
	enum MonsterType
	{
		DRAGON,
		ORC,
		SLIME,
		MAX_TYPES
	};

	struct MonsterData
	{
		const char* name;
		char symbol;
		int health;
		int damage;
		int gold;
	};

	static MonsterData monsterData[MAX_TYPES];

	Monster(MonsterType type) :
		Creature(monsterData[type].name, monsterData[type].symbol, monsterData[type].health, monsterData[type].damage, monsterData[type].gold) {};


	static Monster getRandomMonster(int min = 0, int max = MAX_TYPES - 1)
	{
		int num = static_cast<int>((rand() % (max - min + 1)) + min);
		return Monster(static_cast<MonsterType>(num));
	}

};

Monster::MonsterData Monster::monsterData[Monster::MAX_TYPES] // Массив с характеристаками монстров
{
	{ "dragon", 'D', 20, 4, 100 },
	{ "orc", 'o', 4, 2, 25 },
	{ "slime", 's', 1, 1, 10 }
};

char choise()
{
	char choise;
	do
	{

		std::cin >> choise;
		std::cin.ignore(32767, '\n');

	} while (choise != 'r' && choise != 'f');

	return choise;
}

void attackPlayer(Player& p, Monster& m) // обрабатывает атаку игрока монстром
{
	std::cout << m.getName() << " Your hit the " << m.getAttackDamage() << " damage.\n";
	p.reduceHealth(m.getAttackDamage());
	std::cout << "You have health points left: " << p.getHealthPoint() << '\n';
}

void attackMonster(Player& p, Monster& m) // обрабатывает атаку монстра игроком, включая увеличение уровня игрока
{

	std::cout << "You hit the " << m.getName() << " for " << p.getAttackDamage() << " damage.\n";
	m.reduceHealth(p.getAttackDamage());


	if (m.isDead())
	{
		std::cout << "Your killed the " << m.getName() << ".\n";
		p.levelUp();
		std::cout << "Your a now level " << p.getLevel() << ".\n";
		std::cout << "Your found " << m.getGold() << " gold" << ".\n";
		p.addGold(m.getGold());
		return;
	}
	std::cout << "The monster has health points left: " << m.getHealthPoint() << '\n';
	attackPlayer(p, m);
}


void fightMonster(Player& p, Monster& m) // обрабатывает бой между Игроком и Монстром, и спрашивает у игрока, что он хочет сделать: Run или Fight.
{
	while (!m.isDead() && !p.isDead())
	{
		std::cout << "You have encountered a " << m.getName() << " (" << m.getSymbol() << ")\n";
		std::cout << "Enter r - to run away or f - to fight: ";
		char ch = choise();
		if (ch == 'r')
		{
			if (getRandomNumber(0, 1) == 1)
			{
				std::cout << "You successfully fled.\n";
				return;
			}
			else
			{
				attackPlayer(p, m);
			}
		}
		else
		{
			attackMonster(p, m);
		}
	}

}


int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	srand(static_cast<unsigned int>(time(0))); // устанавливаем значение системных часов в качестве стартового числа
	rand(); // сбрасываем первый результат

	std::string playerName;
	std::cout << "Enter your name: ";
	std::cin >> playerName;

	Player p(playerName);
	std::cout << "Welcome, " << p.getName() << ".\n" << "You have " << p.getHealthPoint() << " health" << " and are carrying " << p.getGold() << " gold\n";
	std::cout << "Цель игры достичь 20 уровня, либо умереть). В игре есть 3 монстра слизь, огр и дракон.\n";
	std::cout << "Колличество здоровья слизи - 1. 4 и 20 соотвественно у огра и дракона.\n";
	std::cout << "Ваш урон равен вашему уровню, стартовый уровень - 1.\n";
	do
	{
		if (p.isDead())
		{
			std::cout << "You died(( at level " << p.getLevel() << " and with " << p.getGold() << " gold." << '\n';
			break;
		}
		if (p.hasWon())
		{
			std::cout << "You Won!!! You managed to get " << p.getGold() << " gold.\n ";
			std::cout << "Bring them to real life.... Buy ;)";
			break;
		}

		Monster m = Monster::getRandomMonster();
		fightMonster(p, m);

	} while (true);


	system("pause");
	return 0;
}