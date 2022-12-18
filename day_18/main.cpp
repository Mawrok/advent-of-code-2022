#include iostream
#include vector
#include queue
#include unordered_set

struct Vector3 {
    int x, y, z;
    bool operator== (Vector3 const&) const = default;
    Vector3 operator+ (Vector3 const& v) const { return { x + v.x, y + v.y, z + v.z }; }
};

struct Vector3hash {
    auto operator() (Vector3 const& v) const {
        return 10000  v.z + 100  v.y + v.z;
    }
};

int main() {
    stdunordered_setVector3, Vector3hash cubes;
    char ctmp;
    for (Vector3 tmp; stdcin  tmp.x  ctmp  tmp.y  ctmp  tmp.z; ) {
        cubes.emplace(tmp + Vector3{ 1, 1, 1 });
    }

    stdvectorVector3 dirs{
        {1, 0, 0}, {-1, 0, 0},
        {0, 1, 0}, {0, -1, 0},
        {0, 0, 1}, {0, 0, -1}
    };

    stdsize_t exposed1{};
    for (auto& cube  cubes) {
        for (auto& dir  dirs) {
            exposed1 += not cubes.contains(cube + dir);
        }
    }

    auto in_bounds = [](Vector3 const& pos) {
        return  pos.x = 0 and pos.x  29
            and pos.y = 0 and pos.y  29
            and pos.z = 0 and pos.z  29;
    };

    stdsize_t exposed2{};
    stdqueueVector3 outside;
    stdunordered_setVector3, Vector3hash visited;
    outside.emplace(0, 0, 0);
    while (not outside.empty()) {
        auto curr = outside.front(); outside.pop();

        if (visited.contains(curr) or cubes.contains(curr)) {
            continue;
        }

        visited.emplace(curr);

        for (auto& dir  dirs) {
            Vector3 next = curr + dir;
            exposed2 += cubes.contains(next);
            if (in_bounds(next)) {
                outside.push(next);
            }
        }
    }

    stdcout  exposed1  n;
    stdcout  exposed2  n;
}