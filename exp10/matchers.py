import cv2
import numpy as np 

class matchers:
    def __init__(self):
        #surf特征提取
        self.surf = cv2.xfeatures2d.SURF_create()
#        self.surf = cv2.xfeatures2d.SIFT_create()
#        self.surf = cv2.ORB_create()
        FLANN_INDEX_KDTREE = 0
        index_params = dict(algorithm=0, trees=5)
        search_params = dict(checks=50)
        self.flann = cv2.FlannBasedMatcher(index_params, search_params)

    def match(self, i1, i2, direction=None):
        #计算i1，i2图像之间的变换矩阵H，先利用SURF对两张图像进行特征提取
        imageSet1 = self.getSURFFeatures(i1)
        imageSet2 = self.getSURFFeatures(i2)
        print("Direction : ", direction) #向左拼接还是向右拼接
                #利用flann对两张图像计算出来的特征描述符进行匹配
        matches = self.flann.knnMatch(imageSet2['des'],imageSet1['des'],k=2)
        #清除错误匹配，将误差大于一定值的舍去
        good = []
        for i , (m, n) in enumerate(matches):
            if m.distance < 0.7*n.distance: #如果第一个邻近距离比第二个邻近距离的0.7倍小，则保留
                good.append((m.trainIdx, m.queryIdx))
        
        if len(good) > 4:
            pointsCurrent = imageSet2['kp']
            pointsPrevious = imageSet1['kp']
                #提取出匹配的点的坐标
            matchedPointsCurrent = np.float32([pointsCurrent[i].pt for (__, i) in good])
            matchedPointsPrev = np.float32([pointsPrevious[i].pt for (i, __) in good])
                # 使用findHomography函数，计算多个二维点对之间的最优单映射变换矩阵 H（3行x3列） ，使用最小均方误差或者RANSAC方法
            H, s = cv2.findHomography(matchedPointsCurrent, matchedPointsPrev, cv2.RANSAC, 4)
            return H
        return None

    def getSURFFeatures(self, im):
        #获得surf特征检测
        gray = cv2.cvtColor(im, cv2.COLOR_BGR2GRAY)
        #kp是特征检测出来的关键点
        kp, des = self.surf.detectAndCompute(gray, None)
        return {'kp':kp, 'des':des}