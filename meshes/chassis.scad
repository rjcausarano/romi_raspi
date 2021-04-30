// romi chassis

// cylinder
h = 12;
fn = 100;
r = 81;

// cutouts
// wheels
l = 75;
w = 30;
offset = 58;
// caster
caster_offset = 64.24;
caster_rad = 15.55;

translate([0, 0, -h/2]){
    difference(){
        // chassis base
        cylinder(h, r=r, $fn=fn);
        // wheels
        translate([-l/2, offset, 0])
        cube([l, w, h]);
        translate([-l/2, -offset-w, 0])
        cube([l, w, h]);
        // caster
        translate([-caster_offset, 0, 0])
        cylinder(h, r=caster_rad, $fn=fn);
    }
}