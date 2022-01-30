import numpy as np
import cv2
import sys
from matchers import matchers
import time

class Stitch:
	def __init__(self, args):
		self.path = args
		fp = open(self.path, 'r')
		filenames = [each.rstrip('\r\n') for each in  fp.readlines()]
		print(filenames)
		self.images = [cv2.resize(cv2.imread(each),(440, 330)) for each in filenames]
		self.count = len(self.images)
		self.left_list, self.right_list, self.center_im = [], [],None
		self.matcher_obj = matchers()
		self.prepare_lists()

	def prepare_lists(self):
        #将一系列图像分为左右两部分，左边的部分向左拼接，右边的部分向右拼接
		print("Number of images : %d"%self.count)
		self.centerIdx = self.count/2 
		print("Center index image : %d"%self.centerIdx)
		self.center_im = self.images[int(self.centerIdx)]
		for i in range(self.count):
			if(i<=self.centerIdx):
				self.left_list.append(self.images[i])
			else:
				self.right_list.append(self.images[i])
		print("Image lists prepared")

	def leftshift(self):
        #左边的部分向左拼接（通过变换左边的矩阵进行拼接，所以要取变换矩阵H的逆）
		a = self.left_list[0]
		for b in self.left_list[1:]:
            #H就是得到的从a到b的变化矩阵
			H = self.matcher_obj.match(a, b, 'left')
			print("Homography is : ", H)
			xh = np.linalg.inv(H)
			print("Inverse Homography :", xh)
			ds = np.dot(xh, np.array([a.shape[1], a.shape[0], 1]));
			ds = ds/ds[-1]
			print("final ds=>", ds)
            #f1表示图像a的start_p，如果开始点为负数，就对它进行平移
			f1 = np.dot(xh, np.array([0,0,1]))
			f1 = f1/f1[-1]
			xh[0][-1] += abs(f1[0])
			xh[1][-1] += abs(f1[1])
			ds = np.dot(xh, np.array([a.shape[1], a.shape[0], 1]))
			offsety = abs(int(f1[1]))
			offsetx = abs(int(f1[0]))
			dsize = (int(ds[0])+offsetx, int(ds[1]) + offsety)
			print("image dsize =>", dsize)
            #透视变换，然后拼接
			tmp = cv2.warpPerspective(a, xh, dsize)
			tmp[offsety:b.shape[0]+offsety, offsetx:b.shape[1]+offsetx] = b
			a = tmp

		self.leftImage = tmp

		
	def rightshift(self):
        #右边的部分向右拼接
		for each in self.right_list:
			H = self.matcher_obj.match(self.leftImage, each, 'right')
			print("Homography :", H)
			txyz = np.dot(H, np.array([each.shape[1], each.shape[0], 1]))
			txyz = txyz/txyz[-1]
			dsize = (int(txyz[0])+self.leftImage.shape[1], int(txyz[1])+self.leftImage.shape[0])
			tmp = cv2.warpPerspective(each, H, dsize)
			cv2.imshow("tp", tmp)
#			cv2.waitKey()
            #这里需要将左边已经拼接好的图像与有点刚进行透视变换的图像进行拼接
			tmp = self.mix_and_match(self.leftImage, tmp)
			print("tmp shape",tmp.shape)
			print("self.leftimage shape=", self.leftImage.shape)
			self.leftImage = tmp



	def mix_and_match(self, leftImage, warpedImage):
		i1y, i1x = leftImage.shape[:2]
		i2y, i2x = warpedImage.shape[:2]
		print(leftImage[-1,-1])

		t = time.time()
		black_l = np.where(leftImage == np.array([0,0,0]))
#		black_wi = np.where(warpedImage == np.array([0,0,0]))
		print(time.time() - t)
		print(black_l[-1])

		for i in range(0, i1x):
			for j in range(0, i1y):
				try:
					if(np.array_equal(leftImage[j,i],np.array([0,0,0])) and  np.array_equal(warpedImage[j,i],np.array([0,0,0]))):
                        #如果像素值相等，就将像素作为该值
						warpedImage[j,i] = [0, 0, 0]
					else:#否则优先考虑黑色像素
						if(np.array_equal(warpedImage[j,i],[0,0,0])):
							warpedImage[j,i] = leftImage[j,i]
						else:
							if not np.array_equal(leftImage[j,i], [0,0,0]):
								bw, gw, rw = warpedImage[j,i]
								bl,gl,rl = leftImage[j,i]
								# b = (bl+bw)/2
								# g = (gl+gw)/2
								# r = (rl+rw)/2
								warpedImage[j, i] = [bl,gl,rl]
				except:
					pass
		return warpedImage #返回拼接好的图像


	def trim_left(self):
		pass

	def showImage(self, string=None):
		if string == 'left':
			cv2.imshow("left image", self.leftImage)
			# cv2.imshow("left image", cv2.resize(self.leftImage, (400,400)))
		elif string == "right":
			cv2.imshow("right Image", self.rightImage)
		cv2.waitKey()


if __name__ == '__main__':
	try:
		args = sys.argv[1]
	except:
		args = "txtlists/files_building.txt"
	finally:
		print("Parameters : ", args)
	s = Stitch(args)
	s.leftshift()
#	s.showImage('left')
	s.rightshift()
	print("done")
	cv2.imwrite("test_building_surf.jpg", s.leftImage)
	print("image written")
	cv2.destroyAllWindows()
	
