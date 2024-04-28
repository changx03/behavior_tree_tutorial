# Behavior Tree Tutorial

From the official source: [https://www.behaviortree.dev/docs/category/tutorials-basic](https://www.behaviortree.dev/docs/category/tutorials-basic)

## Install

Assume you have ROS2 installed

```bash
git clone git@github.com:changx03/behavior_tree_tutorial.git
git submodule init
git submodule update --init
```

## Build

```bash
. ./setup.bash
colcon build --symlink-install
```

## Run the 1st example

```bash
. ./setup.bash
ros2 run bt_demo bt_sample
```
