#include "Table.h"

table_t::table_t() {}
table_t::table_t(std::istream& is) {
	for (uint8_t i = 0; i < 9; i++) {
		for (uint8_t j = 0; j < 9; j++) {
			char c;
			is >> c;
			while ((c < '0' || c>'9') && !is.eof())is >> c;
			if (is.eof())throw "not enought number";
			if (c != '0') {
				place(c - '0', i, j);
				doModifieds();
			}
		}
	}
}
table_t::table_t(const table_t& t) {
	
}
void table_t::place(uint8_t v, uint8_t r, uint8_t c) {
	if (!cells[r][c].contain(v)) return;
	cells[r][c]=v;
	print(std::cout);
	std::cout << "1\n";
	print(std::cout, 1);
	std::cout << "2\n";
	print(std::cout, 2);
	std::cout << "3\n";
	print(std::cout, 3);
	std::cout << "4\n";
	print(std::cout, 4);
	std::cout << "5\n";
	print(std::cout, 5);
	std::cout << "6\n";
	print(std::cout, 6);
	std::cout << "7\n";
	print(std::cout, 7);
	std::cout << "8\n";
	print(std::cout, 8);
	std::cout << "9\n";
	print(std::cout, 9);
	std::cout << "\n";
	for (uint8_t i = 0; i < 9; i += 3) {
		if (r / 3 != i / 3)removePossibleValueInSegmens(v, i, c, false);
		if (c / 3 != i / 3)removePossibleValueInSegmens(v, r, i, true);
	}
	for (uint8_t i = 0; i < 3; i++)
		removePossibleValueInSegmens(v, (r / 3) * 3 + i, c, true);
}
void table_t::removePossibleValue(uint8_t v, uint8_t r, uint8_t c) {
	if (!cells[r][c].contain(v)) return;
	cells[r][c].remove(v);
	modifiedCells.push_back(posdata(r, c, v));
}
void table_t::leavePossibleValue(uint8_t v, uint8_t r, uint8_t c) {
	if (!cells[r][c].contain(v)) return;
	for(uint8_t i=1;i<10;i++)
		if (i != v) {
			cells[r][c].remove(i);
			modifiedCells.push_back(posdata(r, c, i));
		}
}
void table_t::removePossibleValueInSegmens(uint8_t v, uint8_t r, uint8_t c, bool h) {
	if (h) {//horisontal
		for (uint8_t ci = 3 * (c / 3); ci < 3 * (c / 3) + 3; ci++) 
			removePossibleValue(v, r, ci);
	}
	else {//vertical
		for (uint8_t ri = 3 * (r / 3); ri < 3 * (r / 3) + 3; ri++)
			removePossibleValue(v, ri, c);
	}
}
uint8_t table_t::getNumInSegmens(uint8_t v, uint8_t r, uint8_t c, bool h) {
	uint8_t db = 0;
	if (h) {//horisontal
		for (uint8_t ci = 3 * (c / 3); ci < 3 * (c / 3) + 3; ci++)
			if (cells[r][ci].contain(v))db++;
	}
	else {//vertical
		for (uint8_t ri = 3 * (r / 3); ri < 3 * (r / 3) + 3; ri++)
			if (cells[ri][c].contain(v))db++;
	}
	return db;
}
void table_t::doModifiedSegFromCell(posdata cell) {
		if (cells[cell.r][cell.c].getNumOfPos() == 1)
			place(cells[cell.r][cell.c].getOnlyPos(), cell.r, cell.c);
		else {
			if (getNumInSegmens(cell.v, cell.r, cell.c, true) == 0)
				modifiedSegmens.push_back(posdatadir(cell.r, cell.c, cell.v, true));
			if (getNumInSegmens(cell.v, cell.r, cell.c, false) == 0)
				modifiedSegmens.push_back(posdatadir(cell.r, cell.c, cell.v, false));
		}
}
void table_t::doModifiedSegFromCells() {
	for (std::list<posdata>::iterator i = modifiedCells.begin(); i != modifiedCells.end();i++)
		doModifiedSegFromCell(*i);
	modifiedCells.clear();
}
void table_t::doModifiedSeg(const posdatadir seg) {//szerintem itt lehet a hiba
	if (seg.h) {//horizontal
		uint8_t foundc = 10;
		for (uint8_t coffset = 3; coffset < 9; coffset += 3) {
			if (getNumInSegmens(seg.v, seg.r, (seg.c + coffset) % 9, true) != 0) {
				if (foundc == 10)//first found
					foundc = (seg.c + coffset) % 9;
				else
					foundc = 11;
			}
		}
		if (foundc < 10) {
			if (getNumInSegmens(seg.v, seg.r, foundc, true) == 1)
				for (uint8_t cindex = 3 * (foundc / 3); cindex < 3 + 3 * (foundc / 3); cindex++) {
					if (cells[seg.r][cindex].contain(seg.v))
						leavePossibleValue(seg.v, seg.r, cindex);
				}
			else
				for (uint8_t rindex = 3 * (seg.r / 3); rindex < 3 + 3 * (seg.r / 3); rindex++)
					if (rindex != seg.r)
						removePossibleValueInSegmens(seg.v, rindex, foundc, true);
		}
		uint8_t foundr = 10;
		for (uint8_t rindex = 3 * (seg.r / 3); rindex < 3 + 3 * (seg.r / 3); rindex++) {
			if (getNumInSegmens(seg.v, rindex, seg.c, true) != 0) {
				if (foundr == 10)//first found
					foundr = rindex;
				else
					foundr = 11;
			}
		}
		if (foundr < 10) {
			if(getNumInSegmens(seg.v, foundr, seg.c, true)==1)
				for (uint8_t cindex = 3 * (seg.c / 3); cindex < 3 + 3 * (seg.c / 3); cindex++) {
					if (cells[foundr][cindex].contain(seg.v))
						leavePossibleValue(seg.v, foundr, cindex);
				}
			else
				for (uint8_t coffset = 3; coffset < 9; coffset += 3)
					removePossibleValueInSegmens(seg.v, foundr, (seg.c + coffset) % 9, true);
		}
	}
	else {//vertical
		uint8_t foundr = 10;
		for (uint8_t roffset = 3; roffset < 9; roffset += 3) {
			if (getNumInSegmens(seg.v, (seg.r + roffset) % 9, seg.c, false) != 0) {
				if (foundr == 10)//first found
					foundr = (seg.r + roffset) % 9;
				else
					foundr = 11;
			}
		}
		if (foundr < 10) {
			if (getNumInSegmens(seg.v, foundr, seg.c, false) == 1)
				for (uint8_t rindex = 3 * (foundr / 3); rindex < 3 + 3 * (foundr / 3); rindex++) {
					if (cells[rindex][seg.c].contain(seg.v))
						leavePossibleValue(seg.v, rindex, seg.c);
				}
			else
				for (uint8_t cindex = 3 * (seg.c / 3); cindex < 3 + 3 * (seg.c / 3); cindex++)
					if (cindex != seg.c)
						removePossibleValueInSegmens(seg.v, foundr, cindex, false);
		}
		uint8_t foundc = 10;
		for (uint8_t cindex = 3 * (seg.c / 3); cindex < 3 + 3 * (seg.c / 3); cindex++) {
			if (getNumInSegmens(seg.v, seg.r, cindex, false) != 0) {
				if (foundc == 10)//first found
					foundc = cindex;
				else
					foundc = 11;
			}
		}
		if (foundc < 10) {
			if (getNumInSegmens(seg.v, seg.r, foundc, false) == 1)
				for (uint8_t rindex = 3 * (seg.r / 3); rindex < 3 + 3 * (seg.r / 3); rindex++) {
					if (cells[rindex][foundc].contain(seg.v))
						leavePossibleValue(seg.v, rindex, foundc);
				}
			else
				for (uint8_t roffset = 3; roffset < 9; roffset += 3)
					removePossibleValueInSegmens(seg.v, (seg.r + roffset) % 9, foundc, false);
		}
	}
}
void table_t::doModifiedSegs() {
	if (modifiedSegmens.empty())return;
	for (std::list<posdatadir>::iterator i = modifiedSegmens.begin(); i != modifiedSegmens.end(); i++)
		doModifiedSeg(*i);
	modifiedSegmens.clear();
}
void table_t::doModifieds() {
	while (!modifiedCells.empty()) {
		for (std::list<posdata>::iterator i = modifiedCells.begin(); i != modifiedCells.end(); i++)
			std::cout << i->v+0 << " " << i->r + 0 << " " << i->c + 0 << "\n";
		doModifiedSegFromCells();
		for (std::list<posdatadir>::iterator i = modifiedSegmens.begin(); i != modifiedSegmens.end(); i++)
			std::cout << i->v + 0 << " " << i->r + 0 << " " << i->c + 0 << " " << i->h << "\n";
		doModifiedSegs();
	}
}
void table_t::print(std::ostream& os, uint8_t v) const{
	if (v == 0) {
		for (uint8_t i = 0; i < 9; i++) {
			for (uint8_t j = 0; j < 9; j++) {
				os << char(cells[i][j] + 48) << ' ';
			}
			os << "\n";
		}
	}
	else {
		for (uint8_t i = 0; i < 9; i++) {
			for (uint8_t j = 0; j < 9; j++) {
				os << char((cells[i][j].contain(v)?v:0) + 48) << ' ';
			}
			os << "\n";
		}
	}
	os << "\n";
}