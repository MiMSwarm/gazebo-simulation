# Notes on Completion

## Division of Tasks
1. Task Initialization
    1. Partitioning - Sentinels, Scouts
    2. Determining Initial Locations.
    3. Create an initial formation.
    4. Initial movement.
2. Task Conduct
    1. Localization of Scouts.
    2. Swarm Movement - Macroscopic
    3. Swarm Movement - Microscopic
3. Task Termination
4. Task Submission

### Task Initialization
Setting up the Swarm for Mapping and Localizing.

1. Each robot picks a random number between 0 and 1.
2. If the number is above threshold sentinel, else scout.
3. Each sentinel maintains a list of sentinels sorted by the number.
4. If insufficient sentinels, scouts are resampled.
5. If too many sentinels, the list is chopped.
6. Reorganize sentinels into a square lattice.
    - Spacing should be half of the WiFi range.
7. Distribute scouts among the sentinels.
    - At least one robot per sentinel.
    - Max. number depends on total robots, robot size and WiFi range.


### Task Conduct
Carrying out the indoor mapping.

#### Localization
Scouts localize by using two sentinels and a scout.
1. The scout pings each sentinel to get the signal intensity.
2. It then determines its distance from both sentinels using the intensity.
3. Using this, it estimates it's location to `(x, y)` or `(x, -y)`.
4. To determine `±y`, it pings another scout that already knows its location.

#### Macroscopic Control
- An indoor region can be defined as a closed region with one or more doorways, or entrances. These regions can be compartmentalized into nested indoor regions with their own entrances and boundaries.
- The swarm is partitioned into patrols. Each patrol is coordinated by two sentinels and consists of at most four other scouts. Every time the swarm is partitioned, it preserves the patrols.

The algorithm of macroscopic control is given by,
1. The sentinels arrange themselves in a square lattice.
2. The entire swarm enters the first indoor region.
3. The swarm follows an edge until it detects a doorway.
4. The swarm registers the entrance as visited.
5. The swarm partitions into swarms A and B.
6. Swarm A enters the region and begins to map it recursively.
7. Swarm B continues along the original edge toward the next entrance.
8. A swarm can only exit a region after it has mapped it entirely.

A mapping is completed when,
- All entrances have been visited.
- The swarm returns to the entrance.
- The internal section of each region is completely mapped.

**A few heuristics to help out:**
- The sentinel lattice does not need to visit each edge of the region completely. If the region is small and it can detect that the region is mapped completely, it can return to the entrance either directly or by backtracking.




#### Microscopic Control


## See Also

Using robots as localization points
University of Maryland - Prof Deng
Robotic Institute of CMU
Formation coordination.
Samay Kohli and Akash Gupta - GreyOrange
