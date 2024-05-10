#include "Table.h"

table_t::table_t() {

}
void table_t::place(uint8_t v, uint8_t r, uint8_t c) {
	if (!cells[r][c].contain(v)) return;
	for (uint8_t i = 0; i < 9; i += 3) {
		if (r / 3 != i / 3)removePossibleValueInSegmens(v, i, c, true);
		if (c / 3 != i / 3)removePossibleValueInSegmens(v, r, i, false);
	}
	for (uint8_t i = 0; i < 3; i++)
		removePossibleValueInSegmens(v, (r / 3) * 3 + i, c, true);
	cells[r][c]=v;
}
void table_t::removePossibleValue(uint8_t v, uint8_t r, uint8_t c) {
	if (!cells[r][c].contain(v)) return;
	cells[r][c].remove(v);
	if (cells[r][c].getNumOfPos() == 1)
		place(cells[r][c].getOnlyPos(), r, c);
	else {
		modifiedCells.push_back(posdata(r, c, v));
		modifiedCells.unique([](posdata a, posdata b)->bool {return a.c == b.c && a.r == b.r && a.v == b.v; });
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
void table_t::doModifiedSegFromCell(std::list<posdata>::iterator& cell) {
		if (getNumInSegmens(cell->v, cell->r, cell->c, true) == 0)
			modifiedSegmens.push_back(posdatadir(cell->r, cell->c, cell->v, true));
		if (getNumInSegmens(cell->v, cell->r, cell->c, false) == 0)
			modifiedSegmens.push_back(posdatadir(cell->r, cell->c, cell->v, false));
}
void table_t::doModifiedSegFromCells() {
	for (std::list<posdata>::iterator i = modifiedCells.begin(); i != modifiedCells.end();i++)
		doModifiedSegFromCell(i);
	modifiedCells.clear();
}
void table_t::doModifiedSeg(std::list<posdatadir>::iterator i) {
	if (i->h) {//horizontal
		uint8_t foundc = 10;
		for (uint8_t coffset = 3; coffset < 9; coffset += 3) {
			if (getNumInSegmens(i->v, i->r, (i->c + coffset) % 9, true) != 0) {
				if (foundc == 10)//first found
					foundc = (i->c + coffset) % 9;
				else
					foundc = 11;
			}
		}
		if (foundc < 10)
			for (uint8_t rindex = 3 * (i->r / 3); rindex < 3 + 3 * (i->r / 3); rindex++)
				if (rindex != i->r)
					removePossibleValueInSegmens(i->v, rindex, foundc, true);
		uint8_t foundr = 10;
		for (uint8_t rindex = 3 * (i->r / 3); rindex < 3 + 3 * (i->r / 3); rindex++) {
			if (getNumInSegmens(i->v, rindex, i->c, true) != 0) {
				if (foundr == 10)//first found
					foundr = rindex;
				else
					foundr = 11;
			}
		}
		if (foundr < 10)
			for (uint8_t coffset = 3; coffset < 9; coffset += 3)
				removePossibleValueInSegmens(i->v, i->r, (i->c + coffset) % 9, true);
	}
	else {//vertical
		uint8_t foundr = 10;
		for (uint8_t roffset = 3; roffset < 9; roffset += 3) {
			if (getNumInSegmens(i->v, (i->r + roffset) % 9, i->c, true) != 0) {
				if (foundr == 10)//first found
					foundr = (i->r + roffset) % 9;
				else
					foundr = 11;
			}
		}
		if (foundr < 10)
			for (uint8_t cindex = 3 * (i->r / 3); cindex < 3 + 3 * (i->r / 3); cindex++)
				if (cindex != i->c)
					removePossibleValueInSegmens(i->v, foundr, cindex, true);
		uint8_t foundc = 10;
		for (uint8_t cindex = 3 * (i->r / 3); cindex < 3 + 3 * (i->r / 3); cindex++) {
			if (getNumInSegmens(i->v, i->r, cindex, true) != 0) {
				if (foundc == 10)//first found
					foundc = cindex;
				else
					foundc = 11;
			}
		}
		if (foundc < 10)
			for (uint8_t roffset = 3; roffset < 9; roffset += 3)
				removePossibleValueInSegmens(i->v, (i->r + roffset) % 9, foundc, true);
	}
}
void table_t::doModifiedSegs() {
	for (std::list<posdatadir>::iterator i = modifiedSegmens.begin(); i != modifiedSegmens.end(); i++)
		doModifiedSeg(i);
	modifiedSegmens.clear();
}
void table_t::doModifieds() {
	while (!modifiedCells.empty()) {
		doModifiedSegFromCells();
		doModifiedSegs();
	}
}
void table_t::print(std::ostream& os) const{
	for (uint8_t i = 0; i < 9; i++) {
		for (uint8_t j = 0; j < 9; j++) {
			os << char(cells[i][j] + 48) << ' ';
		}
		os << "\n";
	}
}