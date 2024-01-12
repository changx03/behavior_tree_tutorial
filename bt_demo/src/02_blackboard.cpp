#include "behaviortree_cpp/bt_factory.h"

#include <iostream>
#include <string>

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

class ThinkWhatToSay : public BT::SyncActionNode
{
  public:
    ThinkWhatToSay(const std::string& name, const BT::NodeConfig& config)
        : SyncActionNode(name, config)
    {
    }

    static BT::PortsList providedPorts()
    {
        return {BT::OutputPort<std::string>("text")};
    }

    BT::NodeStatus tick() override
    {
        this->setOutput("text", "The answer is 42!");
        return BT::NodeStatus::SUCCESS;
    }
};

int main(int argc, char const* argv[])
{
    BT::BehaviorTreeFactory factory;
    factory.registerNodeType<SaySomething>("SaySomething");
    factory.registerNodeType<ThinkWhatToSay>("ThinkWhatToSay");

    auto tree = factory.createTreeFromFile("xml/robot_says.xml");
    tree.tickWhileRunning();
    return 0;
}