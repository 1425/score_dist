#!/usr/bin/env python3

from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np
from glob import glob

def firsts(a): return [x[0] for x in a]
def seconds(a): return [x[1] for x in a]

def make_graph(filename):
	values=[]
	for line in open(filename,'r').read().splitlines():
		values.append(map(int,line.split(',')))

	fig = plt.figure()
	ax = fig.add_subplot(1, 1, 1, projection='3d')
	X = np.arange(0, 1+max(firsts(values)), 1)
	xlen = len(X)
	Y = np.arange(0, 1+max(seconds(values)), 1)
	ylen = len(Y)
	X, Y = np.meshgrid(X, Y)
	R = np.sqrt(X**2 + Y**2)
	Z = np.sin(R)

	z1=[range(len(Y))]*len(X)

	Z=np.ndarray(shape=(len(X),len(Y)), dtype=float, order='F')

	for i in range(len(X)):
		for j in range(len(Y)):
			Z.itemset((i,j),0)
	for x,y,z in values:
		Z.itemset((x,y),z)
	#print Z
	
	surf = ax.plot_surface(X, Y, Z, rstride=1, cstride=1,
	                       linewidth=0, antialiased=False)
	
	#ax.set_zlim3d(-1, 1)
	
	#plt.show()
	plt.savefig(filename+'.png')

if __name__=='__main__':
	#filename='scores_3col_2016.csv'
	map(make_graph,glob('scores_3col*.csv'))
