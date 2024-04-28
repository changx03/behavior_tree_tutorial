#include "behaviortree_cpp/bt_factory.h"

/** We want to use this custom type */
struct Position2D
{
    double x;
    double y;
};

// Template specialization to converts a string to Position2D.
template <> inline Position2D BT::convertFromString(BT::StringView str)
{
    // StringView is a C++11 version of std::string_view. std::string_view is a read-only, non-owning view of a char sequence.
    // We expect real numbers separated by semicolons
    auto parts = BT::splitString(str, ';');
    if (parts.size() != 2) { throw BT::RuntimeError("invalid input)"); }
    else
    {
        Position2D output;
        output.x = BT::convertFromString<double>(parts[0]);
        output.y = BT::convertFromString<double>(parts[1]);
        return output;
    }
}

class CalculateGoal : public BT::SyncActionNode
{
public:
    CalculateGoal(const std::string& name, const BT::NodeConfig& config)
        : SyncActionNode(name, config)
    {
    }

    static BT::PortsList providedPorts()
    {
        return {BT::OutputPort<Position2D>("goal")};
    }

    BT::NodeStatus tick() override
    {
        Position2D mygoal = {1.1, 2.3};
        this->setOutput<Position2D>("goal", mygoal);
        return BT::NodeStatus::SUCCESS;
    }
};

class PrintTarget : public BT::SyncActionNode
{
public:
    PrintTarget(const std::string& name, const BT::NodeConfig& config)
        : SyncActionNode(name, config)
    {
    }

    static BT::PortsList providedPorts()
    {
        // Optionally, a port can have a human readable description
        const char* description = "Simply print the goal on console...";
        return {BT::InputPort<Position2D>("target", description)};
    }

    BT::NodeStatus tick() override
    {
        auto res = this->getInput<Position2D>("target");
        if (!res) { throw BT::RuntimeError("error reading port [target]:", res.error()); }
        Position2D target = res.value();
        printf("Target positions: [ %.1f, %.1f ]\n", target.x, target.y);
        return BT::NodeStatus::SUCCESS;
    }
};

int main(int argc, char const* argv[])
{
    BT::BehaviorTreeFactory factory;
    factory.registerNodeType<CalculateGoal>("CalculateGoal");
    factory.registerNodeType<PrintTarget>("PrintTarget");

    std::string xml_path = "xml/generic_ports.xml";
    auto tree = factory.createTreeFromFile(xml_path);
    tree.tickWhileRunning();

    return 0;
}