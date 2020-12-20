# quad_iou 

> 计算凸四边形之间的交并比

该代码来自[RRPN_pytorch](https://github.com/mjq11302010044/RRPN_pytorch)中的`rotation/rotate_polygon_nms_kernel.cu`, 
[rotate_polygon_nms_kernel.cu](https://github.com/mjq11302010044/RRPN_pytorch/blob/master/rotation/rotate_polygon_nms_kernel.cu)

## 环境

- visual studio 2019
- c++
- python3

## quad_iou.cpp

该文件将cuda代码改成cpp代码，在cpu上执行

## quad_iou.py

该文件使用clipper的cython版本库pyclipper计算四边形的交并比，用来验证cpp版本的误差范围情况。