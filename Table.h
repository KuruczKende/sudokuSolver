#pragma once
#include <iostream>
#include <list>

class cell {
	bool d[9];//possible num
	uint8_t num;//num of poss value
	uint8_t u;//value
public:
	cell(bool b = true) :d{ b, b, b, b, b, b, b, b, b}, num(b?9:0), u(0) {}
	//returns true: the list contains the value
	bool contain(uint8_t v)const { if (v > 9 || v == 0)throw "out of range"; return d[v-1]; }
	//returns true: the list didnt contained this value before
	bool add(uint8_t v) {
		if (v > 9 || v == 0)throw "out of range";
		bool ret = !d[v-1];
		d[v-1] = true;
		if (ret)num++;
		return ret;
	}
	//returns true: the list contained this value
	bool remove(uint8_t v) {
		if (v > 9 || v == 0)throw "out of range";
		bool ret = d[v - 1];
		d[v - 1] = false;
		if (ret)num--;
		return ret;
	}
	void operator=(uint8_t v) {
		for (uint8_t i = 0; i < 9; i++)d[i] = false;
		num = 0;
		u = v;
	}
	uint8_t getNumOfPos()const { return num; }
	uint8_t getOnlyPos()const{
		if (getNumOfPos() != 1)return 0;
		uint8_t i = 1;
		while (!d[i-1]&&i<10)i++;
		return (i)%10;
	}
	operator uint8_t() const {
		return u;
	}
	/*operator uint8_t& () {
		return u;
	}*/
};
struct pos {
	uint8_t r, c;
	pos(uint8_t r, uint8_t c) :r(r), c(c) {}
};
struct posdata : public pos {
	uint8_t v;
	posdata(uint8_t r, uint8_t c, uint8_t v) :pos(r, c), v(v) {}
};
struct posdatadir : public posdata {
	bool h;
	posdatadir(uint8_t r, uint8_t c, uint8_t v, bool h) :posdata(r, c, v), h(h) {}
};



class table_t {
	cell cells[9][9];//posibble values of cells of the table
public:
	std::list <posdata> modifiedCells;
	std::list <posdatadir> modifiedSegmens;
	table_t();
	table_t(std::istream& is);
	table_t(const table_t& t);
	void print(std::ostream& os, uint8_t v = 0) const;

	void place(uint8_t v, uint8_t r, uint8_t c);
	void removePossibleValue(uint8_t v, uint8_t r, uint8_t c);
	void leavePossibleValue(uint8_t v, uint8_t r, uint8_t c);
	void removePossibleValueInSegmens(uint8_t v, uint8_t r, uint8_t c, bool h);
	uint8_t getNumInSegmens(uint8_t v, uint8_t r, uint8_t c, bool h);
	void doModifiedSegFromCell(posdata cell);
	void doModifiedSegFromCells();
	void doModifiedSeg(posdatadir i);
	void doModifiedSegs();
	void doModifieds();
	/*void place(uint8_t v, pos p);
	void removePossibleValue(uint8_t v, pos p);
	void removePossibleValueInSegmens(uint8_t v, pos p, bool h);
	void place(posdata pd);
	void removePossibleValue(posdata pd);
	void removePossibleValueInSegmens(posdata pd, bool h);*/

};/*
std::ostream& operator<<(std::ostream& os, const table_t t) {
	t.print(os);
	return os;
}*/