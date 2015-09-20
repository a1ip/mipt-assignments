%!

/dir {
	/angle exch def
	angle cos
	angle sin
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

3.5 3.5 translate

newpath
0 dir moveto
72 72 360 {dir lineto} for
closepath
stroke

showpage
