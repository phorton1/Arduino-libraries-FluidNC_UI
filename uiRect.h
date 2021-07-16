//------------------------------------------------
// uiRect - a rectangle
//------------------------------------------------

#include <cstdint>

typedef int16_t s16;


class uiRect
{
    public:

        // expects normalized rectangles
        // empty is indicated by w=0 and h=0
        // cannonical(0,0,0,0)

        s16 x;
        s16 y;
        s16 w;
        s16 h;

        uiRect()
        {
            x = 0;
            y = 0;
            w = 0;
            h = 0;
        }

        uiRect(const uiRect &rect)
        {
            x = rect.x;
            y = rect.y;
            w = rect.w;
            h = rect.h;
        }

        uiRect(s16 ix, s16 iy, s16 iw, s16 ih)
        {
            x = ix;
            y = iy;
            w = iw;
            h = ih;
        }

        void init()
        {
            x = 0;
            y = 0;
            w = 0;
            h = 0;
        }

        void assign(s16 ix, s16 iy, s16 iw, s16 ih)
        {
            x = ix;
            y = iy;
            w = iw;
            h = ih;
        }

        void assign(const uiRect &rect)
        {
            x = rect.x;
            y = rect.y;
            w = rect.w;
            h = rect.h;
        }


        bool isEmpty() const
        {
            return (w <= 0) ||  (h <= 0);
        }


        void makeRelative(const uiRect &rect)
        {
            x += rect.x;
            y += rect.y;
        }


        void makeAbsolute(const uiRect &rect)
        {
            x -= rect.x;
            y -= rect.y;
        }


        bool intersects(s16 ix, s16 iy) const
        {
            return (ix >= x) && (iy >= y) && (ix < x+w) && (iy < y+h);
        }


        bool intersects(const uiRect &rect) const
        {
            uiRect arect(rect);
            arect.intersect(*this);
            return !arect.isEmpty();
        }

        void expand(const uiRect &rect);
        void intersect(const uiRect &rect);

};


extern void debug_rect(const char *name, const uiRect *rect);
