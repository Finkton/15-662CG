# Assignment 2: MeshEdit by Xiaoshan Lu

http://462cmu.github.io/asst2_meshedit/

Extra Credit:

task 1: 
· preserve manifoldness - not allow to flip when the object edge has existed (eg. cannot flip an edge of a tetrahedron), and not allow to flip when the degree of either of the endpoints equal or less than 3. When an edge of an endpoint with a degree of 3 flips, the affected endpoint degree will drop to 2, making it becomes a single vertex of a triangle inside an "open fan", while manifoldness requires every vertex should be in "closed fan".

task 2:
· Split boundary edges by splitting the interior face and adding a single vertex to the boundary face.

task 3:
· preserve manifoldness - not allow to collapse an edge of a tetrahedron, and not allow to collapse when the degree of either of the endpoint of the "diagonal line" is equal or less than 3. Same reason as task 1.
