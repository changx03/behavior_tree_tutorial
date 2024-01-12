#include "behaviortree_cpp/bt_factory.h"

#include <iostream>
#include <string>

class CrossDoor
{
  public:
    void registerNodes(BT::BehaviorTreeFactory& factory)
    {
        factory.registerSimpleCondition("IsDoorClosed", std::bind(&CrossDoor::isDoorClosed, this));

        factory.registerSimpleAction("PassThroughDoor", std::bind(&CrossDoor::passThroughDoor, this));

        factory.registerSimpleAction("OpenDoor", std::bind(&CrossDoor::openDoor, this));

        factory.registerSimpleAction("PickLock", std::bind(&CrossDoor::pickLock, this));

        factory.registerSimpleCondition("SmashDoor", std::bind(&CrossDoor::smashDoor, this));
    }

    BT::NodeStatus isDoorClosed()
    {
        if(!this->_door_open) { return BT::NodeStatus::SUCCESS; }
        else { return BT::NodeStatus::FAILURE; }
    }

    BT::NodeStatus passThroughDoor()
    {
        if(this->_door_open) { return BT::NodeStatus::SUCCESS; }
        else { return BT::NodeStatus::FAILURE; }
    }

    BT::NodeStatus pickLock()
    {
        this->_pick_attempts++;

        if(this->_pick_attempts >= 3)
        {
            std::cout << "[pickLock] You picked the lock." << std::endl;
            return BT::NodeStatus::SUCCESS;
        }
        else
        {
            std::cout << "[pickLock] You try to pick the lock but failed." << std::endl;
            return BT::NodeStatus::FAILURE;
        }
    }

    BT::NodeStatus openDoor()
    {
        if(this->_door_locked)
        {
            std::cout << "[openDoor] The door is locked!" << std::endl;
            return BT::NodeStatus::FAILURE;
        }
        else
        {
            std::cout << "[openDoor] You open the door!" << std::endl;
            return BT::NodeStatus::SUCCESS;
        }
    }

    BT::NodeStatus smashDoor()
    {
        this->_door_open = true;
        std::cout << "[smashDoor] The door is smashed open!" << std::endl;
        return BT::NodeStatus::SUCCESS;
    }

  private:
    bool _door_open = false;
    bool _door_locked = true;
    int _pick_attempts = 0;
};

int main(int argc, char* argv[])

{
    BT::BehaviorTreeFactory factory;
    CrossDoor cross_door;
    cross_door.registerNodes(factory);

    std::string xml_path = "xml/pass_through_door.xml";
    factory.registerBehaviorTreeFromFile(xml_path);
    auto tree = factory.createTree("MainTree");

    BT::printTreeRecursively(tree.rootNode());
    tree.tickWhileRunning();

    return 0;
}
