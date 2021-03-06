﻿#include "DBTRule.h"


CDBTRule::CDBTRule()
{
}


CDBTRule::~CDBTRule()
{
}

void CDBTRule::shuffleCards(std::array<int, 216>& arrCards, int times)
{
	for (int i=0; i<216; i++)
		arrCards[i] = i/4;
	
	int temp = 0;
	std::random_device rd;
	for (int i=0; i<times; i++){
		for (size_t j=0; j<arrCards.size(); j++){
			int s = rd() % 216;
			temp = arrCards[j];
			arrCards[j] = arrCards[s];
			arrCards[s] = temp;
		}
	}
}

bool CDBTRule::checkCards(std::vector<int>& cards)
{
	for (size_t i = 0; i < cards.size(); ++i) {
		if (cards[i] < 0 || cards[i] > 53)
			return false;
	}
	return true;
}

int CDBTRule::getWeight(int card)
{
	//与序号相关，A/2/Jocket/5特殊处理
	if (isRedFive(card))
		return 1000;
	return getWeightNoRedFive(card);
}

int CDBTRule::getAtomWeight(int card)
{
	if (isRedFive(card))
		return 1000;
	if (isJoker(card))
		return 500 + card;

	int w = getColor(card) * 52 + card;
	if (getValue(card) == 0 || getValue(card) == 1)
		w += 52;
	return w;
}

int CDBTRule::getWeightNoRedFive(int card)
{
	//与序号相关，A/2/Jocket特殊处理
	if (isJoker(card))
		return card + 100;

	if (getValue(card) == 0 || getValue(card) == 1)
		return card + 54;

	return card;
}

bool CDBTRule::isNormal(const cards_type ty)
{
	return (ty != type_boom && ty != type_atom);
}

pair<bool, int> CDBTRule::isSingle(const std::vector<int>& cards)
{
	if (cards.size() == 1)
		return pair<bool, int>(true, getWeight(cards[0]));
	return pair<bool, int>(false, 0);
}

pair<bool, int> CDBTRule::isPairs(const std::vector<int>& cards)
{
	if (cards.size()!=2)
		return pair<bool, int>(false, 0);
	//点数必须一样
	if (getValue(cards[0]) != getValue(cards[1]))
		return pair<bool, int>(false, 0);
	//jocker颜色一样
	if (isJoker(cards[0])){
		if (cards[0] != cards[1])
			return pair<bool, int>(false, 0);
		return pair<bool, int>(true, getWeight(cards[0]));
	}
	if (isRedFive(cards[0]) && isRedFive(cards[1]))
		return pair<bool, int>(true, getWeight(cards[0]));
	return pair<bool, int>(true, getWeightNoRedFive(cards[1]));
}

pair<bool, int> CDBTRule::isthree(const std::vector<int>& cards)
{
	if (cards.size() != 3)
		return pair<bool, int>(false, 0);
	if (getValue(cards[0]) != getValue(cards[1])|| getValue(cards[0]) != getValue(cards[2]))
		return pair<bool, int>(false, 0);
	//花色必须不一样
	if (getColor(cards[0]) == getColor(cards[1]) && getColor(cards[0]) == getColor(cards[2]))
		return pair<bool, int>(false, 0);
	//不能有Joker
	if (isJoker(cards[0]))
		return pair<bool, int>(false, 0);
	return pair<bool, int>(true, getWeightNoRedFive(cards[2]));
}

pair<bool, int> CDBTRule::isAtom(const std::vector<int>& cards)
{
	//每张牌都一样
	if (cards.size() == 3 && cards[0] == cards[1] && cards[0] == cards[2])
		return pair<bool, int>(true, getAtomWeight(cards[0]));
	if (cards.size() == 4 && cards[0] == cards[1] && cards[0] == cards[2]
		&& cards[0] == cards[3])
		return pair<bool, int>(true, getAtomWeight(cards[0]));
	return pair<bool, int>(false, 0);
}

pair<bool, int> CDBTRule::isBoom(const std::vector<int>& cards)
{
	auto p = isAtom(cards);
	if (p.first == true)
		return pair<bool, int>(false, 0);

	if (cards.size() < 4 || cards.size() > 16 || isJoker(cards[0]))
		return pair<bool, int>(false, 0);
	//点数一样
	int val = getValue(cards[0]);
	for (size_t i = 1; i < cards.size(); i++) {
		if (getValue(cards[i]) != val)
			return pair<bool, int>(false, 0);
	}
	return pair<bool, int>(true, getWeightNoRedFive(cards.back()));
}

pair<bool, int> CDBTRule::isSister(const std::vector<int>& cards)
{
	size_t num = cards.size();
	if (num < 8 || num % 2 != 0 || isJoker(cards.back()))
		return pair<bool, int>(false, 0);
	
	//必须两两相等
	for (size_t i = 0; i < cards.size(); i += 2){
		if (getValue(cards[i]) != getValue(cards[i+1]))
			return pair<bool, int>(false, 0);
	}

	//带A必须带K
	if (getValue(cards[0]) == 0){
		if (getValue(cards.back()) != 12)
			return pair<bool, int>(false, 0);
		//递增
		for (size_t i = 2; i < cards.size()-2; i += 2) {
			if (getValue(cards[i]) + 1 != getValue(cards[i + 2]))
				return pair<bool, int>(false, 0);
		}
		return pair<bool, int>(true, getWeightNoRedFive(cards[1]));
	}

	//不带A
	for (size_t i = 0; i < cards.size() - 2; i += 2) {
		if (getValue(cards[i]) + 1 != getValue(cards[i + 2]))
			return pair<bool, int>(false, 0);
	}
	return pair<bool, int>(true, getWeightNoRedFive(cards.back()));
}

pair<bool, int> CDBTRule::isPlane(const std::vector<int>& cards)
{
	size_t num = cards.size();
	if (num < 9 || num % 3 != 0 || isJoker(cards.back()))
		return pair<bool, int>(false, 0);
	//三三相等
	for (size_t i = 0; i < cards.size(); i += 3) {
		if ((getValue(cards[i]) != getValue(cards[i + 1]))
			|| (getValue(cards[i]) != getValue(cards[i + 2])))
			return pair<bool, int>(false, 0);
	}

	//带A必须带K
	if (getValue(cards[0]) == 0) {
		if (getValue(cards[num - 1]) != 12)
			return pair<bool, int>(false, 0);
		//递增
		for (size_t i = 3; i < cards.size() - 3; i += 3) {
			if (getValue(cards[i]) + 1 != getValue(cards[i + 3]))
				return pair<bool, int>(false, 0);
		}
		return pair<bool, int>(true, getWeightNoRedFive(cards[2]));
	}
	//不带A
	else {
		for (size_t i = 0; i < cards.size() - 3; i += 3) {
			if (getValue(cards[i]) + 1 != getValue(cards[i + 3]))
				return pair<bool, int>(false, 0);
		}
	}
	return pair<bool, int>(true, getWeightNoRedFive(cards.back()));
}

pair<bool, int> CDBTRule::isThreetwo(const std::vector<int>& cards)
{
	auto p = isBoom(cards);
	if (cards.size() != 5 || p.first == true)
		return pair<bool, int>(false, 0);
	//AAABB
	if (getValue(cards[0]) == getValue(cards[1]) && getValue(cards[0]) == getValue(cards[2])){
		//必须一样
		if (getValue(cards[3]) != getValue(cards[4]))
			return pair<bool, int>(false, 0);
		return pair<bool, int>(true, getWeightNoRedFive(cards[2]));
	}
	//BBAAA
	if (getValue(cards[2]) == getValue(cards[3]) && getValue(cards[2]) == getValue(cards[4])){
		if (getValue(cards[0]) != getValue(cards[1]))
			return pair<bool, int>(false, 0);
		return pair<bool, int>(true, getWeightNoRedFive(cards[4]));
	}
	return pair<bool, int>(false, 0);
}

std::pair<CDBTRule::cards_type, int> CDBTRule::getType(const std::vector<int>& cards)
{
	auto t = isSingle(cards);
	if (t.first)
		return make_pair(type_singal, t.second);
	t = isPairs(cards);
	if (t.first)
		return make_pair(type_pairs, t.second);
	t = isthree(cards);
	if (t.first)
		return make_pair(type_three, t.second);
	t = isBoom(cards);
	if (t.first)
		return make_pair(type_boom, t.second);
	t = isAtom(cards);
	if (t.first)
		return make_pair(type_atom, t.second);
	t = isSister(cards);
	if (t.first)
		return make_pair(type_sister, t.second);
	t = isPlane(cards);
	if (t.first)
		return make_pair(type_plane, t.second);
	t = isThreetwo(cards);
	if (t.first)
		return make_pair(type_threetwo, t.second);

	return std::make_pair(type_unknow, 0);
}

pair<bool, CDBTRule::cards_type> CDBTRule::isBigger(std::vector<int>& cards_per, std::vector<int>& cards_now)
{
	sort(cards_now.begin(), cards_now.end());
	sort(cards_per.begin(), cards_per.end());

	if (!checkCards(cards_now) || cards_now.size() == 0)
		return pair<bool, cards_type>(false, type_unknow);
	auto per = getType(cards_per);
	auto now = getType(cards_now);
	//前一次最大或本次类型不对
	if (now.first == type_unknow || per.second == 1000)
		return pair<bool, cards_type>(false, type_unknow);
	if (cards_per.size() == 0)
		return pair<bool, cards_type>(true, now.first);
	if (isNormal(per.first) && isNormal(now.first)){
		//必须类型数量一样
		if (per.first == now.first && cards_per.size() == cards_now.size())
			return pair<bool, cards_type>(now.second > per.second, now.first);
		return  pair<bool, cards_type>(false, type_unknow);
	}
	if (!isNormal(per.first) && !isNormal(now.first)) {
		if (per.first == type_boom && now.first == type_boom){
			//数量多的大
			if (cards_per.size() == cards_now.size())
				return pair<bool, cards_type>(now.second > per.second, now.first);
			return pair<bool, cards_type>(cards_now.size() > cards_per.size(), now.first);
		}
		if (per.first == type_atom && now.first == type_atom) {
			//数量一样
			if (cards_per.size() == cards_now.size()) {
				return pair<bool, cards_type>(now.second > per.second, now.first);
			}
			//数量不一样，必须大于
			return pair<bool, cards_type>(cards_now.size() > cards_per.size(), now.first);
		}
		if (per.first == type_atom && now.first == type_boom)
			return pair<bool, cards_type>(false, type_unknow);
		return pair<bool, cards_type>(true, now.first);
	}
	if (!isNormal(per.first) && isNormal(now.first))
		return pair<bool, cards_type>(false, type_unknow);
	return pair<bool, cards_type>(true, now.first);
}

int CDBTRule::getScore(const std::vector<int>& cards)
{
	int score = 0;
	for_each(cards.begin(), cards.end(), [&score](int i) {
		int val = getValue(i);
		if (val == 4)
			score += 5;
		else if (val == 9 || val == 12)
			score += 10;
	});
	return score;
}

