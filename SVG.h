#ifndef __SVG__
#define __SVG__

struct SVGRep {
	int x;
	int y;
	char* formula;
	struct SVG* hi;
	struct SVG* hd;
};

typedef struct SVGRep* SVG;

void print_svg(SVG s);

#endif
