#include <satellite.hpp>
#include <unordered_map>

extern std::unordered_map<uint64_t, Virtual_node> grid_map;

class Virtual_node final {
public:
    Virtual_node(uint64_t vid) : v_id(vid) {};

    void change_sid(uint64_t sid);
private:
    uint64_t v_id;
    uint64_t s_id;
    coord coordinates;
};