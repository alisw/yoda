#! /usr/bin/env python

import yoda

s = yoda.Scatter1D("/foo")
s.addPoint(3, 0.1)
s.addPoint(10, (0.1,0.2))
print(s)

# check setting error sources
s.point(0).setXErrs(0.3, "syst1")
s.point(0).setXErrs(0.4, "syst2")
s.point(1).setXErrs(0.5, "syst1")
s.point(1).setXErrs(1.2, "syst2")
print(s)

# check setting total uncertainty from sources
s.updateTotalUncertainty()
assert(abs(s.point(0).xErrs()[0] - 0.5) < 0.0001) # 0.3**2 + 0.4**2 = 0.9**2
assert(abs(s.point(1).xErrs()[1] - 1.3) < 0.0001) # 0.5**2 + 1.2**2 = 1.3**2

# check if clone correctly copies everythign over
s2 = s.clone() 
assert(s.variations() ==  s2.variations()) 
assert(s.point(1).errMap() == s2.point(1).errMap()) 

# check that write out/read in copies the error breakdown info
yoda.write([s], "s1d.yoda")
aos = yoda.read("s1d.yoda")
for _, ao in aos.items():
    print("ao = " + repr(ao))
    print("variations = " + repr(ao.variations()))
    print("hasValidErrorBreakdown = " + repr(ao.hasValidErrorBreakdown()))
    assert(ao.variations() == s.variations()) 
    assert(ao.point(0).errMap() == s.point(0).errMap()) 
