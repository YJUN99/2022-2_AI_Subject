import numpy as np
import os
import cv2
import re
import csv

path_dir = 'C:/Users/YongJun/To Student 2022/ProjectSource2'      # 저장되어 있는 경로 (모여있는 공간)
path_dir_20x20 = 'C:/Users/YongJun/To Student 2022/Data_20'    # 20x20이 저장될 경로
path_dir_8x8 = 'C:/Users/YongJun/To Student 2022/Data_8'    # 8x8이 저장될 경로
file_list = os.listdir(path_dir)
file_list_png = [file for file in file_list if file.endswith(".png")]

for i in range((np.shape(file_list_png)[0])):
    src = cv2.imread(path_dir+'/'+file_list_png[i],cv2.IMREAD_GRAYSCALE)
    
    scr_resized = cv2.resize(src,dsize = (20,20), interpolation=cv2.INTER_AREA)
    img_split_str = re.split('[_| |.]',file_list_png[i])
    
    imarr = np.array(scr_resized)
    labeling = np.ones((1, 20)) * 255
    
    labeling[0, 0] = img_split_str[0]  # 1,2,3,4,5 중에 하나
    labeling[0, 1] = img_split_str[1]  # 20 혹은 8
    
    final = np.vstack([labeling, imarr])

    if(img_split_str[1] == '20'):
        cv2.imwrite(path_dir_20x20+'/'+ img_split_str[0] + '_' +img_split_str[1] + '_' +str(i%12) +'.png',final)
    elif(img_split_str[1] == '8'):
        cv2.imwrite(path_dir_8x8+'/'+ img_split_str[0] + '_' + img_split_str[1] + '_' +str(i%5) +'.png',final)
        