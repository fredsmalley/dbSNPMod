/*
*
*	Author: Fred A Smalley
*	1-28-2015
*
*	Position source file
*/

#include "Position.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// constructors
//

Position::Position(uint32_t b, uint32_t e, bool inc) {
	begin = b;
	end = e;
	if (inc)
		id = num++;
}

Position::Position(uint32_t i, uint32_t b, uint32_t e) {
	id = i;
	begin = b;
	end = e;
}

Position::Position(const Position& p) {
	begin = p.begin;
	end = p.end;
	id = p.id;
}

Position::Position() {
	begin = 0;
	end = 0;
}

//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// get methods
//

uint32_t Position::getBegin() const	{	return begin;	}
uint32_t Position::getEnd() const	{	return end;	}
uint32_t Position::getId() const		{ return id;	}

//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// operators
//

bool Position::operator < (const Position& pos) const {
	if (begin != pos.begin)
		return (begin < pos.begin);
	else
		return (end < pos.end);
}

bool Position::operator > (const Position& pos) const {
	if (end != pos.end)
		return (end > pos.end);
	else
		return (begin > pos.begin);
}

bool Position::operator == (const Position& pos) const {
	return (begin == pos.begin && end == pos.end);
}

bool Position::operator != (const Position& pos) const {
	return !(*this == pos);
}

bool Position::operator << (const Position& pos) const {
	if (begin == end)	// do not want to count insertions twice
		return false;
	else
		return ((begin >= pos.begin) && (end <= pos.end));
}
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
