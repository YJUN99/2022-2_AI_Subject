import csv
import os
import cv2
import numpy as np

path_dir_20x20 = 'C:/Users/YongJun/AI_TMP/Data_20'    # 20x20이 저장된 경로
path_dir_8x8 = 'C:/Users/YongJun/AI_TMP/Data_8'    # 8x8이 저장된 경로

file_list_20x20 = os.listdir(path_dir_20x20)
file_list_8x8 = os.listdir(path_dir_8x8)

file_list_png_20x20 = [file for file in file_list_20x20 if file.endswith(".png")]
file_list_png_8x8 = [file for file in file_list_8x8 if file.endswith(".png")]

f_20x20 = open('./20x20_info.csv', 'w', encoding='utf-8', newline='')
f_8x8 = open('./8x8_info.csv', 'w', encoding='utf-8', newline='')
wr_20x20 = csv.writer(f_20x20)
wr_8x8 = csv.writer(f_8x8)
attribute = (["pixel" + str(j) for j in range(1,401)])
# attribute.append("format")
attribute.append("isOne")
attribute.append("isTwo")
attribute.append("isThree")
attribute.append("isFour")
attribute.append("isFive")

#print(attribute)

wr_20x20.writerow(attribute)
wr_8x8.writerow(attribute)

instance = list()
for i in range(np.shape(file_list_png_20x20)[0]):
    src = cv2.imread(path_dir_20x20+'/'+file_list_png_20x20[i], cv2.IMREAD_GRAYSCALE)
    for j in range(1,21):
        for k in range(0,20):
            instance.append(src[j,k])        
    #instance.append(src[0,1]) # format
    if src[0,0] == 1: instance.extend([1,0,0,0,0])
    if src[0,0] == 2: instance.extend([0,1,0,0,0])
    if src[0,0] == 3: instance.extend([0,0,1,0,0])
    if src[0,0] == 4: instance.extend([0,0,0,1,0])
    if src[0,0] == 5: instance.extend([0,0,0,0,1])
    
    wr_20x20.writerow(instance) 
    instance.clear()
    
for i in range(np.shape(file_list_png_8x8)[0]):
    src = cv2.imread(path_dir_8x8+'/'+file_list_png_8x8[i], cv2.IMREAD_GRAYSCALE)
    for j in range(1,21):
        for k in range(0,20):
            instance.append(src[j,k]) 
    #instance.append(src[0,1]) # format
    if src[0,0] == 1: instance.extend([1,0,0,0,0])
    if src[0,0] == 2: instance.extend([0,1,0,0,0])
    if src[0,0] == 3: instance.extend([0,0,1,0,0])
    if src[0,0] == 4: instance.extend([0,0,0,1,0])
    if src[0,0] == 5: instance.extend([0,0,0,0,1])
    
    wr_8x8.writerow(instance) 
    instance.clear()    
       