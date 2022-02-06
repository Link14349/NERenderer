#define M_PI        3.14159265358979323846264338327950288   /* pi             */
#define M_PI_2      1.57079632679489661923132169163975144   /* pi/2           */
#define M_PI_4      0.785398163397448309615660845819875721  /* pi/4           */
#define M_1_PI      0.318309886183790671537767526745028724  /* 1/pi           */
#define M_2_PI      0.636619772367581343075535053490057448  /* 2/pi           */

float square(float x) { return x * x; }

float Fx(float x, float y, float z, float Tx, float Ty, float Tz) {
    return -Ty * Ty * sin(x) * cos(x) - Tz * Tz * sin(x) * cos(x) * square(cos(y));
}
float Fy(float x, float y, float z, float Tx, float Ty, float Tz) {
    return 2 * Tx * Ty * tan(x) - Tz * Tz * sin(y) * cos(y);
}
float Fz(float x, float y, float z, float Tx, float Ty, float Tz) {
    return 2 * Tx * Tz * tan(x) + 2 * Ty * Tz * tan(y);
}
void range(float* x, float* y, float* z) {
    *x -= (int)(*x * 0.5f * M_1_PI) * 2 * M_PI;
    *y -= (int)(*y * 0.5f * M_1_PI) * 2 * M_PI;
    while (*x > M_PI_2 || *x < -M_PI_2) {
        if (*x > M_PI_2) {
            *x = M_PI - *x;
            if (*y > M_PI_2) *y -= M_PI;
            else *y += M_PI;
        } else if (*x < -M_PI_2) {
            *x = -M_PI - *x;
            if (*y > M_PI_2) *y -= M_PI;
            else *y += M_PI;
        }
    }
    while (*y > M_PI_2 || *y < -M_PI_2) {
        if (*y > M_PI_2) {
            *y = M_PI - *y;
            if (*z > M_PI) *z -= M_PI;
            else *z += M_PI;
        }
        if (*y < -M_PI_2) {
            *y = -M_PI - *y;
            if (*z > M_PI) *z -= M_PI;
            else *z += M_PI;
        }
    }
    *z -= (int)(*z * 0.5f * M_1_PI) * 2 * M_PI;
    if (*z < -M_PI) *z += 2 * M_PI;
    if (*z > M_PI) *z -= 2 * M_PI;
}

float calculateGeo(
        float vx,
        float vy,
        float vz,
        float Tx,
        float Ty,
        float Tz,
        float* distance,// 误差距离
        float x,
        float y,
        float z,
        float h) {
    // 归一化
    float Tlen = 1.0f / sqrt(Tx * Tx + square(cos(x)) * (Ty * Ty + square(cos(y)) * Tz * Tz));
    Tx *= Tlen;
    Ty *= Tlen;
    Tz *= Tlen;
    float dis2 = 1e10;
    float minDistance_distance;// 与顶点最近点与出发点的距离
//    float x = x_, y = y_, z = z_;
//    float Tx = Tx_[i], Ty = Ty_[i], Tz = Tz_[i];
    for (float s = 0; s <= 6.283185307179586f; s += h) {
        float rangedX = x, rangedY = y, rangedZ = z;
        range(&rangedX, &rangedY, &rangedZ);
        float d2 = (rangedX - vx) * (rangedX - vx) + (rangedY - vy) * (rangedY - vy) + (rangedZ - vz) * (rangedZ - vz);
        if (d2 < dis2) {
            dis2 = d2;
            minDistance_distance = s;
        }
        float k1x = Fx(x, y, z, Tx, Ty, Tz);
        float k1y = Fy(x, y, z, Tx, Ty, Tz);
        float k1z = Fz(x, y, z, Tx, Ty, Tz);
        float T1x = Tx + .5f * k1x * h;
        float T1y = Ty + .5f * k1y * h;
        float T1z = Tz + .5f * k1z * h;
        float k2x = Fx(x + .5f * Tx * h, y + .5f * Ty * h, z + .5f * Tz * h, T1x, T1y, T1z);
        float k2y = Fy(x + .5f * Tx * h, y + .5f * Ty * h, z + .5f * Tz * h, T1x, T1y, T1z);
        float k2z = Fz(x + .5f * Tx * h, y + .5f * Ty * h, z + .5f * Tz * h, T1x, T1y, T1z);
        float T2x = Tx + .5f * k2x * h;
        float T2y = Ty + .5f * k2y * h;
        float T2z = Tz + .5f * k2z * h;
        float k3x = Fx(x + .5f * T1x * h, y + .5f * T1y * h, z + .5f * T1z * h, T2x, T2y, T2z);
        float k3y = Fy(x + .5f * T1x * h, y + .5f * T1y * h, z + .5f * T1z * h, T2x, T2y, T2z);
        float k3z = Fz(x + .5f * T1x * h, y + .5f * T1y * h, z + .5f * T1z * h, T2x, T2y, T2z);
        float T3x = Tx + k3x * h;
        float T3y = Ty + k3y * h;
        float T3z = Tz + k3z * h;
        float k4x = Fx(x + T2x * h, y + T2y * h, z + T2z * h, T3x, T3y, T3z);
        float k4y = Fy(x + T2x * h, y + T2y * h, z + T2z * h, T3x, T3y, T3z);
        float k4z = Fz(x + T2x * h, y + T2y * h, z + T2z * h, T3x, T3y, T3z);
        x += .16666666666666666f * (Tx + 2 * T1x + 2 * T2x + T3x) * h;
        y += .16666666666666666f * (Ty + 2 * T1y + 2 * T2y + T3y) * h;
        z += .16666666666666666f * (Tz + 2 * T1z + 2 * T2z + T3z) * h;
        Tx += .16666666666666666f * (k1x + 2 * k2x + 2 * k3x + k4x) * h;
        Ty += .16666666666666666f * (k1y + 2 * k2y + 2 * k3y + k4y) * h;
        Tz += .16666666666666666f * (k1z + 2 * k2z + 2 * k3z + k4z) * h;
//        printf("[%f, %f], ", rangedY, rangedZ);
    }
//    printf("%f\n", dis2);
    *distance = dis2;
    return 2.0f * M_1_PI * atan(minDistance_distance);
}

__kernel void geodesic(
        __global float* vx_,
        __global float* vy_,
        __global float* vz_,
        __global float* Tx_,
        __global float* Ty_,
        __global float* Tz_,
        __global float* distance,// 顶点与出发点的距离
        const float x,
        const float y,
        const float z,
        const float h,
        const unsigned int count) {
    int i = get_global_id(0);
    if (i >= count) return;
    float vx = vx_[i], vy = vy_[i], vz = vz_[i];
    float Tx = vx - x, Ty = vy - y, Tz = vz - z;// 猜测切向量值
    float camera_vertex_distance;
    float Tlen = 1.0f / sqrt(Tx * Tx + square(cos(x)) * (Ty * Ty + square(cos(y)) * Tz * Tz));
    Tx *= Tlen;
    Ty *= Tlen;
    Tz *= Tlen;
    // 牛顿迭代
    float dis2 = 1e10;
    float dxDis2, dyDis2;
    while (dis2 >= 0.01f) {
        // 因为切向量长度是被限定为1的所以实际上只有两个自由变量，这里我们选择Tx, Ty
        float dxDis2, dyDis2;
        camera_vertex_distance = calculateGeo(vx, vy, vz, Tx, Ty, Tz, &dis2, x, y, z, h);
        calculateGeo(vx, vy, vz, Tx + h, Ty, Tz, &dxDis2, x, y, z, h);
        calculateGeo(vx, vy, vz, Tx, Ty + h, Tz, &dyDis2, x, y, z, h);
        float partialX = (dxDis2 - dis2) / h;
        float partialY = (dyDis2 - dis2) / h;
        float _gradF2 = 1.0f / (partialX * partialX + partialY * partialY);
        Tx -= dis2 * partialX * _gradF2;
        Ty -= dis2 * partialY * _gradF2;
    }
    // 最后归一化
    Tlen = 1.0f / sqrt(Tx * Tx + square(cos(x)) * (Ty * Ty + square(cos(y)) * Tz * Tz));
    Tx *= Tlen;
    Ty *= Tlen;
    Tz *= Tlen;
    distance[i] = camera_vertex_distance;
    Tx_[i] = Tx;
    Ty_[i] = Ty;
    Tz_[i] = Tz;
}