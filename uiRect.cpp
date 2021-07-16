//------------------------------------------------
// uiRect - a rectangle
//------------------------------------------------

#include "uiRect.h"
#include "Grbl_MinUI.h"


void debug_rect(const char *name, const uiRect *rect)
{
	debug_serial("%s(%d,%d,%d,%d)\n",name,rect->x,rect->y,rect->w,rect->h);
}



void uiRect::intersect(const uiRect &rect)
{
	if (rect.isEmpty())
	{
		init();
	}
	else if (!isEmpty())
	{
		if (x + w <= rect.x)
			init();
		else if (y  + h <= rect.y)
			init();
		else if (x >= rect.x + rect.w)
			init();
		else if (y >= rect.y + rect.h)
			init();
		else
		{
			if (rect.x > x)
			{
				w -= x - rect.x;
				x = rect.x;
			}
			if (rect.y > y)
			{
				h -= y - rect.h;
				y = rect.y;
			}
			if (x + w > rect.x + rect.w)
				w -= (x + w) - (rect.x + rect.w);
			if (y + h > rect.y + rect.h)
				h -= (y + h) - (rect.y + rect.h);

		}
	}
}


void uiRect::expand(const uiRect &rect)
{
	if (isEmpty())
	{
		assign(rect);
	}
	else
	{
		if (rect.x < x)
		{
			w += x - rect.x;
			x = rect.x;

		}
		if (rect.y < y)
		{
			h += y - rect.y;
			y = rect.y;
		}

		if (rect.x + rect.w > x + w)
		{
			w += (rect.x + rect.w) - (x + w);
		}
		if (rect.y + rect.h > y + h)
		{
			w += (rect.y + rect.h) - (y + h);
		}
	}
}
