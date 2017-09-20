#ifndef __SVG__
#define __SVG__

typedef struct SVG_data SVG_data;
typedef struct SVG SVG;


struct SVG {
	SVG_data* data;
	int xmax;
	int ymax;
};

struct SVG_data {
	int x;
	int y;
	char* formula;
	SVG_data* hi;
	SVG_data* hd;
};

void print_svg(SVG *s);

#endif
