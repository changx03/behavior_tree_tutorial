#include "behaviortree_cpp/bt_factory.h"

#include <chrono>
#include <iostream>
#include <string>

using namespace std::chrono_literals;

struct Pose2D
{
    double x, y, theta;
};

// Template specialization to converts a string to Position2D.
template <> inline Pose2D BT::convertFromString(BT::StringView str)
{
    // We expect real numbers separated by semicolons
    auto parts = BT::splitString(str, ';');
    if(parts.size() != 3) { throw BT::RuntimeError("invalid input)"); }
    else
    {
        Pose2D output;
        output.x = BT::convertFromString<double>(parts[0]);
        output.y = BT::convertFromString<double>(parts[1]);
        output.theta = BT::convertFromString<double>(parts[2]);
        return output;
    }
}

/** Simple function that return a NodeStatus */
BT::NodeStatus CheckBattery()
{
    std::cout << "[ Battery: OK ]" << std::endl;
    return BT::NodeStatus::SUCCESS;
}

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
    MoveBaseAction(const std::string& name, const BT::NodeConfig& config)
        : StatefulActionNode(name, config)
    {
    }

    static BT::PortsList providedPorts()
    {
        return {BT::InputPort<Pose2D>("goal")};
    }

    BT::NodeStatus onStart() override
    {
        if(!this->getInput<Pose2D>("goal", this->_goal)) { throw BT::RuntimeError("missing required input [goal]"); }

        printf("[MoveBase: SEND REQUEST]. goal: x=%f y=%f theta=%f\n", this->_goal.x, this->_goal.y, this->_goal.theta);

        this->_completion_time = std::chrono::system_clock::now() + 220ms;

        return BT::NodeStatus::RUNNING;
    }

    BT::NodeStatus onRunning() override
    {
        std::this_thread::sleep_for(10ms);

        if(std::chrono::system_clock::now() >= this->_completion_time)
        {
            std::cout << "[MoveBase: FINISHED]" << std::endl;
            return BT::NodeStatus::SUCCESS;
        }

        return BT::NodeStatus::RUNNING;
    }

    void onHalted() override
    {
        std::cout << "[MoveBase: ABORTED]" << std::endl;
    }

  private:
    Pose2D _goal;
    std::chrono::system_clock::time_point _completion_time;
};

int main(int argc, char* argv[])
{
    BT::BehaviorTreeFactory factory;
    factory.registerSimpleCondition("BatteryOK", std::bind(CheckBattery));
    factory.registerNodeType<MoveBaseAction>("MoveBase");
    factory.registerNodeType<SaySomething>("SaySomething");

    std::string xml_path = "xml/async_bt.xml";
    auto tree = factory.createTreeFromFile(xml_path);

    // Here, instead of tree.tickWhileRunning(),
    // we prefer our own loop.
    std::cout << "--- ticking\n";
    auto status = tree.tickOnce();
    std::cout << "--- status: " << BT::toStr(status) << std::endl;

    while(status == BT::NodeStatus::RUNNING)
    {
        // Sleep to avoid busy loops.
        // do NOT use other sleep functions!
        // Small sleep time is OK, here we use a large one only to
        // have less messages on the console.
        tree.sleep(100ms);

        std::cout << "--- ticking\n";
        status = tree.tickOnce();
        std::cout << "--- status: " << toStr(status) << std::endl;
    }

    return 0;
}
