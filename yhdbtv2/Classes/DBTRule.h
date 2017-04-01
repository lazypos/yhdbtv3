#pragma once
#include <array>
#include <random>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

class CDBTRule
{
public:
	typedef enum 
	{
		type_unknow,
		type_singal,
		type_pairs,
		type_three,
		type_atom,
		type_boom,
		type_sister,
		type_threetwo,
		type_plane,

	}cards_type;

	typedef enum
	{
		over_not,
		over_draw,
		over_win,
		over_win_dual,
		over_win_quad,
		over_lose,
		over_lose_dual,
		over_lose_quad,

	}over_blance;

public:
	CDBTRule();
	~CDBTRule();

	static void		shuffleCards(std::array<int,216>& arrCards, int times);

	static	bool	checkCards(std::vector<int>& cards);
	static	int		getValue(int card) { return card / 4; };
	static	int		getColor(int card) { return card % 4; }
	static  bool	isJoker(int card) { return getValue(card) == 13; };
	static	bool	isRedFive(int card) { return (getValue(card) == 4 && getColor(card) == 3); };
	static  int		getWeight(int card);
	static  int		getAtomWeight(int card);
	static	int		getWeightNoRedFive(int card);
	static	bool	isNormal(const cards_type ty);

	static	pair<bool,int>	isSingle(const std::vector<int>& cards);
	static  pair<bool, int>	isPairs(const std::vector<int>& cards);
	static	pair<bool, int>	isthree(const std::vector<int>& cards);
	static	pair<bool, int>	isAtom(const std::vector<int>& cards);
	static	pair<bool, int>	isBoom(const std::vector<int>& cards);
	static	pair<bool, int>	isSister(const std::vector<int>& cards);
	static	pair<bool, int>	isPlane(const std::vector<int>& cards);
	static	pair<bool, int>	isThreetwo(const std::vector<int>& cards);

	static	std::pair<cards_type, int>	getType(const std::vector<int>& cards);
	static	pair<bool, cards_type>	isBigger(std::vector<int>& cards_per, std::vector<int>& cards_now);
	static	int		getScore(const std::vector<int>& cards);
};

