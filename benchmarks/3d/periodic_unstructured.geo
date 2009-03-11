Point(1) = {0.2, 0.8, 0,0.005};
Point(2) = {0.6, 0.8, 0, 0.05};
Point(3) = {0.6, 0.6, 0, 0.05};
Point(4) = {0.5, 0.6, 0, 0.05};
Point(5) = {0.5, 0.5, 0, 0.05};
Point(6) = {0.6, 0.5, 0, 0.05};
Point(7) = {0.6, 0.3, 0, 0.2};
Point(8) = {0.2, 0.3, 0, 0.2};
Line(1) = {2, 1};
Line(2) = {1, 8};
Line(3) = {8, 7};
Line(4) = {7, 6};
Line(5) = {6, 5};
Line(6) = {5, 4};
Line(7) = {4, 3};
Line(8) = {3, 2};
Line Loop(9) = {8, 1, 2, 3, 4, 5, 6, 7};
Plane Surface(10) = {9};

// Nice hack to generate periodic surface meshes on non-extrudable
// geometries: extrude the surface, then delete all the "middle"
// entities

Extrude {{0, 1, 0}, {0, 0, 0}, Pi/10} {
  Surface{10}; Layers{1};
}
Delete {
  Volume{1};
  Surface{27, 51, 23, 47, 43, 31, 35, 39};
  Line{26, 22, 46, 21, 42, 38, 30, 34};
}
