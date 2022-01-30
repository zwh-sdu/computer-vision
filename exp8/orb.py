# -*- coding: utf-8 -*-
"""
Created on Fri Nov 12 11:16:57 2021

@author: 张文浩
"""

import cv2
import time

def orb(filename):
    img = cv2.imread(filename) # 读取文件
    img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY) # 转化为灰度图
    orb = cv2.ORB_create() # 新建orb算子
    keyPoint, descriptor = orb.detectAndCompute(img, None) # 特征提取得到关键点以及对应的描述符（特征向量）
    return img, keyPoint, descriptor



def main():
    start = time.time()
    img, keyPoint, descripor = orb('./pic/huawei.jpg')
    end = time.time()
    print('Running time: %s ms '%((end-start)*1000))
    print('找到 %s 个特征点'%(len(keyPoint)))
    img = cv2.drawKeypoints(img,keyPoint,None)
    cv2.imshow('orb', img)
    cv2.waitKey()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main()