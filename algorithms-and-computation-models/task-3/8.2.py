# 25592559

for x in xrange(0, 19):
    c = x;
    p = 0;
    been = [False for t in xrange(0, 19)]
    print x,
    for i in xrange(0, 19):
        #print c,
        if been[c]:
            break
        been[c] = True
        c *= x
        c %= 19
        p += 1
    print '!', p
