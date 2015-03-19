/*
*
*  Author: Fred A Smalley
*  1-28-2015
*
*  Position header file
*/

#ifndef POSITION_h
#define POSITION_h

#include <cstdint>

class Position {
	private:
		static uint32_t	num;

		uint32_t		id;
		uint32_t 	begin;
		uint32_t		end;

	public:
		Position(uint32_t, uint32_t, bool inc=true);
		Position(uint32_t, uint32_t, uint32_t);
		Position(const Position&);
		Position();
		bool operator < (const Position&) const;
		bool operator > (const Position&) const;
		bool operator == (const Position&) const;
		bool operator != (const Position&) const;
		bool operator << (const Position&) const;		// inside span
		uint32_t getId() const;
		uint32_t getBegin() const;
		uint32_t getEnd() const;

	friend class Variation;
};

#endif
