

#include <math.h>
#include <internals.h>

#define PATH_LENGTH(p) abs(p->b - p->a)


//generates a blanking jump between two opt_point_t's
static size_t blank_between(opt_t* opt, lzr_point* points, opt_point_t* a, opt_point_t* b)
{
    size_t i = 0;
    for(; i < opt->anchor_points; i++)
    {
        points[i] = a->base_point;
        LZR_POINT_BLANK(points[i]);
    }

    for(; i < (opt->anchor_points * 2); i++)
    {
        points[i] = b->base_point;
        LZR_POINT_BLANK(points[i]);
    }

    return i;
}


size_t compile_paths(opt_t* opt, lzr_point* points)
{
    //the number of points currently in the output buffer
    //NOTE: must not exceed opt->max_points
    size_t n = 0;

    for(size_t i = 0; i < opt->n_paths; i++)
    {
        opt_path_t* path = (opt->paths + i);
        opt_point_t a = opt->last_known_point;
        opt_point_t b = opt->points[ path->a ]; //first point on the current path

        //if the last_known_point is different than this frames
        //starting point, then an introductory blanking jump
        //is neccessary.

        if( !LZR_POINTS_SAME_POS(a.base_point, b.base_point) )
        {
            //create a blanking jump
            n += blank_between(opt, (points + n), &a, &b);
        }


        //load the drawn points into the output buffer
        size_t l = PATH_LENGTH(path);

        //test if the path is inverted
        if(path->b > path->a)
        {
            //ascending from the path's A point
            for(size_t i = 0; i < l; i++)
                points[n + i] = opt->points[path->a + i].base_point;

            n += l;
        }
        else
        {
            //descending from the path's B point
            for(size_t i = 0; i < l; i++)
                points[n + i] = opt->points[path->a - i].base_point;

            n += l;
        }

        //walk the laser
        opt->last_known_point = opt->points[path->b];
    }

    return n;
}