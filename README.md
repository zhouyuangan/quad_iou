# quad_iou

> Calculate the IoU between convex quadrilaterals | [÷–ŒƒΩÈ…‹](./README_CN.md)

This code comes from `rotation/rotate_polygon_nms_kernel.cu` in [RRPN_pytorch](https://github.com/mjq11302010044/RRPN_pytorch),
[rotate_polygon_nms_kernel.cu](https://github.com/mjq11302010044/RRPN_pytorch/blob/master/rotation/rotate_polygon_nms_kernel.cu)

## environment

- visual studio 2019
- c++
- python3

## quad_iou.cpp

This file changes the cuda code to cpp code and executes it on the cpu

## quad_iou.py

This file uses clipper's cython version library pyclipper to calculate the quadrilateral's IoU to verify the error range of the cpp version.

