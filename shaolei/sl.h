#pragma once

#include <vector>
#include <list>
#include <algorithm>
using std::vector;
using std::list;

struct point
{
	int x;
	int y;
	point(): x(0), y(0) {}
	point(int a, int b): x(a), y(b) {}

	bool operator ==(const point& pt)
	{
		return x == pt.x && y == pt.y;
	}
	bool operator !=(const point& pt)
	{
		return !(*this == pt);
	}
};

class rect
{
	public:
		point top_left;
		point bottom_right;
	public:
		rect(): top_left(), bottom_right() {}
		rect(point tl, point br): top_left(tl), bottom_right(br) {}
	public:
		void set_rect(point tl, point br) {top_left = tl; bottom_right = br;}
		bool point_in_rect(point pt)
		{
			if(pt.x >= top_left.x && pt.y >= top_left.y && pt.x <= bottom_right.x && pt.y <= bottom_right.y)
				return true;
			else
				return false;
		}
};

class block
{
	public:
		enum state {STATE_COVER = 1, STATE_OPEN = 2, STATE_FLAG = 4, STATE_YES = 8, STATE_NO = 16, STATE_NUM = 32, STATE_BOOM = 64};
	private:
		unsigned block_state;
		rect block_location;
		int number;
	public:
		void set_block_location(rect rt) {block_location = rt;}
		rect get_block_location() {return block_location;}
		void set_block_state(unsigned st) {block_state = st;}
		unsigned get_block_state() {return block_state;}
		bool point_in_block(point pt) {return block_location.point_in_rect(pt);}
		bool set_block_flag()
		{
			if((block_state & block::STATE_COVER) != 0)
				if((block_state & block::STATE_FLAG) != 0)
				{
					block_state &= ~block::STATE_FLAG;
					return false;
				}
				else
				{
					block_state |= block::STATE_FLAG;
					return true;
				}
		}
		void set_number(int n)
		{
			if((block_state & block::STATE_NUM) != 0)
				number = n;
		}
		int get_number() {return number;}
	public:
		block(): block_state(0), number(0) {}
};

class sl
{
	public:
		int number_of_mine;
		int rows;
		int columns;
		int block_dimension;
		int state; //1-ready, 2-running, 3-stop
		point top_left_point;
		vector<vector<block> > block_matrix;

	public:
		sl(void);
	public:
		~sl(void);
	public:
		void init_blocks();
		void init_blocks(int r, int c, int n);
		int set_flag(point pt);
		bool open_block(point pt);
		bool auto_open(int i, int j);
		bool get_current_block(point pt, point& ab);
		void freshen(int r, int c, int m);
		void blank_clicked(point ij);
		void cover_open(point ij);
		int check();
		rect get_rect()
		{
			point a(0, 0);
			point b(block_dimension*columns, block_dimension*rows);
			rect rt(a, b);
			return rt;
		}
		void set_state(int st) {state = st;}
		int get_state() {return state;}
		void complete();
		bool right_left_clicked(point pt);

	private:
		bool validate(int a, int b)
		{
			if(a >= 0 && b >=0 && a < rows && b < columns)
				return true;
			else
				return false;
		}
};

