#ifndef __SVG__
#define __SVG__

typedef struct SVG SVG;

struct SVG {
	int x;
	int y;
	char* formula;
	SVG* hi;
	SVG* hd;
};

SVG *CrearSVG(int _x,int _y,char* _formula);
void print_svg(SVG *s);

#endif
