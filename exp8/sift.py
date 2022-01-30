# -*- coding: utf-8 -*-
"""
Created on Fri Nov 12 11:17:18 2021

@author: 张文浩
"""

import cv2
import time

def sift(filename):
    img = cv2.imread(filename) # 读取文件
    img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY) # 转化为灰度图
    sift = cv2.xfeatures2d_SIFT.create()  # 新建一个sift算子
    keyPoint, descriptor = sift.detectAndCompute(img, None) # 特征提取得到关键点以及对应的描述符（特征向量）
    return img, keyPoint, descriptor

def main():
    start = time.time()
    img, keyPoint, descriptor = sift('./pic/huawei.jpg')
    end = time.time()
    print('Running time: %s ms '%((end-start)*1000))
    print('找到 %s 个特征点'%(len(keyPoint)))
    img = cv2.drawKeypoints(img,keyPoint,None)  # 绘制出特征点（关键点）
    cv2.imshow('sift', img)
    cv2.waitKey()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main()