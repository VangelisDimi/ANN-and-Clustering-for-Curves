#pragma once
#ifndef curve_h
#define curve_h

#include <ctime>
#include <cstring>
#include <unistd.h>
#include <vector>
#include <unordered_map>
#include <map>
#include <chrono>

using namespace std;

class Curve : private Points
{
private:
    int vstart = 0, vend = 0;
	int complexity = 0;
    int dimensions = 0;

protected:

public:
    Curve(const Points &points);

    inline Point& operator[](const curve_size_t i) {
        return Points::operator[](vstart + i);
    }

    inline bool empty() const {
        return Points::empty();
    }

    inline int complexity() const {
        return empty() ? 0 : vend - vstart + 1; 
    }
    
    inline int size() const {
        return empty() ? 0 : vend - vstart + 1;
    }
    
    inline int dimensions() const { 
        return empty() ? 0 : Points::dimensions();
    }
};

class Points : public std::vector<Point> {
    int dim;
    
public:
    inline Points(const dimensions_t dim) : dim{dim} {}
    inline Points(const curve_size_t m, const dimensions_t dim) : std::vector<Point>(m, Point(dim)), dim{dim} {}
    inline Points(const curve_size_t m, const Point& p) : std::vector<Point>(m, p), dim{p.dimensions()} {}

};

class Point : public Coordinates {
public:    
    inline Point(const dimensions_t d) : Coordinates(d) {}
    
    inline dimensions_t dimensions() const {
        return size();
    }
    
    inline coordinate_t get(const dimensions_t i) const { 
        return Coordinates::operator[](i); 
    }
    
    inline void set(const dimensions_t i, coordinate_t val) {
        Coordinates::operator[](i) = val;
    }
    
    #pragma omp declare simd
    inline coordinate_t& operator[](const dimensions_t i) { 
        return Coordinates::operator[](i); 
    }
};

#endif