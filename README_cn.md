# tflite 目标检测推理 C++版本

本应用灵感来源于[example](https://github.com/Qengineering/TensorFlow_Lite_SSD_RPi_64-bits)

## 性能指标

性能指标依赖于模型的复杂程度， 在树莓派4B(1.5Ghz 4GRam)上:

1. SSDMobileNet v1: 22FPS
1. EfficientDet-Lite0: 10FPS
1. EfficientDet-Lite1: 5FPS
1. SSDMobileNet v2: 8FPS

## 特性

1. 支持多模型，用于比较
1. 支持摄像头和视频文件
1. 支持参数定制

## 新增模型

1. 在“model_datas”子目录下新建子目录(如"face_mask_v4")用于保存模型数据
1. 在"face_mask_v4"子目录保存模型文件(如"model.tflite")和标签文件(如"labels.txt")
1. 在"face_mask_v4"子目录新建配置文件(model.cfg)，设置相关参数，包括模型文件、标签文件、摄像头索引或视频文件等。

## model.cfg 实例
application_name = 口罩检测(RPi 4B 1.5GHz 4GRAM: EfficientDet-Lite0)
class_0_color = 0,255,0
class_1_color = 255,0,0
model_file=model.tflite
labels_file=labels.txt
camera_index_or_file=0
class_index_offset=0
confidence_threshold=0.4
title_color = 0,0,255

## 目录及文件

1. .vscode: vscode project directory
1. bin: executable output directory
1. build: cmake build directory
1. src: source file directory
1. CMakeLists.txt: cmake file
1. readme.md: this document

## 依赖环境

1. device: Raspberry 4B(RAM:4G 1.5GHz)
1. os: aarch64 [install](https://qengineering.eu/install-raspberry-64-os.html)
1. opencv: 4.5.4 [install](https://qengineering.eu/install-opencv-4.5-on-raspberry-64-os.html)
1. tflite: 2.5.0 [install](https://qengineering.eu/install-tensorflow-2-lite-on-raspberry-64-os.html)
1. cmake: 3.16.3
1. make: 4.2.1
1. gcc: gcc (Debian 8.3.0-6) 8.3.0

## resources
1. [example](https://github.com/Qengineering/TensorFlow_Lite_SSD_RPi_64-bits)
