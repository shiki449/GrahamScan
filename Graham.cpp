// graham scan
// 2020.05.27

#include <iostream>
#include <vector>
#include <algorithm>

struct Point{
    double x, y;

    bool operator < (Point p){    //x座標の最小値を求めるよう
        if(x != p.x) return x < p.x;
        return y < p.y;
    }
};

std::istream& operator>>(std::istream& in, Point& p){
    in >> p.x >> p.y;
    return in;
}

//符号付き面積の利用
int sgn(Point pt1, Point pt2, Point pt3){
    double area = (pt2.x - pt1.x) * (pt3.y - pt1.y) - (pt2.y - pt1.y) * (pt3.x - pt1.x);
    if (area > 0) return -1;
    else if(area < 0) return 1;
    else return 0; 
}

//直線上の時につかう
double distance(Point pt1, Point pt2){
    return (pt2.x - pt1.x) * (pt2.x - pt1.x) + (pt2.y - pt1.y) * (pt2.y - pt1.y);
}

//偏角順にsort
void points_sort(std::vector<Point>& points){

    //基準点(x座標の最小値)を求める
    auto min_itr = std::min_element(points.begin(), points.end());
    std::iter_swap(points.begin(), min_itr);
    Point pt0 = points[0];

    //sortの比較方法の定義
    auto compare = [pt0](Point& pt1, Point& pt2){
        int flag = sgn(pt0, pt1, pt2);
        if (flag == 0) return distance(pt0, pt1) < distance(pt0, pt2);
        else return flag == -1;
    };
    std::sort(points.begin()+1, points.end(), compare);
}

//凸包
std::vector<Point> convex_hull(std::vector<Point> points){
    std::vector<Point> s;

    if (points.size() <= 2){              //点の数が2以下の時はそのまま返す
        return points;
    }else if (points.size() == 3){        //点の数が3の時は3点が直線の時、二点になるため例外処理をした
        for (auto i = 0; i < 2; ++i){
	        s.push_back(points[i]);
        }
    }else{
        for (auto i = 0; i < 3; ++i){
	        s.push_back(points[i]);
        }
    }

    for (auto i = s.size(); i < points.size(); ++i){
        auto k = s.size();
        while(s.size() > 1  &&  sgn(s[k-2], s[k-1], points[i]) != -1){
            s.pop_back();
            k = s.size();
        }
        s.push_back(points[i]);
        std::cout << "stack: ";
        for (auto p : s){
            std::cout << p.x << ' ' << p.y << "  ";
        }
        std::cout << '\n';
    }
    return s;
}

int main(){
    std::vector<Point> points = {};
    int n;
    std::cout << "n: ";
    std::cin >> n;

    points.resize(n);
    for(Point& p : points){
        std::cin >> p;
    }

    points_sort(points);
    std::cout << "---sort---" << '\n';
    for (auto p : points) {
        std::cout << p.x << ' ' << p.y << '\n';
    }
    std::vector<Point> ans = convex_hull(points);

    std::cout << "---convex_hull---" << '\n';
    for (auto p : ans) {
        std::cout << p.x << ' ' << p.y << '\n';
    }
}


/*

n: 12
1 11
2 4
8 5
16 3
14 8
11 10
21 13
6 12
18 22
10 18
9 23
3 19

stack: 1 11  2 4  16 3  
stack: 1 11  2 4  16 3  14 8
stack: 1 11  2 4  16 3  14 8  11 10
stack: 1 11  2 4  16 3  21 13
stack: 1 11  2 4  16 3  21 13  6 12
stack: 1 11  2 4  16 3  21 13  18 22
stack: 1 11  2 4  16 3  21 13  18 22  10 18
stack: 1 11  2 4  16 3  21 13  18 22  9 23
stack: 1 11  2 4  16 3  21 13  18 22  9 23  3 19
---convex_hull---
1 11
2 4
16 3
21 13
18 22
9 23
3 19

最小のx座標が同じときや点が直線になっているとき、点の数nが3より小さいときの例外にも対処できるようにした

*/