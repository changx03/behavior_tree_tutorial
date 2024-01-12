#include "behaviortree_cpp/bt_factory.h"

#include <iostream>
#include <string>

static const char* xml_text = R"(
<root BTCPP_format="4">
  <BehaviorTree>
    <Sequence>
      <Script code=" msg:='hello world' " />
      <Script code=" A:=THE_ANSWER; B:=3.14; color:=RED " />
        <Precondition if="A>B && color != BLUE" else="FAILURE">
          <Sequence>
            <SaySomething message="{A}"/>
            <SaySomething message="{B}"/>
            <SaySomething message="{msg}"/>
            <SaySomething message="{color}"/>
        </Sequence>
      </Precondition>
    </Sequence>
  </BehaviorTree>
</root>

 )";

class SaySomething : public BT::SyncActionNode
{
  public:
    SaySomething(const std::string& name, const BT::NodeConfig& config)
        : BT::SyncActionNode(name, config)
    {
    }

    BT::NodeStatus tick() override
    {
        std::string msg;
        getInput("message", msg);
        std::cout << msg << std::endl;
        return BT::NodeStatus::SUCCESS;
    }

    static BT::PortsList providedPorts()
    {
        return {BT::InputPort<std::string>("message")};
    }
};

enum Color
{
    RED = 1,
    BLUE = 2,
    GREEN = 3
};

int main(int argc, char* argv[])
{
    BT::BehaviorTreeFactory factory;
    factory.registerNodeType<SaySomething>("SaySomething");

    // We can add these enums to the scripting language.
    // Check the limits of magic_enum
    factory.registerScriptingEnums<Color>();

    // Or we can manually assign a number to the label "THE_ANSWER".
    // This is not affected by any range limitation
    factory.registerScriptingEnum("THE_ANSWER", 42);

    auto tree = factory.createTreeFromText(xml_text);
    tree.tickWhileRunning();

    return 0;
}
