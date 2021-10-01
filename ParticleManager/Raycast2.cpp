#include "Raycast2.h"

using namespace pc;
using namespace std;

const double epsilon = numeric_limits<float>().epsilon();
const numeric_limits<double> DOUBLE;
const double MIN = DOUBLE.min();
const double MAX = DOUBLE.max();

//struct Point
//{
//    const double mX, mY;
//};

struct Edge
{
    const Point a, b;

    bool operator()(const Point& p) const
    {
        // Makes sure that the end point of the segment is larger than the.
        if (a.mY > b.mY)
            return Edge{ b, a }(p);
        // If current point's y is equal to the start or end of the segment's y.
        if (p.mY == a.mY || p.mY == b.mY)
            return operator()({ p.mX, p.mY + (const float)epsilon, 0 });
        // If current point is inside boundaries.
        if (p.mY > b.mY || p.mY < a.mY || p.mX > max(a.mX, b.mX))
            return false;

        if (p.mX < min(a.mX, b.mX))
            return true;
        auto blue = abs(a.mX - p.mX) > MIN ? (p.mY - a.mY) / (p.mX - a.mX) : MAX;
        auto red = abs(a.mX - b.mX) > MIN ? (b.mY - a.mY) / (b.mX - a.mX) : MAX;
        return blue >= red;
    }
};

struct Figure
{
    const string  name;
    const initializer_list<Edge> edges;

    bool contains(const Point& p) const
    {
        auto c = 0;
        for (auto e : edges) if (e(p)) c++;
        return c % 2 != 0;
    }

    template<unsigned char W = 3>
    void check(const initializer_list<Point>& points, ostream& os) const
    {
        os << "Is point inside figure " << name << '?' << endl;
        for (auto p : points)
            os << "  (" << setw(W) << p.mX << ',' << setw(W) << p.mY << "): " << boolalpha << contains(p) << endl;
        os << endl;
    }
};

bool Raycast2::checkEdge(Vector2* edgeStart, Vector2* edgeEnd, Vector2 testCollisionPoint)
{
    // Makes sure that the end point of the segment is larger than the.
    if (edgeStart->mY > edgeEnd->mY)
        return checkEdge( edgeEnd, edgeStart, testCollisionPoint);
    // If current point's y is equal to the start or end of the segment's y.
    if (testCollisionPoint.mY == edgeStart->mY || testCollisionPoint.mY == edgeEnd->mY)
        return checkEdge(edgeStart, edgeEnd, { testCollisionPoint.mX, testCollisionPoint.mY + (const float)epsilon });
    // If current point is inside boundaries.
    if (testCollisionPoint.mY > edgeEnd->mY || testCollisionPoint.mY < edgeStart->mY || testCollisionPoint.mX > max(edgeStart->mX, edgeEnd->mX))
        return false;

    if (testCollisionPoint.mX < min(edgeStart->mX, edgeEnd->mX))
        return true;
    auto blue = abs(edgeStart->mX - testCollisionPoint.mX) > MIN ? (testCollisionPoint.mY - edgeStart->mY) / (testCollisionPoint.mX - edgeStart->mX) : MAX;
    auto red = abs(edgeStart->mX - edgeEnd->mX) > MIN ? (edgeEnd->mY - edgeStart->mY) / (edgeEnd->mX - edgeStart->mX) : MAX;
    return blue >= red;
}

bool Raycast2::checkPolygon(const std::vector<Vector2>& vectors, Vector2 testCollisionPoint)
{
    if (vectors.size() < 3) return false;
    pc::Vector2 p0 = pc::Vector2(vectors[0]);
    pc::Vector2 p1 = pc::Vector2(vectors[1]);
    pc::Vector2 p2 = pc::Vector2(vectors[2]);

    auto c = 0;
    if (checkEdge(&p0, &p1, testCollisionPoint)) c++;
    if (checkEdge(&p1, &p2, testCollisionPoint)) c++;
    if (checkEdge(&p2, &p0, testCollisionPoint)) c++;

    return c % 2 != 0;
}

bool Raycast2::performRaycast(vector<ParticleCuboid*>& cuboids, Vector2 rayStartPos)
{
    const initializer_list<Point> points = { {5.0, 5.0, 0}, {5.0, 8.0, 0}, {-10.0, 5.0, 0}, {0.0, 5.0, 0}, {10.0, 5.0, 0}, {8.0, 5.0, 0}, {10.0, 10.0, 0} };
    
    /*const Figure square = { "Square",
        {  {{0.0, 0.0}, {10.0, 0.0}}, {{10.0, 0.0}, {10.0, 10.0}}, {{10.0, 10.0}, {0.0, 10.0}}, {{0.0, 10.0}, {0.0, 0.0}} }
    };

    const Figure square_hole = { "Square hole",
        {  {{0.0, 0.0}, {10.0, 0.0}}, {{10.0, 0.0}, {10.0, 10.0}}, {{10.0, 10.0}, {0.0, 10.0}}, {{0.0, 10.0}, {0.0, 0.0}},
           {{2.5, 2.5}, {7.5, 2.5}}, {{7.5, 2.5}, {7.5, 7.5}}, {{7.5, 7.5}, {2.5, 7.5}}, {{2.5, 7.5}, {2.5, 2.5}}
        }
    };

    const Figure strange = { "Strange",
        {  {{0.0, 0.0}, {2.5, 2.5}}, {{2.5, 2.5}, {0.0, 10.0}}, {{0.0, 10.0}, {2.5, 7.5}}, {{2.5, 7.5}, {7.5, 7.5}},
           {{7.5, 7.5}, {10.0, 10.0}}, {{10.0, 10.0}, {10.0, 0.0}}, {{10.0, 0}, {2.5, 2.5}}
        }
    };

    const Figure exagon = { "Exagon",
        {  {{3.0, 0.0}, {7.0, 0.0}}, {{7.0, 0.0}, {10.0, 5.0}}, {{10.0, 5.0}, {7.0, 10.0}}, {{7.0, 10.0}, {3.0, 10.0}},
           {{3.0, 10.0}, {0.0, 5.0}}, {{0.0, 5.0}, {3.0, 0.0}}
        }
    };*/

    for (auto cuboid : cuboids)
    {
        for (auto index : cuboid->mFaces)
        {
            //checkPolygon({ cuboid->mParticles[index[0]]->getPosition(), cuboid->mParticles[index[2]]->getPosition() }, rayStartPos);
        }
    }
    /*for (auto f : { square, square_hole, strange, exagon })
        f.check(points, cout);*/

    return EXIT_SUCCESS;
}

// Written by Peter Nilsson and Christian Larsson, licensed under CC BY-SA 4.0