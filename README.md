TSP-ants
========

Solving the traveling salesman problem using swarm intelligence.

Requirements:
Get Qt 5.0 from http://qt-project.org/downloads. Once installed, open Project.pro, compile and run.

Options:
Ant Speed is how far ants will move, in pixels, per cycle. If set to infinity ants will not be shown, 
only the results of all the ants completing their tours will be shown.

Pheromone decay is by what factor the pheromone on each edge decays after each complete tour of all the ants.

Distance exponent and pheromone exponent are the exponents applied to the distance and pheromone components 
when an ant is picking which city to visit next. As both pheromone and distance will be less than 0, higher
values for the exponent will decrease the weight of that parameter.

Default edge pheromone is how much pheromone each edge starts with.

Target framerate is how many frames per second, maximum, each update of the simulation will take.

If show pheromone is checked, pheromone strengths along the edges will be shown as green lines. The thicker the line,
the stronger the pheromone. Note that the best tour found so far will always be shown in cyan.

Generate cities is used to generate a random set of cities of the given size. Cities can be added manually by 
left-clicking on the simulation area, and removed by right clicking on a city.

Three solved tours can be used, taken from http://elib.zib.de/pub/mp-testdata/tsp/tsplib/tsp/index.html. Their
optimal path is shown as dotted lines.



While running, cities can be added or removed via mouse clicks and a good path should form taking the new cities into
account. Cities cannot be added or removed when using one of the solved tours.
