
#include "lzr_ilda.h"

// the ILDA default color table
const ilda_color ilda_palette[] = {
//    R    G    B
    { 255, 0,   0   }, // Red
    { 255, 16,  0   },
    { 255, 32,  0   },
    { 255, 48,  0   },
    { 255, 64,  0   },
    { 255, 80,  0   },
    { 255, 96,  0   },
    { 255, 112, 0   },
    { 255, 128, 0   },
    { 255, 144, 0   },
    { 255, 160, 0   },
    { 255, 176, 0   },
    { 255, 192, 0   },
    { 255, 208, 0   },
    { 255, 224, 0   },
    { 255, 240, 0   },

    { 255, 255, 0   }, // Yellow
    { 224, 255, 0   },
    { 192, 255, 0   },
    { 160, 255, 0   },
    { 128, 255, 0   },
    { 96,  255, 0   },
    { 64,  255, 0   },
    { 32,  255, 0   },
    { 0,   255, 0   }, // Green
    { 0,   255, 36  },
    { 0,   255, 73  },
    { 0,   255, 109 },
    { 0,   255, 146 },
    { 0,   255, 182 },
    { 0,   255, 219 },
    { 0,   255, 255 }, // Cyan

    { 0,   227, 255 }, 
    { 0,   198, 255 },
    { 0,   170, 255 },
    { 0,   142, 255 },
    { 0,   113, 255 },
    { 0,   85,  255 },
    { 0,   56,  255 },
    { 0,   28,  255 },
    { 0,   0,   255 }, // Blue
    { 32,  0,   255 },
    { 64,  0,   255 },
    { 96,  0,   255 },
    { 128, 0,   255 },
    { 160, 0,   255 },
    { 192, 0,   255 },
    { 224, 0,   255 },

    { 255, 0,   255 }, // Magenta
    { 255, 32,  255 },
    { 255, 64,  255 },
    { 255, 96,  255 },
    { 255, 128, 255 },
    { 255, 160, 255 },
    { 255, 192, 255 },
    { 255, 224, 255 },
    { 255, 255, 255 }, // White
    { 255, 224, 224 },
    { 255, 192, 192 },
    { 255, 160, 160 },
    { 255, 128, 128 },
    { 255, 96,  96  },
    { 255, 64,  64  },
    { 255, 32,  32  }
};

const size_t ilda_color_count = sizeof(ilda_palette) / sizeof(ilda_color);



void current_palette_init(ilda_parser* ilda, size_t n_colors)
{
    //get the data for the current projector we're working with
    ilda_projector* proj = GET_CURRENT_PROJECTOR_DATA(ilda);

    //free any old palette
    free_projector_palette(proj);

    //malloc the new array of colors
    proj->n_colors = n_colors;
    proj->colors = (ilda_color*) calloc(sizeof(ilda_color), n_colors);
}


void free_projector_palette(ilda_projector* p)
{
    if(p->colors != NULL) free(p->colors);
    p->colors = NULL;
    p->n_colors = 0;
}


void current_palette_set(ilda_parser* ilda, size_t i, ilda_color c)
{
    //get the data for the current projector we're working with
    ilda_projector* proj = GET_CURRENT_PROJECTOR_DATA(ilda);

    //check that we're inside the array
    if(i > proj->n_colors)
    {
        perror("Error setting palette color: index out of range");
        return;
    }

    //set the color
    proj->colors[i] = c;
}


ilda_color current_palette_get(ilda_parser* ilda, size_t i)
{
    //get the data for the current projector we're working with
    ilda_projector* proj = GET_CURRENT_PROJECTOR_DATA(ilda);

    if((proj->colors == NULL) || (proj->n_colors == 0))
    {
        //if no palette was defined, lookup in the default
        if(i < ilda_color_count) return ilda_palette[i];
        else                     return ilda_palette[ILDA_WHITE];
    }
    else
    {
        //use the custom palette
        if(i < proj->n_colors) return proj->colors[i];
        else                return proj->colors[ILDA_WHITE];
    }
}
