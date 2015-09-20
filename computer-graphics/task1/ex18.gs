%!

/box {
	newpath
	 0.05  0.05 moveto
	-0.05  0.05 lineto
	-0.05 -0.05 lineto
	 0.05 -0.05 lineto
	 0.05  0.05 lineto
	closepath
	gsave
	1.0 1.0 1.0 setrgbcolor
	fill
	grestore
	stroke
} def

72 dup scale
1 72 div setlinewidth

0.5 0.5 translate

/Times-Roman findfont
0.4 scalefont
setfont
%72 450 moveto
0 0 moveto
(Andrey Konovalov, 073) show

2.5 2.5 translate

gsave
1.0 0.0 0.0 setrgbcolor
newpath
0.0 0.0 moveto
0.5 0.5 rlineto
0.5 -0.5 rlineto
0.5 0.0 rlineto
0.5 0.5 rlineto
0.5 0.5 rlineto
stroke
grestore

0.0 0.0 translate
box
0.5 0.5 translate
box
0.5 -0.5 translate
box
0.5 0.0 translate
box
0.5 0.5 translate
box
0.5 0.5 translate
box

showpage
