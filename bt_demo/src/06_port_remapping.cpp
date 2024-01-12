#include "behaviortree_cpp/bt_factory.h"

#include <chrono>
#include <iostream>
#include <string>

// Custom type
struct Pose2D
{
    double x, y, theta;
};

namespace BT
{
// This template specialization is needed only if you want
// to AUTOMATICALLY convert a NodeParameter into a Pose2D
// In other words, implement it if you want to be able to do:
//
//   TreeNode::getInput<Pose2D>(key, ...)
//
template <> inline Pose2D convertFromString(StringView key)
{
    // three real numbers separated by semicolons
    auto parts = BT::splitString(key, ';');
    if(parts.size() != 3) { throw BT::RuntimeError("invalid input)"); }

    Pose2D output;
    output.x = convertFromString<double>(parts[0]);
    output.y = convertFromString<double>(parts[1]);
    output.theta = convertFromString<double>(parts[2]);
    return output;
}
} // end namespace BT

class SaySomething : public BT::SyncActionNode
{
  public:
    SaySomething(const std::string& name, const BT::NodeConfig& config)
        : SyncActionNode(name, config)
    {
    }

    static BT::PortsList providedPorts()
    {
        return {BT::InputPort<std::string>("message")};
    }

    BT::NodeStatus tick() override
    {
        BT::Expected<std::string> msg = this->getInput<std::string>("message");
        if(!msg) { throw BT::RuntimeError("Missing required input [message]: ", msg.error()); }

        std::cout << "Robot says: " << msg.value() << std::endl;
        return BT::NodeStatus::SUCCESS;
    }
};

class MoveBaseAction : public BT::StatefulActionNode
{
  public:
    // Any TreeNode with ports must have a constructor with this signature
    MoveBaseAction(const std::string& name, const BT::NodeConfig& config)
        : StatefulActionNode(name, config)
    {
    }

    // It is mandatory to define this static method.
    static BT::PortsList providedPorts()
    {
        return {BT::InputPort<Pose2D>("goal")};
    }

    // this function is invoked once at the beginning.
    BT::NodeStatus onStart() override
    {
        if(!getInput<Pose2D>("goal", _goal)) { throw BT::RuntimeError("missing required input [goal]"); }
        printf("[ MoveBase: SEND REQUEST ]. goal: x=%.1f y=%.1f theta=%.1f\n", _goal.x, _goal.y, _goal.theta);

        // We use this counter to simulate an action that takes a certain
        // amount of time to be completed (220 ms)
        _completion_time = std::chrono::system_clock::now() + std::chrono::milliseconds(220);

        return BT::NodeStatus::RUNNING;
    }

    // If onStart() returned RUNNING, we will keep calling
    // this method until it return something different from RUNNING
    BT::NodeStatus onRunning() override
    {
        // Pretend that we are checking if the reply has been received
        // you don't want to block inside this function too much time.
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        // Pretend that, after a certain amount of time,
        // we have completed the operation
        if(std::chrono::system_clock::now() >= _completion_time)
        {
            std::cout << "[ MoveBase: FINISHED ]" << std::endl;
            return BT::NodeStatus::SUCCESS;
        }
        return BT::NodeStatus::RUNNING;
    }

    // callback to execute if the action was aborted by another node
    void onHalted() override
    {
        printf("[ MoveBase: ABORTED ]");
    }

  private:
    Pose2D _goal;
    std::chrono::system_clock::time_point _completion_time;
};

int main(int argc, char const* argv[])
{
    BT::BehaviorTreeFactory factory;
    factory.registerNodeType<SaySomething>("SaySomething");
    factory.registerNodeType<MoveBaseAction>("MoveBase");

    factory.registerBehaviorTreeFromFile("xml/port_remapping.xml");
    auto tree = factory.createTree("MainTree");
    tree.tickWhileRunning();

    // let's visualize some information about the current state of the blackboards.
    std::cout << "\n------ First BB ------" << std::endl;
    tree.subtrees[0]->blackboard->debugMessage();
    std::cout << "\n------ Second BB------" << std::endl;
    tree.subtrees[1]->blackboard->debugMessage();
    return 0;
}
