#include "behaviortree_cpp/bt_factory.h"

#include <iostream>
#include <fstream>
#include <string>

/** Example of custom SyncActionNode (synchronous action) without ports. */
class ApproachObject : public BT::SyncActionNode
{
  public:
    ApproachObject(const std::string& name)
        : BT::SyncActionNode(name, {})
    {
    }

    /** You must override the virtual function tick() */
    BT::NodeStatus tick() override
    {
        std::cout << "ApproachObject: " << this->name() << std::endl;
        return BT::NodeStatus::SUCCESS;
    }
};

/** Simple function that return a NodeStatus */
BT::NodeStatus CheckBattery()
{
    std::cout << "[ Battery: OK ]" << std::endl;
    return BT::NodeStatus::SUCCESS;
}

/** We want to wrap into an ActionNode the methods open() and close() */
class GripperInterface
{
  public:
    GripperInterface()
        : _open(true)
    {
    }

    BT::NodeStatus open()
    {
        this->_open = true;
        std::cout << "GripperInterface::open" << std::endl;
        return BT::NodeStatus::SUCCESS;
    }

    BT::NodeStatus close()
    {
        this->_open = false;
        std::cout << "GripperInterface::close" << std::endl;
        return BT::NodeStatus::SUCCESS;
    }

  private:
    bool _open;
};

int main(int argc, char const* argv[])
{
    BT::BehaviorTreeFactory factory;

    // The recommended way to create a Node is through inheritance.
    factory.registerNodeType<ApproachObject>("ApproachObject");

    // Registering a SimpleActionNode using a function pointer.
    // You can use C++11 lambdas or std::bind
    factory.registerSimpleCondition("CheckBattery", [&](BT::TreeNode&) { return CheckBattery(); });

    // You can also create SimpleActionNodes using methods of a class
    GripperInterface gripper;
    factory.registerSimpleAction("OpenGripper", [&](BT::TreeNode&) { return gripper.open(); });
    factory.registerSimpleAction("CloseGripper", [&](BT::TreeNode&) { return gripper.close(); });

    // Trees are created at deployment-time (i.e. at run-time, but only
    // once at the beginning).

    // IMPORTANT: when the object "tree" goes out of scope, all the
    // TreeNodes are destroyed
    std::string xml_path = "xml/my_tree.xml";
    auto tree = factory.createTreeFromFile(xml_path);
    tree.tickWhileRunning();

    return 0;
}
