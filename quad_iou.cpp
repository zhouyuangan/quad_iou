// quad_iou.cpp: 定义应用程序的入口点。
//

#include <vector>
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;

// define function
void print_arr(const float*, const int);


inline float trangle_area(float* a, float* b, float* c) {
    return ((a[0] - c[0]) * (b[1] - c[1]) - (a[1] - c[1]) * (b[0] - c[0])) / 2.0;
}

inline float area(float* int_pts, int num_of_inter) {

    float area = 0.0;
    for (int i = 0; i < num_of_inter - 2; i++) {
        area += fabs(trangle_area(int_pts, int_pts + 2 * i + 2, int_pts + 2 * i + 4));
    }
    return area;
}


/// <summary>
/// 对交点集合进行排序
/// </summary>
/// <param name="int_pts"></param>
/// <param name="num_of_inter"></param>
inline void reorder_pts(float* int_pts, int num_of_inter) {
    if (num_of_inter > 0) {
        // 交区域的中心点
        float center[2];

        center[0] = 0.0;
        center[1] = 0.0;

        for (int i = 0; i < num_of_inter; i++) {
            center[0] += int_pts[2 * i];
            center[1] += int_pts[2 * i + 1];
        }
        // 计算这算什么点？中心？重心？
        center[0] /= num_of_inter;
        center[1] /= num_of_inter;

        float vs[16];
        float v[2];
        float d;
        for (int i = 0; i < num_of_inter; i++) {
            v[0] = int_pts[2 * i] - center[0];
            v[1] = int_pts[2 * i + 1] - center[1];
            d = sqrt(v[0] * v[0] + v[1] * v[1]);
            v[0] = v[0] / d;
            v[1] = v[1] / d;
            if (v[1] < 0) {
                v[0] = -2 - v[0];
            }
            vs[i] = v[0];
        }
        printf("before:\n");
        print_arr(int_pts, num_of_inter);
        float temp, tx, ty;
        int j;
        for (int i = 1; i < num_of_inter; ++i) {
            // 前一个点的x值小于当前的点x值
            if (vs[i - 1] > vs[i]) {
                temp = vs[i];
                tx = int_pts[2 * i];
                ty = int_pts[2 * i + 1];
                j = i;
                while (j > 0 && vs[j - 1] > temp) {
                    vs[j] = vs[j - 1];
                    int_pts[j * 2] = int_pts[j * 2 - 2];
                    int_pts[j * 2 + 1] = int_pts[j * 2 - 1];
                    j--;
                }
                vs[j] = temp;
                int_pts[j * 2] = tx;
                int_pts[j * 2 + 1] = ty;
            }
        }
        // print
        printf("after:\n");
        print_arr(int_pts, num_of_inter);
    }

}


void print_arr(const float* arr, const size_t n) {
    printf("[");
    for (size_t i = 0; i < n; i++)
    {
        if (i <= (n-1)) {
            printf("[%f,%f],", arr[2 * i], arr[2 * i + 1]);
        }
        else
        {
            printf("[%f,%f]", arr[2 * i], arr[2 * i + 1]);
        }
    }
    printf("]");
    return;
}

/// <summary>
/// 判断两条线段是否相交，并求出其交点的坐标
/// </summary>
/// <param name="pts1">线段ab</param>
/// <param name="pts2">线段cd</param>
/// <param name="i"></param>
/// <param name="j"></param>
/// <param name="temp_pts"></param>
/// <returns></returns>
inline bool inter2line(float* pts1, float* pts2, int i, int j, float* temp_pts) {

    float a[2];
    float b[2];
    float c[2];
    float d[2];

    float area_abc, area_abd, area_cda, area_cdb;

    // a和b是pts1的同一条线的端点
    a[0] = pts1[2 * i];
    a[1] = pts1[2 * i + 1];

    b[0] = pts1[2 * ((i + 1) % 4)];
    b[1] = pts1[2 * ((i + 1) % 4) + 1];

    // c和d是pts2的同一条线的端点
    c[0] = pts2[2 * j];
    c[1] = pts2[2 * j + 1];

    d[0] = pts2[2 * ((j + 1) % 4)];
    d[1] = pts2[2 * ((j + 1) % 4) + 1];

    area_abc = trangle_area(a, b, c);
    area_abd = trangle_area(a, b, d);
    // 如果ab和cd没有交点，那么面积将是同号，不相交
    if (area_abc * area_abd >= 0) {
        return false;
    }

    area_cda = trangle_area(c, d, a);
    // area_abc和area_abd异号，这里要注意了
    area_cdb = area_cda + area_abc - area_abd;

    if (area_cda * area_cdb >= 0) {
        return false;
    }
    // 利用面积比来确定交点在ab线段上的比例
    float t = area_cda / (area_abd - area_abc);
    // 计算交点相对于a点的x,y方向的偏移量
    float dx = t * (b[0] - a[0]);
    float dy = t * (b[1] - a[1]);
    // 根据a点计算出交点的坐标
    temp_pts[0] = a[0] + dx;
    temp_pts[1] = a[1] + dy;

    return true;
}

inline bool in_rect(float pt_x, float pt_y, float* pts) {

    float ab[2];
    float ad[2];
    float ap[2];

    float abab;
    float abap;
    float adad;
    float adap;

    ab[0] = pts[2] - pts[0];
    ab[1] = pts[3] - pts[1];

    ad[0] = pts[6] - pts[0];
    ad[1] = pts[7] - pts[1];

    ap[0] = pt_x - pts[0];
    ap[1] = pt_y - pts[1];

    abab = ab[0] * ab[0] + ab[1] * ab[1];
    abap = ab[0] * ap[0] + ab[1] * ap[1];
    adad = ad[0] * ad[0] + ad[1] * ad[1];
    adap = ad[0] * ap[0] + ad[1] * ap[1];

    return abab >= abap && abap >= 0 && adad >= adap && adap >= 0;
}

/// <summary>
/// 计算两个四边形的交点及总个数
/// </summary>
/// <param name="pts1">poly1,长度为8</param>
/// <param name="pts2">poly2,长度为8</param>
/// <param name="int_pts">空的数组，长度为16</param>
/// <returns></returns>
inline int inter_pts(float* pts1, float* pts2, float* int_pts) {

    int num_of_inter = 0;

    for (int i = 0; i < 4; i++) {
        //[pts1[2 * i], pts1[2 * i + 1]]是poly1其中一点
        //判断 poly1的角点是否在poly2中
        if (in_rect(pts1[2 * i], pts1[2 * i + 1], pts2)) {
            // 如果poly1的角点在poly2内部，那么将这个点视为交并点，追加到int_pts中
            int_pts[num_of_inter * 2] = pts1[2 * i];
            int_pts[num_of_inter * 2 + 1] = pts1[2 * i + 1];
            num_of_inter++;
        }
        //判断 poly2的角点是否在poly1中
        if (in_rect(pts2[2 * i], pts2[2 * i + 1], pts1)) {
            // 如果poly2的角点在poly1内部，那么将这个点视为交并点，追加到int_pts中
            int_pts[num_of_inter * 2] = pts2[2 * i];
            int_pts[num_of_inter * 2 + 1] = pts2[2 * i + 1];
            num_of_inter++;
        }
    }

    float temp_pts[2];

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            bool has_pts = inter2line(pts1, pts2, i, j, temp_pts);
            //如果计算确定存在交点，那么更新交点集合
            if (has_pts) {
                int_pts[num_of_inter * 2] = temp_pts[0];
                int_pts[num_of_inter * 2 + 1] = temp_pts[1];
                num_of_inter++;
            }
        }
    }


    return num_of_inter;
}

/*
convert rbox into quadrangle
*/
inline void convert_region(float* pts, float const* const region) {
    pts[0] = region[0];
    pts[1] = region[1];
    pts[2] = region[2];
    pts[3] = region[3];
    pts[4] = region[4];
    pts[5] = region[5];
    pts[6] = region[6];
    pts[7] = region[7];

}

inline float inter(float const* const region1, float const* const region2) {
    float pts1[8];
    float pts2[8];
    float int_pts[16];  // 两个四边形相交最多8个相交点
    int num_of_inter;
    
    convert_region(pts1, region1);
    convert_region(pts2, region2);
    // 计算两个四边形的交点及总个数
    num_of_inter = inter_pts(pts1, pts2, int_pts);
    // 对交点执行排序
    reorder_pts(int_pts, num_of_inter);

    return area(int_pts, num_of_inter);


}

/// <summary>
/// 使用鞋带法计算四边形的面积
/// </summary>
/// <param name="region"></param>
/// <returns></returns>
inline float poly_area(float const* const region) {
    float x1 = region[0];
    float y1 = region[1];
    float x2 = region[2];
    float y2 = region[3];
    float x3 = region[4];
    float y3 = region[5];
    float x4 = region[6];
    float y4 = region[7];
    // 鞋带法 Shoelace formula
    // 原理 https://en.wikipedia.org/wiki/Shoelace_formula
    float _area = fabs(x1 * y2 + x2 * y3 + x3 * y4 + x4 * y1 - y1 * x2 - y2 * x3 - y3 * x4 - y4 * x1) / 2;
    return _area;
}

/// <summary>
/// 计算两凸四边形的交并比
/// </summary>
/// <param name="region1"></param>
/// <param name="region2"></param>
/// <returns></returns>
inline float poly_iou(float const* const region1, float const* const region2) {
    float area1 = poly_area(region1);
    float area2 = poly_area(region2);
    float area_inter = inter(region1, region2);

    return area_inter / (area1 + area2 - area_inter);
}

int main()
{
    float quad0[8] = { 94,99,155,292,512,219,463,70 };
    float quad1[8] = { 76,159,53,292,485,340,505,105 };
	cout << "IoU: " << poly_iou(quad0, quad1) << endl;
	return 0;
}
