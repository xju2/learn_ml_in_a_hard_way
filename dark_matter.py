#!/usr/bin/env python

import matplotlib.pyplot as plt
plt.rcParams['font.sans-serif'] = ['SimHei']

prop_cycle = plt.rcParams['axes.prop_cycle']
colors = prop_cycle.by_key()['color']

font_name = "STKaiti"
labels = [u'常规物质', u'暗物质', u'暗能量']
sizes = [5, 27, 68]
explode = (0, 0.1, 0)  # only "explode" the 2nd slice (i.e. 'Hogs')

fig1, ax1 = plt.subplots(constrained_layout=False)
fig1.suptitle(u'宇宙的成分', fontsize=20, fontweight='bold')
fig1.subplots_adjust(top=0.8)

rr = ax1.pie(sizes, explode=explode, labels=labels, autopct='%.f%%',
        shadow=False, startangle=90)
_ = ax1.axis('equal')  # Equal aspect ratio ensures that pie is drawn as a circle.

for i,t in enumerate(rr[1]):
    t.set_size(20)
    t.set_color(colors[i])

for p in rr[2]:
    p.set_size(16)
    p.set_fontname('monospace')

plt.savefig("dark_matter.png")
