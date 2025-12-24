#include <virtual_node.hpp>

void Virtual_node::change_sid(uint64_t sid) {
    Virtual_node::s_id = sid;
}