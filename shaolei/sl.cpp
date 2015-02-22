#include "StdAfx.h"
#include "sl.h"
#include <ctime>
#include <set>
#include <utility>
using namespace std;

sl::sl(void)
: number_of_mine(20)
, rows(9)
, columns(9)
, block_dimension(20)
, state(1)
{
}

sl::~sl(void)
{
}

void sl::init_blocks(int r, int c, int n)
{
	rows = r;
	columns = c;
	number_of_mine = n;
	init_blocks();
}

void sl::init_blocks()
{
	for(int i = 0; i < rows; i++)
	{
		vector<block> tmp_row_blocks;
		for(int j = 0; j < columns; j++)
		{
			point a(j * block_dimension + top_left_point.x, i * block_dimension + top_left_point.y);
			point b((j + 1) * block_dimension + top_left_point.x, (i+1) * block_dimension + top_left_point.y);
			rect rt(a, b);
			block tmp_block;
			tmp_block.set_block_location(rt);
			tmp_block.set_block_state(block::STATE_COVER | block::STATE_NO);
			tmp_row_blocks.push_back(tmp_block);
		}
		block_matrix.push_back(tmp_row_blocks);
	}

	set<pair<int, int> > rand_number;
	srand(time(0));
	while(rand_number.size() < number_of_mine)
	{
		int a = rand() % rows;
		int b = rand() % columns;
		rand_number.insert(make_pair(a, b));
	}
	set<pair<int, int> >::iterator is = rand_number.begin();
	for(; is != rand_number.end(); ++is)
	{
		block_matrix[is->first][is->second].set_block_state(block::STATE_COVER | block::STATE_YES);
		TRACE("%d %d\n",is->first,is->second);
	}

	for(int i = 0; i < rows; ++i)
		for(int j = 0; j < columns; ++j)
		{
			int count = 0;
			if(validate(i-1, j-1) && ((block_matrix[i-1][j-1].get_block_state() & block::STATE_YES) != 0))
				++count;
			if(validate(i, j-1) && ((block_matrix[i][j-1].get_block_state() & block::STATE_YES) != 0))
				++count;
			if(validate(i+1, j-1) && ((block_matrix[i+1][j-1].get_block_state() & block::STATE_YES) != 0))
				++count;
			if(validate(i+1, j) && ((block_matrix[i+1][j].get_block_state() & block::STATE_YES) != 0))
				++count;
			if(validate(i+1, j+1) && ((block_matrix[i+1][j+1].get_block_state() & block::STATE_YES) != 0))
				++count;
			if(validate(i, j+1) && ((block_matrix[i][j+1].get_block_state() & block::STATE_YES) != 0))
				++count;
			if(validate(i-1, j+1) && ((block_matrix[i-1][j+1].get_block_state() & block::STATE_YES) != 0))
				++count;
			if(validate(i-1, j) && ((block_matrix[i-1][j].get_block_state() & block::STATE_YES) != 0))
				++count;
			if(count && ((block_matrix[i][j].get_block_state() & block::STATE_YES) == 0))
			{
				block_matrix[i][j].set_block_state(block::STATE_COVER | block::STATE_NUM);
				block_matrix[i][j].set_number(count); 
			}
		}
}

int sl::set_flag(point pt)
{
	if(state == 3)//stop
		return 0;

	point ij;
	if(get_current_block(pt, ij))
	{
		if((block_matrix[ij.x][ij.y].get_block_state() & block::STATE_COVER) ==0)
			return 0;

		if(block_matrix[ij.x][ij.y].set_block_flag())
			return 1;
		else
			return -1;
	}

	return 0;
}

bool sl::open_block(point pt)
{
	point ij;
	if(get_current_block(pt, ij))
	{
		if(state == 1) //stop
			set_state(2);
		else if(state == 3)
			return false;

		unsigned st = block_matrix[ij.x][ij.y].get_block_state();
		if((st & block::STATE_OPEN) != 0 || (st & block::STATE_FLAG) != 0)
			return true;

		if((st & block::STATE_YES) != 0)
		{
			block_matrix[ij.x][ij.y].set_block_state(block::STATE_OPEN | block::STATE_BOOM | block::STATE_YES);
			return true;
		}

		if((st & block::STATE_NUM) != 0)
		{
			block_matrix[ij.x][ij.y].set_block_state(block::STATE_OPEN | block::STATE_NUM);
			return true;
		}

		if((st & block::STATE_NO) != 0)
		{
			blank_clicked(ij);
			return true;
		}
		

		return true;
	}
	return false;
}


void sl::complete()
{
	for(vector<vector<block> >::size_type i = 0; i < block_matrix.size(); i++)
		for(vector<block>::size_type j = 0; j < block_matrix[i].size(); j++)
			cover_open(point(i, j));

}

bool sl::get_current_block(point pt, point& ab)
{
	for(vector<vector<block> >::size_type i = 0; i < block_matrix.size(); i++)
		for(vector<block>::size_type j = 0; j < block_matrix[i].size(); j++)
		{
			if(block_matrix[i][j].point_in_block(pt))
			{
				//cur_block = block_matrix[i][j];
				ab.x = i;
				ab.y = j;
				return true;
			}
		}

	return false;
}

void sl::freshen(int r, int c, int m)
{
	set_state(1);
	block_matrix.clear();
	init_blocks(r, c, m);
}

void sl::blank_clicked(point ij)
{
	list<point> lt;
	lt.push_back(ij);
	list<point>::iterator il = lt.begin();
	while(il != lt.end())
	{
		cover_open(point(il->x, il->y));

		if(validate(il->x-1, il->y-1))
		{
			if(((block_matrix[il->x-1][il->y-1].get_block_state() & block::STATE_NO) != 0) && (find(lt.begin(), lt.end(), point(il->x-1,il->y-1)) == lt.end()))
				lt.push_back(point(il->x-1,il->y-1));
			else if((block_matrix[il->x-1][il->y-1].get_block_state() & block::STATE_NUM) != 0)
				cover_open(point(il->x-1, il->y-1));
		}

		if(validate(il->x, il->y-1))
		{
			if(((block_matrix[il->x][il->y-1].get_block_state() & block::STATE_NO) != 0) && (find(lt.begin(), lt.end(), point(il->x,il->y-1)) == lt.end()))
				lt.push_back(point(il->x,il->y-1));
			else if((block_matrix[il->x][il->y-1].get_block_state() & block::STATE_NUM) != 0)
				cover_open(point(il->x, il->y-1));
		}

		if(validate(il->x+1, il->y-1))
		{
			if(((block_matrix[il->x+1][il->y-1].get_block_state() & block::STATE_NO) != 0) && (find(lt.begin(), lt.end(), point(il->x+1,il->y-1)) == lt.end()))
				lt.push_back(point(il->x+1,il->y-1));
			else if((block_matrix[il->x+1][il->y-1].get_block_state() & block::STATE_NUM) != 0)
				cover_open(point(il->x+1, il->y-1));
		}

		if(validate(il->x+1, il->y))
		{
			if(((block_matrix[il->x+1][il->y].get_block_state() & block::STATE_NO) != 0) && (find(lt.begin(), lt.end(), point(il->x+1,il->y)) == lt.end()))
				lt.push_back(point(il->x+1,il->y));
			else if((block_matrix[il->x+1][il->y].get_block_state() & block::STATE_NUM) != 0)
				cover_open(point(il->x+1, il->y));
		}

		if(validate(il->x+1, il->y+1))
		{
			if(((block_matrix[il->x+1][il->y+1].get_block_state() & block::STATE_NO) != 0) && (find(lt.begin(), lt.end(), point(il->x+1,il->y+1)) == lt.end()))
				lt.push_back(point(il->x+1,il->y+1));
			else if((block_matrix[il->x+1][il->y+1].get_block_state() & block::STATE_NUM) != 0)
				cover_open(point(il->x+1, il->y+1));
		}

		if(validate(il->x, il->y+1))
		{
			if(((block_matrix[il->x][il->y+1].get_block_state() & block::STATE_NO) != 0) && (find(lt.begin(), lt.end(), point(il->x,il->y+1)) == lt.end()))
				lt.push_back(point(il->x,il->y+1));
			else if((block_matrix[il->x][il->y+1].get_block_state() & block::STATE_NUM) != 0)
				cover_open(point(il->x, il->y+1));
		}

		if(validate(il->x-1, il->y+1))
		{
			if(((block_matrix[il->x-1][il->y+1].get_block_state() & block::STATE_NO) != 0) && (find(lt.begin(), lt.end(), point(il->x-1,il->y+1)) == lt.end()))
				lt.push_back(point(il->x-1,il->y+1));
			else if((block_matrix[il->x-1][il->y+1].get_block_state() & block::STATE_NUM) != 0)
				cover_open(point(il->x-1, il->y+1));
		}

		if(validate(il->x-1, il->y))
		{
			if(((block_matrix[il->x-1][il->y].get_block_state() & block::STATE_NO) != 0) && (find(lt.begin(), lt.end(), point(il->x-1,il->y)) == lt.end()))
				lt.push_back(point(il->x-1,il->y));
			else if((block_matrix[il->x-1][il->y].get_block_state() & block::STATE_NUM) != 0)
				cover_open(point(il->x-1, il->y));
		}
			
	

		++il;
	}
	
}



void sl::cover_open(point ij)
{
	unsigned st = block_matrix[ij.x][ij.y].get_block_state();
	st &= ~block::STATE_COVER;
	st |= block::STATE_OPEN;
	block_matrix[ij.x][ij.y].set_block_state(st);
}

int sl::check()
{
	int blank_count = 0;
	int flag_count = 0;
	vector<vector<block> >::iterator iv = block_matrix.begin();
	vector<block>::iterator iiv;
	for(; iv != block_matrix.end(); ++iv)
		for(iiv = iv->begin(); iiv != iv->end(); ++iiv)
		{
			if((iiv->get_block_state() & block::STATE_BOOM) != 0)
			{
				set_state(3);
				complete();
				return -1;
			}
			if(((iiv->get_block_state() & block::STATE_OPEN) != 0) && ((iiv->get_block_state() & block::STATE_YES) ==0))
				++blank_count;
			if(((iiv->get_block_state() & block::STATE_FLAG) != 0) && ((iiv->get_block_state() & block::STATE_YES) != 0))
				++flag_count;
		}

	if(flag_count == number_of_mine || blank_count == rows*columns - number_of_mine)
	{
		set_state(3);
		complete();
		return 1;
	}

	return 0;
}

bool sl::right_left_clicked(point pt)
{

	if(state != 2) // running
		return false;

	int count = 0;
	if (validate(pt.x-1, pt.y-1) && ((block_matrix[pt.x-1][pt.y-1].get_block_state() & block::STATE_FLAG) != 0))	++count;
	if (validate(pt.x, pt.y-1) && ((block_matrix[pt.x][pt.y-1].get_block_state() & block::STATE_FLAG) != 0))	++count;
	if (validate(pt.x+1, pt.y-1) && ((block_matrix[pt.x+1][pt.y-1].get_block_state() & block::STATE_FLAG) != 0))	++count;
	if (validate(pt.x+1, pt.y) && ((block_matrix[pt.x+1][pt.y].get_block_state() & block::STATE_FLAG) != 0))	++count;
	if (validate(pt.x+1, pt.y+1) && ((block_matrix[pt.x+1][pt.y+1].get_block_state() & block::STATE_FLAG) != 0))	++count;
	if (validate(pt.x, pt.y+1) && ((block_matrix[pt.x][pt.y+1].get_block_state() & block::STATE_FLAG) != 0))	++count;
	if (validate(pt.x-1, pt.y+1) && ((block_matrix[pt.x-1][pt.y+1].get_block_state() & block::STATE_FLAG) != 0))	++count;
	if (validate(pt.x-1, pt.y) && ((block_matrix[pt.x-1][pt.y].get_block_state() & block::STATE_FLAG) != 0))	++count;
	if(block_matrix[pt.x][pt.y].get_number() != count)
		return false;

	if((block_matrix[pt.x][pt.y].get_block_state() & block::STATE_NUM) != 0)
	{
		if (validate(pt.x-1, pt.y-1))	auto_open(pt.x-1, pt.y-1);
		if (validate(pt.x, pt.y-1))		auto_open(pt.x, pt.y-1);
		if (validate(pt.x+1, pt.y-1))	auto_open(pt.x+1, pt.y-1);
		if (validate(pt.x+1, pt.y))		auto_open(pt.x+1, pt.y);
		if (validate(pt.x+1, pt.y+1))	auto_open(pt.x+1, pt.y+1);
		if (validate(pt.x, pt.y+1))		auto_open(pt.x, pt.y+1);
		if (validate(pt.x-1, pt.y+1))	auto_open(pt.x-1, pt.y+1);
		if (validate(pt.x-1, pt.y))		auto_open(pt.x-1, pt.y);
		return true;
	}

	return false;
}


bool sl::auto_open(int i, int j)
{

		unsigned st = block_matrix[i][j].get_block_state();
		if((st & block::STATE_OPEN) != 0 || (st & block::STATE_FLAG) != 0)
			return true;

		if((st & block::STATE_YES) != 0)
		{
			block_matrix[i][j].set_block_state(block::STATE_OPEN | block::STATE_BOOM | block::STATE_YES);
			return true;
		}

		if((st & block::STATE_NUM) != 0)
		{
			block_matrix[i][j].set_block_state(block::STATE_OPEN | block::STATE_NUM);
			return true;
		}

		if((st & block::STATE_NO) != 0)
		{
			blank_clicked(point(i, j));
			return true;
		}

	return false;
}