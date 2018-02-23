# MiniMappers

**Final Year Computer Science Project**  
A SLAM implementation using Swarm Robotics for search and rescue.


## Requirements
- Gazebo 9 available at http://gazebosim.org/

### For building the plugins.
- `gcc` (tested on 7.2.0)
- `pkg-config`
- `libgazebo9-dev` package to build plugins.

## Testing
There are two kinds of worlds being implemented, one for testing core components and one for the demonstration in college. The testing worlds are,
1. `sonar_setup_test.world` to test the sonar structure.
2. `model_setup_test.world` to test the model structure.
3. `sonar_check_test.world` to test the sonar mounted on the model.
The worlds designed for demonstration are,
1. `icy.world` — an ice-themed maze with a swarm of MiniMappers.

To test the simulations, you'll need to build the plugins. So first run,
```sh
$ make
```

Once the plugins have built, ensure `simulate.sh` has execute permissions and then run the script. Pass the world to render as the argument without `.world`.
```sh
$ chmod u+x simulate.sh
$ ./simulate.sh model_setup_test
```

The simulator should shut down after closing the Gazebo client (GUI). If the gazebo server isn't shutting down. Press `ctrl-c` to send the `SIGINT` signal to the server. To check if the server has shut down, run
```sh
$ ps -ax | grep "gzserver" | grep -v grep
```
If the server is still running, `kill` it.
```sh
$ kill $(ps ax | grep "sleep" | grep -v grep | awk '{print $1;}')
```

## Tutorials

### Vehicle
This is the beginner's tutorial for Gazebo. The model config and SDF are in ```tutorial/vehicle/spec```. To test the simulation, run
```sh
$ cd tutorial/vehicle/spec
$ gazebo vehicle
```

### Velodyne LiDAR
This is the intermediate tutorial for Gazebo. `velodyne.world` contains the specification for the world. `model` contains the 3D model for the LiDAR (including meshes). `velodyne_plugin.cc` is a plugin that allows the upper section of the LiDAR to rotate. `vel.cc` allows you to dynamically specify the speed of rotation.

To test the simulation, copy the model files to `~/.gazebo/models/` so that gazebo recognizes the model without any extra effort.
```sh
$ mkdir -p ~/.gazebo/models/velodyne_hdl32/
$ cd tutorial/velodyne/
$ cp -r model/* velodyne_hdl32/
```
Next up, you need to build the plugin and the test code.
```sh
$ mkdir build/
$ cd build/
$ cmake ..
$ make
```
Now to run the code, you'll need to open two terminals. Make sure you're in the `tutorial/velodyne/build` folder in both terminals. Then, on the first terminal, run
```sh
$ gazebo --verbose ../velodyne.world
```
You should see a LiDAR that spins quite fast. To slow things down, on the second terminal run,
```sh
$ ./vel 2
```
Change 2 to whatever speed you desire.

#### Troubleshooting
Now, there is an issue that might cause the build to fail with either of these messages,
```
No rule to make target '/usr/lib/libblas.so', needed by 'vel'.  Stop.
No rule to make target '/usr/lib/liblapack.so', needed by 'vel'.  Stop.
```

If this is the case ensure you have the BLAS and LAPACK libraries installed and a symlink to the libraries exists in `/usr/lib`. On Ubuntu, these packages are `libblas-dev` and `liblapack-dev`. On Ubuntu 17.10 (64bit), the symlinks can be constructed by running,
```sh
$ sudo ln -s /usr/lib/x86_64-linux-gnu/libblas.so /usr/lib/libblas.so
$ sudo ln -s /usr/lib/x86_64-linux-gnu/liblapack.so /usr/lib/liblapack.so
```

> ***Do not run the above commands without properly verifying that it does not overwrite a pre-existing file or cause a broken link (especially if you're not on Ubuntu 17.10, 64bit). I am not responsible for messing up your system.***

## Documentation

### Primary Texts

| **Document** | **Purpose**                                     |
| ------------ | ----------------------------------------------- |
| Synopsis     | Summary of work and currently planned progress. |
| Seminar 1    | Presented for first review.                     |

### Reference Texts
- Stephen Se, David Lowe, and Jim Little. Vision-based mobile robot localization and mapping using scale-invariant features. In Robotics and Automation, 2001. Proceedings 2001 ICRA. IEEE International Conference on, volume 2, pages 2051–2058. IEEE, 2001.
- Joseph A Rothermich, M İhsan Ecemiş, and Paolo Gaudiano. Distributed localization and mapping with a robotic swarm. In International Workshop on Swarm Robotics, pages 58–69. Springer, 2004.
- Sebastian Thrun and Yufeng Liu. Multi-robot slam with sparse extended information filers. Robotics Research, pages 254–266, 2005.
- Andrew Howard, Lynne E Parker, and Gaurav S Sukhatme. The SDR experience: Experiments with a large-scale heterogeneous mobile robot team. In Experimental Robotics IX, pages 121–130. Springer, 2006.
- Iñaki Navarro and Fernando Matı́a. An introduction to swarm robotics. ISRN Robotics, 2013, 2012.
- Ying Tan and Zhong-yang Zheng. Research advance in swarm robotics. Defence Technology, 9(1):18–39, 2013.
- Levent Bayındır. A review of swarm robotics tasks. Neurocomputing, 172:292–321. Elsevier, 2016.
- Marius Valerian Paulet, Andrei Salceanu and Oana Maria Neacsu. Ultrasonic radar. In Electrical and Power Engineering (EPE), 2016 International Conference and Exposition on, pages 551–554. IEEE, 2016.


### Reference Links
- [Gazebo Tutorials](http://gazebosim.org/tutorials/browse)
- [HC-SR04 Data Sheet](http://centmesh.csc.ncsu.edu/ff_drone_f14_finals/Sensor1/files/hcsr04.pdf) on [CentMesh](http://centmesh.csc.ncsu.edu/)