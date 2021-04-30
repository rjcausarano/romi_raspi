// all measurements in millimeters

h = 8;
r = 35;
fn = 100;

rotate([90, 0, 0]){
    translate([0, 0, -h/2])
    cylinder(h, r=r, $fn=fn);
}