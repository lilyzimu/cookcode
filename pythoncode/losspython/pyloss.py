import caffe
import numpy as np

class pcalosslayer(caffe.layer):
	"""
	computer loss between ldr and estimated ldr. 
	input: 	bottom[0] estimated CRF index, 
		bottom[1] meanpcavector[pcaRange], 
		bottom[2] eigenvector[pcaCoefsNum][pcaRange],
		bottom[3] curvepcacoefs[curveNum][pcaCoefsNum], 
		bottom[4] raw
		bottom[5] png 
	"""

	def setup(self, bottom, top):
		# check input pair
        	if len(bottom) != 6:
            	raise Exception("Need index, meanpcavector, eigenvector, curvepcacoefs, raw and ldr to compute distance.")

	def reshape(self, bottom, top):
		# loss output is scalar
	        top[0].reshape(1)

	def forward(self, bottom, top):
		basisPCA*curve + mean - png

	def backward(self, top, propagate_down, bottom):
		

